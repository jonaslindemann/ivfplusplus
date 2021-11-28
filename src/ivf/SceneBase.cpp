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
	m_preComposite->render();
	m_composite->render();
	m_postComposite->render();
    
    if (m_renderFlatShadow)
    {
        glPushMatrix();
        glScaled(1.0, 0.0, 1.0);
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        GlobalState::getInstance()->disableColorOutput();
        GlobalState::getInstance()->disableTextureRendering();
        glColor3d(m_shadowColor[0], m_shadowColor[1], m_shadowColor[2]);
        m_preComposite->render();
        m_composite->render();
        m_postComposite->render();
        GlobalState::getInstance()->enableColorOutput();
        GlobalState::getInstance()->enableTextureRendering();
        glPopAttrib();
        glPopMatrix();
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
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			if (m_multipassEvent!=nullptr)
				m_multipassEvent->onMultipass(renderPass);
			else
				this->doMultipass(renderPass);	
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
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

			glPushMatrix();

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

			glPopMatrix();

			glPushMatrix();

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

			glPopMatrix();
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

			glPushMatrix();

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

			glPopMatrix();

			glPushMatrix();

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

			glPopMatrix();
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
}

Shape* SceneBase::removeChild(int idx)
{
	return m_composite->removeChild(idx);
	m_dirty = true;
}

Shape* SceneBase::removeChild(Shape *shape)
{
	return m_composite->removeShape(shape);
	m_dirty = true;
}

void SceneBase::deleteAll()
{
	m_composite->deleteAll();
	m_dirty = true;
    this->doPostClear();
}

void SceneBase::clear()
{
	m_composite->clear();
	m_dirty = true;
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

