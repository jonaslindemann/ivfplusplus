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

#include <ivfmath/Vec3d.h>

namespace ivf {

IvfSmartPointer(CTexCoordList);

/**
 * Normal list class
 *
 * Maintains a list of texture coords for use with the CVertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024. 
 *
 * @author Jonas Lindemann
 */
class IVF_API CTexCoordList : public CBase {
private:
	GLfloat* m_vertices;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** CTexCoordList constructor */
	CTexCoordList();

	/** CTexCoordList destructor */
	virtual ~CTexCoordList();

	IvfClassInfo("CTexCoordList",CBase);
	IvfStdFactory(CTexCoordList);

	/** Add a texture coordinate */
	void add(double s, double t);

	/** Clear texture coordinate list */
	void clear();

	/** 
	 * Get specified texture coordinate
	 *
	 * param idx should be between 0 to getSize()-1
	 */
	void getVertex(int idx, double &s, double &t);

	/** Set size of coordinate list */
	void setSize(int size);

	/** Return size of coordinate list */
	int getSize();

	/** 
	 * Set specified texture coordinate
	 *
	 * param idx should be between 0 to getSize()-1
	 */
	void setVertex(int idx, double s, double t);

	/** Returns a pointer to the array */
	void* getData();

	/** Return array data type, in this case GL_FLOAT */
	GLenum getDataType();

	/** Return array stride, in this case 2*sizeof(GLfloat) */
	GLsizei getStride();

	/** Return number of components in array, in this case 2 */
	GLint getCompSize();
protected:
	double getComp(int idx);
	void setComp(int idx, double value);
	void add(double comp);
};

}