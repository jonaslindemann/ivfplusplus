//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#include <ivf/SceneBase.h>
#include <ivf/GlobalState.h>
#include <ivf/Material.h>
#include <ivf/LegacyGL.h>
#include <ivf/ShaderProgram.h>
#include <ivf/Lighting.h>
#include <ivf/rc.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

using namespace ivf;

SceneBase::SceneBase()
{
	m_view = new Camera();
	m_composite = new Composite();
	m_preComposite = new Composite();
	m_postComposite = new Composite();

	m_culling = new Culling();
	m_culling->setComposite(m_composite);
	m_culling->setCullView(m_view);

	m_selection = new BufferSelection();
	m_selection->setView(m_view);
	m_selection->setComposite(m_composite);

	m_lighting = Lighting::getInstance();

	m_lightMode = LM_LOCAL;
	m_stereoMode = SM_NONE;
	m_colorPair = CP_RED_GREEN;

	m_useCulling = false;
	m_dirty = false;
	m_nPasses = 1;
	m_multiPass = false;

	m_multipassEvent = nullptr;
    m_renderFlatShadow = false;
    m_shadowColor[0] = 0.35;
    m_shadowColor[1] = 0.35;
    m_shadowColor[2] = 0.35;

	m_preShadow = true;
	m_postShadow = true;

	m_useShadowMap = false;
	m_shadowDirty = true;
	m_shadowMapSize = 2048;
	m_shadowLightDir = glm::normalize(glm::vec3(-0.45f, -1.0f, -0.35f));
	m_shadowCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	m_shadowRadius = 10.0f;
	m_shadowStrength = 0.5f;
}

SceneBase::~SceneBase()
{
}



void SceneBase::setMultipass(bool flag)
{
	m_multiPass = flag;
}

bool SceneBase::getMultipass()
{
	return m_multiPass;
}

void SceneBase::setPasses(int passes)
{
	m_nPasses = passes;
}

int SceneBase::getPasses()
{
	return m_nPasses;
}

void SceneBase::defaultLocalLighting(int pass)
{
	if (m_lightMode == LM_LOCAL)
		if (m_lighting != nullptr)
			m_lighting->render();
}

void SceneBase::doLocalLighting(int pass)
{
	defaultLocalLighting(pass);
}

void SceneBase::defaultViewSetup(int pass)
{
	if (m_view!=nullptr)
		m_view->render();
}

void SceneBase::doViewSetup(int pass)
{
	defaultViewSetup(pass);
}

void SceneBase::defaultWorldLighting(int pass)
{
	if (m_lightMode == LM_WORLD)
		if (m_lighting != nullptr)
			m_lighting->render();
}

void SceneBase::doWorldLighting(int pass)
{
	defaultWorldLighting(pass);
}

