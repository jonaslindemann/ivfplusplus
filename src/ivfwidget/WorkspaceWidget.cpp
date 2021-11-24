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

#include <ivfwidget/WorkspaceWidget.h>

#include <ivf3dui/UIInteractiveBase.h>

using namespace ivf;

WorkspaceWidget::WorkspaceWidget()
{
	
}

WorkspaceWidget::~WorkspaceWidget()
{
	
}

void WorkspaceWidget::doInit(int width, int height)
{
	IvfDbg2("WorkspaceWidget::doInit: Start.");
	
	// Initialize variables
	
	m_editMode = EM_VIEW;
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
	m_selectOver = false;
	
	m_lastOver = nullptr;
	
	// Initialize Ivf++ camera
	
	m_camera = new Camera();
	m_camera->setPosition(0.0, 8.0, 8.0);
	
	// Create scene composite
	
	m_workspace = new Workspace();
	m_workspace->setCamera(m_camera);
	m_workspace->setRelativeCursorSize(0.2);
	m_workspace->setWorkspaceSize(10.0);
	
	// Setup lighting
	
	Lighting* lighting = Lighting::getInstance();
	
	Light* light = lighting->getLight(0);
	light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	light->enable();
	
	onInit(width, height);
	
	IvfDbg2("WorkspaceWidget::doInit: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doResize(int width, int height)
{
	IvfDbg2("WorkspaceWidget::doResize: Start.");
	
	WidgetBase::doResize(width, height); // This must be here!
	
	//m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
	
	onResize(width, height);
	
	IvfDbg2("WorkspaceWidget::doResize: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doRender()
{
	IvfDbg2("WorkspaceWidget::doRender: Start.");
	
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
	
	IvfDbg2("WorkspaceWidget::doRender: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doDestroy()
{
	IvfDbg2("WorkspaceWidget::doDestroy: Start.");
	onDestroy();
	IvfDbg2("WorkspaceWidget::doDestroy: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doMouseDown(int x, int y)
{
	IvfDbg2("WorkspaceWidget::doMouseDown: Start.");
	
	m_beginX = x;
	m_beginY = y;
	
	Vec3d vec;
	ShapePtr shape;
	UIInteractiveBasePtr uiShape;
	
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

			if (uiShape==nullptr)
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
	
	IvfDbg2("WorkspaceWidget::doMouseDown: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doMouseMove(int x, int y)
{
	IvfDbg2("WorkspaceWidget::doMouseMove: Start.");
	
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
			if (getModifierKey() == WidgetBase::MT_SHIFT)
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
			
			Vec3d pos;
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
			
			ShapePtr shape = m_workspace->getSelectedShape();
			UIInteractiveBasePtr uiShape = queryUIShape(shape);
			UIInteractiveBasePtr lastUIShape = queryUIShape(m_lastOver);
			
			Vec3d vec;
			
			if (uiShape==nullptr)
			{
				if (shape!=m_lastOver)
				{
					// We don't leave from nullptr
					
					if (lastUIShape!=nullptr)
					{
						lastUIShape->doControlLeave(vec);
						onControlLeave(lastUIShape);
						lastUIShape->doControlUp(vec);
						onControlUp(lastUIShape);
						lastUIShape = nullptr;
						doRedraw();
					}
					else
					{
						if (m_lastOver!=nullptr)
							onShapeLeave(m_lastOver);

						m_lastOver = nullptr;
					}
				}
				
				onShapeDrag(m_workspace->getSelectedShape());
			}
			else
			{
				if (shape!=m_lastOver)
				{
					// We don't leave from nullptr
					
					if (lastUIShape!=nullptr)
					{
						lastUIShape->doControlLeave(vec);
						onControlLeave(lastUIShape);
						lastUIShape->doControlUp(vec);
						onControlUp(lastUIShape);
						lastUIShape = nullptr;
					}
					else
					{
						if (m_lastOver!=nullptr)
							onShapeLeave(m_lastOver);

						m_lastOver = nullptr;
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
				
				ShapePtr shape = m_workspace->getSelectedShape();
				UIInteractiveBasePtr uiShape = queryUIShape(shape);
				UIInteractiveBasePtr lastUIShape = queryUIShape(m_lastOver);
				
				Vec3d vec;
				
				if (uiShape==nullptr)
				{
					if (shape!=m_lastOver)
					{
						// We don't leave from nullptr
						
						if (lastUIShape!=nullptr)
						{
							lastUIShape->doControlLeave(vec);
							onControlLeave(lastUIShape);
							//lastUIShape->doControlUp(vec);
							//onControlUp(lastUIShape);
							doRedraw();
						}
						else
						{
							if (m_lastOver!=nullptr)
								onShapeLeave(m_lastOver);

						}
					}
					
					onShapeOver(m_workspace->getSelectedShape());
				}
				else
				{
					if (shape!=m_lastOver)
					{
						// We don't leave from nullptr
						
						if (lastUIShape!=nullptr)
						{
							lastUIShape->doControlLeave(vec);
							onControlLeave(lastUIShape);
							//lastUIShape->doControlUp(vec);
							//onControlUp(lastUIShape);
						}
						else
						{
							if (m_lastOver!=nullptr)
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
	
	IvfDbg2("WorkspaceWidget::doMouseMove: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doMouseUp(int x, int y)
{
	IvfDbg2("WorkspaceWidget::doMouseUp: Start.");
	
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	Vec3d vec;
	ShapePtr shape;
	UIInteractiveBasePtr uiShape;
	UIInteractiveBasePtr lastUIShape;

	switch (m_editMode) {
	case EM_INTERACT:
		m_workspace->pick(x, y);

		shape = m_workspace->getSelectedShape();
		uiShape = queryUIShape(shape);
		lastUIShape = queryUIShape(m_lastOver);

		if ((lastUIShape!=nullptr)&&(lastUIShape!=uiShape))
		{
			lastUIShape->doControlClick(vec, 0);
			onControlClick(lastUIShape);
			lastUIShape->doControlUp(vec);
			onControlUp(lastUIShape);
			doRedraw();
		}

		if (uiShape==nullptr)
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
	
	IvfDbg2("WorkspaceWidget::doMouseUp: End.");
}

// ------------------------------------------------------------
void WorkspaceWidget::doKeyboard(int key, int x, int y)
{
	IvfDbg2("WorkspaceWidget::doKeyboard: Start.");
	
	onKeyboard(key, x, y);
	
	IvfDbg2("WorkspaceWidget::doKeyboard: End.");
}

void WorkspaceWidget::addChild(Shape *shape)
{
	m_workspace->addChild(shape);
}

void WorkspaceWidget::setEditMode(TEditMode mode)
{
	m_editMode = mode;
}

WorkspaceWidget::TEditMode WorkspaceWidget::getEditMode()
{
	return m_editMode;
}

void WorkspaceWidget::onCursor(double x, double y, double z)
{
	
}

Camera* WorkspaceWidget::getCamera()
{
	return m_camera;
}

Workspace* WorkspaceWidget::getWorkspace()
{
	return m_workspace;
}

void WorkspaceWidget::onShapeDown(Shape *shape)
{
	
}

void WorkspaceWidget::onShapeClick(Shape *shape)
{
	
}

void WorkspaceWidget::onShapeUp(Shape *shape)
{
	
}

void WorkspaceWidget::onShapeOver(Shape *shape)
{
	
}

void WorkspaceWidget::onShapeLeave(Shape *shape)
{
	
}

void WorkspaceWidget::enableShapeOver()
{
	m_selectOver = true;
}

void WorkspaceWidget::disableShapeOver()
{
	m_selectOver = false;
}

bool WorkspaceWidget::isShapeOverEnabled()
{
	return m_selectOver;
}

void WorkspaceWidget::onShapeDrag(Shape *shape)
{
	
}

UIInteractiveBasePtr WorkspaceWidget::queryUIShape(Shape *shape)
{
	UIInteractiveBasePtr uiShape;

	if (shape!=nullptr)
	{
		if (shape->isClass("UIInteractiveBase"))
		{
			Shape* s = shape;
			uiShape = (UIInteractiveBase*)s;
		}
	}

	return uiShape;
}

void WorkspaceWidget::onControlOver(UIInteractiveBase *uiControl)
{

}

void WorkspaceWidget::onControlLeave(UIInteractiveBase *uiControl)
{

}

void WorkspaceWidget::onControlDrag(UIInteractiveBase *uiControl)
{

}

void WorkspaceWidget::onControlDown(UIInteractiveBase *uiControl)
{

}

void WorkspaceWidget::onControlClick(UIInteractiveBase *uiControl)
{

}

void WorkspaceWidget::onControlUp(UIInteractiveBase *uiControl)
{

}
