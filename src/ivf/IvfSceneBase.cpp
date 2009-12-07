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

CIvfSceneBase::CIvfSceneBase()
{
	m_view = new CIvfCamera();
	m_composite = new CIvfComposite();
	m_preComposite = new CIvfComposite();
	m_postComposite = new CIvfComposite();

	m_culling = new CIvfCulling();
	m_culling->setComposite(m_composite);
	m_culling->setCullView(m_view);

	m_selection = new CIvfBufferSelection();
	m_selection->setView(m_view);
	m_selection->setComposite(m_composite);

	m_lighting = CIvfLighting::getInstance();

	m_lightMode = LM_LOCAL;
	m_stereoMode = SM_NONE;
	m_colorPair = CP_RED_GREEN;

	m_useCulling = false;
	m_dirty = false;
}

CIvfSceneBase::~CIvfSceneBase()
{
}

void CIvfSceneBase::defaultLocalLighting()
{
	if (m_lightMode == LM_LOCAL)
		if (m_lighting != NULL)
			m_lighting->render();
}

void CIvfSceneBase::doLocalLighting()
{
	defaultLocalLighting();
}

void CIvfSceneBase::defaultViewSetup()
{
	if (m_view!=NULL)
		m_view->render();
}

void CIvfSceneBase::doViewSetup()
{
	defaultViewSetup();
}

void CIvfSceneBase::defaultWorldLighting()
{
	if (m_lightMode == LM_WORLD)
		if (m_lighting != NULL)
			m_lighting->render();
}

void CIvfSceneBase::doWorldLighting()
{
	defaultWorldLighting();
}

void CIvfSceneBase::defaultSceneRender()
{
	m_preComposite->render();
	m_composite->render();
	m_postComposite->render();
}

void CIvfSceneBase::doRender()
{
	defaultSceneRender();
}

void CIvfSceneBase::doViewAndRender()
{
	doLocalLighting();
	doViewSetup();
	doWorldLighting();
	doRender();
}

void CIvfSceneBase::defaultRendering()
{
	doViewAndRender();
}

void CIvfSceneBase::doCreateGeometry()
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
			CIvfView* view = m_view;
			CIvfCamera* camera = (CIvfCamera*)view;

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

			camera->setStereoEye(CIvfCamera::SE_LEFT);
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

			camera->setStereoEye(CIvfCamera::SE_RIGHT);
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
			CIvfView* view = m_view;
			CIvfCamera* camera = (CIvfCamera*)view;

			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

			glPushMatrix();

			if (m_lightMode == LM_LOCAL)
				if (m_lighting != NULL)
					m_lighting->render();

			camera->setStereoEye(CIvfCamera::SE_LEFT);
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

			camera->setStereoEye(CIvfCamera::SE_RIGHT);
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


void CIvfSceneBase::setLightMode(TLightMode mode)
{
	m_lightMode = mode;
}

CIvfSceneBase::TLightMode CIvfSceneBase::getLightMode()
{
	return m_lightMode;
}

void CIvfSceneBase::setCamera(CIvfCamera *camera)
{
	this->setView(camera);
}

CIvfComposite* CIvfSceneBase::getComposite()
{
	return m_composite;
}

void CIvfSceneBase::setUseCulling(bool flag)
{
	m_useCulling = flag;

	if (m_useCulling)
		m_composite->initBoundingSphere();
}

bool CIvfSceneBase::getUseCulling()
{
	return m_useCulling;
}

void CIvfSceneBase::addChild(CIvfShape *shape)
{
	if (m_useCulling)
		shape->initBoundingSphere();

	m_composite->addChild(shape);
	m_dirty = true;
}

CIvfShape* CIvfSceneBase::removeChild(int idx)
{
	return m_composite->removeChild(idx);
	m_dirty = true;
}

CIvfShape* CIvfSceneBase::removeChild(CIvfShape *shape)
{
	return m_composite->removeShape(shape);
	m_dirty = true;
}

void CIvfSceneBase::deleteAll()
{
	m_composite->deleteAll();
	m_dirty = true;
}

void CIvfSceneBase::clear()
{
	m_composite->clear();
	m_dirty = true;
}

int CIvfSceneBase::pick(int x, int y)
{
	if (m_dirty)
		updateSelection();

	return m_selection->pick(x, y);
}

void CIvfSceneBase::updateSelection()
{
	m_selection->update();
}

CIvfShape* CIvfSceneBase::getSelectedShape()
{
	return m_selection->getSelectedShape();
}

void CIvfSceneBase::setView(CIvfView *view)
{
	m_view = view;
	m_culling->setCullView(m_view);
	m_selection->setView(m_view);
}

CIvfView* CIvfSceneBase::getView()
{
	return m_view;
}

CIvfCamera* CIvfSceneBase::getCamera()
{
	CIvfView* view = m_view;

	if (m_view->isClass("CIvfCamera"))
		return (CIvfCamera*) view;
	else
		return NULL;
}

CIvfComposite* CIvfSceneBase::getPostComposite()
{
	return m_postComposite;
}

CIvfComposite* CIvfSceneBase::getPreComposite()
{
	return m_preComposite;
}

void CIvfSceneBase::doResize(int width, int height)
{
	m_view->setViewPort(width, height);
	m_view->initialize();
}

void CIvfSceneBase::setStereoMode(TStereoMode mode)
{
	m_stereoMode = mode;

	if (m_view->isClass("CIvfCamera"))
	{
		CIvfView* view = m_view;
		CIvfCamera* camera = (CIvfCamera*)view;

		switch (m_stereoMode) {
		case SM_NONE:
			CIvfGlobalState::getInstance()->disableGreyscaleRendering();
			camera->setStereo(false);
			break;
		case SM_ANAGLYPH:
			CIvfGlobalState::getInstance()->enableGreyscaleRendering();
		case SM_QUAD_BUFFER:
			camera->setStereo(true);
			break;
		default:
			camera->setStereo(false);
			break;
		}
	}
}

CIvfShape* CIvfSceneBase::removeShape(CIvfShape *shape)
{
	return m_composite->removeShape(shape);
}

CIvfShape* CIvfSceneBase::getSelectedShape(int idx)
{
	return m_selection->getSelectedShape(idx);
}

int CIvfSceneBase::getSelectionSize()
{
	return m_selection->getSize();
}

CIvfSelectedShapesVector& CIvfSceneBase::getSelectedShapes()
{
	return m_selection->getSelectedShapes();
}

void CIvfSceneBase::setAnaglyphColorPair(TAnaglyphColorPair colorPair)
{
	m_colorPair = colorPair;
}

CIvfSceneBase::TAnaglyphColorPair CIvfSceneBase::getAnaglyphColorPair()
{
	return m_colorPair;
}
