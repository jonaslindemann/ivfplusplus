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

#include <ivf/SplineTube.h>

using namespace ivf;

CSplineTube::CSplineTube()
{
	m_spline = nullptr;
	m_resolution = 0.1;
}

CSplineTube::~CSplineTube()
{

}

void CSplineTube::setSpline(CSpline3d *spline)
{
	m_spline = spline;
}

CSpline3d* CSplineTube::getSpline()
{
	return m_spline;
}

void CSplineTube::setPrecision(double resolution)
{
	m_resolution = resolution;
}

void CSplineTube::refresh()
{
	if (m_spline!=nullptr)
	{
		double t = 0.0;
		int i;
		double x, y, z;

		double nPoints = (double)m_spline->getSize()/m_resolution;
		int iPoints = (int)nPoints + 1;
		CVec3d p;
		
		setSpineSize(iPoints);

		for (i=0; i<iPoints; i++)
		{
			p = m_spline->getPosition(t);
			p.getComponents(x, y, z);
			setSpineCoord(i, x, y, z);
			t+=m_resolution;
		}
	}
	CTubeExtrusion::refresh();
}

void CSplineTube::setSides(int sides)
{
	CTubeExtrusion::setSides(sides);
	this->refresh();
}
