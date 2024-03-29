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

#include <ivf/config.h>

#include <ivfmath/Plane.h>

using namespace ivf;

Plane::Plane()
{
	m_normal[0] = 0.0;
	m_normal[1] = 0.0;
	m_normal[2] = 1.0;

	m_distance = 0.0;
}

Plane::~Plane()
{

}

void Plane::setNormal(double vx, double vy, double vz)
{
	double l;

	l = sqrt(pow(vx,2)+pow(vy,2)+pow(vz,2));

	m_normal[0] = vx/l;
	m_normal[1] = vy/l;
	m_normal[2] = vz/l;
}

void Plane::getNormal(double &vx, double &vy, double &vz)
{
	vx = m_normal[0];
	vy = m_normal[1];
	vz = m_normal[2];
}

void Plane::setDistance(double d)
{
	m_distance = d;
}

double Plane::getDistance()
{
	return m_distance;
}

bool Plane::isInFront(double x, double y, double z)
{
	if ( (m_normal[0]*x + 
		m_normal[1]*y + 
		m_normal[2]*z + m_distance)>0)
		return true;
	else
		return false;
}

bool Plane::isSphereInFront(double x, double y, double z, double radius)
{
	double distanceFromPlane;

	distanceFromPlane = 
		m_normal[0]*x + 
		m_normal[1]*y + 
		m_normal[2]*z + 
		m_distance;

	if (distanceFromPlane>0)
	{
		if ((distanceFromPlane-radius)>0)
			return true;
		else
			return false;
	}
	else
		return false;

	/*
	if (distanceFromPlane>radius)
		return true;
	else
	{
		if (distanceFromPlane>-radius)
			return true;
		else
			return false;
	}
	*/
}

void Plane::normalFromVectors(double *v1, double *v2)
{
	m_normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	m_normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
	m_normal[2] = v1[0] * v2[1] - v1[1] * v2[0];

	this->setNormal(m_normal[0], m_normal[1], m_normal[2]);
}

void Plane::distanceFromPoint(double x, double y, double z)
{
	m_distance = -(m_normal[0]*x + m_normal[1]*y + m_normal[2]*z);
}

void Plane::intersect(
  double x0, double y0, double z0, 
  double vx, double vy, double vz, 
  double &x, double &y, double &z)
{
	double t = -(x0*m_normal[0] + y0*m_normal[1] + z0*m_normal[2]+m_distance)/
		(m_normal[0]*vx + m_normal[1]*vy + m_normal[2]*vz);

    x = x0 + vx*t;
    y = y0 + vy*t;
    z = z0 + vz*t;
}
