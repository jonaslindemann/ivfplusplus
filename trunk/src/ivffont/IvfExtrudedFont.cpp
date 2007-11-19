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

#include <ivffont/IvfExtrudedFont.h>

CIvfExtrudedFont::CIvfExtrudedFont(const char* filename)
:CIvfVectorFont(filename)
{
	// Add construction code here
	m_ftglExtrudedFont = new FTGLExtrdFont(this->getFilename());
	m_ftglExtrudedFont->CharMap(ft_encoding_unicode);
	m_ftglExtrudedFont->FaceSize(1);
	m_ftglExtrudedFont->Depth(0.5);
}

CIvfExtrudedFont::~CIvfExtrudedFont()
{
	delete m_ftglExtrudedFont;
}

void CIvfExtrudedFont::createGeometry()
{
	m_ftglExtrudedFont->Render(this->getText());
}

void CIvfExtrudedFont::doSetFaceSize(const unsigned int faceSize)
{
	m_ftglExtrudedFont->FaceSize(faceSize);
}

void CIvfExtrudedFont::doSetDepth(const double depth)
{
	m_ftglExtrudedFont->Depth((double)depth);
}

void CIvfExtrudedFont::doSetRealFaceSize(const double size)
{
	this->setFaceSize(48);
	double scaleFactor = size/48.0;
	this->setScale(scaleFactor, scaleFactor, 1.0);
	this->setNormalize(true);
}

void CIvfExtrudedFont::doGetSize(double & xsize, double & ysize, double & zsize)
{
	float llx, lly, llz, urx, ury, urz;
	m_ftglExtrudedFont->BBox(this->getText(), llx, lly, llz, urx, ury, urz);
	xsize = (double)urx - (double)llx;
	ysize = (double)ury - (double)lly;
	zsize = (double)urz - (double)llz;
}
