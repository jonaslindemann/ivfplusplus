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

#include "IvfCoordList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIvfCoordList::CIvfCoordList()
{

}

CIvfCoordList::~CIvfCoordList()
{

}

void CIvfCoordList::readFromStream(std::istream &in)
{

}

void CIvfCoordList::saveToStream(std::ostream &out)
{

}

void CIvfCoordList::addCoord(double x, double y, double z)
{

}

void CIvfCoordList::addCoord(double x, double y, double z, float r, float g, float b)
{

}

void CIvfCoordList::addCoord(CIvfVec3d* coord)
{

}

void CIvfCoordList::setColor(long idx, float r, float g, float b)
{

}

void CIvfCoordList::getColor(long idx, float &r, float &g, float &b)
{

}
	
void CIvfCoordList::getCoord(long idx, double &x, double &y, double &z)
{

}

CIvfVec3d* CIvfCoordList::getCoord(long idx)
{
	return NULL;
}

long CIvfCoordList::getSize()
{
	return -1;
}

void CIvfCoordList::clear()
{

}

void CIvfCoordList::setCoord(int idx, double x, double y, double z)
{
	
}
