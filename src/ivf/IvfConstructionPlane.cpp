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

#include <ivf/IvfConstructionPlane.h>

CIvfConstructionPlane::CIvfConstructionPlane()
{
	m_ucs = new CIvfUcs3d();
	m_cursor = new CIvfCursor();
	m_cursor->addReference();
	m_grid = new CIvfGrid();
	m_grid->addReference();

	m_size[0] = 10.0;
	m_size[1] = 10.0;
	m_cursorLocked = false;
	m_relativeCursorSize = 0.1;
	m_relativeAxisSize = 0.05;
	m_snapToGrid = true;
	m_gridSpacing = 0.2;
	m_camera = NULL;

	m_active = true;

	m_updateCursorSize = true;
	m_updateAxisSize = true;

	initPlane();
}

CIvfConstructionPlane::~CIvfConstructionPlane()
{
	delete m_ucs;

	m_camera->deleteReference();
	if (!m_camera->referenced())
		delete m_camera;

	m_cursor->deleteReference();
	if (!m_cursor->referenced())
		delete m_cursor;

	m_grid->deleteReference();
	if (!m_grid->referenced())
		delete m_grid;
}

void CIvfConstructionPlane::setSize(double width, double height)
{
	m_size[0] = width;
	m_size[1] = height;
	initPlane();
}

void CIvfConstructionPlane::initPlane()
{
	m_grid->setSize(m_size[0], m_size[1]);
	m_grid->setGridSpacing(m_gridSpacing);
	m_grid->refresh();
	m_ucs->setSnapUnit(m_gridSpacing);
	m_cursor->setSize(m_size[0]*0.1);
	this->getRotationQuat(m_v[0], m_v[1], m_v[2], m_theta);
}

void CIvfConstructionPlane::setRotationQuat(double vx, double vy, double vz, double theta)
{
	m_ucs->setRotation(vx, vy, vz, theta);
	m_grid->setRotationQuat(vx, vy, vz, theta);
	m_v[0] = vx;
	m_v[1] = vy;
	m_v[2] = vz;
	m_theta = theta;
}

void CIvfConstructionPlane::setPosition(double x, double y, double z)
{
	m_grid->setPosition(x, y, z);
	m_ucs->setTranslation(x, y, z);
}

void CIvfConstructionPlane::updateCursor(int x, int y)
{
	if (m_camera!=NULL)
	{

		CIvfVec3d pickVector;
		CIvfVec3d position;
		CIvfVec3d intersection;

		if (!m_cursorLocked)
		{
			// Calculate pick vector

			pickVector = m_camera->pickVector(x, y);

			// Get camera position

			position = m_camera->getPosition();

			// Calculate intersection with XZ plane

			intersection = m_ucs->intersect(position, pickVector);

			// Align cursor with coordinate system

			m_cursor->setRotationQuat(m_v[0], m_v[1], m_v[2], m_theta);

			// Snap to transformed system

			intersection = m_ucs->transform(intersection);

			if (m_snapToGrid)
				intersection = m_ucs->snap(intersection);

			m_last = intersection;

			intersection = m_ucs->transformWorld(intersection);

			m_cursor->setPosition(intersection);


		}
		else
		{
			// Calculate pick vector

			pickVector = m_camera->pickVector(x, y);

			// Get camera position

			position = m_camera->getPosition();

			// Calculate intersection with XZ plane

			intersection = m_ucs->intersect(position, pickVector);

			// Snap to transformed system

			intersection = m_ucs->transform(intersection);

			if (m_snapToGrid)
				intersection = m_ucs->snap(intersection);

			CIvfVec3d delta;
			delta = m_last - intersection;

			double sign;
			if ( (m_cursorY-m_cursorStartY)>0 )
				sign = -1.0;
			else
				sign = 1.0;

			CIvfVec3d wlast;
			wlast = m_ucs->transformWorld(m_last);
			CIvfVec3d newPos;
			newPos = wlast + sign*delta.length()*m_ucs->getYAxis();

			//m_ucs->transformWorld(newPos);
			m_cursor->setPosition(newPos);


			/*
			// Calculate y movement
			double sign;

			if ( (m_cursorY-m_cursorStartY)>0 )
				sign = -1.0;
			else
				sign = 1.0;

			dist = sign*sqrt(pow(m_cursorX-m_cursorStartX,2) + pow(m_cursorY - m_cursorStartY,2));
			camera->getViewPort(viewWidth, viewHeight);
			dy = 10*m_objectSize * (dist / sqrt(pow(viewHeight,2) + pow(viewWidth,2)));

			// move cursor in transformed y direction

			m_cursor->getPosition(ix, iy, iz);
			m_world->transform(ix, iy, iz, ix, iy, iz);
			iy = dy;

			if (m_snapToGrid)
				m_world->snap(ix, iy, iz);

			m_world->transformWorld(ix, iy, iz, ix, iy, iz);
			m_cursor->setPosition(ix, iy, iz);
			*/
		}

		m_cursorX = x;
		m_cursorY = y;
	}
}


