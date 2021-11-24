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

ShapePlacementHandler::ShapePlacementHandler(WidgetBase* widget, Camera* camera, Scene* scene, Composite* composite)
:CoordinateInputHandler(widget, camera)
{
	m_scene = scene;
	m_composite = composite;
	
	m_cursor = new NodeCursor();
	m_scene->getPostComposite()->addChild(m_cursor);
	
	m_ruler = new Ruler();
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

ShapePlacementHandler::~ShapePlacementHandler()
{
	this->removeMouseDown3dEvent(this);
	this->removeMouseMove3dEvent(this);
	this->removeMouseUp3dEvent(this);
}

// ------------------------------------------------------------
void ShapePlacementHandler::onMouseDown3d(double x, double y, double z)
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
void ShapePlacementHandler::onMouseMove3d(double x, double y, double z)
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
void ShapePlacementHandler::onMouseUp3d(double x, double y, double z)
{
	if (m_lastButtonLeft)
	{
		if (m_moveDone)
			return;

		m_cursor->setPosition(x, y, z);
		m_ruler->setStartPoint(x, 0.0, z);
		m_ruler->setEndPoint(x, y, z);
		
		CubePtr cube;
		ShapePtr node;
		
		switch (m_inputMethod) {
		case IM_CLICK_MODIFIER:
			if (m_operatingMode==OM_CREATE_SHAPE)
			{
				if (m_createShapeEvent!=nullptr)
				{
					ShapePtr newShape;
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
				m_cursor->setType(NodeCursor::CT_Y);
				m_clickCount++;
			}
			else
			{
				if (m_operatingMode==OM_CREATE_SHAPE)
				{
					if (m_createShapeEvent!=nullptr)
					{
						ShapePtr newShape;
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
								ShapePtr newShape;
								ShapePtr shape = m_shapeSelection->getShape(i);
								
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
					m_cursor->setState(GLBase::OS_OFF);
					m_ruler->setState(GLBase::OS_OFF);
				}

				m_clickCount = 0;
				this->setLockXZNoEvent(false);
				m_cursor->setType(NodeCursor::CT_XZ);
			}
			break;
		default:
			
			break;
		}
		getWidget()->redraw();
	}
	m_lastButtonLeft = false;
}

void ShapePlacementHandler::setCreateShapeEvent(CreateShapeEvent *event)
{
	m_createShapeEvent = event;
}

void ShapePlacementHandler::setMoveSelectionEvent(MoveSelectionEvent *event)
{
	m_moveSelectionEvent = event;
}

void ShapePlacementHandler::setInputMethod(TInputMethod method)
{
	m_inputMethod = method;
}

ShapePlacementHandler::TInputMethod ShapePlacementHandler::getInputMethod()
{
	return m_inputMethod;
}

void ShapePlacementHandler::setShapeRepresentation(Shape *shape)
{
	m_shapeRepresentation = shape;
	m_cursor->setShape(shape);
}

void ShapePlacementHandler::doActivate()
{
	m_cursor->setState(GLBase::OS_ON);
}

void ShapePlacementHandler::doDeactivate()
{
	m_cursor->setState(GLBase::OS_OFF);
	m_clickCount = 0;
	this->setLockXZNoEvent(false);
	m_cursor->setType(NodeCursor::CT_XZ);
}

void ShapePlacementHandler::doLockXZ(bool flag)
{
	if (flag)
		m_cursor->setType(NodeCursor::CT_Y);
	else
		m_cursor->setType(NodeCursor::CT_XZ);
}

NodeCursor* ShapePlacementHandler::getCursor()
{
	return m_cursor;
}

void ShapePlacementHandler::setMoveShape(Shape *shape)
{
	m_moveShape = shape;
}

void ShapePlacementHandler::setOperatingMode(TOperatingMode mode)
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

ShapePlacementHandler::TOperatingMode ShapePlacementHandler::getOperatingMode()
{
	return m_operatingMode;
}

void ShapePlacementHandler::setShapeSelection(ShapeSelection *shapeSelection)
{
	m_shapeSelection = shapeSelection;
}

void ShapePlacementHandler::initiateMove(double x, double y, double z)
{
	m_moveDone = false;
	m_moving = true;
	m_startPoint.setComponents(x, y, z);
	setPlanePosition(x, y, z);
	m_cursor->setPosition(x, y, z);
	m_cursor->setState(GLBase::OS_ON);
	m_ruler->setState(GLBase::OS_ON);
}

void ShapePlacementHandler::initiateMove(Vec3d &vec)
{
	m_moveDone = false;
	m_moving = true;
	m_startPoint = vec;
	m_cursor->setPosition(vec);
	m_cursor->setState(GLBase::OS_ON);
	m_ruler->setState(GLBase::OS_ON);
	setPlanePosition(vec);
}

void ShapePlacementHandler::finalizeMove()
{
	setPlanePosition(0.0, 0.0, 0.0);
	if (m_shapeSelection!=nullptr)
		m_shapeSelection->setOffset(0.0, 0.0, 0.0);
	m_cursor->setState(GLBase::OS_OFF);
	//m_ruler->setState(CIvfGLBase::OS_OFF);
	m_moving = false;
}

bool ShapePlacementHandler::isMoving()
{
	return m_moving;
}

void ShapePlacementHandler::setCopyShapeEvent(CopyShapeEvent *event)
{
	m_copyShapeEvent = event;
}

void ShapePlacementHandler::setFinishCopyEvent(FinishCopyEvent *event)
{
	m_finishCopyEvent = event;
}

void ShapePlacementHandler::setFinishMoveEvent(FinishMoveEvent *event)
{
	m_finishMoveEvent = event;
}

void ShapePlacementHandler::setCursorUpdateEvent(CursorUpdateEvent *event)
{
	m_cursorUpdateEvent = event;
}

void ShapePlacementHandler::reset()
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
