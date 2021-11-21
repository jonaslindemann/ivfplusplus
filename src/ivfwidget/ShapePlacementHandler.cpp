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

#include <ivfwidget/ShapePlacementHandler.h>

#include <ivf/Cube.h>

using namespace ivf;

CShapePlacementHandler::CShapePlacementHandler(CWidgetBase* widget, CCamera* camera, CScene* scene, CComposite* composite)
:CCoordinateInputHandler(widget, camera)
{
	m_scene = scene;
	m_composite = composite;
	
	m_cursor = new CNodeCursor();
	m_scene->getPostComposite()->addChild(m_cursor);
	
	m_ruler = new CRuler();
	m_ruler->setStartPoint(0.0, 0.0, 0.0);
	m_ruler->setEndPoint(0.0, 0.0, 0.0);
	m_ruler->setTickStep(0.5);
	
	m_scene->addChild(m_ruler);
	
	m_createShapeEvent = nullptr;
	m_moveSelectionEvent = nullptr;
	m_copyShapeEvent = nullptr;
	m_finishCopyEvent = nullptr;
	m_finishMoveEvent = nullptr;
	m_cursorUpdateEvent = nullptr;
	
	m_moveShape = nullptr;
	
	m_inputMethod = IM_CLICK_MODIFIER;
	m_operatingMode = OM_CREATE_SHAPE;
	m_clickCount = 0;

	m_lastButtonLeft = false;

	m_moving = false;
	m_moveDone = false;
	
	this->addMouseDown3dEvent(this);
	this->addMouseMove3dEvent(this);
	this->addMouseUp3dEvent(this);
}

CShapePlacementHandler::~CShapePlacementHandler()
{
	this->removeMouseDown3dEvent(this);
	this->removeMouseMove3dEvent(this);
	this->removeMouseUp3dEvent(this);
}

// ------------------------------------------------------------
void CShapePlacementHandler::onMouseDown3d(double x, double y, double z)
{
	m_lastButtonLeft = getWidget()->isLeftButtonDown();
	if (!m_moveDone)
	{
		if (getWidget()->isLeftButtonDown())
		{
			m_startPoint.setComponents(x, y, z);

			m_cursor->setPosition(x, y, z);
			m_ruler->setStartPoint(x, 0.0, z);
			m_ruler->setEndPoint(x, y, z);
			getWidget()->redraw();
		}
	}
}

// ------------------------------------------------------------
void CShapePlacementHandler::onMouseMove3d(double x, double y, double z)
{
	if (!m_moveDone)
	{

	double dx, dy, dz;

	m_currentPoint.setComponents(x, y, z);
	m_moveDelta = m_currentPoint - m_startPoint ;
	m_moveDelta.getComponents(dx, dy, dz);

	m_cursor->setPosition(x, y, z);
	m_ruler->setStartPoint(x, 0.0, z);
	m_ruler->setEndPoint(x, y, z);

	if ((m_operatingMode==OM_MOVE_SHAPE)||(m_operatingMode==OM_COPY_SHAPE))
		if (m_shapeSelection!=nullptr)
			m_shapeSelection->setOffset(dx, dy, dz);

	if (m_operatingMode==OM_MOVE_SHAPE)
	{
		if (m_cursorUpdateEvent)
			m_cursorUpdateEvent->onCursorUpdate();
	}
	
	getWidget()->redraw();
	}
}

