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

#include <ivf/IvfViewport.h>

#include <ivf/IvfGL.h>

CViewport::CViewport()
{
	m_pos[0] = 0;
	m_pos[1] = 0;
	m_size[0] = 100;
	m_size[1] = 100;
}

CViewport::~CViewport()
{

}

void CViewport::setPosition(int x, int y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

void CViewport::setSize(int width, int height)
{
	m_size[0] = width;
	m_size[1] = height;
}

void CViewport::getPosition(int &x, int &y)
{
	x = m_pos[0];
	y = m_pos[1];
}

void CViewport::getSize(int &width, int &height)
{
	width = m_size[0];
	height = m_size[1];
}

void CViewport::apply()
{
	glViewport(m_pos[0], m_pos[1], m_size[0], m_size[1]);
}

double CViewport::getAspect()
{
	if (m_size[1]==0)		  
		return m_size[0]-m_pos[0];
	else		
		return (double)(m_size[0])/(double)(m_size[1]);
}
