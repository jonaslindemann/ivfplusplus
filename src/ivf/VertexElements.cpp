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

#include <ivf/VertexElements.h>

using namespace ivf;

CVertexElements::CVertexElements()
{
	m_glPrimitive = GL_QUADS;
}

CVertexElements::~CVertexElements()
{

}

void CVertexElements::doCreateGeometry()
{
    /* set up the array data */
	if (m_colors)
	{
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( m_colors->getCompSize(), m_colors->getDataType(), m_colors->getStride(), m_colors->getData() );
	}

	if (m_vertices)
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( m_vertices->getCompSize(), m_vertices->getDataType(), m_vertices->getStride(), m_vertices->getData() );
	}

	if (m_normals)
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( m_normals->getDataType(), m_normals->getStride(), m_normals->getData() );
	}

	if ((m_indices)&&(m_vertices))
		glDrawElements ( m_glPrimitive, m_indices->getSize(), m_indices->getDataType(), m_indices->getData() );

	if (m_colors)
		glDisableClientState( GL_COLOR_ARRAY );

	if (m_vertices)
		glDisableClientState( GL_VERTEX_ARRAY );

	if (m_normals)
		glDisableClientState( GL_NORMAL_ARRAY );
}

void CVertexElements::setIndices(CVertexIndex *index)
{
	m_indices = index;
}

void CVertexElements::setVertices(CVertexList *vertices)
{
	m_vertices = vertices;
}

void CVertexElements::setColors(CColorList *colors)
{
	m_colors = colors;
}

void CVertexElements::setNormals(CNormalList *normals)
{
	m_normals = normals;
}

void CVertexElements::setPrimitive(TPrimitiveType primitive)
{
	switch (primitive) {
	case PT_POINTS:
		m_glPrimitive = GL_POINTS;
		break;
	case PT_LINES:
		m_glPrimitive = GL_LINES;
		break;
	case PT_LINE_STRIP:
		m_glPrimitive = GL_LINE_STRIP;
		break;
	case PT_LINE_LOOP:
		m_glPrimitive = GL_LINE_LOOP;
		break;
	case PT_TRIANGLES:
		m_glPrimitive = GL_TRIANGLES;
		break;
	case PT_TRIANGLE_STRIP:
		m_glPrimitive = GL_TRIANGLE_STRIP;
		break;
	case PT_TRIANGLE_FAN:
		m_glPrimitive = GL_TRIANGLE_FAN;
		break;
	case PT_QUADS:
		m_glPrimitive = GL_QUADS;
		break;
	case PT_QUAD_STRIP:
		m_glPrimitive = GL_QUAD_STRIP;
		break;
	case PT_POLYGON:
		m_glPrimitive = GL_POLYGON;
		break;
	default:
		m_glPrimitive = GL_POINTS;
		break;
	}
}

CVertexElements::TPrimitiveType CVertexElements::getPrimitive()
{
	switch (m_glPrimitive) {
	case GL_POINTS:
		return PT_POINTS;
		break;
	case GL_LINES:
		return PT_LINES;
		break;
	case GL_LINE_STRIP:
		return PT_LINE_STRIP;
		break;
	case GL_LINE_LOOP:
		return PT_LINE_LOOP;
		break;
	case GL_TRIANGLES:
		return PT_TRIANGLES;
		break;
	case GL_TRIANGLE_STRIP:
		return PT_TRIANGLE_STRIP;
		break;
	case GL_TRIANGLE_FAN:
		return PT_TRIANGLE_FAN;
		break;
	case GL_QUADS:
		return PT_QUADS;
		break;
	case GL_QUAD_STRIP:
		return PT_QUAD_STRIP;
		break;
	case GL_POLYGON:
		return PT_POLYGON;
		break;
	default:
		return PT_POINTS;
		break;
	}
}

