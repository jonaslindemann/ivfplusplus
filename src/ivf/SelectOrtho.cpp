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

// IvfSelectOrtho.cpp: implementation of the CSelectOrtho class.
//
//////////////////////////////////////////////////////////////////////

#include <ivf/SelectOrtho.h>

using namespace ivf;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectOrtho::CSelectOrtho()
{

}

CSelectOrtho::~CSelectOrtho()
{

}

void CSelectOrtho::customView()
{
	glOrtho(0.0, m_width, m_height, 0.0, 0.0, 1.0);
}

void CSelectOrtho::initializeSelect(int x, int y, int w, int h)
{
	GLint viewport[4];
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = m_width;
	viewport[3] = m_height;
	gluPickMatrix((GLdouble)(x), (GLdouble)m_height - (GLdouble)(y), (GLdouble)w, (GLdouble)h, viewport);
	this->customView();
}

void CSelectOrtho::setViewport(int width, int height)
{
	m_width = width;
	m_height = height;
}

void CSelectOrtho::getViewport(int &width, int &height)
{
	width = m_width;
	height = m_height;
}
