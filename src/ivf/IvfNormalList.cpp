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

#include <ivf/IvfNormalList.h>

CIvfNormalList::CIvfNormalList()
{
	m_size = 0;
	m_blockSize = 1024;

	m_normals = new GLfloat[m_blockSize];
	m_allocSize = 1024;
	m_next = 0;
}

CIvfNormalList::~CIvfNormalList()
{
	delete [] m_normals;
}

void CIvfNormalList::add(double x, double y, double z)
{
	add(x);
	add(y);
	add(z);
}

void CIvfNormalList::add(double comp)
{
	if (m_next==m_allocSize)
	{
		m_allocSize = m_allocSize*2;
		IvfDbg1("CIvfNormalList: Expanding to " << m_allocSize << " normals. (allocated size)");
		GLfloat* newNormals = new GLfloat[m_allocSize];
		
		int i;

		for (i=0; i<m_allocSize-m_allocSize/2; i++)
			newNormals[i] = m_normals[i];

		delete [] m_normals;

		m_normals = newNormals;
	}

	m_normals[m_next] = (GLfloat)comp;
	m_next++;
}

int CIvfNormalList::getSize()
{
	return m_next / 3;
}

void CIvfNormalList::getNormal(int idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_next/3))
	{
		x = getComp(idx*3);
		y = getComp(idx*3+1);
		z = getComp(idx*3+2);
	}
}

double CIvfNormalList::getComp(int idx)
{
	if ((idx>=0)&&(idx<m_next))
	{
		return m_normals[idx];
	}
	else
		return -1.0;
}

void* CIvfNormalList::getData()
{
	return (void*)m_normals;
}

GLenum CIvfNormalList::getDataType()
{
	return GL_FLOAT;
}

GLsizei CIvfNormalList::getStride()
{
	return 3*sizeof(GLfloat);
}

void CIvfNormalList::clear()
{
	delete [] m_normals;

	m_allocSize = 1024;
	m_normals = new GLfloat[m_allocSize];
	m_next = 0;
}
