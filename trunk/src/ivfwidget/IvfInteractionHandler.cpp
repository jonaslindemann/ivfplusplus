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

#include <ivfwidget/IvfInteractionHandler.h>

#define IvfDispatchEvent(event,eventMethod,shape) \
	if (event!=NULL) \
		event->eventMethod(shape); 

CIvfInteractionHandler::CIvfInteractionHandler(CIvfWidgetBase* widget, CIvfScene* scene)
{
	m_widget = widget;
	m_scene = scene;

	m_shapeDownEvent = NULL;
	m_shapeClickEvent = NULL;
	m_shapeUpEvent = NULL;
	m_shapeEnterEvent = NULL;
	m_shapeOverEvent = NULL;
	m_shapeLeaveEvent = NULL;
	m_shapeDragEvent = NULL;

	m_controlEnterEvent = NULL;
	m_controlOverEvent = NULL;
	m_controlLeaveEvent = NULL;
	m_controlDragEvent = NULL;
	m_controlUpEvent = NULL;
	m_controlClickEvent = NULL;
	m_controlDownEvent = NULL;

	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);

	m_lastPos[0] = -1;
	m_lastPos[1] = -1;
}

CIvfInteractionHandler::~CIvfInteractionHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
}

void CIvfInteractionHandler::doMouseUp(int x, int y)
{
	CIvfVec3d vec;

	vec = m_scene->getCamera()->pickVector(x, y);

	if (m_currentShape!=NULL)
	{
		IvfDispatchEvent(m_shapeUpEvent,onShapeUp,m_currentShape);
		IvfDispatchEvent(m_shapeClickEvent,onShapeClick,m_currentShape);
	}

	if (m_currentControl!=NULL)
	{
		m_currentControl->doControlUp(vec);
		IvfDispatchEvent(m_controlUpEvent,onControlUp,m_currentControl);
		m_currentControl->doControlClick(vec, 0);
		IvfDispatchEvent(m_controlClickEvent,onControlClick,m_currentControl);

		m_widget->redraw();
	}

	m_lastPos[0] = x;
	m_lastPos[1] = y;
}

void CIvfInteractionHandler::doMouseMove(int x, int y)
{
	CIvfVec3d vec;

	vec = m_scene->getCamera()->pickVector(x, y);

	if ((m_lastPos[0]!=x)||(m_lastPos[1]!=y))
	{
		if ((m_shapeLeaveEvent!=NULL)||
			(m_shapeEnterEvent!=NULL)||
			(m_shapeOverEvent!=NULL)||
			(m_controlLeaveEvent!=NULL)||
			(m_controlEnterEvent!=NULL)||
			(m_controlOverEvent!=NULL))
		{
			m_scene->pick(x, y);
			
			m_currentShape = m_scene->getSelectedShape();

			if (m_currentShape!=NULL)
			{
				if (m_currentShape->isClass("CIvfUIInteractiveBase"))
				{
					CIvfShape* temp = m_currentShape;
					m_currentControl = (CIvfUIInteractiveBase*)temp;
					m_currentShape = NULL;
					m_lastShape = NULL;
				}
			}
			else
				m_currentControl = NULL;

			if (m_currentControl!=m_lastControl)
			{
				if (m_lastControl!=NULL)
				{
					m_lastControl->doControlLeave(vec);
					IvfDispatchEvent(m_controlLeaveEvent,onControlLeave, m_lastControl);
				}
				
				if (m_currentControl!=NULL)
					IvfDispatchEvent(m_controlEnterEvent, onControlEnter, m_currentControl);
			}
			
			if (m_currentShape!=m_lastShape)
			{
				if (m_lastShape!=NULL)
					IvfDispatchEvent(m_shapeLeaveEvent,onShapeLeave,m_lastShape);
				
				if (m_currentShape!=NULL)
					IvfDispatchEvent(m_shapeEnterEvent,onShapeEnter,m_currentShape);
			}

			if (m_currentControl!=NULL)
			{
				m_currentControl->doControlOver(vec);
				IvfDispatchEvent(m_controlOverEvent, onControlOver, m_currentControl);
			}
			
			if (m_currentShape!=NULL)
			{
				IvfDispatchEvent(m_shapeOverEvent, onShapeOver, m_currentShape);
			}

			//if ((m_currentControl!=NULL)||(m_lastControl!=NULL))
				m_widget->redraw();
			
			m_lastShape = m_currentShape;
			m_lastControl = m_currentControl;
		}
	}

	m_lastPos[0] = x;
	m_lastPos[1] = y;
}

