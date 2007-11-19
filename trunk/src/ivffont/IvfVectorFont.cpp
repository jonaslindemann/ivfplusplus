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

#include <ivffont/IvfVectorFont.h>

CIvfVectorFont::CIvfVectorFont(const char* filename)
:CIvfFont(filename)
{
	// Add construction code here
}

void CIvfVectorFont::setRealFaceSize(const double size)
{
	m_realFaceSize = size;
	this->doSetRealFaceSize(size);
}

double CIvfVectorFont::getRealFaceSize(void)
{
	return m_realFaceSize;
}

void CIvfVectorFont::doSetRealFaceSize(const double size)
{
}

void CIvfVectorFont::getSize(double & xsize, double & ysize, double & zsize)
{
	this->doGetSize(xsize, ysize, zsize);
}


void CIvfVectorFont::doGetSize(double & xsize, double & ysize, double & zsize)
{
	xsize = 0.0;
	ysize = 0.0;
	zsize = 0.0;
}
