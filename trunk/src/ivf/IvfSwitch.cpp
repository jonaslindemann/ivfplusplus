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

// Implementation of: public class CIvfSwitch

#include <ivf/IvfSwitch.h>

// ------------------------------------------------------------
CIvfSwitch::CIvfSwitch ()
		:CIvfComposite()
{
	m_currentChild = 0;
}

// ------------------------------------------------------------
CIvfSwitch::~CIvfSwitch ()
{
}

// ------------------------------------------------------------
void CIvfSwitch::createGeometry()
{
	if (this->getSize()>0)
	{
		CIvfShape* shape = this->getChild(m_currentChild);
		shape->render();
	}
}

// ------------------------------------------------------------
void CIvfSwitch::setCurrentChild(int child)
{
	if ((m_currentChild>=0)&&(m_currentChild<this->getSize()))
	{
		m_currentChild = child;
	}
}

// ------------------------------------------------------------
int CIvfSwitch::getCurrentChild()
{
	return m_currentChild;
}

// ------------------------------------------------------------
void CIvfSwitch::cycleForward()
{
	if (m_currentChild<this->getSize()-1)
		m_currentChild++;
	else
		m_currentChild = 0;
}

// ------------------------------------------------------------
void CIvfSwitch::cycleBackwards()
{
	if (m_currentChild>0)
		m_currentChild--;
	else
		m_currentChild = this->getSize()-1;
}

