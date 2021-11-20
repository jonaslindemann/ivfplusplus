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

#ifndef _CIvfNormalList_h_
#define _CIvfNormalList_h_

#include <ivf/IvfBase.h>

#include <ivf/IvfGL.h>

IvfSmartPointer(CNormalList);

/**
 * Normal list class
 *
 * Maintains a list of normals for use with the CVertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024.
 *
 * @author Jonas Lindemann
 */
class IVF_API CNormalList : public CBase {
private:
	GLfloat* m_normals;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** CNormalList constructor */
	CNormalList();

	/** CNormalList destructor */
	virtual ~CNormalList();

	IvfClassInfo("CNormalList",CBase);

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

#endif 
