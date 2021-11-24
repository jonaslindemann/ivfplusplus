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

#include <ivfgle/GleCoordArray.h>

using namespace ivf;

GleCoordArray::GleCoordArray()
{
	m_coords = nullptr;
	m_size = 0;
}

GleCoordArray::GleCoordArray(int size)
{
	m_coords = nullptr;
	m_size = 0;

	setSize(size);
}

GleCoordArray::~GleCoordArray()
{
	if (m_coords!=nullptr)
		delete [] m_coords;
}

void GleCoordArray::setSize(int size)
{
	// Delete previous spine coords/colors if any
	
	if (m_coords!=nullptr)
		delete [] m_coords;
		
	// Set new size
		
	m_size = size;
		
	// Allocate memory for new spine
		
	m_coords = new gleDouble[m_size][3];
}

void GleCoordArray::setCoord(int idx, double x, double y, double z)
{
	if ((idx>=0)&&(idx<m_size))
	{
		m_coords[idx][0] = x;
		m_coords[idx][1] = y;
		m_coords[idx][2] = z;
	}
}

void GleCoordArray::getCoord(int idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_size))
	{
		x = m_coords[idx][0];
		y = m_coords[idx][1];
		z = m_coords[idx][2];
	}
}

void GleCoordArray::setCoord(int idx, Vec3d &vec)
{
	if ((idx>=0)&&(idx<m_size))
	{
		double x, y, z;

		vec.getComponents(x, y, z);
		m_coords[idx][0] = x;
		m_coords[idx][1] = y;
		m_coords[idx][2] = z;
	}
}

Vec3d& GleCoordArray::getCoord(int idx)
{
	Vec3d& r = ivfGetTempVec3d();

	if ((idx>=0)&&(idx<m_size))
		r.setComponents(m_coords[idx][0], m_coords[idx][1], m_coords[idx][2]);

	return r;
}

int GleCoordArray::getSize()
{
	return m_size;
}

void* GleCoordArray::getData()
{
	if (m_coords!=nullptr)
		return &m_coords[0];
	else
		return nullptr;
}

void GleCoordArray::calcFirstAndLast()
{
	if (m_size>=4)
	{
		Vec3d p1;
		Vec3d p2;
		Vec3d p3;
		Vec3d v;

		p1 = this->getCoord(1);
		p2 = this->getCoord(2);

		v = p1 - p2;
		v.normalize();

		p3 = p1 + v;

		this->setCoord(0, p3);

		p1 = this->getCoord(m_size-2);
		p2 = this->getCoord(m_size-3);

		v = p1 - p2;
		v.normalize();

		p3 = p1 + v;

		this->setCoord(m_size-1, p3);
	}
}
