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

#include <ivfwidget/FlyHandler.h>

using namespace ivf;

FlyHandler::FlyHandler(WidgetBase* widget, Camera* camera)
{
	m_mouseDelta[0] = 0;
	m_mouseDelta[1] = 0;
	m_mouseDownPos[0] = 0;
	m_mouseDownPos[1] = 0;
	m_mouseSensitivity = 0.5;
	
	m_speed = 0.0;
	m_verticalSpeed = 0.0;
	m_horisontalSpeed = 0.0;

	m_speedStep = 0.5;
	m_maxSpeed = 10.0;
	
	m_camera = camera;

	m_widget = widget;
	
	m_firstFrame = true;
	m_doRedraw = true;

	m_widget->addKeyboardEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);
	m_widget->addMouseMoveEvent(this);
}

FlyHandler::~FlyHandler()
{
	m_widget->removeKeyboardEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
	m_widget->removeMouseMoveEvent(this);
}

void FlyHandler::doMouseDown(int x, int y)
{
	m_mouseDownPos[0] = x;
	m_mouseDownPos[1] = y;
}

void FlyHandler::doMouseMove(int x, int y)
{
	if (m_widget!=nullptr)
	{
		if (m_widget->isAnyButtonDown())
		{
			m_mouseDelta[0] = x - m_mouseDownPos[0];
			m_mouseDelta[1] = y - m_mouseDownPos[1];
			doRedraw();
		}
		else
		{
			m_mouseDelta[0] = 0;
			m_mouseDelta[1] = 0;
		}
	}
}

void FlyHandler::doMouseUp(int x, int y)
{
	m_mouseDelta[0] = 0;
	m_mouseDelta[1] = 0;
}

void FlyHandler::doKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'w':
		m_speed += m_speedStep;
		break;
	case 's':
		m_speed -= m_speedStep;
		break;
	case 'x':
		m_speed = 0.0;
		break;
	case 'q':
		m_verticalSpeed += m_speedStep;
		break;
	case 'a':
		m_verticalSpeed -= m_speedStep;
		break;
	case 'z':
		m_verticalSpeed = 0.0;
		break;
	default:
		
		break;
	};

	if (m_speed>0)
	{
		if (m_speed>m_maxSpeed)
			m_speed = m_maxSpeed;
	}
	else
	{
		if (fabs(m_speed)>m_maxSpeed)
			m_speed = -m_maxSpeed;
	}

	if (m_verticalSpeed>0)
	{
		if (m_verticalSpeed>m_maxSpeed)
			m_verticalSpeed = m_maxSpeed;
	}
	else
	{
		if (fabs(m_verticalSpeed)>m_maxSpeed)
			m_verticalSpeed = -m_maxSpeed;
	}

}

void FlyHandler::update()
{
	if (m_widget!=nullptr)
	{
		if (m_firstFrame)
		{
			m_t = m_widget->elapsedTime();
			m_firstFrame = false;
			doRedraw();
			return;
		}
		else
		{
			m_dt = m_widget->elapsedTime()-m_t;
			m_t = m_widget->elapsedTime();
			
			if (m_widget->getModifierKey()==WidgetBase::MT_NONE)
			{
				if (m_widget->isLeftButtonDown())
					m_camera->turn(m_mouseDelta[1]*m_mouseSensitivity*m_dt, m_mouseDelta[0]*m_mouseSensitivity*m_dt, 0.0);
				if (m_widget->isRightButtonDown())
					m_camera->turn(m_mouseDelta[1]*m_mouseSensitivity*m_dt, 0.0, -m_mouseDelta[0]*m_mouseSensitivity*m_dt);
			}
			if (m_widget->getModifierKey()==WidgetBase::MT_SHIFT)
			{
				m_verticalSpeed = -m_mouseDelta[1]*m_mouseSensitivity*m_dt;
				m_horisontalSpeed = -m_mouseDelta[0]*m_mouseSensitivity*m_dt;
			}
			else
			{
				m_verticalSpeed = 0.0;
				m_horisontalSpeed = 0.0;
			}

			m_camera->moveForward(m_speed*m_dt);
			m_camera->moveVertical(m_verticalSpeed*m_dt);
			m_camera->moveSideways(m_horisontalSpeed*m_dt);
			
			doRedraw();
		}
	}
}


void FlyHandler::setMouseSensitivity(double value)
{
	m_mouseSensitivity = value;
}

double FlyHandler::getMouseSensitivity()
{
	return m_mouseSensitivity;
}

void FlyHandler::setMaxSpeed(double speed)
{
	m_maxSpeed = speed;
}

void FlyHandler::setSpeedStep(double step)
{
	m_speedStep = step;
}

double FlyHandler::getSpeedStep()
{
	return m_speedStep;
}

double FlyHandler::getMaxSpeed()
{
	return m_maxSpeed;
}

void FlyHandler::setRedraw(bool flag)
{
	m_doRedraw = flag;
}

void FlyHandler::doRedraw()
{
	if (m_doRedraw)
		m_widget->redraw();
}

void FlyHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void FlyHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void FlyHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void FlyHandler::onKeyboard(int key, int x, int y)
{
	if (isActive())
		doKeyboard(key, x, y);
}

void FlyHandler::doActivate()
{
	if (m_camera!=nullptr)
		m_camera->setType(Camera::CT_FLY);
}

void FlyHandler::doDeactivate()
{
	if (m_camera!=nullptr)
		m_camera->setType(Camera::CT_NORMAL);
}
