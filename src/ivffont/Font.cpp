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

#include <ivffont/IvfFont.h>

#include <iostream>

using namespace std;

CIvfFont::CIvfFont(const std::string& filename)
{
	// Add construction code here

	m_filename = filename;
	m_text = "Ivf++";
}

CIvfFont::~CIvfFont()
{
}

const std::string CIvfFont::getFilename(void)
{
	return m_filename;
}

const std::string CIvfFont::getText(void)
{
	return m_text;
}

void CIvfFont::setText(const std::string& text)
{
	m_text = text;
	this->doSetText(text);
}

void CIvfFont::setFaceSize(const unsigned int faceSize)
{
	m_faceSize = faceSize;
	this->doSetFaceSize(faceSize);
}

unsigned int CIvfFont::getFaceSize(void)
{
	return m_faceSize;
}

void CIvfFont::setDepth(const double depth)
{
	m_depth = depth;
	this->doSetDepth(depth);
}

double CIvfFont::getDepth(void)
{
	return m_depth;
}

void CIvfFont::doSetText(const std::string& text)
{
}

void CIvfFont::doSetFaceSize(const unsigned int faceSize)
{
}

void CIvfFont::doSetDepth(const double depth)
{
}

void CIvfFont::getSize(double & xsize, double & ysize, double & zsize)
{

}
