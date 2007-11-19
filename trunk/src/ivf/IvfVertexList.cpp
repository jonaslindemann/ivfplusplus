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

#include <ivf/IvfVertexList.h>

CIvfVertexList::CIvfVertexList()
{
	m_size = 0;
	m_blockSize = 1024;

	m_vertices = new GLfloat[m_blockSize];
	m_allocSize = 1024;
	m_next = 0;
}

CIvfVertexList::~CIvfVertexList()
{
	delete [] m_vertices;
}

void CIvfVertexList::add(double x, double y, double z)
{
	add(x);
	add(y);
	add(z);
}

void CIvfVertexList::add(double comp)
{
	if (m_next==m_allocSize)
	{
		m_allocSize = m_allocSize*2;
		IvfDbg1("CIvfVertexList: Expanding to " << m_allocSize << " vertices. (allocated size)");
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

int CIvfVertexList::getSize()
{
	return m_next / 3;
}

void CIvfVertexList::getVertex(int idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_next/3))
	{
		x = getComp(idx*3);
		y = getComp(idx*3+1);
		z = getComp(idx*3+2);
	}
	else
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
}

double CIvfVertexList::getComp(int idx)
{
	if ((idx>=0)&&(idx<m_next))
	{
		return m_vertices[idx];
	}
	else
		return -1.0;
}

void* CIvfVertexList::getData()
{
	return (void*)m_vertices;
}

GLenum CIvfVertexList::getDataType()
{
	return GL_FLOAT;
}

GLsizei CIvfVertexList::getStride()
{
	return 3*sizeof(GLfloat);
}

GLint CIvfVertexList::getCompSize()
{
	return 3;
}

CIvfVec3d& CIvfVertexList::getVertex(int idx)
{
	CIvfVec3d& pos = ivfGetTempVec3d();
	double x, y, z;

	this->getVertex(idx, x, y, z);
	pos.setComponents(x, y, z);

	return pos;
}

void CIvfVertexList::setSize(int size)
{
	delete [] m_vertices;

	m_allocSize = size;
	m_vertices = new GLfloat[m_allocSize];
	m_next = m_allocSize;
}

void CIvfVertexList::setVertex(int idx, double x, double y, double z)
{
	if ((idx>=0)&&(idx<m_next/3))
	{
		setComp(idx*3, x);
		setComp(idx*3+1, y);
		setComp(idx*3+2, z);
	}
	else
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
}

void CIvfVertexList::setComp(int idx, double value)
{
	if ((idx>=0)&&(idx<m_next))
	{
		m_vertices[idx] = value;
	}
}
