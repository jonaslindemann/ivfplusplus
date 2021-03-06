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

#include <ivf/IvfPointSet.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIvfPointSet::CIvfPointSet()
{
	m_useColor = false;
	m_pointSize = 1;
}

CIvfPointSet::~CIvfPointSet()
{

}

void CIvfPointSet::doCreateGeometry()
{
	CIvfIndex* coordIdx;
	CIvfIndex* colorIdx;
	long i, j;
	int oldSize;

	glPushAttrib(GL_LIGHTING|GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glGetIntegerv(GL_POINT_SIZE, &oldSize);
	glPointSize(m_pointSize);

	if (m_useColor)
		glEnable(GL_COLOR_MATERIAL);

	for (i=0; i<(int)m_coordIndexSet.size(); i++)
	{
		glBegin(GL_POINTS);

		coordIdx = m_coordIndexSet[i];
		if (m_useColor)
		{
			if (i<(int)m_colorIndexSet.size())
				colorIdx = m_colorIndexSet[i];
		}


		for (j=0; j<coordIdx->getSize(); j++)
		{
			if (m_useColor)
				glColor3fv(m_colorSet[colorIdx->getIndex(j)]->getColor());
			else
				if (CIvfShape::getMaterial()!=NULL)
					CIvfShape::getMaterial()->render();
				else
					glColor3f(1.0f, 1.0f, 1.0f);

			glVertex3dv(m_coordSet[coordIdx->getIndex(j)]->getComponents());
		}
		glEnd();
	}

	glPointSize(oldSize);

	glPopAttrib();
}

void CIvfPointSet::setUseColor(bool flag)
{
	m_useColor = flag;
}

bool CIvfPointSet::getUseColor()
{
	return m_useColor;
}

void CIvfPointSet::setPointSize(int size)
{
	m_pointSize = size;
}

int CIvfPointSet::getPointSize()
{
	return m_pointSize;
}
