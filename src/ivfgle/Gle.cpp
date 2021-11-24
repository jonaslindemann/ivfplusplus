//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#include <ivfgle/Gle.h>

#include <GL/gle.h>

using namespace ivf;

Gle* Gle::m_instance = 0;
SingletonDestroyer<Gle> Gle::m_destroyer;

Gle* Gle::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new Gle(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

Gle::Gle()
{
	// Add protected construction code here
}

void Gle::setNumSides(int sides)
{
	gleSetNumSides(sides);	
}

int Gle::getNumSides()
{
	return gleGetNumSides();	
}

void Gle::setJoinStyle(int style)
{
	gleSetJoinStyle(style);
}

int Gle::getJoinStyle()
{
	return gleGetJoinStyle();
}
