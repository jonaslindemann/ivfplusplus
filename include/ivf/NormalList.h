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

#include <ivf/Base.h>
#include <ivf/GL.h>

namespace ivf {

IvfSmartPointer(NormalList);

/**
 * Normal list class
 *
 * Maintains a list of normals for use with the VertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024.
 *
 * @author Jonas Lindemann
 */
class IVF_API NormalList : public Base {
private:
	GLfloat* m_normals;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** NormalList constructor */
	NormalList();

	/** NormalList destructor */
	virtual ~NormalList();

	IvfClassInfo("NormalList",Base);
	IvfStdFactory(NormalList);

	/** Add a normal to the list */
	void add(double x, double y, double z);

	/** Clear normal list */
	void clear();

	/** Return normal at position idx */
	void getNormal(int idx, double &x, double &y, double &z);

	/** Return the number of normals in the list */
	int getSize();

	/** Return the array stride in this case 3 */
	GLsizei getStride();

	/** Returns the array data type in this case GL_FLOAT */
	GLenum getDataType();

	/** Return the pointer of the array */
	void* getData();
protected:
	double getComp(int idx);
	void add(double comp);
};

}