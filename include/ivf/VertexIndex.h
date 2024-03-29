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

#include <ivf/GL.h>
#include <ivf/IndexBase.h>

namespace ivf {

IvfSmartPointer(VertexIndex);

/**
 * Vertex index list class
 *
 * Maintains a list of vertex indices for use with the VertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024. 
 *
 * @author Jonas Lindemann
 */
class IVF_API VertexIndex : public IndexBase {
private:
	GLuint* m_index;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** VertexIndex constructor */
	VertexIndex();

	/** VertexIndex destructor */
	virtual ~VertexIndex();

	IvfClassInfo("VertexIndex",IndexBase);
	IvfStdFactory(VertexIndex);

	/** Return value at position */
	long getIndex(long pos);

	/** Return size of index */
	long getSize();

	/** Return OpenGL data type of index */
	GLenum getDataType();

	/** Return pointer to data */
	void* getData();

	/** Clear all values */
	void clear();

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em 0 going to \em size-1
	 */
	void createLinear(long size);

	/**
	 * Creates a linear index
	 *
	 * Creates a linear index starting at 
	 * \em start going to \em start+size-1
	 */
	void createLinear(long start, long size);

	/**
	 * Creates a constant index
	 * 
	 * Creates a constant index with each value
	 * assigned \em value and the \em size long.
	 */
	void createConstant(long value, long size);

	/** Assigns values from another index */
	void assignFrom(IndexBase* idx);

	/** Assigns \em idx the values of this index */
	void assignTo(IndexBase* idx);

	/** Add values from array \em i to this index */ 
	void addArray(long* i, long size);

	void addLast(long idx);

	void setIndex(long idx, long value);
};

}