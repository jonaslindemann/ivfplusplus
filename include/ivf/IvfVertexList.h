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

#ifndef _CIvfVertexList_h_
#define _CIvfVertexList_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfGL.h>

#include <ivfmath/IvfVec3d.h>

IvfSmartPointer(CIvfVertexList);

/**
 * Vertex list class
 *
 * Maintains a list of vertex coordinates for use with the CIvfVertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024. 
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfVertexList : public CIvfBase {
private:
	GLfloat* m_vertices;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** CIvfVertexList constructor */
	CIvfVertexList();

	/** CIvfVertexList destructor */
	virtual ~CIvfVertexList();

	IvfClassInfo("CIvfVertexList",CIvfBase);

	/** Add vertex coordinate */
	void add(double x, double y, double z);

	/** 
	 * Set specified vertex coordinate
	 *
	 * param idx should be between 0 to getSize()-1
	 */
	void setVertex(int idx, double x, double y, double z);

	/** 
	 * Get specified vertex coordinate
	 *
	 * param idx should be between 0 to getSize()-1
	 */
	void getVertex(int idx, double &x, double &y, double &z);

	/** 
	 * Get specified vertex coordinate
	 *
	 * param idx should be between 0 to getSize()-1
	 */
	CIvfVec3d& getVertex(int idx);

	/** Set size of vertex list */
	void setSize(int size);

	/** Return size of vertex list */
	int getSize();

	/** Returns pointer to array */
	void* getData();

	/** Return datatype of array, in this case GL_FLOAT */
	GLenum getDataType();

	/** Return array stride, in this case 3*sizeof(GLfloat) */
	GLsizei getStride();

	/** Return the number of components in the list, in this case 3 */
	GLint getCompSize();
protected:
	void setComp(int idx, double value);
	double getComp(int idx);
	void add(double comp);
};

#endif 
