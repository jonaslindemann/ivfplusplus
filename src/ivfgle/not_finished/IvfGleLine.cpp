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

#include <ivfgle/IvfGleLine.h>

CIvfGleLine::CIvfGleLine()
{
	m_nSections = 2;
	
	m_colorArray = new CIvfGleColorArray(m_nSections+2);
	m_coordArray = new CIvfGleCoordArray(m_nSections+2);
	m_radiusArray = new CIvfGleScalarArray(m_nSections+2);
	m_twistArray = new CIvfGleScalarArray(m_nSections+2);
}

CIvfGleLine::~CIvfGleLine()
{
	// Add clean up code here
}

void CIvfGleLine::setSections(int sections)
{
	m_nSections = sections;

	m_colorArray->setSize(m_nSections+2);
	m_coordArray->setSize(m_nSections+2);
	m_radiusArray->setSize(m_nSections+2);
	m_twistArray->setSize(m_nSections+2);

	this->updateLine()
}

void CIvfGleLine::updateLine()
{
	// n = 5
	// P0 ---- | ---- | ---- | ---- P1
	// S0      S1     S2     S3     S4

	CIvfVec3d v;
	CIvfVec3d p;
	double l;
	double dl;

	v = m_endPoint - m_startPoint;
	l = v.length();
	v.normalize();

	p = m_startPoint;

	dl = l / (m_nSections-1);

	int i;

	for (i=1; i<m_coordArray->getSize()-1; i++)
	{
		m_coordArray->setCoord(i, m_startPoint);
		m_startPoint = m_startPoint + dl * v;
	}

	m_coordArray->calcFirstAndLast();
}

void CIvfGleLine::setLineRepresentation(TLineRepresentation lineRep)
{
	m_lineRep = lineRep;
}

void CIvfGleLine::setStartPoint(double x, double y, double z)
{
	m_startPoint.setComponents(x, y, z);
}

void CIvfGleLine::setEndPoint(double x, double y, double z)
{
	m_endPoint.setComponents(x, y, z);
}
