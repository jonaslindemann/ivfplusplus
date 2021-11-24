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

#include <ivf/ColorList.h>

using namespace ivf;

ColorList::ColorList()
{
	m_size = 0;
	m_blockSize = 1024;

	m_color = new GLfloat[m_blockSize];
	m_allocSize = 1024;
	m_next = 0;
}

ColorList::~ColorList()
{
	delete [] m_color;
}

void ColorList::add(float red, float green, float blue)
{
	add(red);
	add(green);
	add(blue);
}

void ColorList::add(float comp)
{
	if (m_next==m_allocSize)
	{
		m_allocSize = m_allocSize*2;
		IvfDbg1("CIvfColorList: Expanding to " << m_allocSize << " normals. (allocated size)");
		GLfloat* newColor = new GLfloat[m_allocSize];
		
		int i;

		for (i=0; i<m_allocSize-m_allocSize/2; i++)
			newColor[i] = m_color[i];

		delete [] m_color;

		m_color = newColor;
	}

	m_color[m_next] = (GLfloat)comp;
	m_next++;
}

int ColorList::getSize()
{
	return m_next / 3;
}

void ColorList::getColor(int idx, float &x, float &y, float &z)
{
	if ((idx>=0)&&(idx<m_next/3))
	{
		x = getComp(idx*3);
		y = getComp(idx*3+1);
		z = getComp(idx*3+2);
	}
}

float ColorList::getComp(int idx)
{
	if ((idx>=0)&&(idx<m_next))
	{
		return m_color[idx];
	}
	else
		return -1.0;
}

void* ColorList::getData()
{
	return (void*)m_color;
}

GLenum ColorList::getDataType()
{
	return GL_FLOAT;
}

GLsizei ColorList::getStride()
{
	return 3*sizeof(GLfloat);
}

GLint ColorList::getCompSize()
{
	return 3;
}

void ColorList::clear()
{
	delete [] m_color;

	m_allocSize = 1024;
	m_color = new GLfloat[m_allocSize];
	m_next = 0;
}
