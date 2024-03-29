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

#pragma once

#include <ivf/IndexBase.h>

#include <ivf/config.h>

namespace ivf {

IvfSmartPointer(Index);

/**
 * Index class
 *
 * This class is used to define an index used in the 
 * CIvfGLPrimitive derived classes.
 */
class IVF_API Index : public IndexBase {
private:
	std::vector<long> m_indices;
public:
	/** CIvfIndex constructor */
	Index();

	/** CIvfIndex destructor */
	virtual ~Index();

	IvfClassInfo("Index",IndexBase);
	IvfStdFactory(Index);

	/** Return value at position */
	virtual long getIndex(long pos) override;

	/** Return size of index */
	virtual long getSize() override;

	/** Clear all values */
	virtual void clear() override;

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em 0 going to \em size-1
	 */
	virtual void createLinear(long size) override;

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em start going to \em start+size-1
	 */
	virtual void createLinear(long start, long size) override;

	/**
	 * Creates a constant index
	 * 
	 * Creates a constant index with each value
	 * assigned \em value and the \em size long.
	 */
	virtual void createConstant(long value, long size) override;

	/** Assigns values from another index */
	virtual void assignFrom(IndexBase* idx) override;

	/** Assigns \em idx the values of this index */
	virtual void assignTo(IndexBase* idx) override;

	/** Add values from array \em i to this index */ 
	virtual void addArray(long* i, long size) override;

	virtual void addLast(long idx) override;

	void setIndex(long pos, long value);

};

}