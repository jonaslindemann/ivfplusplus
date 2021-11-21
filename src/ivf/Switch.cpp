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

// Implementation of: public class CSwitch

#include <ivf/Switch.h>

using namespace ivf;

// ------------------------------------------------------------
CSwitch::CSwitch ()
		:CComposite()
{
	m_currentChild = 0;
}

// ------------------------------------------------------------
CSwitch::~CSwitch ()
{
}

// ------------------------------------------------------------
void CSwitch::doCreateGeometry()
{
	if (this->getSize()>0)
	{
		CShape* shape = this->getChild(m_currentChild);
		shape->render();
	}
}

// ------------------------------------------------------------
void CSwitch::setCurrentChild(int child)
{
	if ((m_currentChild>=0)&&(m_currentChild<this->getSize()))
	{
		m_currentChild = child;
	}
}

// ------------------------------------------------------------
int CSwitch::getCurrentChild()
{
	return m_currentChild;
}

// ------------------------------------------------------------
void CSwitch::cycleForward()
{
	if (m_currentChild<this->getSize()-1)
		m_currentChild++;
	else
		m_currentChild = 0;
}

// ------------------------------------------------------------
void CSwitch::cycleBackwards()
{
	if (m_currentChild>0)
		m_currentChild--;
	else
		m_currentChild = this->getSize()-1;
}

