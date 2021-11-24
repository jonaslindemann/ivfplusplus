//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "CoordList.h"

using namespace ivf;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CoordList::CoordList()
{

}

CoordList::~CoordList()
{

}

void CoordList::readFromStream(std::istream &in)
{

}

void CoordList::saveToStream(std::ostream &out)
{

}

void CoordList::addCoord(double x, double y, double z)
{

}

void CoordList::addCoord(double x, double y, double z, float r, float g, float b)
{

}

void CoordList::addCoord(Vec3d* coord)
{

}

void CoordList::setColor(long idx, float r, float g, float b)
{

}

void CoordList::getColor(long idx, float &r, float &g, float &b)
{

}
	
void CoordList::getCoord(long idx, double &x, double &y, double &z)
{

}

Vec3d* CoordList::getCoord(long idx)
{
	return nullptr;
}

long CoordList::getSize()
{
	return -1;
}

void CoordList::clear()
{

}

void CoordList::setCoord(int idx, double x, double y, double z)
{
	
}
