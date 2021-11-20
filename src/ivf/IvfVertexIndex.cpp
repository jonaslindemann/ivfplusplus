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

#include <ivf/IvfVertexIndex.h>

// ------------------------------------------------------------

CVertexIndex::CVertexIndex()
{
	m_size = 0;
	m_blockSize = 1024;

	m_index = new GLuint[m_blockSize];
	m_allocSize = 1024;
	m_next = 0;
}

// ------------------------------------------------------------
CVertexIndex::~CVertexIndex()
{

}

// ------------------------------------------------------------
void CVertexIndex::addLast(long idx)
{
	if (m_next==m_allocSize)
	{
		m_allocSize = m_allocSize*2;
		IvfDbg1("CVertexIndex: Expanding to " << m_allocSize << " indices. (allocated size)");
		GLuint* newIndex = new GLuint[m_allocSize];
		
		int i;

		for (i=0; i<m_allocSize-m_allocSize/2; i++)
			newIndex[i] = m_index[i];

		delete [] m_index;

		m_index = newIndex;
	}

	m_index[m_next] = (GLuint)idx;
	m_next++;
}

// ------------------------------------------------------------
long CVertexIndex::getIndex(long pos)
{
	if ((pos>=0)&&(pos<m_next))
		return (long)m_index[pos];
	else
		return -1;
}

// ------------------------------------------------------------
long CVertexIndex::getSize()
{
	return m_next;
}

// ------------------------------------------------------------
void CVertexIndex::clear()
{
	delete [] m_index;
	m_index = new GLuint[m_blockSize];
	m_allocSize = 1024;
	m_next = 0;
}

// ------------------------------------------------------------
void CVertexIndex::createLinear(long size)
{
	long i;
	this->clear();
	for (i=0; i<size; i++)
		this->add(i);
}

// ------------------------------------------------------------
void CVertexIndex::addArray(long *i, long size)
{
	long j;

	for (j=0; j<size; j++)
		this->add(i[j]);
}

// ------------------------------------------------------------
void CVertexIndex::assignFrom(CIndexBase *idx)
{
	long i;

	this->clear();

	for (i=0; i<idx->getSize(); i++)
		this->add(idx->getIndex(i));
}

// ------------------------------------------------------------
void CVertexIndex::assignTo(CIndexBase *idx)
{
	idx->clear();
	idx->assignFrom(this);
}

// ------------------------------------------------------------
void CVertexIndex::createLinear(long start, long size)
{
	long i;
	this->clear();
	for (i=0; i<size; i++)
		this->add(i+start);
}

// ------------------------------------------------------------
void CVertexIndex::createConstant(long value, long size)
{
	long i;
	this->clear();
	for (i=0; i<size; i++)
		this->add(value);
}


void* CVertexIndex::getData()
{
	return (void*)m_index;
}

GLenum CVertexIndex::getDataType()
{
	return GL_UNSIGNED_INT;
}


void CVertexIndex::setIndex(long idx, long value)
{
	if ((idx>=0)&&(idx<m_next))
	{
		m_index[idx] = value;
	}
}
