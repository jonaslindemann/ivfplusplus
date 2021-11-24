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

#include <ivfmath/ViewFrustum.h>
#include <ivfmath/BoundingSphere.h>

using namespace ivf;

ViewFrustum::ViewFrustum()
{
	m_near = new Plane();
	m_far = new Plane();
	m_left = new Plane();
	m_right = new Plane();
	m_top = new Plane();
	m_bottom = new Plane();
}

ViewFrustum::~ViewFrustum()
{
	delete m_near;
	delete m_far;
	delete m_left;
	delete m_right;
	delete m_top;
	delete m_bottom;
}

Plane* ViewFrustum::getNearPlane()
{
	return m_near;
}

Plane* ViewFrustum::getFarPlane()
{
	return m_far;
}

Plane* ViewFrustum::getLeftPlane()
{
	return m_left;
}

Plane* ViewFrustum::getRightPlane()
{
	return m_right;
}

Plane* ViewFrustum::getTopPlane()
{
	return m_top;
}

Plane* ViewFrustum::getBottomPlane()
{
	return m_bottom;
}

bool ViewFrustum::isInside(BoundingSphere *bSphere)
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
