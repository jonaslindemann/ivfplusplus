//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "IvfArrayCoordList.h"

CIvfArrayCoordList::CIvfArrayCoordList()
{
	m_size = 0;
	m_colors = NULL;
	m_coords = NULL;
}

CIvfArrayCoordList::~CIvfArrayCoordList()
{
	if (m_coords!=NULL)
		delete [] m_coords;
	
	if (m_colors!=NULL)
		delete [] m_colors;
}

void CIvfArrayCoordList::addCoord(double x, double y, double z)
{
	this->resize(m_size+1);
	m_coords[m_size-1][0] = x;
	m_coords[m_size-1][1] = y;
	m_coords[m_size-1][2] = z;

	m_colors[m_size-1][0] = 1.0f;
	m_colors[m_size-1][1] = 1.0f;
	m_colors[m_size-1][2] = 1.0f;
}

void CIvfArrayCoordList::addCoord(double x, double y, double z, float r, float g, float b)
{
	this->resize(m_size+1);
	m_coords[m_size-1][0] = x;
	m_coords[m_size-1][1] = y;
	m_coords[m_size-1][2] = z;

	m_colors[m_size-1][0] = r;
	m_colors[m_size-1][1] = g;
	m_colors[m_size-1][2] = b;
}

void CIvfArrayCoordList::addCoord(CIvfVec3d* coord)
{
	double x, y, z;
	this->resize(m_size+1);
	coord->getComponents(x, y, z);
	m_coords[m_size-1][0] = x;
	m_coords[m_size-1][1] = y;
	m_coords[m_size-1][2] = z;

	m_colors[m_size-1][0] = 1.0f;
	m_colors[m_size-1][1] = 1.0f;
	m_colors[m_size-1][2] = 1.0f;
}

void CIvfArrayCoordList::setColor(long idx, float r, float g, float b)
{
	if ((idx>=0)&&(idx<m_size))
	{
		m_colors[idx][0] = r;
		m_colors[idx][1] = g;
		m_colors[idx][2] = b;
	}
}

void CIvfArrayCoordList::getColor(long idx, float &r, float &g, float &b)
{
	if ((idx>=0)&&(idx<m_size))
	{
		r = m_colors[idx][0];
		g = m_colors[idx][1];
		b = m_colors[idx][2];
	}	
}
	
void CIvfArrayCoordList::getCoord(long idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_size))
	{
		x = m_coords[idx][0];
		y = m_coords[idx][1];
		z = m_coords[idx][2];
	}
}

CIvfVec3d* CIvfArrayCoordList::getCoord(long idx)
{
	return NULL;
}

long CIvfArrayCoordList::getSize()
{
	return m_size;
}

void CIvfArrayCoordList::clear()
{
	this->resize(0);
}

void CIvfArrayCoordList::resize(int newSize)
{
	if (newSize>=0)
	{
		double (*coords)[3];
		float (*colors)[3];
		int i;
		
		coords = new double[newSize][3];
		colors = new float[newSize][3];

		for (i=0; i<newSize; i++)
		{
			if (i<m_size)
			{
				coords[i][0] = m_coords[i][0];
				coords[i][1] = m_coords[i][1];
				coords[i][2] = m_coords[i][2];
			}
			else
			{
				coords[i][0] = 0.0;
				coords[i][1] = 0.0;
				coords[i][2] = 0.0;
			}
		}
		
		m_size = newSize;
		
		if (m_coords!=NULL)
			delete [] m_coords;
		
		if (m_colors!=NULL)
			delete [] m_colors;

		m_coords = coords;
		m_colors = colors;
	}
}

double** CIvfArrayCoordList::getCoordArray()
{
	return (double**)m_coords;
}

float** CIvfArrayCoordList::getColorArray()
{
	return (float**)m_colors;
}

void CIvfArrayCoordList::setCoord(int idx, double x, double y, double z)
{
	if ((idx>=0)&&(idx<m_size))
	{
		m_coords[idx][0] = x;
		m_coords[idx][1] = y;
		m_coords[idx][2] = z;
	}	
}