// ------------------------------------------------------------
void CShapePlacementHandler::onMouseUp3d(double x, double y, double z)
{
	if (m_lastButtonLeft)
	{
		if (m_moveDone)
			return;

		m_cursor->setPosition(x, y, z);
		m_ruler->setStartPoint(x, 0.0, z);
		m_ruler->setEndPoint(x, y, z);
		
		CCubePtr cube;
		CShapePtr node;
		
		switch (m_inputMethod) {
		case IM_CLICK_MODIFIER:
			if (m_operatingMode==OM_CREATE_SHAPE)
			{
				if (m_createShapeEvent!=nullptr)
				{
					CShapePtr newShape;
					m_createShapeEvent->onCreateShapeEvent(x, y, z, newShape);
					
					if (newShape!=nullptr)
					{
						if (m_composite!=nullptr)
							m_composite->addChild(newShape);
						else
							m_scene->addChild(newShape);
					}
				}
			}
			else
			{
				
			}
			break;
		case IM_TWO_CLICK:
			if (m_clickCount==0)
			{
				this->setLockXZ(true);
				m_cursor->setType(CNodeCursor::CT_Y);
				m_clickCount++;
			}
			else
			{
				if (m_operatingMode==OM_CREATE_SHAPE)
				{
					if (m_createShapeEvent!=nullptr)
					{
						CShapePtr newShape;
						m_createShapeEvent->onCreateShapeEvent(x, y, z, newShape);
						
						if (newShape!=nullptr)
						{
							if (m_composite!=nullptr)
								m_composite->addChild(newShape);
							else
								m_scene->addChild(newShape);
						}
					}
				}
				else
				{
					if (m_operatingMode==OM_MOVE_SHAPE)
					{
						if (m_moveSelectionEvent)
						{
							if (m_moveSelectionEvent->onMoveSelection())
							{
								double dx, dy, dz;
								m_moveDelta.getComponents(dx, dy, dz);
								m_shapeSelection->moveShapes(dx, dy, dz);
								setPlanePosition(x, y, z);
							}
						}
						else
						{
							double dx, dy, dz;
							m_moveDelta.getComponents(dx, dy, dz);
							m_shapeSelection->moveShapes(dx, dy, dz);
							m_shapeSelection->setOffset(0.0, 0.0, 0.0);
							setPlanePosition(x, y, z);
						}
						if (m_finishMoveEvent)
							m_finishMoveEvent->onFinishMove();
					}
					else
					{
						if (m_copyShapeEvent!=nullptr)
						{
							int i;
							double dx, dy, dz;
							double nx, ny, nz;

							for (i=0; i<m_shapeSelection->getSize(); i++)
							{
								CShapePtr newShape;
								CShapePtr shape = m_shapeSelection->getShape(i);
								
								shape->getPosition(nx, ny, nz);
								m_moveDelta.getComponents(dx, dy, dz);

								nx += dx;
								ny += dy;
								nz += dz;
								
								m_copyShapeEvent->onCopyShape(nx, ny, nz, shape, newShape);

								if (m_composite!=nullptr)
									m_composite->addChild(newShape);
								else
									m_scene->addChild(newShape);
							}

							if (m_finishCopyEvent!=nullptr)
								m_finishCopyEvent->onFinishCopy();

							setPlanePosition(x, y, z);
						}
					}
					//m_cursor->setPosition(0.0, 0.0, 0.0);
					m_currentPoint.setComponents(x, y, z);
					m_startPoint.setComponents(x, y, z);
					m_shapeSelection->setOffset(0.0, 0.0, 0.0);
					m_moveDone = true;
					m_cursor->setState(CGLBase::OS_OFF);
					m_ruler->setState(CGLBase::OS_OFF);
				}

				m_clickCount = 0;
				this->setLockXZNoEvent(false);
				m_cursor->setType(CNodeCursor::CT_XZ);
			}
			break;
		default:
			
			break;
		}
		getWidget()->redraw();
	}
	m_lastButtonLeft = false;
}

void CShapePlacementHandler::setCreateShapeEvent(CCreateShapeEvent *event)
{
	m_createShapeEvent = event;
}

void CShapePlacementHandler::setMoveSelectionEvent(CMoveSelectionEvent *event)
{
	m_moveSelectionEvent = event;
}

void CShapePlacementHandler::setInputMethod(TInputMethod method)
{
	m_inputMethod = method;
}

CShapePlacementHandler::TInputMethod CShapePlacementHandler::getInputMethod()
{
	return m_inputMethod;
}

void CShapePlacementHandler::setShapeRepresentation(CShape *shape)
{
	m_shapeRepresentation = shape;
	m_cursor->setShape(shape);
}

