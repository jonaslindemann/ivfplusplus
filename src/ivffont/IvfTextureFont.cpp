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

#include <ivffont/IvfTextureFont.h>

CIvfTextureFont::CIvfTextureFont(const std::string& filename)
:CIvfVectorFont(filename)
{
	m_ftglTextureFont = new FTGLTextureFont(this->getFilename().c_str());
	m_ftglTextureFont->CharMap(ft_encoding_unicode);
	m_ftglTextureFont->FaceSize(1,72);
	m_resolution = 72;
}

CIvfTextureFont::~CIvfTextureFont()
{
	delete m_ftglTextureFont;
}

void CIvfTextureFont::doCreateGeometry()
{
	m_ftglTextureFont->Render(this->getText().c_str());
}

void CIvfTextureFont::doSetFaceSize(const unsigned int size)
{
	m_ftglTextureFont->FaceSize(size,72);
}

void CIvfTextureFont::doSetRealFaceSize(const double size)
{
	const unsigned int fontSize = 48;
	double scaleFactor = 72.0*size/(double)fontSize/(double)m_resolution;
	m_ftglTextureFont->FaceSize(fontSize, m_resolution);
	this->setScale(scaleFactor, scaleFactor, scaleFactor);
	this->setNormalize(true);
}

void CIvfTextureFont::setResolution(const unsigned int resolution)
{
	m_resolution = resolution;
	this->setRealFaceSize(this->getRealFaceSize());
}
