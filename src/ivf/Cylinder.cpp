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

// Implementation of: public class CIvfCylinder

#include <ivf/config.h>
#include <ivf/Cylinder.h>

using namespace ivf;

// ------------------------------------------------------------
CCylinder::CCylinder ()
		:CCone()
		//TODO: check and complete member initialisation list!
{
	setRadius(1.0);
}

// ------------------------------------------------------------
CCylinder::~CCylinder ()
{
}

// ------------------------------------------------------------
void CCylinder::setRadius (const double radius)
{
	setBottomRadius(radius);
	setTopRadius(radius);
}

// ------------------------------------------------------------
double CCylinder::getRadius ()
{
	return getBottomRadius();
}


//void CIvfCylinder::setHeight(const double height)
//{
//	CIvfCone::setHeight(height);
//}

