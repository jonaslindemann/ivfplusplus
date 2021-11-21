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

// Implementation of: public class COldGrid

#include <ivf/config.h>
#include <ivf/OldGrid.h>

using namespace ivf;


// ------------------------------------------------------------
COldGrid::COldGrid ()
		:CSimpleLineSet()
		//TODO: check and complete member initialisation list!
{
	m_rows = 200;
	m_cols = 200;
	m_xUnit = 0.1;
	m_yUnit = 0.1;
	m_xCenter = ((double)m_cols/2.0)*m_xUnit;
	m_yCenter = ((double)m_rows/2.0)*m_yUnit;
	m_xMark = 10;
	m_yMark = 10;
	m_markColor[0] = 0.6;
	m_markColor[1] = 0.6;
	m_markColor[2] = 0.6;
	m_axisColor[0] = 1.0;
	m_axisColor[1] = 1.0;
	m_axisColor[2] = 1.0;
	m_gridColor[0] = 0.4;
	m_gridColor[1] = 0.4;
	m_gridColor[2] = 0.4;
	createGrid();
}

// ------------------------------------------------------------
COldGrid::~COldGrid ()
{
}

// ------------------------------------------------------------
void COldGrid::setSize(GLint rows, GLint cols)
{
	m_rows = rows;
	m_cols = cols;
	m_xCenter = ((double)m_cols/2.0)*m_xUnit;
	m_yCenter = ((double)m_rows/2.0)*m_yUnit;
	doUpdateBoundingSphere();
	createGrid();
}

// ------------------------------------------------------------
void COldGrid::setUnits(GLdouble x, GLdouble y)
{
	m_xUnit = x;
	m_yUnit = y;
	m_xCenter = ((double)m_cols/2.0)*m_xUnit;
	m_yCenter = ((double)m_rows/2.0)*m_yUnit;
	createGrid();
}

// ------------------------------------------------------------
void COldGrid::createGrid()
{
	int i;
	int k;
	setCoordSize((m_rows+1)*2 + (m_cols+1)*2);
	setColorSize((m_rows+1)*2 + (m_cols+1)*2);
	setLineSize((m_rows+1)*3 + (m_cols+1)*3);

	k = 0;
	for (i = 0; i<=m_rows; i++)
	{
		setCoord(i * 2,     -m_xCenter, 0.0, i*m_yUnit - m_yCenter);

		if ((i%m_yMark)==0)
			setColor(i * 2, m_markColor[0],m_markColor[1],m_markColor[2]);
		else
			setColor(i * 2, m_gridColor[0],m_gridColor[1],m_gridColor[2]);

		if (i==m_rows/2)
			setColor(i * 2, m_axisColor[0],m_axisColor[1],m_axisColor[2]);

		setCoord(i * 2 + 1, m_xUnit*m_cols - m_xCenter, 0.0, i*m_yUnit - m_yCenter);

		if ((i%m_yMark)==0)
			setColor(i * 2 + 1, m_markColor[0],m_markColor[1],m_markColor[2]);
		else
			setColor(i * 2 + 1, m_gridColor[0],m_gridColor[1],m_gridColor[2]);

		if (i==m_rows/2)
			setColor(i * 2 + 1, m_axisColor[0],m_axisColor[1],m_axisColor[2]);

		setLineIndex(k, i*2);
		k++;
		setLineIndex(k, i*2 + 1);
		k++;
		setLineIndex(k, -1);
		k++;
	}

	for (i = 0; i<=m_cols; i++)
	{
		setCoord(i * 2 + m_rows*2+2, i*m_xUnit - m_xCenter, 0.0, - m_yCenter);

		if ((i%m_xMark)==0)
			setColor(i * 2 + m_rows*2+2, m_markColor[0],m_markColor[1],m_markColor[2]);
		else
			setColor(i * 2 + m_rows*2+2, m_gridColor[0],m_gridColor[1],m_gridColor[2]);

		if (i==m_cols/2)
			setColor(i * 2 + m_rows*2+2, m_axisColor[0],m_axisColor[1],m_axisColor[2]);

		setCoord(i * 2 + 1 + m_rows*2+2, i*m_xUnit - m_xCenter, 0.0, m_rows*m_yUnit - m_yCenter);

		if ((i%m_xMark)==0)
			setColor(i * 2 + 1 + m_rows*2+2, m_markColor[0],m_markColor[1],m_markColor[2]);
		else
			setColor(i * 2 + 1 + m_rows*2+2, m_gridColor[0],m_gridColor[1],m_gridColor[2]);

		if (i==m_cols/2)
			setColor(i * 2 + + 1+ m_rows*2+2, m_axisColor[0],m_axisColor[1],m_axisColor[2]);

		setLineIndex(k, i*2 + m_rows*2+2);
		k++;
		setLineIndex(k, i*2 + 1 + m_rows*2+2);
		k++;
		setLineIndex(k, -1);
		k++;
	}
}

// ------------------------------------------------------------
void COldGrid::setMark(int x, int y)
{
	m_xMark = x;
	m_yMark = y;
}

void COldGrid::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		double xSize, ySize;
		double radius;

		xSize = m_cols*m_xUnit;
		ySize = m_rows*m_yUnit;

		radius = sqrt(pow(xSize,2) + pow(ySize,2)) / 2.0;
		getBoundingSphere()->setRadius(radius);
	}
}
