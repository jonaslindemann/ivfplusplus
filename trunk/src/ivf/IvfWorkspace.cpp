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

#include <ivf/IvfWorkspace.h>

CIvfWorkspace::CIvfWorkspace()
{
	m_rootPlane = new CIvfConstructionPlane();
	m_rootPlane->setSize(10.0, 10.0);

	if (this->getView()->isClass("CIvfCamera"))
		m_rootPlane->setCamera((CIvfCamera*)this->getView());

	m_workplanes.push_back(m_rootPlane);

	m_currentPlane = m_rootPlane;
	m_currentPlaneIdx = 0;
	m_usePlanes = true;
}

CIvfWorkspace::~CIvfWorkspace()
{
	clearPlanes();
	delete m_rootPlane;
}

void CIvfWorkspace::addPlane(CIvfConstructionPlane *plane)
{
	m_workplanes.push_back(plane);
	if (this->getView()->isClass("CIvfCamera"))
		plane->setCamera((CIvfCamera*)this->getView());
	plane->addReference();
}

void CIvfWorkspace::clearPlanes()
{
	int i;

	for (i=0; i<(int)m_workplanes.size(); i++)
	{
		if (i!=0)
		{
			CIvfConstructionPlane* plane = m_workplanes[i];
			plane->deleteReference();

			if (!plane->referenced())
				delete plane;
		}
	}
	m_workplanes.clear();
	m_workplanes.push_back(m_rootPlane);
}

void CIvfWorkspace::setCurrentPlane(int idx)
{
	if ((idx>=0)&&(idx<(int)m_workplanes.size()))
	{
		m_currentPlane->deactivate();
		m_currentPlane->unlockCursor();
		m_currentPlane = m_workplanes[idx];
		m_currentPlane->activate();
		m_currentPlaneIdx = 0;
	}
}

void CIvfWorkspace::nextPlane()
{
	m_currentPlane->deactivate();
	m_currentPlane->unlockCursor();

	if (m_currentPlaneIdx<(int)m_workplanes.size()-1)
		m_currentPlaneIdx++;
	else
		m_currentPlaneIdx = 0;

	m_currentPlane = m_workplanes[m_currentPlaneIdx];
	m_currentPlane->activate();
}

CIvfConstructionPlane* CIvfWorkspace::getCurrentPlane()
{
	return m_currentPlane;
}

void CIvfWorkspace::prevPlane()
{
	m_currentPlane->deactivate();
	m_currentPlane->unlockCursor();

	if (m_currentPlaneIdx>0)
		m_currentPlaneIdx--;
	else
		m_currentPlaneIdx = m_workplanes.size()-1;

	m_currentPlane = m_workplanes[m_currentPlaneIdx];
	m_currentPlane->activate();
}

void CIvfWorkspace::updateCursor(int x, int y)
{
	m_currentPlane->updateCursor(x, y);
}

void CIvfWorkspace::doCreateGeometry()
{
	CIvfSceneBase::doCreateGeometry();

	if (m_usePlanes)
	{
		m_rootPlane->render();
        
		int i;
        
		for (i=0; i<(int)m_workplanes.size(); i++)
			m_workplanes[i]->render();
	}
}

bool CIvfWorkspace::isCursorLocked()
{
	return m_currentPlane->isCursorLocked();
}

void CIvfWorkspace::lockCursor()
{
	m_currentPlane->lockCursor();
}

void CIvfWorkspace::unlockCursor()
{
	m_currentPlane->unlockCursor();
}

CIvfVec3d& CIvfWorkspace::getCursorPosition()
{
	CIvfVec3d& r = ivfGetTempVec3d();
	r = m_currentPlane->getCursorPosition();
	return r;
}

void CIvfWorkspace::setView(CIvfView *view)
{
	CIvfSceneBase::setView(view);
	if (view->isClass("CIvfCamera"))
		m_currentPlane->setCamera((CIvfCamera*)view);
}

void CIvfWorkspace::setRelativeCursorSize(double size)
{
	int i;

	for (i=0; i<(int)m_workplanes.size(); i++)
		m_workplanes[i]->setRelativeCursorSize(size);
}

void CIvfWorkspace::setWorkspaceSize(double size)
{
	m_rootPlane->setSize(size, size);
}

void CIvfWorkspace::updateSizes()
{
	m_currentPlane->updateSizes();
}

void CIvfWorkspace::setRelativeAxisSize(double size)
{
	int i;

	for (i=0; i<(int)m_workplanes.size(); i++)
		m_workplanes[i]->setRelativeAxisSize(size);
}

void CIvfWorkspace::setUsePlanes(bool flag)
{
	m_usePlanes = flag;
}

bool CIvfWorkspace::getUsePlanes()
{
	return m_usePlanes;
}

void CIvfWorkspace::setUseCursor(bool flag)
{
	m_useCursor = flag;
}

bool CIvfWorkspace::getUseCursor()
{
	return m_useCursor;
}

double CIvfWorkspace::getWorkspaceSize()
{
	return m_rootPlane->getWidth();
}

void CIvfWorkspace::setSize(double size)
{
	m_rootPlane->setSize(size, size);
}

double CIvfWorkspace::getSize()
{
	return m_rootPlane->getWidth();
}


void CIvfWorkspace::enableCursor()
{
	getCurrentPlane()->enableCursor();
}

void CIvfWorkspace::disableCursor()
{
	getCurrentPlane()->disableCursor();
}
