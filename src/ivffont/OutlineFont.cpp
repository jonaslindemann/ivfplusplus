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

#include <ivffont/IvfOutlineFont.h>

CIvfOutlineFont::CIvfOutlineFont(const std::string& filename)
:CIvfVectorFont(filename)
{
	m_ftglOutlineFont = new FTGLOutlineFont(this->getFilename().c_str());
	m_ftglOutlineFont->CharMap(ft_encoding_unicode);
	m_ftglOutlineFont->FaceSize(1);
}

CIvfOutlineFont::~CIvfOutlineFont()
{
	delete m_ftglOutlineFont;
}

void CIvfOutlineFont::doCreateGeometry()
{
	m_ftglOutlineFont->Render(this->getText().c_str());
}

void CIvfOutlineFont::doSetFaceSize(const unsigned int size)
{
	m_ftglOutlineFont->FaceSize(size);
}

void CIvfOutlineFont::doSetRealFaceSize(const double size)
{
	this->setFaceSize(48);
	double scaleFactor = size/48.0;
	this->setScale(scaleFactor, scaleFactor, 1.0);
	this->setNormalize(true);
}

void CIvfOutlineFont::doGetSize(double & xsize, double & ysize, double & zsize)
{
	float llx, lly, llz, urx, ury, urz;
	m_ftglOutlineFont->BBox(this->getText().c_str(), llx, lly, llz, urx, ury, urz);
	xsize = (double)urx - (double)llx;
	ysize = (double)ury - (double)lly;
	zsize = (double)urz - (double)llz;
}
