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

#include <ivfgle/GlePolyCylinder.h>

using namespace ivf;

GlePolyCylinder::GlePolyCylinder()
{
	m_radius = 0.1;
}

void GlePolyCylinder::doCreateGeometry()
{
	if ((m_pointArray!=nullptr)&&(m_colorArray==nullptr))
	{
		gleDouble (*temp)[3];
		temp = (double (*)[3])m_pointArray->getData();

		glePolyCylinder(
			m_pointArray->getSize(),
			temp,
			nullptr,
			m_radius
		);
	}

	if ((m_pointArray!=nullptr)&&(m_colorArray!=nullptr))
	{
		if (m_pointArray->getSize()==m_colorArray->getSize())
		{
			gleDouble (*temp1)[3];
			temp1 = (double (*)[3])m_pointArray->getData();

			float (*temp2)[3];
			temp2 = (float (*)[3])m_colorArray->getData();

			glEnable(GL_COLOR_MATERIAL);

			glePolyCylinder(
				m_pointArray->getSize(),
				temp1,
				temp2,
				m_radius
			);

			glDisable(GL_COLOR_MATERIAL);

		}
	}
}

void GlePolyCylinder::setRadius(double radius)
{
	m_radius = radius;
}

double GlePolyCylinder::getRadius()
{
	return m_radius;
}
