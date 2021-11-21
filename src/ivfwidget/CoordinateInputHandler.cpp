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

#include <ivfwidget/CoordinateInputHandler.h>

using namespace ivf;

CCoordinateInputHandler::CCoordinateInputHandler(CWidgetBase* widget, CCamera* camera)
{
	m_widget = widget;
	m_camera = camera;

	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);

	m_lockXZ = false;
	m_snap = true;

	m_ucsXZ = new CUcs3d();
	m_ucsXZ->setSnapUnit(0.5);
	m_ucsXY = new CUcs3d();
	m_ucsXY->setSnapUnit(0.5);
	m_ucsXY->setRotation(1.0, 0.0, 0.0, 90.0);
}

CCoordinateInputHandler::~CCoordinateInputHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
}

void CCoordinateInputHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void CCoordinateInputHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void CCoordinateInputHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}


void CCoordinateInputHandler::doMouseDown(int x, int y)
{
	m_mousePos[0] = x;
	m_mousePos[1] = y;

	double vx, vy, vz;
	m_intersection = calcIntersection(x, y);
	m_intersection.getComponents(vx, vy, vz);

	dispatchMouseDown3dEvent(vx, vy, vz);
}

void CCoordinateInputHandler::doMouseMove(int x, int y)
{
	m_mousePos[0] = x;
	m_mousePos[1] = y;

	double vx, vy, vz;
	m_intersection = calcIntersection(x, y);
	m_intersection.getComponents(vx, vy, vz);
	
	dispatchMouseMove3dEvent(vx, vy, vz);
}

void CCoordinateInputHandler::doMouseUp(int x, int y)
{
	m_mousePos[0] = x;
	m_mousePos[1] = y;

	double vx, vy, vz;
	m_intersection = calcIntersection(x, y);
	m_intersection.getComponents(vx, vy, vz);

	dispatchMouseUp3dEvent(vx, vy, vz);
}

void CCoordinateInputHandler::addMouseDown3dEvent(CMouseDown3dEvent *event)
{
	m_mouseDown3dEvents.push_back(event);
}

void CCoordinateInputHandler::addMouseMove3dEvent(CMouseMove3dEvent *event)
{
	m_mouseMove3dEvents.push_back(event);
}

void CCoordinateInputHandler::addMouseUp3dEvent(CMouseUp3dEvent *event)
{
	m_mouseUp3dEvents.push_back(event);
}

CVec3d& CCoordinateInputHandler::calcIntersection(int x, int y)
{
	if (!m_lockXZ)
	{
		CVec3d forward = m_camera->pickVector(x, y);
		CVec3d eye = m_camera->getPosition();
		m_temp = m_ucsXZ->intersect(eye, forward);
		
		if (m_snap)
			m_temp = m_ucsXZ->snap(m_temp);

		m_xzIntersection = m_temp;

		return m_temp;
	}
	else
	{
		double x1, y1, z1;
		double x2, y2, z2;
		CVec3d forward = m_camera->pickVector(x, y);
		CVec3d eye = m_camera->getPosition();
		m_intersection.getComponents(x1, y1, z1);
		m_ucsXY->setTranslation(x1, y1, z1);
		m_temp = m_ucsXY->intersect(eye, forward);
		m_temp.getComponents(x2, y2, z2);
		m_temp.setComponents(x1, y2, z1);

		if (m_snap)
			m_temp = m_ucsXY->snap(m_temp);

		return m_temp;
	}
}

void CCoordinateInputHandler::setLockXZ(bool flag)
{
	m_lockXZ = flag;
	
	if (!m_lockXZ)
	{
		double x, y, z;
		m_xzIntersection.getComponents(x, y, z);
		dispatchMouseMove3dEvent(x, y, z);
	}

	doLockXZ(flag);
}

bool CCoordinateInputHandler::getLockXZ()
{
	return m_lockXZ;
}

CWidgetBase* CCoordinateInputHandler::getWidget()
{
	return m_widget;
}

CCamera* CCoordinateInputHandler::getCamera()
{
	return m_camera;
}


void CCoordinateInputHandler::doLockXZ(bool flag)
{

}

void CCoordinateInputHandler::setPlanePosition(CVec3d& vec)
{
	double x, y, z;

	vec.getComponents(x, y, z);
	m_ucsXZ->setTranslation(x, y, z);
	m_ucsXY->setTranslation(x, y, z);
	calcIntersection(m_mousePos[0],m_mousePos[1]);
}

void CCoordinateInputHandler::setPlanePosition(double x, double y, double z)
{	
	m_ucsXZ->setTranslation(x, y, z);
	m_ucsXY->setTranslation(x, y, z);
	calcIntersection(m_mousePos[0],m_mousePos[1]);
}

void CCoordinateInputHandler::setLockXZNoEvent(bool flag)
{
	m_lockXZ = flag;
	calcIntersection(m_mousePos[0],m_mousePos[1]);
	doLockXZ(flag);
}

void CCoordinateInputHandler::dispatchMouseDown3dEvent(double x, double y, double z)
{
	std::vector<CMouseDown3dEvent*>::iterator it;

	for (it=m_mouseDown3dEvents.begin(); it!=m_mouseDown3dEvents.end(); it++)
		(*it)->onMouseDown3d(x, y, z);
}

void CCoordinateInputHandler::dispatchMouseMove3dEvent(double x, double y, double z)
{
	std::vector<CMouseMove3dEvent*>::iterator it;

	for (it=m_mouseMove3dEvents.begin(); it!=m_mouseMove3dEvents.end(); it++)
		(*it)->onMouseMove3d(x, y, z);
}

void CCoordinateInputHandler::dispatchMouseUp3dEvent(double x, double y, double z)
{
	std::vector<CMouseUp3dEvent*>::iterator it;

	for (it=m_mouseUp3dEvents.begin(); it!=m_mouseUp3dEvents.end(); it++)
		(*it)->onMouseUp3d(x, y, z);
}

void CCoordinateInputHandler::removeMouseDown3dEvent(CMouseDown3dEvent *event)
{
	std::vector<CMouseDown3dEvent*>::iterator it;

	for (it=m_mouseDown3dEvents.begin(); it!=m_mouseDown3dEvents.end(); it++)
	{
		if ((*it)==event)
		{
			m_mouseDown3dEvents.erase(it);
			return;
		}
	}
}

void CCoordinateInputHandler::removeMouseMove3dEvent(CMouseMove3dEvent *event)
{
	std::vector<CMouseMove3dEvent*>::iterator it;

	for (it=m_mouseMove3dEvents.begin(); it!=m_mouseMove3dEvents.end(); it++)
	{
		if ((*it)==event)
		{
			m_mouseMove3dEvents.erase(it);
			return;
		}
	}
}

void CCoordinateInputHandler::removeMouseUp3dEvent(CMouseUp3dEvent *event)
{
	std::vector<CMouseUp3dEvent*>::iterator it;

	for (it=m_mouseUp3dEvents.begin(); it!=m_mouseUp3dEvents.end(); it++)
	{
		if ((*it)==event)
		{
			m_mouseUp3dEvents.erase(it);
			return;
		}
	}
}