void CIvfConstructionPlane::setCamera(CIvfCamera *camera)
{
	if (m_camera!=NULL)
	{
		m_camera->deleteReference();
		if (!m_camera->referenced())
			delete m_camera;
	}

	m_camera = camera;
	m_camera->addReference();
}

void CIvfConstructionPlane::setGridSpacing(double spacing)
{
	m_gridSpacing = spacing;
	initPlane();
}

CIvfUcs3d* CIvfConstructionPlane::getUcs()
{
	return m_ucs;
}

void CIvfConstructionPlane::doCreateGeometry()
{
	m_grid->render();
	if (m_active) m_cursor->render();
}

void CIvfConstructionPlane::setCursor(CIvfCursor *cursor)
{
	m_cursor->deleteReference();
	if (!m_cursor->referenced())
		delete m_cursor;

	m_cursor = cursor;
	m_cursor->addReference();
}

void CIvfConstructionPlane::lockCursor()
{
	m_cursorLocked = true;
	m_cursorStartX = m_cursorX;
	m_cursorStartY = m_cursorY;
}

bool CIvfConstructionPlane::isCursorLocked()
{
	return m_cursorLocked;
}

void CIvfConstructionPlane::unlockCursor()
{
	m_cursorLocked = false;
}

void CIvfConstructionPlane::activate()
{
	m_active = true;
	m_grid->setUseAxis(true);
	m_grid->setUseGrid(true);
}

void CIvfConstructionPlane::deactivate()
{
	m_active = false;
	m_grid->setUseAxis(false);
	m_grid->setUseGrid(false);
}

bool CIvfConstructionPlane::isActive()
{
	return m_active;
}

void CIvfConstructionPlane::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=NULL)
	{
		m_grid->initBoundingSphere();
		getBoundingSphere()->setRadius(m_grid->getBoundingSphere()->getRadius());
	}
}

void CIvfConstructionPlane::getPosition(double &x, double &y, double &z)
{
	m_grid->getPosition(x, y, z);
}

CIvfVec3d& CIvfConstructionPlane::getCursorPosition()
{
	CIvfVec3d& pos = ivfGetTempVec3d();
	pos = m_cursor->getPosition();
	return pos;
}

void CIvfConstructionPlane::setUpdateCursorSize(bool flag)
{
	m_updateCursorSize = true;
}

void CIvfConstructionPlane::updateCursorSize()
{
	if (m_camera!=NULL)
	{
		if (m_updateCursorSize)
		{
			CIvfVec3d cameraPos;
			CIvfVec3d cursorPos;
			CIvfVec3d delta;

			cameraPos = m_camera->getPosition();
			cursorPos = m_cursor->getPosition();
			delta = cursorPos - cameraPos;

			m_cursor->setSize(delta.length()*0.1);
		}
	}
}

void CIvfConstructionPlane::setRelativeCursorSize(double size)
{
	m_relativeCursorSize = size;
}

double CIvfConstructionPlane::getRelativeCursorSize()
{
	return m_relativeCursorSize;
}

void CIvfConstructionPlane::updateAxisSize()
{
	if (m_camera!=NULL)
	{
		if (m_updateAxisSize)
		{
			CIvfVec3d cameraPos;
			CIvfVec3d axisPos;
			CIvfVec3d delta;

			cameraPos = m_camera->getPosition();
			axisPos = m_grid->getPosition();
			delta = axisPos - cameraPos;

			m_grid->setAxisSize(delta.length()*0.1);
		}
	}
}

void CIvfConstructionPlane::updateSizes()
{
	updateCursorSize();
	updateAxisSize();
}

void CIvfConstructionPlane::setRelativeAxisSize(double size)
{
	m_relativeAxisSize = size;
}

double CIvfConstructionPlane::getRelativeAxisSize()
{
	return m_relativeAxisSize;
}

void CIvfConstructionPlane::setUpdateAxisSize(bool flag)
{
	m_updateAxisSize = flag;
}

double CIvfConstructionPlane::getWidth()
{
	return m_size[0];
}

double CIvfConstructionPlane::getDepth()
{
	return m_size[1];
}

void CIvfConstructionPlane::disableCursor()
{
	m_cursor->setState(OS_OFF);
}

void CIvfConstructionPlane::enableCursor()
{
	m_cursor->setState(OS_ON);
}

bool CIvfConstructionPlane::isCursorEnabled()
{
	return (m_cursor->getState()==OS_ON);
}

CIvfGrid* CIvfConstructionPlane::getGrid()
{
	return m_grid;
}
