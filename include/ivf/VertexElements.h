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

IvfSmartPointer(VertexElements);

/**
 * Vertex element class
 *
 * Implements the OpenGL glDrawElements command
 */
class IVF_API VertexElements : public Shape {
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
	VertexIndexPtr m_indices;
	VertexListPtr  m_vertices;
	ColorListPtr   m_colors;
	NormalListPtr  m_normals;
	GLenum			   m_glPrimitive;
public:
	/** VertexElements constructor */
	VertexElements();

	/** VertexElements destructor */
	virtual ~VertexElements();

	IvfClassInfo("VertexElements",Shape);
	IvfStdFactory(VertexElements);

	/** Set index list */
	void setIndices(VertexIndex* index);

	/** Set vertex list */
	void setVertices(VertexList* vertices);

	/** Set color list */
	void setColors(ColorList* colors);

	/** Set normal list */
	void setNormals(NormalList* normals);

	/** Set primitive type to be drawn */
	void setPrimitive(TPrimitiveType primitive);

	/** Return primitive type */
	TPrimitiveType getPrimitive();

protected:
	virtual void doCreateGeometry();
};

}