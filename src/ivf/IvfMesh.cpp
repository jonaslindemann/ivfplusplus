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

#include <ivf/IvfMesh.h>

CMesh::CMesh()
{
	m_controlPoints = NULL;
	m_initDone = false;
	m_meshRows = 20;
	m_meshCols = 20;
	m_meshType = MT_ORDER_4;
	m_meshOrientation = MO_CW;
	this->setSize(4,4);
}

CMesh::~CMesh()
{
	if (m_controlPoints!=NULL)
		delete [] m_controlPoints;
}

void CMesh::setSize(int xSize, int ySize)
{
	m_xSize = xSize;
	m_ySize = ySize;
	this->initialize();
}

void CMesh::getSize(int &xSize, int &ySize)
{
	xSize = m_xSize;
	ySize = m_ySize;
}

void CMesh::initialize()
{
	if (m_controlPoints!=NULL)
		delete [] m_controlPoints;

	m_controlPoints = new GLdouble[m_ySize*m_xSize*3];

	m_initDone = false;
}

void CMesh::doCreateGeometry()
{

	glPushAttrib(GL_LIGHTING|GL_AUTO_NORMAL|GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	switch (m_meshOrientation) {
	case MO_CW:
		glFrontFace(GL_CW);
		break;
	case MO_CCW:
		glFrontFace(GL_CCW);
		break;
	}
	glEnable(GL_NORMALIZE);
	glEnable(GL_MAP2_VERTEX_3);
	switch (m_meshType) {
	case MT_ORDER_2:
		glMap2d(GL_MAP2_VERTEX_3,
			0, 1,  // U ranges 0..1
			3,         // U stride, 3 floats per coord
			2,         // U is 2nd order, ie. linear
			0, 1,  // V ranges 0..1
			m_ySize * 3,     // V stride, row is 2 coords, 3 floats per coord
			2,         // V is 2nd order, ie linear
			m_controlPoints);  // control points
		break;
	case MT_ORDER_4:
		glMap2d(GL_MAP2_VERTEX_3,
			0, 1,  // U ranges 0..1
			3,         // U stride, 3 floats per coord
			4,         // U is 2nd order, ie. linear
			0, 1,  // V ranges 0..1
			m_ySize * 3,     // V stride, row is 2 coords, 3 floats per coord
			4,         // V is 2nd order, ie linear
			m_controlPoints);  // control points
		break;
	default:
		break;
	}

	glMapGrid2d(
		m_meshCols, 0.0, 1.0,
		m_meshRows, 0.0, 1.0
		);

	glEvalMesh2(GL_FILL, 0, m_meshCols, 0, m_meshRows);
	glFrontFace(GL_CCW);
	glPopAttrib();
}

void CMesh::setControlPoint(int i, int j, double x, double y, double z)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		m_controlPoints[3*(j+m_ySize*i)+0] = x;
		m_controlPoints[3*(j+m_ySize*i)+1] = y;
		m_controlPoints[3*(j+m_ySize*i)+2] = z;
		m_initDone = false;
	}
}

void CMesh::getControlPoint(int i, int j, double &x, double &y, double &z)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		x = m_controlPoints[3*(i+m_ySize*j)+0];
		y = m_controlPoints[3*(i+m_ySize*j)+1];
		z = m_controlPoints[3*(i+m_ySize*j)+2];
		m_initDone = false;
	}
}

void CMesh::createMesh(double width, double height)
{
	int i, j;

	for (i=0; i<m_ySize; i++)
		for (j=0; j<m_xSize; j++)
		{
			m_controlPoints[3*(i+m_ySize*j)+0] = width/2.0 - (double)j*width/(double)(m_xSize-1);
			m_controlPoints[3*(i+m_ySize*j)+1] = 0.0;
			m_controlPoints[3*(i+m_ySize*j)+2] = height/2.0 - (double)i*height/(double)(m_ySize-1);
			m_initDone = false;
		}
}

void CMesh::setControlPoint(int i, int j, double y)
{
	if ((i>=0)&&(i<m_ySize)&&(j>=0)&&(j<m_xSize))
	{
		m_controlPoints[3*(i+m_ySize*j)+1] = y;
		m_initDone = false;
	}
}

void CMesh::setMeshType(TMeshType meshType)
{
	m_meshType = meshType;

	switch (m_meshType) {
	case MT_ORDER_2:
		m_xSize = 2;
		m_ySize = 2;
		initialize();
		break;
	case MT_ORDER_4:
		m_xSize = 4;
		m_ySize = 4;
		initialize();
		break;
	default:

		break;
	}
}

void CMesh::setMeshResolution(int rows, int cols)
{
	m_meshRows = rows;
	m_meshCols = cols;
}

void CMesh::setMeshOrientation(TMeshOrientation meshOrientation)
{
	m_meshOrientation = meshOrientation;
}
