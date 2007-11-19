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

#include "IvfColorMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIvfColorMap::CIvfColorMap()
{

}

CIvfColorMap::~CIvfColorMap()
{
	this->clear();
}

void CIvfColorMap::open(char *fname)
{
	using namespace std;

	fstream f;
	int nColors;
	int i;
	float r, g, b;
	CIvfColorPtr color;

	// Clear previous color map
	
	this->clear();

	// Open color map file

	f.open(fname, ios::in);

	// Read colors
	
	f >> nColors;

	for (i=0; i<nColors; i++)
	{
		f >> r >> g >> b;
		color = new CIvfColor();
		color->setColor(r, g, b);
		m_colors.push_back(color);
	}

	// Close file

	f.close();
}

void CIvfColorMap::clear()
{
	m_colors.clear();
}

void CIvfColorMap::getColor(double value, float &r, float &g, float &b)
{
	if (m_colors.size()>0)
	{
		double clampedValue;
		int colorIndex;
		CIvfColor* color;

		// Clamp value

		clampedValue = value;
		
		if (value>1)
			clampedValue = 1.0;

		if (value<0)
			clampedValue = 0.0;

		colorIndex = (int)( (double)(m_colors.size()-1)*clampedValue );

		color = m_colors[colorIndex];
		color->getColor(r, g, b);
	}
	else
		r = g = b = 1.0;
}
