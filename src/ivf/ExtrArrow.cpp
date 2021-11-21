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

#include <ivf/ExtrArrow.h>

#include <ivf/config.h>

#include <ivfmath/BoundingSphere.h>

#include <GL/gle.h>

using namespace ivf;

// ------------------------------------------------------------
CExtrArrow::CExtrArrow()
:CShape()
{
	m_direction[0] = 1.0;
	m_direction[1] = 0.0;
	m_direction[2] = 0.0;
	m_length = 1.0;
	m_head = 0.2 * m_length;
	m_headRadius = 0.03*m_length;
	m_tailRadius = 0.02*m_length;
	m_offset = 0.0;
}

// ------------------------------------------------------------
CExtrArrow::~CExtrArrow()
{

}

// ------------------------------------------------------------
void CExtrArrow::doCreateGeometry()
{
	glePolyCone(6,m_coords,nullptr,m_radius);
}

// ------------------------------------------------------------
void CExtrArrow::setDirection(double ex, double ey, double ez)
{
	double l;
	l = sqrt(pow(ex,2) + pow(ey,2) + pow(ez,2));
	m_direction[0] = ex/l;
	m_direction[1] = ey/l;
	m_direction[2] = ez/l;
	this->initExtrusion();
}

// ------------------------------------------------------------
void CExtrArrow::getDirection(double &ex, double &ey, double &ez)
{
	ex = m_direction[0];
	ey = m_direction[1];
	ez = m_direction[2];
}

// ------------------------------------------------------------
void CExtrArrow::initExtrusion()
{
	m_coords[0][0] = (m_offset-1.0)*m_direction[0];
	m_coords[0][1] = (m_offset-1.0)*m_direction[1];
	m_coords[0][2] = (m_offset-1.0)*m_direction[2];
	m_radius[0] = 0.0;

	m_coords[1][0] = m_offset*m_direction[0];
	m_coords[1][1] = m_offset*m_direction[1];
	m_coords[1][2] = m_offset*m_direction[2];
	m_radius[1] = m_tailRadius;

	m_coords[2][0] = (m_length-m_head+m_offset)*m_direction[0];
	m_coords[2][1] = (m_length-m_head+m_offset)*m_direction[1];
	m_coords[2][2] = (m_length-m_head+m_offset)*m_direction[2];
	m_radius[2] = m_tailRadius;

	m_coords[3][0] = (m_length-m_head+m_offset)*m_direction[0];
	m_coords[3][1] = (m_length-m_head+m_offset)*m_direction[1];
	m_coords[3][2] = (m_length-m_head+m_offset)*m_direction[2];
	m_radius[3] = m_headRadius;

	m_coords[4][0] = (m_length+m_offset)*m_direction[0];
	m_coords[4][1] = (m_length+m_offset)*m_direction[1];
	m_coords[4][2] = (m_length+m_offset)*m_direction[2];
	m_radius[4] = 0.0;

	m_coords[5][0] = (m_length+m_offset+1)*m_direction[0];
	m_coords[5][1] = (m_length+m_offset+1)*m_direction[1];
	m_coords[5][2] = - (m_length+m_offset+1)*m_direction[2];
	m_radius[5] = 0.0;
}

// ------------------------------------------------------------
void CExtrArrow::setSize(double length, double head)
{
	m_length = length;
	m_head = head;
	this->initExtrusion();
}

// ------------------------------------------------------------
void CExtrArrow::setRadius(double head, double tail)
{
	m_headRadius = head;
	m_tailRadius = tail;
	this->initExtrusion();
}

// ------------------------------------------------------------
void CExtrArrow::getSize(double &length, double &head)
{
	length = m_length;
	head = m_head;
}

// ------------------------------------------------------------
void CExtrArrow::getRadius(double &head, double &tail)
{
	head = m_headRadius;
	tail = m_tailRadius;
}

// ------------------------------------------------------------
void CExtrArrow::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		CBoundingSphere* bSphere = getBoundingSphere();
		bSphere->setRadius(m_length/2.0);
	}
}

void CExtrArrow::setOffset(double offset)
{
	m_offset = offset;
	initExtrusion();
}

double CExtrArrow::getOffset()
{
	return m_offset;
}

void CExtrArrow::setDirection(CVec3d &vec)
{
	double vx, vy, vz;
	vec.getComponents(vx, vy, vz);
	this->setDirection(vx, vy, vz);
}
