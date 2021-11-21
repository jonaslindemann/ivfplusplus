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

IvfSmartPointer(CColorList);

/**
 * Color list class
 *
 * Maintains a list of colors for use with the CVertexElements class.
 * The list is initially 1024 elements and the size is doubled if exceeding
 * 1024.
 *
 * @author Jonas Lindemann
 */
class IVF_API CColorList : public CBase {
private:
	GLfloat* m_color;
	long m_allocSize;
	long m_size;
	long m_blockSize;
	long m_next;
public:
	/** CIvfColorList constructor */
	CColorList();

	/** CIvfColorList destructor */
	virtual ~CColorList();

	IvfClassInfo("CColorList",CBase);
	IvfStdFactory(CColorList);

	/** Clear the color list */
	void clear();

	/** Return the color at idx */
	void getColor(int idx, float &red, float &green, float &blue);

	/** Returns the number of colors in the list */
	int getSize();

	/** Adds a color to the list */
	void add(float red, float green, float blue);

	/** Returns the number of color components for each color (3) */
	GLint getCompSize();

	/** Returns the array stride, in this case 3*sizeof(GLfloat) */
	GLsizei getStride();

	/** Returns array datatype, in this case GL_FLOAT */
	GLenum getDataType();

	/** Return the pointer to the array */
	void* getData();
protected:
	float getComp(int idx);
	void add(float comp);
};

}