void SceneBase::defaultSceneRender(int pass)
{
	this->renderShadowMap();

	m_preComposite->render();
	m_composite->render();
	m_postComposite->render();
    
    if (m_renderFlatShadow)
    {
        // Everything this pass does to set itself up -- the flatten, the
        // lighting and texture disables, the shadow colour -- is fixed-function
        // only. On the shader path none of it arrived: the scene redrew at full
        // height on top of itself, untextured and in whatever material each
        // shape carried. A black text label came out as a solid black rectangle
        // sitting over the model rather than as a shadow on the ground.

        const bool shaderPath = rcIsShaderActive();

        lgPushMatrix();
        lgScaled(1.0, 0.0, 1.0);

        if (shaderPath)
        {
            rcPushMatrix();
            rcScale(1.0f, 0.0f, 1.0f);
        }

        // Turning lighting off has to go through Lighting rather than straight
        // to GL. lgDisableLegacy() is a no-op in core, where glDisable(GL_LIGHTING)
        // does not exist, so the cache that Material consults would still have
        // said "lit" and every shape would have re-uploaded its own material over
        // the shadow colour. Only in core, which is exactly where nobody looks.

        const bool lightingWasEnabled = Lighting::getInstance()->isEnabled();

        lgPushAttrib(GL_ENABLE_BIT);
        Lighting::getInstance()->disable();
        lgDisableLegacy(GL_TEXTURE_2D);
        GlobalState::getInstance()->disableColorOutput();
        GlobalState::getInstance()->disableTextureRendering();
        lgColor3d(m_shadowColor[0], m_shadowColor[1], m_shadowColor[2]);

        if (shaderPath)
        {
            // The shader is told the same three things. Emission carries the
            // colour, with every lit term zeroed, so the result is flat whatever
            // the lights are doing -- the shader has no "lighting disabled"
            // switch and this needs none. Diffuse alpha stays at 1 so the alpha
            // test does not throw the shadow away.

            rcSetUseTexture(false);

            ShaderProgram *prog = rcShader();

            if (prog != nullptr)
            {
                prog->setUniformVec4("uMatAmbient", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                prog->setUniformVec4("uMatDiffuse", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                prog->setUniformVec4("uMatSpecular", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
                prog->setUniformVec4("uMatEmission",
                                     glm::vec4((float)m_shadowColor[0], (float)m_shadowColor[1],
                                               (float)m_shadowColor[2], 1.0f));
            }
        }

		if (m_preShadow)
			m_preComposite->render();

		m_composite->render();

		if (m_postShadow)
			m_postComposite->render();

		GlobalState::getInstance()->enableColorOutput();
        GlobalState::getInstance()->enableTextureRendering();

        if (lightingWasEnabled)
            Lighting::getInstance()->enable();

        lgPopAttrib();

        if (shaderPath)
        {
            // Emission is sticky, so clear it rather than leaving every later
            // unmaterialled object glowing in the shadow colour.

            ShaderProgram *prog = rcShader();

            if (prog != nullptr)
                prog->setUniformVec4("uMatEmission", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            rcPopMatrix();
        }

        lgPopMatrix();
    }
}

void SceneBase::doRender(int pass)
{
	defaultSceneRender(pass);
}

void SceneBase::doViewAndRender(int pass)
{
	doLocalLighting(pass);
	doViewSetup(pass);
	doWorldLighting(pass);
	doRender(pass);
}

void SceneBase::defaultRendering()
{
	if (m_multiPass)
	{
		int renderPass = 0;

		for (renderPass = 0; renderPass<m_nPasses; renderPass++)
		{
			lgMatrixMode(GL_PROJECTION);
			lgPushMatrix();
			lgMatrixMode(GL_MODELVIEW);
			lgPushMatrix();
			if (m_multipassEvent!=nullptr)
				m_multipassEvent->onMultipass(renderPass);
			else
				this->doMultipass(renderPass);	
			lgMatrixMode(GL_PROJECTION);
			lgPopMatrix();
			lgMatrixMode(GL_MODELVIEW);
			lgPopMatrix();
		}
	}
	else
		doViewAndRender(0);
}

void SceneBase::doMultipass(int pass)
{
	doLocalLighting(pass);
	doViewSetup(pass);
	doWorldLighting(pass);
	doRender(pass);
}

void SceneBase::doCreateGeometry()
{
	// Anything may have touched GL material state since the previous frame --
	// overlays, UI toolkits, application code -- so start each scene traversal
	// without assumptions about what is currently applied.

	Material::invalidateStateCache();

	if (m_useCulling)
		m_culling->cull();

	switch (m_stereoMode) {
	case SM_NONE:
		defaultRendering();
		break;
	case SM_ANAGLYPH:
		if (m_view->isClass("Camera"))
		{
			View* view = m_view;
			Camera* camera = (Camera*)view;

			lgPushMatrix();

			switch (m_colorPair) {
			case CP_RED_GREEN:
				glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
				break;
			case CP_RED_BLUE:
				glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
				break;
			case CP_RED_CYAN:
				glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
				break;
			default:
				glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
				break;
			}

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != nullptr)
					m_lighting->render();

			camera->setStereoEye(Camera::SE_LEFT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != nullptr)
					m_lighting->render();

			m_preComposite->render();
			m_composite->render();
			m_postComposite->render();

			lgPopMatrix();

			lgPushMatrix();

			glClear(GL_DEPTH_BUFFER_BIT);

			switch (m_colorPair) {
			case CP_RED_GREEN:
				glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
				break;
			case CP_RED_BLUE:
				glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
				break;
			case CP_RED_CYAN:
				glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
				break;
			default:
				glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
				break;
			}

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != nullptr)
					m_lighting->render();

			camera->setStereoEye(Camera::SE_RIGHT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != nullptr)
					m_lighting->render();

			m_preComposite->render();
			m_composite->render();
			m_postComposite->render();

			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

			lgPopMatrix();
		}
		else
			defaultRendering();
		break;
	case SM_QUAD_BUFFER:
		if (m_view->isClass("Camera"))
		{
			View* view = m_view;
			Camera* camera = (Camera*)view;

			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			lgPushMatrix();

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != nullptr)
					m_lighting->render();

			camera->setStereoEye(Camera::SE_LEFT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != nullptr)
					m_lighting->render();

			m_preComposite->render();
			m_composite->render();
			m_postComposite->render();

			lgPopMatrix();

			lgPushMatrix();

			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != nullptr)
					m_lighting->render();

			camera->setStereoEye(Camera::SE_RIGHT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != nullptr)
					m_lighting->render();

			m_preComposite->render();
			m_composite->render();
			m_postComposite->render();

			lgPopMatrix();
		}
		else
			defaultRendering();
		break;
	}
}


