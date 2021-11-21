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

#include <ivf/Shape.h>
#include <ivf/VertexIndex.h>
#include <ivf/VertexList.h>
#include <ivf/ColorList.h>
#include <ivf/NormalList.h>

#include <ivf/GL.h>

namespace ivf {

IvfSmartPointer(CVertexElements);

/**
 * Vertex element class
 *
 * Implements the OpenGL glDrawElements command
 */
class IVF_API CVertexElements : public CShape {
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
	CVertexIndexPtr m_indices;
	CVertexListPtr  m_vertices;
	CColorListPtr   m_colors;
	CNormalListPtr  m_normals;
	GLenum			   m_glPrimitive;
public:
	/** CVertexElements constructor */
	CVertexElements();

	/** CVertexElements destructor */
	virtual ~CVertexElements();

	IvfClassInfo("CVertexElements",CShape);
	IvfStdFactory(CVertexElements);

	/** Set index list */
	void setIndices(CVertexIndex* index);

	/** Set vertex list */
	void setVertices(CVertexList* vertices);

	/** Set color list */
	void setColors(CColorList* colors);

	/** Set normal list */
	void setNormals(CNormalList* normals);

	/** Set primitive type to be drawn */
	void setPrimitive(TPrimitiveType primitive);

	/** Return primitive type */
	TPrimitiveType getPrimitive();

protected:
	virtual void doCreateGeometry();
};

}