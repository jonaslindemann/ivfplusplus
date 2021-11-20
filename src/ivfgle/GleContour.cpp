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

#include <ivfgle/GleContour.h>

using namespace ivf;

CGleContour::CGleContour()
{
	m_coords = NULL;
	m_normals = NULL;
	m_size = 0;
}

CGleContour::CGleContour(int size)
{
	m_coords = NULL;
	m_normals = NULL;
	m_size = 0;

	setSize(size);
}

CGleContour::~CGleContour()
{
	if (m_coords!=NULL)
		delete [] m_coords;

	if (m_normals!=NULL)
		delete [] m_normals;
}

void CGleContour::setSize(int size)
{
	// Delete previous spine coords/colors if any
	
	if (m_coords!=NULL)
		delete [] m_coords;
		
	if (m_normals!=NULL)
		delete [] m_normals;

	// Set new size
		
	m_size = size;
		
	// Allocate memory for new spine
		
	m_coords = new gleDouble[m_size][2];
	m_normals = new gleDouble[m_size-1][2];
}

void CGleContour::setCoord(int idx, double x, double y)
{
	if ((idx>=0)&&(idx<m_size))
	{
		m_coords[idx][0] = x;
		m_coords[idx][1] = y;
	}
}

void CGleContour::getCoord(int idx, double &x, double &y)
{
	if ((idx>=0)&&(idx<m_size))
	{
		x = m_coords[idx][0];
		y = m_coords[idx][1];
	}
}

int CGleContour::getSize()
{
	return m_size;
}

void* CGleContour::getCoordData()
{
	if (m_coords!=NULL)
		return &m_coords[0];
	else
		return NULL;
}

void* CGleContour::getNormalData()
{
	if (m_normals!=NULL)
		return &m_normals[0];
	else
		return NULL;
}

void CGleContour::calcNormals()
{
	int i;

	double x1, y1;
	double x2, y2;
	double l;

	for (i=0; i<m_size-1; i++)
	{
		x1 = m_coords[i][0];
		y1 = m_coords[i][1];
		x2 = m_coords[i+1][0];
		y2 = m_coords[i+1][1];

		m_normals[i][0] = -(-y2+y1);
		m_normals[i][1] = -(x2-x1);

		l = sqrt(pow(m_normals[i][0],2)+pow(m_normals[i][1],2));

		m_normals[i][0] /= l;
		m_normals[i][1] /= l;
	}
}
