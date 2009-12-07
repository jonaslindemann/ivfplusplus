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

// Implementation of: public class CIvfSimpleLineSet

#include <ivf/ivfconfig.h>
#include <ivf/IvfSimpleLineSet.h>



// ------------------------------------------------------------
CIvfSimpleLineSet::CIvfSimpleLineSet ()
		:CIvfShape()
{
	m_nCoords = 0;
	m_nCoordIndex = 0;
	m_nColors = 0;
	m_coords = NULL;
	m_coordIndex = NULL;
	m_colors = NULL;
}

// ------------------------------------------------------------
CIvfSimpleLineSet::~CIvfSimpleLineSet ()
{
	if (m_coordIndex!=NULL)
		delete [] m_coordIndex;
	if (m_coords!=NULL)
		delete [] m_coords;
	if (m_colors!=NULL)
		delete [] m_colors;
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::doCreateGeometry()
{
	int i;
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<m_nCoordIndex; i++)
	{
		if ((m_colors!=NULL)&&(m_coordIndex[i]>=0))
		{
			glColor3d(
				m_colors[ m_coordIndex[i] ][0],
				m_colors[ m_coordIndex[i] ][1],
				m_colors[ m_coordIndex[i] ][2]);
		}
		if (m_coordIndex[i] == -1)
		{
			glEnd();
			if (i != m_nCoordIndex-1)
				glBegin(GL_LINE_STRIP);
		}
		else
			glVertex3dv(m_coords[ m_coordIndex[i] ]);
	}
	glPopAttrib();
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setCoordSize(int n)
{
	m_nCoords = n;
	if (m_coords!=NULL)
		delete [] m_coords;
	m_coords = new double[m_nCoords][3];
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setLineSize(int n)
{
	m_nCoordIndex = n;
	if (m_coordIndex!=NULL)
		delete [] m_coordIndex;
	m_coordIndex = new int[m_nCoordIndex];
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setCoord(int n, double x, double y, double z)
{
	if (m_coords!=NULL)
	{
		if (n<m_nCoords)
		{
			m_coords[n][0] = x;
			m_coords[n][1] = y;
			m_coords[n][2] = z;
		}
	}
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setLineIndex(int n, int idx)
{
	if (m_coordIndex!=NULL)
	{
		if (n<m_nCoordIndex)
		{
			m_coordIndex[n] = idx;
		}
	}
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setColorSize(int n)
{
	m_nColors = n;
	if (m_colors!=NULL)
		delete [] m_colors;
	m_colors = new double[m_nColors][3];
}

// ------------------------------------------------------------
void CIvfSimpleLineSet::setColor(int n, double red, double green, double blue)
{
	if (m_colors!=NULL)
	{
		if (n<m_nColors)
		{
			m_colors[n][0] = red;
			m_colors[n][1] = blue;
			m_colors[n][2] = green;
		}
	}
}

