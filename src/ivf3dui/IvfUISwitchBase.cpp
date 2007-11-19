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

#include <ivf3dui/IvfUISwitchBase.h>

CIvfUISwitchBase::CIvfUISwitchBase()
{
	m_state = false;
}

CIvfUISwitchBase::~CIvfUISwitchBase()
{

}

void CIvfUISwitchBase::doControlClick(CIvfVec3d vec, int button)
{

}

void CIvfUISwitchBase::doControlOver(CIvfVec3d vec)
{
	this->setHighlight(CIvfShape::HS_ON);
}

void CIvfUISwitchBase::doControlLeave(CIvfVec3d vec)
{
	this->setHighlight(CIvfShape::HS_OFF);
}

void CIvfUISwitchBase::on()
{
	if (!m_state)
	{
		m_state = true;
		doStateChange(m_state);
	}
}

void CIvfUISwitchBase::off()
{
	if (m_state)
	{
		m_state = false;
		doStateChange(m_state);
	}
}

bool CIvfUISwitchBase::isOn()
{
	return m_state;
}

bool CIvfUISwitchBase::isOff()
{
	return !m_state;
}

bool CIvfUISwitchBase::getState()
{
	return m_state;
}

void CIvfUISwitchBase::setState(bool state)
{
	if (m_state!=state)
	{
		m_state = state;
		doStateChange(m_state);
	}
}

void CIvfUISwitchBase::doStateChange(bool state)
{

}

void CIvfUISwitchBase::doControlDown(CIvfVec3d vec, int button)
{
	setState(!m_state);
	this->setHighlight(CIvfShape::HS_OFF);
}

void CIvfUISwitchBase::doControlUp(CIvfVec3d vec)
{

}
