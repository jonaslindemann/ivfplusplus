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

#include <ivffont/IvfText.h>

CIvfText::CIvfText()
{
	m_text = "Ivf++";
}

void CIvfText::createGeometry()
{
	m_font->setText(m_text.c_str());
	m_font->render();
}

void CIvfText::setFont(CIvfFont* font)
{
	m_font = font;
}

CIvfFont* CIvfText::getFont(void)
{
	return m_font;
}

void CIvfText::setText(const char* text)
{
	m_text = text;
	if (m_font!=NULL)
		m_font->setText(m_text.c_str());
}

const char* CIvfText::getText(void)
{
	return m_text.c_str();
}

void CIvfText::getSize(double & xsize, double & ysize, double & zsize)
{
	m_font->getSize(xsize, ysize, zsize);
}
