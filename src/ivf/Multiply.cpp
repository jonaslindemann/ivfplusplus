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

#include <ivf/Multiply.h>

#include <ivf/Transform.h>

using namespace ivf;

Multiply::Multiply()
{

}

void Multiply::setShape(Shape* shape)
{
	m_shape = shape;
}

Shape* Multiply::getShape()
{
	return m_shape;
}

void Multiply::setOffsets(double dx, double dy, double dz)
{
	m_offsets[0] = dx;
	m_offsets[1] = dy;
	m_offsets[2] = dz;
}

void Multiply::getOffsets(double &dx, double &dy, double &dz)
{
	dx = m_offsets[0];
	dy = m_offsets[1];
	dz = m_offsets[2];
}

void Multiply::setRepeat(int x, int y, int z)
{
	m_repeat[0] = x;
	m_repeat[1] = y;
	m_repeat[2] = z;
}

void Multiply::getRepeat(int &x, int &y, int &z)
{
	x = m_repeat[0];
	y = m_repeat[1];
	z = m_repeat[2];
}

void Multiply::setStartIndices(int i, int j, int k)
{
	m_startIndices[0] = i;
	m_startIndices[1] = j;
	m_startIndices[2] = k;
}

void Multiply::getStartIndices(int &i, int &j, int &k)
{
	i = m_startIndices[0];
	j = m_startIndices[1];
	k = m_startIndices[2];
}

void Multiply::doCreateGeometry()
{
	if (m_shape!=nullptr)
	{
		int i, j, k;

		int i0 = m_startIndices[0];
		int j0 = m_startIndices[1];
		int k0 = m_startIndices[2];

		int imax = i0 + m_repeat[0] - 1;
		int jmax = j0 + m_repeat[1] - 1;
		int kmax = k0 + m_repeat[2] - 1;

		TransformPtr xfm = new Transform();
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

void Multiply::refresh()
{
	if (m_shape!=nullptr)
		m_shape->refresh();
}
