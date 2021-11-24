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

#include "VectorCoordList.h"

using namespace ivf;

VectorCoordList::VectorCoordList()
{

}

VectorCoordList::~VectorCoordList()
{
	this->clear();
}

void VectorCoordList::addCoord(Vec3d *coord)
{
	ColorPtr color = new Color();
	m_coords.push_back(Vec3dPtr(coord));
	m_colors.push_back(color);
}

void VectorCoordList::addCoord(double x, double y, double z)
{
	ColorPtr color = new Color();
	Vec3dPtr coord = new Vec3d();
	coord->setComponents(x, y, z);
	m_coords.push_back(coord);
	m_colors.push_back(color);
}

Vec3d* VectorCoordList::getCoord(long idx)
{
	return m_coords[idx];
}

void VectorCoordList::getCoord(long idx, double &x, double &y, double &z)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_coords[idx]->getComponents(x, y, z);
}

void VectorCoordList::clear()
{
	m_coords.clear();
	m_colors.clear();
}

long VectorCoordList::getSize()
{
	return static_cast<int>(m_coords.size());
}

void VectorCoordList::setColor(long idx, float r, float g, float b)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_colors[idx]->setColor(r, g, b);
}

void VectorCoordList::addCoord(double x, double y, double z, float r, float g, float b)
{
	ColorPtr color = new Color();
	Vec3dPtr coord = new Vec3d();
	coord->setComponents(x, y, z);
	color->setColor(r, g, b);
	m_coords.push_back(coord);
	m_colors.push_back(color);
}

void VectorCoordList::getColor(long idx, float &r, float &g, float &b)
{
	if ((idx>=0)&&(idx<m_coords.size()))
		m_colors[idx]->getColor(r, g, b);	
}

void VectorCoordList::saveToStream(std::ostream &out)
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

void VectorCoordList::readFromStream(std::istream &in)
{
	using namespace std;
	
	long nCoords;
	long i;
	double x, y, z;
	float r, g, b;
	Vec3dPtr point;

	this->clear();

	in >> nCoords;

	for (i=0; i<nCoords; i++)
	{
		in >> x >> y >> z;
		in >> r >> g >> b;
		point = new Vec3d();
		point->setComponents(x, y, z);
		m_coords.push_back(point);
	}
}
