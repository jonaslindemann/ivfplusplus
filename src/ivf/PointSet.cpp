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

#include <ivf/PointSet.h>
#include <ivf/LegacyGL.h>

using namespace ivf;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PointSet::PointSet()
{
	m_useColor = false;
	m_pointSize = 1;
}

PointSet::~PointSet()
{

}

void PointSet::doCreateGeometry()
{
	Index* coordIdx;
	Index* colorIdx;
	long i, j;
	int oldSize;

	// Point size has to be applied before the modern path draws, not after --
	// glPointSize is ordinary state that the VAO knows nothing about, and the
	// early return below used to skip it entirely, leaving every point one pixel
	// across. It stays a plain gl call because glPointSize is valid in core.

	glGetIntegerv(GL_POINT_SIZE, &oldSize);
	glPointSize(m_pointSize);

	if (buildAndDrawVAO(GL_POINTS))
	{
		glPointSize((GLfloat)oldSize);
		return;
	}

	lgPushAttrib(GL_LIGHTING|GL_COLOR_MATERIAL);
	lgDisableLegacy(GL_LIGHTING);

	if (m_useColor)
		lgEnableLegacy(GL_COLOR_MATERIAL);

	for (i=0; i<(int)m_coordIndexSet.size(); i++)
	{
		lgBegin(GL_POINTS);

		coordIdx = m_coordIndexSet[i];
		if (m_useColor)
		{
			if (i<(int)m_colorIndexSet.size())
				colorIdx = m_colorIndexSet[i];
		}


		for (j=0; j<coordIdx->getSize(); j++)
		{
			if (m_useColor)
				lgColor3fv(m_colorSet[colorIdx->getIndex(j)]->getColor());
			else
				if (Shape::getMaterial()!=nullptr)
					Shape::getMaterial()->render();
				else
					lgColor3f(1.0f, 1.0f, 1.0f);

			lgVertex3dv(m_coordSet[coordIdx->getIndex(j)]->getComponents());
		}
		lgEnd();
	}

	glPointSize(oldSize);

	lgPopAttrib();
}

void PointSet::setUseColor(bool flag)
{
	m_useColor = flag;
}

bool PointSet::getUseColor()
{
	return m_useColor;
}

void PointSet::setPointSize(int size)
{
	m_pointSize = size;
}

int PointSet::getPointSize()
{
	return m_pointSize;
}
