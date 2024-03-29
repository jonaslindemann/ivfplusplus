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

#include <ivf/TexCoordList.h>

using namespace ivf;

TexCoordList::TexCoordList()
{
	m_allocSize = 1024;
	m_vertices = new GLfloat[m_allocSize];
	m_next = 0;
}

TexCoordList::~TexCoordList()
{
	delete [] m_vertices;
}

void TexCoordList::add(double s, double t)
{
	add(s);
	add(t);
}

void TexCoordList::add(double comp)
{
	if (m_next==m_allocSize)
	{
		m_allocSize = m_allocSize*2;
		IvfDbg1("TexCoordList: Expanding to " << m_allocSize << " vertices. (allocated size)");
		GLfloat* newVertices = new GLfloat[m_allocSize];
		
		int i;

		for (i=0; i<m_allocSize-m_allocSize/2; i++)
			newVertices[i] = m_vertices[i];

		delete [] m_vertices;

		m_vertices = newVertices;
	}

	m_vertices[m_next] = (GLfloat)comp;
	m_next++;
}

int TexCoordList::getSize()
{
	return m_next / 3;
}

void TexCoordList::getVertex(int idx, double &s, double &t)
{
	if ((idx>=0)&&(idx<m_next/2))
	{
		s = getComp(idx*2);
		t = getComp(idx*2+1);
	}
	else
	{
		s = 0.0;
		t = 0.0;
	}
}

double TexCoordList::getComp(int idx)
{
	if ((idx>=0)&&(idx<m_next))
	{
		return m_vertices[idx];
	}
	else
		return -1.0;
}

void* TexCoordList::getData()
{
	return (void*)m_vertices;
}

GLenum TexCoordList::getDataType()
{
	return GL_FLOAT;
}

GLsizei TexCoordList::getStride()
{
	return 2*sizeof(GLfloat);
}

GLint TexCoordList::getCompSize()
{
	return 2;
}

void TexCoordList::setSize(int size)
{
	delete [] m_vertices;

	m_allocSize = size;
	m_vertices = new GLfloat[m_allocSize];
	m_next = m_allocSize;
}

void TexCoordList::setVertex(int idx, double s, double t)
{
	if ((idx>=0)&&(idx<m_next/2))
	{
		setComp(idx*2, s);
		setComp(idx*2+1, t);
	}
	else
	{
		s = 0.0;
		t = 0.0;
	}
}

void TexCoordList::setComp(int idx, double value)
{
	if ((idx>=0)&&(idx<m_next))
	{
		m_vertices[idx] = value;
	}
}

void TexCoordList::clear()
{
	delete [] m_vertices;
	m_allocSize = 1024;
	m_vertices = new GLfloat[m_allocSize];
	m_next = 0;
}