void CShapePlacementHandler::doActivate()
{
	m_cursor->setState(CGLBase::OS_ON);
}

void CShapePlacementHandler::doDeactivate()
{
	m_cursor->setState(CGLBase::OS_OFF);
	m_clickCount = 0;
	this->setLockXZNoEvent(false);
	m_cursor->setType(CNodeCursor::CT_XZ);
}

void CShapePlacementHandler::doLockXZ(bool flag)
{
	if (flag)
		m_cursor->setType(CNodeCursor::CT_Y);
	else
		m_cursor->setType(CNodeCursor::CT_XZ);
}

CNodeCursor* CShapePlacementHandler::getCursor()
{
	return m_cursor;
}

void CShapePlacementHandler::setMoveShape(CShape *shape)
{
	m_moveShape = shape;
}

void CShapePlacementHandler::setOperatingMode(TOperatingMode mode)
{
	m_operatingMode = mode;

	if ((m_operatingMode==OM_MOVE_SHAPE)||(m_operatingMode==OM_COPY_SHAPE))
		m_cursor->setShape(nullptr);
	else
	{
		m_moveDone = false;
		m_cursor->setShape(m_shapeRepresentation);
	}
}

CShapePlacementHandler::TOperatingMode CShapePlacementHandler::getOperatingMode()
{
	return m_operatingMode;
}

void CShapePlacementHandler::setShapeSelection(CShapeSelection *shapeSelection)
{
	m_shapeSelection = shapeSelection;
}

void CShapePlacementHandler::initiateMove(double x, double y, double z)
{
	m_moveDone = false;
	m_moving = true;
	m_startPoint.setComponents(x, y, z);
	setPlanePosition(x, y, z);
	m_cursor->setPosition(x, y, z);
	m_cursor->setState(CGLBase::OS_ON);
	m_ruler->setState(CGLBase::OS_ON);
}

void CShapePlacementHandler::initiateMove(CVec3d &vec)
{
	m_moveDone = false;
	m_moving = true;
	m_startPoint = vec;
	m_cursor->setPosition(vec);
	m_cursor->setState(CGLBase::OS_ON);
	m_ruler->setState(CGLBase::OS_ON);
	setPlanePosition(vec);
}

void CShapePlacementHandler::finalizeMove()
{
	setPlanePosition(0.0, 0.0, 0.0);
	if (m_shapeSelection!=nullptr)
		m_shapeSelection->setOffset(0.0, 0.0, 0.0);
	m_cursor->setState(CGLBase::OS_OFF);
	//m_ruler->setState(CIvfGLBase::OS_OFF);
	m_moving = false;
}

bool CShapePlacementHandler::isMoving()
{
	return m_moving;
}

void CShapePlacementHandler::setCopyShapeEvent(CCopyShapeEvent *event)
{
	m_copyShapeEvent = event;
}

void CShapePlacementHandler::setFinishCopyEvent(CFinishCopyEvent *event)
{
	m_finishCopyEvent = event;
}

void CShapePlacementHandler::setFinishMoveEvent(CFinishMoveEvent *event)
{
	m_finishMoveEvent = event;
}

void CShapePlacementHandler::setCursorUpdateEvent(CCursorUpdateEvent *event)
{
	m_cursorUpdateEvent = event;
}

void CShapePlacementHandler::reset()
{
	setPlanePosition(0.0, 0.0, 0.0);
	if (m_shapeSelection!=nullptr)
		m_shapeSelection->setOffset(0.0, 0.0, 0.0);
	//m_ruler->setState(CIvfGLBase::OS_OFF);
	if ((m_operatingMode==OM_MOVE_SHAPE)||(m_operatingMode==OM_COPY_SHAPE))
		m_cursor->setShape(nullptr);
	else
	{
		m_moveDone = false;
		m_cursor->setShape(m_shapeRepresentation);
	}

	setLockXZ(false);

	m_clickCount=0;

	m_moving = false;
	m_moveDone = false;
}
