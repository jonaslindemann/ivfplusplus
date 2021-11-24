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

#include <ivfmath/BoundingSphere.h>

using namespace ivf;

BoundingSphere::BoundingSphere()
{
	m_radius = 0.0;
	m_globalPos[0] = 0.0;
	m_globalPos[1] = 0.0;
	m_globalPos[2] = 0.0;
}

BoundingSphere::~BoundingSphere()
{

}

double BoundingSphere::getRadius()
{
	return m_radius;
}

void BoundingSphere::setRadius(double radius)
{
	m_radius = radius;
}

void BoundingSphere::setGlobalPosition(double x, double y, double z)
{
	m_globalPos[0] = x;
	m_globalPos[1] = y;
	m_globalPos[2] = z;
}

void BoundingSphere::getGlobalPosition(double &x, double &y, double &z)
{
	x = m_globalPos[0];
	y = m_globalPos[1];
	z = m_globalPos[2];
}
