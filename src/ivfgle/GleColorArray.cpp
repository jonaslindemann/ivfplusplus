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

#include <ivfgle/GleColorArray.h>

using namespace ivf;

CGleColorArray::CGleColorArray()
{
	m_colors = nullptr;
	m_size = 0;
}

CGleColorArray::CGleColorArray(int size)
{
	m_colors = nullptr;
	m_size = 0;

	setSize(size);
}

CGleColorArray::~CGleColorArray()
{
	if (m_colors!=nullptr)
		delete [] m_colors;
}

void CGleColorArray::setSize(int size)
{
	// Delete previous spine colors/colors if any
	
	if (m_colors!=nullptr)
		delete [] m_colors;
		
	// Set new size
		
	m_size = size;
		
	// Allocate memory for new spine
		
	m_colors = new float[m_size][3];
}

void CGleColorArray::setColor(int idx, float r, float g, float b)
{
	if ((idx>=0)&&(idx<m_size))
	{
		m_colors[idx][0] = r;
		m_colors[idx][1] = g;
		m_colors[idx][2] = b;
	}
}

void CGleColorArray::getColor(int idx, float &r, float &g, float &b)
{
	if ((idx>=0)&&(idx<m_size))
	{
		r = m_colors[idx][0];
		g = m_colors[idx][1];
		b = m_colors[idx][2];
	}
}

int CGleColorArray::getSize()
{
	return m_size;
}

void* CGleColorArray::getData()
{
	if (m_colors!=nullptr)
		return &m_colors[0];
	else
		return nullptr;	
}
