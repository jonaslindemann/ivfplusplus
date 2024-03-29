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

#include <ivfwidget/MouseViewHandler.h>

using namespace ivf;

MouseViewHandler::MouseViewHandler(WidgetBase* widget, Camera* camera)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
	m_width = 1;
	m_height = 1;

	m_panScalefactor = 1.0/100.0;
	m_zoomScalefactor = 1.0/50.0;
	m_rotateScalefactor = 1.0/100.0;

	m_autoScaleBias = 1.0;

	m_autoScale = true;

	m_camera = camera;
	m_widget = widget;

	m_keyHandling = KH_NORMAL;

	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);
	m_widget->addResizeEvent(this);
}

MouseViewHandler::~MouseViewHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
	m_widget->removeResizeEvent(this);
}

void MouseViewHandler::doMouseDown(int x, int y)
{
	m_beginX = x;
	m_beginY = y;
}

void MouseViewHandler::doMouseMove(int x, int y)
{
	if (m_widget!=nullptr)
	{
		m_angleX = 0.0;
		m_angleY = 0.0;
		m_moveX = 0.0;
		m_moveY = 0.0;
		m_zoomX = 0.0;
		m_zoomY = 0.0;
		
		if (isRotating())
		{
			m_angleX = (x - m_beginX);
			m_angleY = (y - m_beginY);
			m_beginX = x;
			m_beginY = y;
			
			m_camera->rotatePositionY(m_angleX*m_rotateScalefactor);
			m_camera->rotatePositionX(m_angleY*m_rotateScalefactor);

			if (m_autoScale)
				calcAutoScale();

			m_widget->redraw();
		}

		if (isZoomOrPan())
		{
			if (isZoom())
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
			
			m_camera->moveSideways(m_moveX*m_panScalefactor);
			m_camera->moveVertical(m_moveY*m_panScalefactor);
			m_camera->moveDepth(m_zoomY*m_zoomScalefactor);
			
			if (m_autoScale)
				calcAutoScale();

			m_widget->redraw();
		}

	}
}

void MouseViewHandler::doMouseUp(int x, int y)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
}

void MouseViewHandler::setPanScalefactor(double factor)
{
	m_panScalefactor = factor;
}

void MouseViewHandler::setZoomScalefactor(double factor)
{
	m_zoomScalefactor = factor;
}

double MouseViewHandler::getPanScalefactor()
{
	return m_panScalefactor;
}

double MouseViewHandler::getZoomScalefactor()
{
	return m_zoomScalefactor;
}

void MouseViewHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void MouseViewHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void MouseViewHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void MouseViewHandler::onResize(int width, int height)
{
	m_width = width;
	m_height = height;
	
	if (isActive())
		doResize(width, height);
}

void MouseViewHandler::doResize(int width, int height)
{
	if (m_autoScale)
	{
		calcAutoScale();
	}
}

void MouseViewHandler::setAutoScale(bool flag)
{
	m_autoScale = flag;
}

bool MouseViewHandler::getAutoScale()
{
	return m_autoScale;
}

void MouseViewHandler::setAutoScaleBias(double bias)
{
	m_autoScaleBias = bias;
}

double MouseViewHandler::getAutoScaleBias()
{
	return m_autoScaleBias;
}

void MouseViewHandler::calcAutoScale()
{
	Vec3d target = m_camera->getTarget();
	Vec3d eye = m_camera->getPosition();
	Vec3d eyeTarget = target-eye;

	m_panScalefactor = m_autoScaleBias*eyeTarget.length()/m_width;
	m_zoomScalefactor = m_autoScaleBias*eyeTarget.length()*m_autoScaleBias/m_width;
	m_rotateScalefactor = 4.0/m_width;
}

void MouseViewHandler::setKeyHandling(TKeyHandling keyHandling)
{
	m_keyHandling = keyHandling;
}

MouseViewHandler::TKeyHandling MouseViewHandler::getKeyHandling()
{
	return m_keyHandling;
}

bool MouseViewHandler::isRotating()
{
	switch (m_keyHandling) {
	case KH_NORMAL:
		return m_widget->isLeftButtonDown();
	case KH_MIDDLE_BUTTON:
		return (m_widget->isMiddleButtonDown() && 
			((m_widget->getModifierKey()!=WidgetBase::MT_SHIFT)
			 &&(m_widget->getModifierKey()!=WidgetBase::MT_CTRL)));
	default:
		return false;
	}
}

bool MouseViewHandler::isZoom()
{
	switch (m_keyHandling) {
	case KH_NORMAL:
		return m_widget->isRightButtonDown()&&
			(m_widget->getModifierKey()==WidgetBase::MT_SHIFT);
	case KH_MIDDLE_BUTTON:
		return (m_widget->isMiddleButtonDown() && 
			((m_widget->getModifierKey()==WidgetBase::MT_SHIFT)
			 &&(m_widget->getModifierKey()!=WidgetBase::MT_CTRL)));
	default:
		return false;
	}
}

bool MouseViewHandler::isPan()
{
	switch (m_keyHandling) {
	case KH_NORMAL:
		return m_widget->isRightButtonDown();
	case KH_MIDDLE_BUTTON:
		return (m_widget->isMiddleButtonDown() && 
			((m_widget->getModifierKey()!=WidgetBase::MT_SHIFT)
			 &&(m_widget->getModifierKey()==WidgetBase::MT_CTRL)));
	default:
		return false;
	}
}

bool MouseViewHandler::isZoomOrPan()
{
	return isZoom()||isPan();
}
