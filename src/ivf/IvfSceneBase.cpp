//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#include <ivf/IvfSceneBase.h>

#include <ivf/IvfGlobalState.h>

CSceneBase::CSceneBase()
{
	m_view = new CCamera();
	m_composite = new CComposite();
	m_preComposite = new CComposite();
	m_postComposite = new CComposite();

	m_culling = new CCulling();
	m_culling->setComposite(m_composite);
	m_culling->setCullView(m_view);

	m_selection = new CBufferSelection();
	m_selection->setView(m_view);
	m_selection->setComposite(m_composite);

	m_lighting = CLighting::getInstance();

	m_lightMode = LM_LOCAL;
	m_stereoMode = SM_NONE;
	m_colorPair = CP_RED_GREEN;

	m_useCulling = false;
	m_dirty = false;
	m_nPasses = 1;
	m_multiPass = false;

	m_multipassEvent = NULL;
    m_renderFlatShadow = false;
    m_shadowColor[0] = 0.35;
    m_shadowColor[1] = 0.35;
    m_shadowColor[2] = 0.35;
}

CSceneBase::~CSceneBase()
{
}



void CSceneBase::setMultipass(bool flag)
{
	m_multiPass = flag;
}

bool CSceneBase::getMultipass()
{
	return m_multiPass;
}

void CSceneBase::setPasses(int passes)
{
	m_nPasses = passes;
}

int CSceneBase::getPasses()
{
	return m_nPasses;
}

void CSceneBase::defaultLocalLighting(int pass)
{
	if (m_lightMode == LM_LOCAL)
		if (m_lighting != NULL)
			m_lighting->render();
}

void CSceneBase::doLocalLighting(int pass)
{
	defaultLocalLighting(pass);
}

void CSceneBase::defaultViewSetup(int pass)
{
	if (m_view!=NULL)
		m_view->render();
}

void CSceneBase::doViewSetup(int pass)
{
	defaultViewSetup(pass);
}

void CSceneBase::defaultWorldLighting(int pass)
{
	if (m_lightMode == LM_WORLD)
		if (m_lighting != NULL)
			m_lighting->render();
}

void CSceneBase::doWorldLighting(int pass)
{
	defaultWorldLighting(pass);
}

void CSceneBase::defaultSceneRender(int pass)
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
        CGlobalState::getInstance()->disableColorOutput();
        CGlobalState::getInstance()->disableTextureRendering();
        glColor3d(m_shadowColor[0], m_shadowColor[1], m_shadowColor[2]);
        m_preComposite->render();
        m_composite->render();
        m_postComposite->render();
        CGlobalState::getInstance()->enableColorOutput();
        CGlobalState::getInstance()->enableTextureRendering();
        glPopAttrib();
        glPopMatrix();
    }
}

void CSceneBase::doRender(int pass)
{
	defaultSceneRender(pass);
}

void CSceneBase::doViewAndRender(int pass)
{
	doLocalLighting(pass);
	doViewSetup(pass);
	doWorldLighting(pass);
	doRender(pass);
}

void CSceneBase::defaultRendering()
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
			if (m_multipassEvent!=NULL)
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

void CSceneBase::doMultipass(int pass)
{
	doLocalLighting(pass);
	doViewSetup(pass);
	doWorldLighting(pass);
	doRender(pass);
}

