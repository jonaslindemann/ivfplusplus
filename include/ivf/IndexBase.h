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

#pragma once

#include <ivf/Base.h>

#define IVF_IDX_TRIANGLES 0
#define IVF_IDX_QUADS     1

namespace ivf {

IvfSmartPointer(CIndexBase);

/**
 * Abstract base class for an index
 */
class IVF_API CIndexBase : public CBase {
private:
	int m_topology;
public:
	/** CIvfIndex constructor */
	CIndexBase();

	/** CIvfIndex destructor */
	virtual ~CIndexBase();

	IvfClassInfo("CIndexBase",CBase);

	/** Return value at position */
	virtual long getIndex(long pos);

	/** Return size of index */
	virtual long getSize();

	/** Set size of index */
	virtual void setSize(int size);

	/** Clear all values */
	virtual void clear();

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em 0 going to \em size-1
	 */
	virtual void createLinear(long size);

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em start going to \em start+size-1
	 */
	virtual void createLinear(long start, long size);

	/**
	 * Creates a constant index
	 * 
	 * Creates a constant index with each value
	 * assigned \em value and the \em size long.
	 */
	virtual void createConstant(long value, long size);

	/** Assigns values from another index */
	virtual void assignFrom(CIndexBase* idx);

	/** Assigns \em idx the values of this index */
	virtual void assignTo(CIndexBase* idx);

	/** Add values from array \em i to this index */ 
	virtual void addArray(long* i, long size);

	/** Add values */
	void add(long i1, long i2, long i3, long i4, long i5);

	/** Add values */
	void add(long i1, long i2, long i3, long i4);

	/** Add values */
	void add(long i1, long i2, long i3);

	/** Add values */
	void add(long i1, long i2);

	/** Add values */
	void add(long idx);

	virtual void addLast(long idx);

	/** 
	 * Sets topology hint 
	 *
	 * This information is used by the CPolySet class
	 * to determine the topology of the index
	 */
	void setTopology(int topology);

	/** Return topology hint */
	int getTopology();
	void setIndex(long pos, long value);
};

}