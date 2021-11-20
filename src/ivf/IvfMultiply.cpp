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

#include <ivf/IvfMultiply.h>

#include <ivf/IvfTransform.h>

CMultiply::CMultiply()
{

}

void CMultiply::setShape(CShape* shape)
{
	m_shape = shape;
}

CShape* CMultiply::getShape()
{
	return m_shape;
}

void CMultiply::setOffsets(double dx, double dy, double dz)
{
	m_offsets[0] = dx;
	m_offsets[1] = dy;
	m_offsets[2] = dz;
}

void CMultiply::getOffsets(double &dx, double &dy, double &dz)
{
	dx = m_offsets[0];
	dy = m_offsets[1];
	dz = m_offsets[2];
}

void CMultiply::setRepeat(int x, int y, int z)
{
	m_repeat[0] = x;
	m_repeat[1] = y;
	m_repeat[2] = z;
}

void CMultiply::getRepeat(int &x, int &y, int &z)
{
	x = m_repeat[0];
	y = m_repeat[1];
	z = m_repeat[2];
}

void CMultiply::setStartIndices(int i, int j, int k)
{
	m_startIndices[0] = i;
	m_startIndices[1] = j;
	m_startIndices[2] = k;
}

void CMultiply::getStartIndices(int &i, int &j, int &k)
{
	i = m_startIndices[0];
	j = m_startIndices[1];
	k = m_startIndices[2];
}

void CMultiply::doCreateGeometry()
{
	if (m_shape!=NULL)
	{
		int i, j, k;

		int i0 = m_startIndices[0];
		int j0 = m_startIndices[1];
		int k0 = m_startIndices[2];

		int imax = i0 + m_repeat[0] - 1;
		int jmax = j0 + m_repeat[1] - 1;
		int kmax = k0 + m_repeat[2] - 1;

		CTransformPtr xfm = new CTransform();
		xfm->addChild(m_shape);

		for (i=i0; i<=imax; i++)
			for (j=j0; j<=jmax; j++)
				for (k=k0; k<=kmax; k++)
				{
					xfm->setPosition(
						(double)i*m_offsets[0],
						(double)j*m_offsets[1],
						(double)k*m_offsets[2]
						);
					xfm->render();
				}

	}
}

void CMultiply::refresh()
{
	if (m_shape!=NULL)
		m_shape->refresh();
}