void CIvfInteractionHandler::doMouseDown(int x, int y)
{
	m_lastPos[0] = x;
	m_lastPos[1] = y;
	m_scene->pick(x, y);

	CIvfVec3d vec;

	vec = m_scene->getCamera()->pickVector(x, y);

	m_currentShape = m_scene->getSelectedShape();

	if (m_currentShape!=NULL)
	{
		if (m_currentShape->isClass("CIvfUIInteractiveBase"))
		{
			CIvfShape* temp = m_currentShape;
			m_currentControl = (CIvfUIInteractiveBase*)temp;
			m_currentShape = NULL;
			m_lastShape = NULL;
		}
	}
	else
		m_currentControl = NULL;

	if (m_currentShape!=NULL)
		IvfDispatchEvent(m_shapeDownEvent,onShapeDown,m_currentShape);

	if (m_currentControl!=NULL)
	{
		m_currentControl->doControlDown(vec, 0);
		IvfDispatchEvent(m_controlDownEvent,onControlDown,m_currentControl);
		m_widget->redraw();
	}
}

void CIvfInteractionHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void CIvfInteractionHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void CIvfInteractionHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}



void CIvfInteractionHandler::setShapeDownEvent(CIvfShapeDownEvent *event)
{
	m_shapeDownEvent = event;
}

void CIvfInteractionHandler::setShapeUpEvent(CIvfShapeUpEvent *event)
{
	m_shapeUpEvent = event;
}

void CIvfInteractionHandler::setShapeClickEvent(CIvfShapeClickEvent *event)
{	
	m_shapeClickEvent = event;
}

void CIvfInteractionHandler::setShapeOverEvent(CIvfShapeOverEvent *event)
{
	m_shapeOverEvent = event;
}

void CIvfInteractionHandler::setShapeLeaveEvent(CIvfShapeLeaveEvent *event)
{
	m_shapeLeaveEvent = event;
}

void CIvfInteractionHandler::setShapeDragEvent(CIvfShapeDragEvent *event)
{
	m_shapeDragEvent = event;
}

void CIvfInteractionHandler::setControlDownEvent(CIvfControlDownEvent *event)
{
	m_controlDownEvent = event;
}

void CIvfInteractionHandler::setControlUpEvent(CIvfControlUpEvent *event)
{
	m_controlUpEvent = event;
}

void CIvfInteractionHandler::setControlClickEvent(CIvfControlClickEvent *event)
{
	m_controlClickEvent = event;
}

void CIvfInteractionHandler::setControlOverEvent(CIvfControlOverEvent *event)
{
	m_controlOverEvent = event;
}

void CIvfInteractionHandler::setControlLeaveEvent(CIvfControlLeaveEvent *event)
{
	m_controlLeaveEvent = event;
}

void CIvfInteractionHandler::setControlDragEvent(CIvfControlDragEvent *event)
{
	m_controlDragEvent = event;
}

void CIvfInteractionHandler::setShapeEnterEvent(CIvfShapeEnterEvent *event)
{
	m_shapeEnterEvent = event;
}

void CIvfInteractionHandler::setControlEnterEvent(CIvfControlEnterEvent *event)
{
	m_controlEnterEvent = event;
}
