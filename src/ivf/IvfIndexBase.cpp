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

#include <ivf/IvfIndexBase.h>

// ------------------------------------------------------------
// ------------------------------------------------------------

CIvfIndexBase::CIvfIndexBase()
{

}

// ------------------------------------------------------------
CIvfIndexBase::~CIvfIndexBase()
{

}

// ------------------------------------------------------------
void CIvfIndexBase::addLast(long idx)
{
	addLast(idx);
}

// ------------------------------------------------------------
void CIvfIndexBase::add(long idx)
{
	addLast(idx);
}

// ------------------------------------------------------------
long CIvfIndexBase::getIndex(long pos)
{
	return -1;
}

// ------------------------------------------------------------
long CIvfIndexBase::getSize()
{
	return -1;
}

// ------------------------------------------------------------
void CIvfIndexBase::clear()
{
	//m_indices.clear();
}

// ------------------------------------------------------------
void CIvfIndexBase::createLinear(long size)
{
	/*
	long i;
	m_indices.clear();
	for (i=0; i<this->size; i++)
		this->add(i);
	*/
}

// ------------------------------------------------------------
void CIvfIndexBase::add(long i1, long i2)
{
	this->add(i1);
	this->add(i2);
}

// ------------------------------------------------------------
void CIvfIndexBase::add(long i1, long i2, long i3)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
}

// ------------------------------------------------------------
void CIvfIndexBase::add(long i1, long i2, long i3, long i4)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
	this->add(i4);
}

// ------------------------------------------------------------
void CIvfIndexBase::addArray(long *i, long size)
{
	/*
	long j;

	for (j=0; j<size; j++)
		this->add(i[j]);
	*/
}

// ------------------------------------------------------------
void CIvfIndexBase::assignFrom(CIvfIndexBase *idx)
{
	/*
	long i;

	this.clear();

	for (i=0; i<idx->getSize(); i++)
		this->add(idx->getIndex(i));
	*/
}

// ------------------------------------------------------------
void CIvfIndexBase::assignTo(CIvfIndexBase *idx)
{
	idx->clear();
	idx->assignFrom(this);
}

// ------------------------------------------------------------
void CIvfIndexBase::createLinear(long start, long size)
{
	/*
	long i;
	this.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(i+start);
	*/
}

// ------------------------------------------------------------
void CIvfIndexBase::createConstant(long value, long size)
{
	/*
	long i;
	m_indices.clear();
	for (i=0; i<size; i++)
		m_indices.push_back(value);
	*/
}

// ------------------------------------------------------------
void CIvfIndexBase::add(long i1, long i2, long i3, long i4, long i5)
{
	this->add(i1);
	this->add(i2);
	this->add(i3);
	this->add(i4);
	this->add(i5);
}

// ------------------------------------------------------------
void CIvfIndexBase::setTopology(int topology)
{
	m_topology = topology;
}

// ------------------------------------------------------------
int CIvfIndexBase::getTopology()
{
	return m_topology;
}

// ------------------------------------------------------------
void CIvfIndexBase::setSize(int size)
{

}

// ------------------------------------------------------------
void CIvfIndexBase::setIndex(long pos, long value)
{

}