void SceneBase::setLightMode(TLightMode mode)
{
	m_lightMode = mode;
}

SceneBase::TLightMode SceneBase::getLightMode()
{
	return m_lightMode;
}

void SceneBase::setCamera(Camera *camera)
{
	this->setView(camera);
}

Composite* SceneBase::getComposite()
{
	return m_composite;
}

void SceneBase::setUseCulling(bool flag)
{
	m_useCulling = flag;

	if (m_useCulling)
		m_composite->initBoundingSphere();
}

bool SceneBase::getUseCulling()
{
	return m_useCulling;
}

void SceneBase::addChild(Shape *shape)
{
	if (m_useCulling)
		shape->initBoundingSphere();

	m_composite->addChild(shape);
	m_dirty = true;
	this->invalidateShadowMap();
}

Shape* SceneBase::removeChild(int idx)
{
	// The two assignments below used to sit after the return, where they never
	// ran. That left m_dirty false after a removal, so pick() went on consulting
	// a selection map that still held the shape that had just been taken out.

	Shape* removed = m_composite->removeChild(idx);

	m_dirty = true;
	this->invalidateShadowMap();

	return removed;
}

Shape* SceneBase::removeChild(Shape *shape)
{
	Shape* removed = m_composite->removeShape(shape);

	m_dirty = true;
	this->invalidateShadowMap();

	return removed;
}

void SceneBase::deleteAll()
{
	m_composite->deleteAll();
	m_dirty = true;
	this->invalidateShadowMap();
    this->doPostClear();
}

void SceneBase::clear()
{
	m_composite->clear();
	m_dirty = true;
	this->invalidateShadowMap();
    this->doPostClear();
}

int SceneBase::pick(int x, int y)
{
	if (m_dirty)
		updateSelection();

	return m_selection->pick(x, y);
}

void SceneBase::updateSelection()
{
	m_selection->update();
}

Shape* SceneBase::getSelectedShape()
{
	return m_selection->getSelectedShape();
}

void SceneBase::setView(View *view)
{
	m_view = view;
	m_culling->setCullView(m_view);
	m_selection->setView(m_view);
}

View* SceneBase::getView()
{
	return m_view;
}

Camera* SceneBase::getCamera()
{
	View* view = m_view;

	if (m_view->isClass("Camera"))
		return (Camera*) view;
	else
		return nullptr;
}

Composite* SceneBase::getPostComposite()
{
	return m_postComposite;
}

Composite* SceneBase::getPreComposite()
{
	return m_preComposite;
}

void SceneBase::doResize(int width, int height)
{
	m_view->setViewPort(width, height);
	m_view->initialize();
}

void SceneBase::doPostClear()
{
    
}


void SceneBase::setStereoMode(TStereoMode mode)
{
	m_stereoMode = mode;

	if (m_view->isClass("Camera"))
	{
		View* view = m_view;
		Camera* camera = (Camera*)view;

		switch (m_stereoMode) {
		case SM_NONE:
			GlobalState::getInstance()->disableGreyscaleRendering();
			camera->setStereo(false);
			break;
		case SM_ANAGLYPH:
			GlobalState::getInstance()->enableGreyscaleRendering();
		case SM_QUAD_BUFFER:
			camera->setStereo(true);
			break;
		default:
			camera->setStereo(false);
			break;
		}
	}
}

Shape* SceneBase::removeShape(Shape *shape)
{
	return m_composite->removeShape(shape);
}

Shape* SceneBase::getSelectedShape(int idx)
{
	return m_selection->getSelectedShape(idx);
}

int SceneBase::getSelectionSize()
{
	return m_selection->getSize();
}

