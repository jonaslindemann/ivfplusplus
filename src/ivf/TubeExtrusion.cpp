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

// Implementation of: public class CTubeExtrusion

#include <ivf/config.h>
#include <ivf/TubeExtrusion.h>
#include <ivfmath/Vec3d.h>

using namespace ivf;

// ------------------------------------------------------------
CTubeExtrusion::CTubeExtrusion ()
		:CExtrusion()
		//TODO: check and complete member initialisation list!
{
	m_calcFirstLast = TRUE;
	m_nSides = 8;
	m_radius = 0.1;
}

// ------------------------------------------------------------
CTubeExtrusion::~CTubeExtrusion ()
{
}

// ------------------------------------------------------------
void CTubeExtrusion::refresh()
{
	initExtrusion();
}

// ------------------------------------------------------------
void CTubeExtrusion::setCalcFirstLast(int flag)
{
	m_calcFirstLast = flag;
}

// ------------------------------------------------------------
void CTubeExtrusion::setSides(long n)
{
	m_nSides = n;
	initExtrusion();
}

// ------------------------------------------------------------
void CTubeExtrusion::setRadius(double radius)
{
	m_radius = radius;
	initExtrusion();
}

// ------------------------------------------------------------
void CTubeExtrusion::initExtrusion()
{
	int i, nSides;
	double r, angle, x, y;
	
	// Create section

	nSides = m_nSides;
	r = m_radius;
	
	setSectionSize(nSides + 1);
	
	for (i = 0; i<=nSides; i++)
	{
		angle = 2.0*M_PI*( ((double)i) / ((double)nSides) );
		x = r * cos(angle);
		y = r * sin(angle);
		setSectionCoord(i, x, y);
		setSectionNormal(i, x/r, y/r);
	}
	
	// Calculate start and endpoint

	if (m_calcFirstLast==TRUE)
	{
		
		CVec3d p1, p2;
		CVec3d v1;
		double x, y, z, ex, ey, ez;
		
		this->getSpineCoord(1, x, y, z);
		p1.setComponents(x, y, z);
		this->getSpineCoord(2, x, y, z);
		p2.setComponents(x, y, z);
		
		v1.setFromPoints(p1, p2);
		v1.normalize();
		v1.getComponents(ex, ey, ez);
		
		if ( (ex==0)&&(ez==0)&&(ey!=0) )
			setUpVector(1.0, 0.0, 0.0);
		else
			setUpVector(0.0, 1.0, 0.0);
		
		// First point
		
		p1.getComponents(x, y, z);
		x = -0.1 * ex + x;
		y = -0.1 * ey + y;
		z = -0.1 * ez + z;
		setSpineCoord(0, x, y, z);
		
		this->getSpineCoord(this->getSpineSize()-3, x, y, z);
		p1.setComponents(x, y, z);
		this->getSpineCoord(this->getSpineSize()-2, x, y, z);
		p2.setComponents(x, y, z);
		
		v1.setFromPoints(p1, p2);
		v1.normalize();
		v1.getComponents(ex, ey, ez);
		
		if ( (ex==0)&&(ez==0)&&(ey!=0) )
			setUpVector(1.0, 0.0, 0.0);
		else
			setUpVector(0.0, 1.0, 0.0);
		
		// First point
		
		p1.getComponents(x, y, z);
		x = 0.1 * ex + x;
		y = 0.1 * ey + y;
		z = 0.1 * ez + z;
		setSpineCoord(this->getSpineSize()-1, x, y, z);
	}
	
}

