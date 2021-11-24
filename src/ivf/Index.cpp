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

#include <ivf/Index.h>

using namespace ivf;

// ------------------------------------------------------------
// ------------------------------------------------------------

Index::Index()
{

}

// ------------------------------------------------------------
Index::~Index()
{

}

// ------------------------------------------------------------
void Index::addLast(long idx)
{
	m_indices.push_back(idx);
}

// ------------------------------------------------------------
long Index::getIndex(long pos)
{
	return m_indices[pos];
}

// ------------------------------------------------------------
long Index::getSize()
{
	return static_cast<int>(m_indices.size());
}

// ------------------------------------------------------------
void Index::clear()
{
	m_indices.clear();
}

// ------------------------------------------------------------
void Index::createLinear(long size)
{
	long i;
	m_indices.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(i);
}

// ------------------------------------------------------------
void Index::addArray(long *i, long size)
{
	long j;

	for (j=0; j<size; j++)
		this->add(i[j]);
}

// ------------------------------------------------------------
void Index::assignFrom(IndexBase *idx)
{
	long i;

	m_indices.clear();

	for (i=0; i<idx->getSize(); i++)
		this->add(idx->getIndex(i));
}

// ------------------------------------------------------------
void Index::assignTo(IndexBase *idx)
{
	idx->clear();
	idx->assignFrom(this);
}

// ------------------------------------------------------------
void Index::createLinear(long start, long size)
{
	long i;
	m_indices.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(i+start);
}

// ------------------------------------------------------------
void Index::createConstant(long value, long size)
{
	long i;
	m_indices.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(value);
}

// ------------------------------------------------------------
void Index::setIndex(long pos, long value)
{
	if ((pos>=0)&&(pos<(int)m_indices.size()))
		m_indices[pos] = value;
}
