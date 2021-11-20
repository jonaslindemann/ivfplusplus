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

#include <ivf/IndexBase.h>

using namespace ivf;

// ------------------------------------------------------------
// ------------------------------------------------------------

CIndexBase::CIndexBase()
{

}

// ------------------------------------------------------------
CIndexBase::~CIndexBase()
{

}

// ------------------------------------------------------------
void CIndexBase::addLast(long idx)
{
	addLast(idx);
}

// ------------------------------------------------------------
void CIndexBase::add(long idx)
{
	addLast(idx);
}

// ------------------------------------------------------------
long CIndexBase::getIndex(long pos)
{
	return -1;
}

// ------------------------------------------------------------
long CIndexBase::getSize()
{
	return -1;
}

// ------------------------------------------------------------
void CIndexBase::clear()
{
	//m_indices.clear();
}

// ------------------------------------------------------------
void CIndexBase::createLinear(long size)
{
	/*
	long i;
	m_indices.clear();
	for (i=0; i<this->size; i++)
		this->add(i);
	*/
}

// ------------------------------------------------------------
void CIndexBase::add(long i1, long i2)
{
	this->add(i1);
	this->add(i2);
}

// ------------------------------------------------------------
void CIndexBase::add(long i1, long i2, long i3)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
}

// ------------------------------------------------------------
void CIndexBase::add(long i1, long i2, long i3, long i4)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
	this->add(i4);
}

// ------------------------------------------------------------
void CIndexBase::addArray(long *i, long size)
{
	/*
	long j;

	for (j=0; j<size; j++)
		this->add(i[j]);
	*/
}

// ------------------------------------------------------------
void CIndexBase::assignFrom(CIndexBase *idx)
{
	/*
	long i;

	this.clear();

	for (i=0; i<idx->getSize(); i++)
		this->add(idx->getIndex(i));
	*/
}

// ------------------------------------------------------------
void CIndexBase::assignTo(CIndexBase *idx)
{
	idx->clear();
	idx->assignFrom(this);
}

// ------------------------------------------------------------
void CIndexBase::createLinear(long start, long size)
{
	/*
	long i;
	this.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(i+start);
	*/
}

// ------------------------------------------------------------
void CIndexBase::createConstant(long value, long size)
{
	/*
	long i;
	m_indices.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(value);
	*/
}

// ------------------------------------------------------------
void CIndexBase::add(long i1, long i2, long i3, long i4, long i5)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
	this->add(i4);
	this->add(i5);
}

// ------------------------------------------------------------
void CIndexBase::setTopology(int topology)
{
	m_topology = topology;
}

// ------------------------------------------------------------
int CIndexBase::getTopology()
{
	return m_topology;
}

// ------------------------------------------------------------
void CIndexBase::setSize(int size)
{

}

// ------------------------------------------------------------
void CIndexBase::setIndex(long pos, long value)
{

}