SelectedShapesVector& SceneBase::getSelectedShapes()
{
	return m_selection->getSelectedShapes();
}

void SceneBase::setAnaglyphColorPair(TAnaglyphColorPair colorPair)
{
	m_colorPair = colorPair;
}

SceneBase::TAnaglyphColorPair SceneBase::getAnaglyphColorPair()
{
	return m_colorPair;
}

void SceneBase::setMultipassEvent(MultipassEvent* evt)
{
	m_multipassEvent = evt;
}

void SceneBase::setRenderFlatShadow(bool flag)
{
    m_renderFlatShadow = flag;
}

void SceneBase::setShadowColor(double red, double green, double blue)
{
    m_shadowColor[0] = red;
    m_shadowColor[1] = green;
    m_shadowColor[2] = blue;
}

glm::mat4 SceneBase::calcLightSpaceMatrix() const
{
	const glm::vec3 dir = glm::normalize(m_shadowLightDir);
	const float radius = (m_shadowRadius > 1e-4f) ? m_shadowRadius : 1.0f;

	// Stand the light off by twice the radius, so the whole sphere sits in front
	// of it whatever direction it comes from.

	const glm::vec3 eye = m_shadowCenter - dir * (radius * 2.0f);

	// lookAt degenerates when the up vector is parallel to the view direction,
	// which is exactly the case a light straight overhead produces.

	const glm::vec3 up = (std::fabs(dir.y) > 0.99f) ? glm::vec3(0.0f, 0.0f, 1.0f)
	                                                : glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::mat4 lightView = glm::lookAt(eye, m_shadowCenter, up);

	// Fitted to the bounding sphere and no larger. Every unit of slack here is
	// resolution thrown away: the map is a fixed number of texels spread over
	// whatever the frustum covers.

	const glm::mat4 lightProj = glm::ortho(-radius, radius, -radius, radius,
	                                       radius * 0.5f, radius * 3.5f);

	return lightProj * lightView;
}

void SceneBase::renderShadowMap()
{
	// Shadow mapping is a shader technique; there is nothing behind the fixed
	// function pipeline to sample a depth texture with. Picking renders the same
	// composites to answer a different question and must not pay for this.

	if (!m_useShadowMap || !rcIsShaderActive() || rcPickMode() || rcDepthPass())
		return;

	if (m_shadowMap == nullptr)
		m_shadowMap = new ShadowMap();

	const bool wasValid = m_shadowMap->isValid();

	if (!m_shadowMap->initialize(m_shadowMapSize))
	{
		// A target that will not complete is not worth retrying every frame, and
		// leaving m_useShadowMap set would keep the shader sampling a texture
		// that was never rendered.

		m_useShadowMap = false;
		rcSetShadowMap(0, 4);
		return;
	}

	// A map that was just created holds nothing yet, whatever the flag says.

	if (!wasValid)
		m_shadowDirty = true;

	if (m_shadowDirty)
	{
		const glm::mat4 lightSpaceMatrix = this->calcLightSpaceMatrix();

		m_shadowMap->setLightSpaceMatrix(lightSpaceMatrix);

		if (!this->renderShadowDepth(lightSpaceMatrix))
			return;

		m_shadowDirty = false;
	}

	// Done every frame, cached or not: the texture unit and the uniforms are
	// global state that anything else drawing in this context may have changed,
	// and re-asserting them costs a handful of calls per frame rather than per
	// shape.

	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap->depthTexture());
	glActiveTexture(GL_TEXTURE0);

	rcSetShadowMap(m_shadowMap->depthTexture(), 4);
	rcSetLightSpaceMatrix(m_shadowMap->lightSpaceMatrix());
	rcSetShadowLightDirection(m_shadowLightDir);
	rcSetShadowStrength(m_shadowStrength);
}

