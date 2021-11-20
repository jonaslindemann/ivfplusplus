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

#include <ivf/IvfColor.h>

// ------------------------------------------------------------
// ------------------------------------------------------------

CColor::CColor()
{
	this->setColor(0.8f, 0.8f, 0.8f);
	this->setAlfa(1.0);
}

// ------------------------------------------------------------
CColor::~CColor()
{

}

// ------------------------------------------------------------
void CColor::setColor(float red, float green, float blue)
{
	m_color[0] = red;
	m_color[1] = green;
	m_color[2] = blue;
}

// ------------------------------------------------------------
void CColor::getColor(float &red, float &green, float &blue)
{
	red = m_color[0];
	green = m_color[1];
	blue = m_color[2];
}

// ------------------------------------------------------------
const float* CColor::getColor()
{
	return &m_color[0];
}

// ------------------------------------------------------------
void CColor::setAlfa(float alfa)
{
	m_color[3] = alfa;
}

// ------------------------------------------------------------
float CColor::getAlfa()
{
	return m_color[3];
}