void CSceneBase::doCreateGeometry()
{
	if (m_useCulling)
		m_culling->cull();

	switch (m_stereoMode) {
	case SM_NONE:
		defaultRendering();
		break;
	case SM_ANAGLYPH:
		if (m_view->isClass("CIvfCamera"))
		{
			CView* view = m_view;
			CCamera* camera = (CCamera*)view;

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
				if (m_lighting != NULL)
					m_lighting->render();

			camera->setStereoEye(CCamera::SE_LEFT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != NULL)
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
				if (m_lighting != NULL)
					m_lighting->render();

			camera->setStereoEye(CCamera::SE_RIGHT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != NULL)
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
		if (m_view->isClass("CIvfCamera"))
		{
			CView* view = m_view;
			CCamera* camera = (CCamera*)view;

			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			glPushMatrix();

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != NULL)
					m_lighting->render();

			camera->setStereoEye(CCamera::SE_LEFT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != NULL)
					m_lighting->render();

			m_preComposite->render();
			m_composite->render();
			m_postComposite->render();

			glPopMatrix();

			glPushMatrix();

			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != NULL)
					m_lighting->render();

			camera->setStereoEye(CCamera::SE_RIGHT);
			camera->initialize();

			camera->render();
			if (m_lightMode == LM_WORLD)
				if (m_lighting != NULL)
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


void CSceneBase::setLightMode(TLightMode mode)
{
	m_lightMode = mode;
}

CSceneBase::TLightMode CSceneBase::getLightMode()
{
	return m_lightMode;
}

void CSceneBase::setCamera(CCamera *camera)
{
	this->setView(camera);
}

CComposite* CSceneBase::getComposite()
{
	return m_composite;
}

void CSceneBase::setUseCulling(bool flag)
{
	m_useCulling = flag;

	if (m_useCulling)
		m_composite->initBoundingSphere();
}

bool CSceneBase::getUseCulling()
{
	return m_useCulling;
}

void CSceneBase::addChild(CShape *shape)
{
	if (m_useCulling)
		shape->initBoundingSphere();

	m_composite->addChild(shape);
	m_dirty = true;
}

CShape* CSceneBase::removeChild(int idx)
{
	return m_composite->removeChild(idx);
	m_dirty = true;
}

CShape* CSceneBase::removeChild(CShape *shape)
{
	return m_composite->removeShape(shape);
	m_dirty = true;
}

void CSceneBase::deleteAll()
{
	m_composite->deleteAll();
	m_dirty = true;
    this->doPostClear();
}

void CSceneBase::clear()
{
	m_composite->clear();
	m_dirty = true;
    this->doPostClear();
}

int CSceneBase::pick(int x, int y)
{
	if (m_dirty)
		updateSelection();

	return m_selection->pick(x, y);
}

void CSceneBase::updateSelection()
{
	m_selection->update();
}

CShape* CSceneBase::getSelectedShape()
{
	return m_selection->getSelectedShape();
}

void CSceneBase::setView(CView *view)
{
	m_view = view;
	m_culling->setCullView(m_view);
	m_selection->setView(m_view);
}

CView* CSceneBase::getView()
{
	return m_view;
}

CCamera* CSceneBase::getCamera()
{
	CView* view = m_view;

	if (m_view->isClass("CIvfCamera"))
		return (CCamera*) view;
	else
		return NULL;
}

CComposite* CSceneBase::getPostComposite()
{
	return m_postComposite;
}

CComposite* CSceneBase::getPreComposite()
{
	return m_preComposite;
}

void CSceneBase::doResize(int width, int height)
{
	m_view->setViewPort(width, height);
	m_view->initialize();
}

void CSceneBase::doPostClear()
{
    
}


void CSceneBase::setStereoMode(TStereoMode mode)
{
	m_stereoMode = mode;

	if (m_view->isClass("CIvfCamera"))
	{
		CView* view = m_view;
		CCamera* camera = (CCamera*)view;

		switch (m_stereoMode) {
		case SM_NONE:
			CGlobalState::getInstance()->disableGreyscaleRendering();
			camera->setStereo(false);
			break;
		case SM_ANAGLYPH:
			CGlobalState::getInstance()->enableGreyscaleRendering();
		case SM_QUAD_BUFFER:
			camera->setStereo(true);
			break;
		default:
			camera->setStereo(false);
			break;
		}
	}
}

CShape* CSceneBase::removeShape(CShape *shape)
{
	return m_composite->removeShape(shape);
}

CShape* CSceneBase::getSelectedShape(int idx)
{
	return m_selection->getSelectedShape(idx);
}

int CSceneBase::getSelectionSize()
{
	return m_selection->getSize();
}

CIvfSelectedShapesVector& CSceneBase::getSelectedShapes()
{
	return m_selection->getSelectedShapes();
}

void CSceneBase::setAnaglyphColorPair(TAnaglyphColorPair colorPair)
{
	m_colorPair = colorPair;
}

CSceneBase::TAnaglyphColorPair CSceneBase::getAnaglyphColorPair()
{
	return m_colorPair;
}

void CSceneBase::setMultipassEvent(CMultipassEvent* evt)
{
	m_multipassEvent = evt;
}

void CSceneBase::setRenderFlatShadow(bool flag)
{
    m_renderFlatShadow = flag;
}

void CSceneBase::setShadowColor(double red, double green, double blue)
{
    m_shadowColor[0] = red;
    m_shadowColor[1] = green;
    m_shadowColor[2] = blue;
}

