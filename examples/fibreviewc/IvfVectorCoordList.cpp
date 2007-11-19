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

#include "IvfVectorCoordList.h"

CIvfVectorCoordList::CIvfVectorCoordList()
{

}

CIvfVectorCoordList::~CIvfVectorCoordList()
{
	this->clear();
}

void CIvfVectorCoordList::addCoord(CIvfVec3d *coord)
{
	CIvfColorPtr color = new CIvfColor();
	m_coords.push_back(CIvfVec3dPtr(coord));
	m_colors.push_back(color);
}

void CIvfVectorCoordList::addCoord(double x, double y, double z)
{
	CIvfColorPtr color = new CIvfColor();
	CIvfVec3dPtr coord = new CIvfVec3d();
	coord->setComponents(x, y, z);
	m_coords.push_back(coord);
	m_colors.push_back(color);
}

CIvfVec3d* CIvfVectorCoordList::getCoord(long idx)
{
	return m_coords[idx];
}

void CIvfVectorCoordList::getCoord(long idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_coords[idx]->getComponents(x, y, z);
}

void CIvfVectorCoordList::clear()
{
	m_coords.clear();
	m_colors.clear();
}

long CIvfVectorCoordList::getSize()
{
	return m_coords.size();
}

void CIvfVectorCoordList::setColor(long idx, float r, float g, float b)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_colors[idx]->setColor(r, g, b);
}

void CIvfVectorCoordList::addCoord(double x, double y, double z, float r, float g, float b)
{
	CIvfColorPtr color = new CIvfColor();
	CIvfVec3dPtr coord = new CIvfVec3d();
	coord->setComponents(x, y, z);
	color->setColor(r, g, b);
	m_coords.push_back(coord);
	m_colors.push_back(color);
}

void CIvfVectorCoordList::getColor(long idx, float &r, float &g, float &b)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_colors[idx]->getColor(r, g, b);	
}

void CIvfVectorCoordList::saveToStream(std::ostream &out)
{
	using namespace std;
	
	long i;
	double x, y, z;
	float r, g, b;
	out << m_coords.size() << endl;
	
	for (i=0; i<m_coords.size(); i++)
	{
		m_coords[i]->getComponents(x, y, z);
		m_colors[i]->getColor(r, g, b);
		out << x << " " << y << " " << z << endl;
		out << r << " " << g << " " << b << endl;
	}
}

void CIvfVectorCoordList::readFromStream(std::istream &in)
{
	using namespace std;
	
	long nCoords;
	long i;
	double x, y, z;
	float r, g, b;
	CIvfVec3dPtr point;

	this->clear();

	in >> nCoords;

	for (i=0; i<nCoords; i++)
	{
		in >> x >> y >> z;
		in >> r >> g >> b;
		point = new CIvfVec3d();
		point->setComponents(x, y, z);
		m_coords.push_back(point);
	}
}