bool SceneBase::renderShadowDepth(const glm::mat4& lightSpaceMatrix)
{
	// Save what the pass is about to change. This runs inside the caller's frame,
	// so everything here has to be handed back.

	GLint previousFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFbo);

	GLint previousViewport[4];
	glGetIntegerv(GL_VIEWPORT, previousViewport);

	const GLboolean hadCullFace = glIsEnabled(GL_CULL_FACE);
	const GLboolean hadDepthTest = glIsEnabled(GL_DEPTH_TEST);
	const GLboolean hadBlend = glIsEnabled(GL_BLEND);

	GLint previousCullMode = GL_BACK;
	glGetIntegerv(GL_CULL_FACE_MODE, &previousCullMode);

	GLboolean previousDepthMask = GL_TRUE;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &previousDepthMask);

	GLint previousDepthFunc = GL_LESS;
	glGetIntegerv(GL_DEPTH_FUNC, &previousDepthFunc);

	const glm::mat4 savedProjection = rcProjection();
	const glm::mat4 savedView = rcView();

	// rcIsShaderActive() above guarantees a program was bound, so the previous
	// shader is only null if the depth program would not link -- in which case
	// the scene shader is still current and drawing the scene now would fill the
	// map with nonsense.

	ShaderProgram* previousShader = rcUseDepthShader();

	if (previousShader == nullptr)
	{
		m_useShadowMap = false;
		rcSetShadowMap(0, 4);
		return false;
	}

	m_shadowMap->bind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	// Culling front faces means the depth written is the far side of each solid,
	// which puts the bias error inside the object where it cannot show. Beams are
	// closed tubes, so this is safe for them; it is the reason a flat receiver
	// like a ground plane still needs a bias at all.

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glClear(GL_DEPTH_BUFFER_BIT);

	// The depth program takes uProjection and uView like any other, so installing
	// the light's matrices is all it takes for the existing traversal to draw
	// from the light instead of from the camera.

	rcSetProjection(lightSpaceMatrix);
	rcSetView(glm::mat4(1.0f));
	rcSetDepthPass(true);

	if (m_preShadow)
		m_preComposite->render();

	m_composite->render();

	if (m_postShadow)
		m_postComposite->render();

	rcSetDepthPass(false);

	// Restore

	rcSetProjection(savedProjection);
	rcSetView(savedView);

	rcSetShader(previousShader);
	rcUseShader();

	glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFbo);
	glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);

	glCullFace((GLenum)previousCullMode);
	glDepthMask(previousDepthMask);
	glDepthFunc((GLenum)previousDepthFunc);

	if (!hadCullFace)
		glDisable(GL_CULL_FACE);
	if (!hadDepthTest)
		glDisable(GL_DEPTH_TEST);
	if (hadBlend)
		glEnable(GL_BLEND);

	return true;
}

void SceneBase::setUseShadowMap(bool flag)
{
	if (flag && !m_useShadowMap)
		m_shadowDirty = true;

	m_useShadowMap = flag;

	if (!flag)
		rcSetShadowMap(0, 4);
}

bool SceneBase::useShadowMap() const
{
	return m_useShadowMap;
}

void SceneBase::setShadowMapSize(int size)
{
	if ((size > 0) && (size != m_shadowMapSize))
	{
		m_shadowMapSize = size;
		m_shadowDirty = true;
	}
}

int SceneBase::shadowMapSize() const
{
	return m_shadowMapSize;
}

void SceneBase::setShadowLightDirection(double x, double y, double z)
{
	const glm::vec3 dir((float)x, (float)y, (float)z);

	if (glm::dot(dir, dir) > 1e-12f)
	{
		const glm::vec3 normalized = glm::normalize(dir);

		if (normalized != m_shadowLightDir)
		{
			m_shadowLightDir = normalized;
			m_shadowDirty = true;
		}
	}
}

void SceneBase::setShadowBounds(double centerX, double centerY, double centerZ, double radius)
{
	const glm::vec3 center((float)centerX, (float)centerY, (float)centerZ);
	const float r = (radius > 0.0) ? (float)radius : m_shadowRadius;

	// Compared rather than assigned unconditionally: this is re-applied whenever
	// the application re-states its shadow settings, and an invalidation on every
	// such call would defeat the cache for no change at all.

	if ((center != m_shadowCenter) || (r != m_shadowRadius))
	{
		m_shadowCenter = center;
		m_shadowRadius = r;
		m_shadowDirty = true;
	}
}

void SceneBase::setShadowStrength(double strength)
{
	m_shadowStrength = (float)((strength < 0.0) ? 0.0 : ((strength > 1.0) ? 1.0 : strength));
}

double SceneBase::shadowStrength() const
{
	return (double)m_shadowStrength;
}

void SceneBase::invalidateShadowMap()
{
	m_shadowDirty = true;
}

void ivf::SceneBase::setShadowPrePost(bool renderPre, bool renderPost)
{
	m_preShadow = renderPre;
	m_postShadow = renderPost;
}


// ------------------------------------------------------------
bool SceneBase::hasModernPath()
{
	return true;
}
