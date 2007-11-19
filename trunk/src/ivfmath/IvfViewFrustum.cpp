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

#include <ivfmath/IvfViewFrustum.h>
#include <ivfmath/IvfBoundingSphere.h>

CIvfViewFrustum::CIvfViewFrustum()
{
	m_near = new CIvfPlane();
	m_far = new CIvfPlane();
	m_left = new CIvfPlane();
	m_right = new CIvfPlane();
	m_top = new CIvfPlane();
	m_bottom = new CIvfPlane();
}

CIvfViewFrustum::~CIvfViewFrustum()
{
	delete m_near;
	delete m_far;
	delete m_left;
	delete m_right;
	delete m_top;
	delete m_bottom;
}

CIvfPlane* CIvfViewFrustum::getNearPlane()
{
	return m_near;
}

CIvfPlane* CIvfViewFrustum::getFarPlane()
{
	return m_far;
}

CIvfPlane* CIvfViewFrustum::getLeftPlane()
{
	return m_left;
}

CIvfPlane* CIvfViewFrustum::getRightPlane()
{
	return m_right;
}

CIvfPlane* CIvfViewFrustum::getTopPlane()
{
	return m_top;
}

CIvfPlane* CIvfViewFrustum::getBottomPlane()
{
	return m_bottom;
}

bool CIvfViewFrustum::isInside(CIvfBoundingSphere *bSphere)
{
	double x, y, z, radius;

	radius = bSphere->getRadius();
	bSphere->getGlobalPosition(x, y, z);

	if (m_far->isSphereInFront(x, y, z, radius))
		return false;

	if (m_near->isSphereInFront(x, y, z, radius))
		return false;

	if (m_left->isSphereInFront(x, y, z, radius))
		return false;

	if (m_right->isSphereInFront(x, y, z, radius))
		return false;

	if (m_top->isSphereInFront(x, y, z, radius))
		return false;

	if (m_bottom->isSphereInFront(x, y, z, radius))
		return false;

	return true;
}
