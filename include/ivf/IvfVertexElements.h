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

#ifndef _CIvfVertexElements_h_
#define _CIvfVertexElements_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfVertexIndex.h>
#include <ivf/IvfVertexList.h>
#include <ivf/IvfColorList.h>
#include <ivf/IvfNormalList.h>

#include <ivf/IvfGL.h>

IvfSmartPointer(CIvfVertexElements);

/**
 * Vertex element class
 *
 * Implements the OpenGL glDrawElements command
 */
class IVF_API CIvfVertexElements : public CIvfShape {
public:
	enum TPrimitiveType {
		PT_POINTS,
		PT_LINES,
		PT_LINE_STRIP,
		PT_LINE_LOOP,
		PT_TRIANGLES,
		PT_TRIANGLE_STRIP,
		PT_TRIANGLE_FAN,
		PT_QUADS,
		PT_QUAD_STRIP,
		PT_POLYGON
	};

private:
	CIvfVertexIndexPtr m_indices;
	CIvfVertexListPtr  m_vertices;
	CIvfColorListPtr   m_colors;
	CIvfNormalListPtr  m_normals;
	GLenum			   m_glPrimitive;
public:
	/** CIvfVertexElements constructor */
	CIvfVertexElements();

	/** CIvfVertexElements destructor */
	virtual ~CIvfVertexElements();

	IvfClassInfo("CIvfVertexElements",CIvfShape);

	/** Set index list */
	void setIndices(CIvfVertexIndex* index);

	/** Set vertex list */
	void setVertices(CIvfVertexList* vertices);

	/** Set color list */
	void setColors(CIvfColorList* colors);

	/** Set normal list */
	void setNormals(CIvfNormalList* normals);

	/** Set primitive type to be drawn */
	void setPrimitive(TPrimitiveType primitive);

	/** Return primitive type */
	TPrimitiveType getPrimitive();

protected:
	virtual void doCreateGeometry();
};

#endif 
