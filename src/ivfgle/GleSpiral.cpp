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

#include <ivfgle/GleSpiral.h>

using namespace ivf;

CGleSpiral::CGleSpiral()
{
	int i, j;

	for (i=0; i<2; i++)
		for (j=0; j<3; j++)
		{
			m_startXfm[i][j] = 0.0;
			m_xfmTheta[i][j] = 0.0;
		}

	m_startXfm[0][0] = 1.0;
	m_startXfm[1][1] = 1.0;
}

void CGleSpiral::doCreateGeometry()
{
	if (m_contour==NULL)
		return;


	gleDouble (*temp2)[2];
	temp2 = (double (*)[2])m_contour->getCoordData();

	gleDouble (*temp3)[2];
	temp3 = (double (*)[2])m_contour->getNormalData();

	gleSpiral(
		m_contour->getSize(),
		temp2,
		temp3,
		m_up,
		1.0,			// Start radius
		-0.1,			// Radius change per rev.
		0.0,            // Start z value
		1.0,            // Change in z per rev
		m_startXfm,
		m_xfmTheta,
		0.0,            // Start angle in x-y plane
		360.0*10       // Degrees to spiral around
	);
}

void CGleSpiral::setContour(CGleContour *contour)
{
	m_contour = contour;
}

void CGleSpiral::setContourUp(double vx, double vy, double vz)
{
	m_up[0] = vx;
	m_up[1] = vy;
	m_up[2] = vz;
}

void CGleSpiral::setContourUp(CVec3d &vec)
{
	double vx, vy, vz;
	vec.getComponents(vx, vy, vz);
	m_up[0] = vx;
	m_up[1] = vy;
	m_up[2] = vz;

}
