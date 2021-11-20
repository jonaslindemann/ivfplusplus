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

#include <ivfwidget/WorkspaceWidget.h>

#include <ivf3dui/UIInteractiveBase.h>

using namespace ivf;

CWorkspaceWidget::CWorkspaceWidget()
{
	
}

CWorkspaceWidget::~CWorkspaceWidget()
{
	
}

void CWorkspaceWidget::doInit(int width, int height)
{
	IvfDbg2("CWorkspaceWidget::doInit: Start.");
	
	// Initialize variables
	
	m_editMode = EM_VIEW;
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
	m_selectOver = false;
	
	m_lastOver = NULL;
	
	// Initialize Ivf++ camera
	
	m_camera = new CCamera();
	m_camera->setPosition(0.0, 8.0, 8.0);
	
	// Create scene composite
	
	m_workspace = new CWorkspace();
	m_workspace->setCamera(m_camera);
	m_workspace->setRelativeCursorSize(0.2);
	m_workspace->setWorkspaceSize(10.0);
	
	// Setup lighting
	
	CLighting* lighting = CLighting::getInstance();
	
	CLight* light = lighting->getLight(0);
	light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	light->enable();
	
	onInit(width, height);
	
	IvfDbg2("CWorkspaceWidget::doInit: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doResize(int width, int height)
{
	IvfDbg2("CWorkspaceWidget::doResize: Start.");
	
	CWidgetBase::doResize(width, height); // This must be here!
	
	//m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
	
	onResize(width, height);
	
	IvfDbg2("CWorkspaceWidget::doResize: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doRender()
{
	IvfDbg2("CWorkspaceWidget::doRender: Start.");
	
	m_camera->rotatePositionY(m_angleX/100.0);
	m_camera->rotatePositionX(m_angleY/100.0);
	m_camera->moveSideways(m_moveX/100.0);
	m_camera->moveVertical(m_moveY/100.0);
	m_camera->moveDepth(m_zoomY/50.0);
	
	m_camera->moveSideways(m_moveX*m_workspace->getWorkspaceSize()/1000.0);
	m_camera->moveVertical(m_moveY*m_workspace->getWorkspaceSize()/1000.0);
	m_camera->moveDepth(m_zoomY*m_workspace->getWorkspaceSize()/500.0);
	
	m_workspace->render();
	
	onRender();
	
	IvfDbg2("CWorkspaceWidget::doRender: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doDestroy()
{
	IvfDbg2("CWorkspaceWidget::doDestroy: Start.");
	onDestroy();
	IvfDbg2("CWorkspaceWidget::doDestroy: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doMouseDown(int x, int y)
{
	IvfDbg2("CWorkspaceWidget::doMouseDown: Start.");
	
	m_beginX = x;
	m_beginY = y;
	
	CVec3d vec;
	CShapePtr shape;
	CUIInteractiveBasePtr uiShape;
	
	if ( isLeftButtonDown() )
	{
        switch (m_editMode) {
        case EM_CREATE_POINT:
        case EM_CREATE_LINE:
        case EM_CREATE_POLY:
			doRedraw();
            break;
        case EM_INTERACT:
            m_workspace->pick(x, y);
			shape = m_workspace->getSelectedShape();
			uiShape = queryUIShape(shape);

			if (uiShape==NULL)
				onShapeDown(shape);
			else
			{
				uiShape->doControlDown(vec, 0);
				onControlDown(uiShape);
				doRedraw();
			}
            break;
        default:
            break;
        }
	}
	
	onMouseDown(x, y);
	
	IvfDbg2("CWorkspaceWidget::doMouseDown: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doMouseMove(int x, int y)
{
	IvfDbg2("CWorkspaceWidget::doMouseMove: Start.");
	
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	TEditMode oldMode = m_editMode;

	if (getModifierKey()==MT_CTRL)
		setEditMode(EM_VIEW);
	
    switch (m_editMode) {
    case EM_VIEW:
		if (isLeftButtonDown())
		{
			m_angleX = (x - m_beginX);
			m_angleY = (y - m_beginY);
			m_beginX = x;
			m_beginY = y;
			m_workspace->updateSizes();
			doRedraw();
		}
		
		if (isRightButtonDown())
		{
			if (getModifierKey() == CWidgetBase::MT_SHIFT)
			{
				m_zoomX = (x - m_beginX);
				m_zoomY = (y - m_beginY);
			}
			else
			{
				m_moveX = (x - m_beginX);
				m_moveY = (y - m_beginY);
			}
			m_beginX = x;
			m_beginY = y;
			m_workspace->updateSizes();
			doRedraw();
		}
        break;
    case EM_CREATE_POINT:
    case EM_CREATE_LINE:
    case EM_CREATE_POLY:
		if ( (!isLeftButtonDown())&&(!isRightButtonDown()) )
		{
			m_workspace->updateCursor(x, y);
			
			CVec3d pos;
			pos = m_workspace->getCursorPosition();
			double wx, wy, wz;
			pos.getComponents(wx, wy, wz);
			
			onCursor(wx, wy, wz);
			
			doRedraw();
		}
        break;
    case EM_INTERACT:
		if (isAnyButtonDown())
		{
			m_workspace->pick(x, y);
			
			CShapePtr shape = m_workspace->getSelectedShape();
			CUIInteractiveBasePtr uiShape = queryUIShape(shape);
			CUIInteractiveBasePtr lastUIShape = queryUIShape(m_lastOver);
			
			CVec3d vec;
			
			if (uiShape==NULL)
			{
				if (shape!=m_lastOver)
				{
					// We don't leave from NULL
					
					if (lastUIShape!=NULL)
					{
						lastUIShape->doControlLeave(vec);
						onControlLeave(lastUIShape);
						lastUIShape->doControlUp(vec);
						onControlUp(lastUIShape);
						lastUIShape = NULL;
						doRedraw();
					}
					else
					{
						if (m_lastOver!=NULL)
							onShapeLeave(m_lastOver);

						m_lastOver = NULL;
					}
				}
				
				onShapeDrag(m_workspace->getSelectedShape());
			}
			else
			{
				if (shape!=m_lastOver)
				{
					// We don't leave from NULL
					
					if (lastUIShape!=NULL)
					{
						lastUIShape->doControlLeave(vec);
						onControlLeave(lastUIShape);
						lastUIShape->doControlUp(vec);
						onControlUp(lastUIShape);
						lastUIShape = NULL;
					}
					else
					{
						if (m_lastOver!=NULL)
							onShapeLeave(m_lastOver);

						m_lastOver = NULL;
					}
				}
				
				uiShape->doControlDrag(vec, 0);
				onControlDrag(uiShape);
				
				doRedraw();
			}
			
			m_lastOver = shape;
		}
		else
		{
			if (m_selectOver)
			{
				m_workspace->pick(x, y);
				
				CShapePtr shape = m_workspace->getSelectedShape();
				CUIInteractiveBasePtr uiShape = queryUIShape(shape);
				CUIInteractiveBasePtr lastUIShape = queryUIShape(m_lastOver);
				
				CVec3d vec;
				
				if (uiShape==NULL)
				{
					if (shape!=m_lastOver)
					{
						// We don't leave from NULL
						
						if (lastUIShape!=NULL)
						{
							lastUIShape->doControlLeave(vec);
							onControlLeave(lastUIShape);
							//lastUIShape->doControlUp(vec);
							//onControlUp(lastUIShape);
							doRedraw();
						}
						else
						{
							if (m_lastOver!=NULL)
								onShapeLeave(m_lastOver);

						}
					}
					
					onShapeOver(m_workspace->getSelectedShape());
				}
				else
				{
					if (shape!=m_lastOver)
					{
						// We don't leave from NULL
						
						if (lastUIShape!=NULL)
						{
							lastUIShape->doControlLeave(vec);
							onControlLeave(lastUIShape);
							//lastUIShape->doControlUp(vec);
							//onControlUp(lastUIShape);
						}
						else
						{
							if (m_lastOver!=NULL)
								onShapeLeave(m_lastOver);
						}
					}
					
					uiShape->doControlOver(vec);
					onControlOver(uiShape);
					
					doRedraw();
				}
				
				m_lastOver = shape;
			}
		}
        break;
    default:
        break;
    }

	m_editMode = oldMode;
	
	onMouseMove(x, y);
	
	IvfDbg2("CWorkspaceWidget::doMouseMove: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doMouseUp(int x, int y)
{
	IvfDbg2("CWorkspaceWidget::doMouseUp: Start.");
	
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	CVec3d vec;
	CShapePtr shape;
	CUIInteractiveBasePtr uiShape;
	CUIInteractiveBasePtr lastUIShape;

	switch (m_editMode) {
	case EM_INTERACT:
		m_workspace->pick(x, y);

		shape = m_workspace->getSelectedShape();
		uiShape = queryUIShape(shape);
		lastUIShape = queryUIShape(m_lastOver);

		if ((lastUIShape!=NULL)&&(lastUIShape!=uiShape))
		{
			lastUIShape->doControlClick(vec, 0);
			onControlClick(lastUIShape);
			lastUIShape->doControlUp(vec);
			onControlUp(lastUIShape);
			doRedraw();
		}

		if (uiShape==NULL)
		{
			onShapeClick(m_workspace->getSelectedShape());
			onShapeUp(m_workspace->getSelectedShape());
		}
		else
		{
			uiShape->doControlClick(vec, 0);
			onControlClick(uiShape);
			uiShape->doControlUp(vec);
			onControlUp(uiShape);
			doRedraw();
		}
		break;
	default:
		break;
	}
	
	onMouseUp(x, y);
	
	IvfDbg2("CWorkspaceWidget::doMouseUp: End.");
}

// ------------------------------------------------------------
void CWorkspaceWidget::doKeyboard(int key, int x, int y)
{
	IvfDbg2("CWorkspaceWidget::doKeyboard: Start.");
	
	onKeyboard(key, x, y);
	
	IvfDbg2("CWorkspaceWidget::doKeyboard: End.");
}

void CWorkspaceWidget::addChild(CShape *shape)
{
	m_workspace->addChild(shape);
}

void CWorkspaceWidget::setEditMode(TEditMode mode)
{
	m_editMode = mode;
}

CWorkspaceWidget::TEditMode CWorkspaceWidget::getEditMode()
{
	return m_editMode;
}

void CWorkspaceWidget::onCursor(double x, double y, double z)
{
	
}

CCamera* CWorkspaceWidget::getCamera()
{
	return m_camera;
}

CWorkspace* CWorkspaceWidget::getWorkspace()
{
	return m_workspace;
}

void CWorkspaceWidget::onShapeDown(CShape *shape)
{
	
}

void CWorkspaceWidget::onShapeClick(CShape *shape)
{
	
}

void CWorkspaceWidget::onShapeUp(CShape *shape)
{
	
}

void CWorkspaceWidget::onShapeOver(CShape *shape)
{
	
}

void CWorkspaceWidget::onShapeLeave(CShape *shape)
{
	
}

void CWorkspaceWidget::enableShapeOver()
{
	m_selectOver = true;
}

void CWorkspaceWidget::disableShapeOver()
{
	m_selectOver = false;
}

bool CWorkspaceWidget::isShapeOverEnabled()
{
	return m_selectOver;
}

void CWorkspaceWidget::onShapeDrag(CShape *shape)
{
	
}

CUIInteractiveBasePtr CWorkspaceWidget::queryUIShape(CShape *shape)
{
	CUIInteractiveBasePtr uiShape;

	if (shape!=NULL)
	{
		if (shape->isClass("CUIInteractiveBase"))
		{
			CShape* s = shape;
			uiShape = (CUIInteractiveBase*)s;
		}
	}

	return uiShape;
}

void CWorkspaceWidget::onControlOver(CUIInteractiveBase *uiControl)
{

}

void CWorkspaceWidget::onControlLeave(CUIInteractiveBase *uiControl)
{

}

void CWorkspaceWidget::onControlDrag(CUIInteractiveBase *uiControl)
{

}

void CWorkspaceWidget::onControlDown(CUIInteractiveBase *uiControl)
{

}

void CWorkspaceWidget::onControlClick(CUIInteractiveBase *uiControl)
{

}

void CWorkspaceWidget::onControlUp(CUIInteractiveBase *uiControl)
{

}
