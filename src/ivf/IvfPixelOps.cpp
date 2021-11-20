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

#include <ivf/IvfPixelOps.h>

#include <ivf/IvfGL.h>

CPixelOps* CPixelOps::m_instance = 0;
CSingletonDestroyer<CPixelOps> CPixelOps::m_destroyer;

CPixelOps* CPixelOps::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CPixelOps(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CPixelOps::CPixelOps()
{

}

void CPixelOps::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void CPixelOps::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

bool CPixelOps::isDepthTestEnabled()
{
	GLboolean depthTest;
	glGetBooleanv(GL_DEPTH_TEST, &depthTest);
	return (bool)depthTest;
}
