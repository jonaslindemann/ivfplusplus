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

// Implementation of: public class CIvfCube

#include <ivf/config.h>
#include <ivf/Cube.h>

using namespace ivf;

// ------------------------------------------------------------
Cube::Cube ()
		:Brick()
		//TODO: check and complete member initialisation list!
{
}

// ------------------------------------------------------------
Cube::~Cube ()
{
}

// ------------------------------------------------------------
void Cube::setSize (const double length)
{
	Brick::setSize(length,length,length);
}

// ------------------------------------------------------------
void Cube::getSize (double &length)
{
	double dval;
	Brick::getSize(dval,dval,dval);
	length = dval;
}


