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

#include <ivf3dui/UISwitchBase.h>

using namespace ivf;

CUISwitchBase::CUISwitchBase()
{
	m_state = false;
}

CUISwitchBase::~CUISwitchBase()
{

}

void CUISwitchBase::doControlClick(CVec3d vec, int button)
{

}

void CUISwitchBase::doControlOver(CVec3d vec)
{
	this->setHighlight(CShape::HS_ON);
}

void CUISwitchBase::doControlLeave(CVec3d vec)
{
	this->setHighlight(CShape::HS_OFF);
}

void CUISwitchBase::on()
{
	if (!m_state)
	{
		m_state = true;
		doStateChange(m_state);
	}
}

void CUISwitchBase::off()
{
	if (m_state)
	{
		m_state = false;
		doStateChange(m_state);
	}
}

bool CUISwitchBase::isOn()
{
	return m_state;
}

bool CUISwitchBase::isOff()
{
	return !m_state;
}

bool CUISwitchBase::getState()
{
	return m_state;
}

void CUISwitchBase::setState(bool state)
{
	if (m_state!=state)
	{
		m_state = state;
		doStateChange(m_state);
	}
}

void CUISwitchBase::doStateChange(bool state)
{

}

void CUISwitchBase::doControlDown(CVec3d vec, int button)
{
	setState(!m_state);
	this->setHighlight(CShape::HS_OFF);
}

void CUISwitchBase::doControlUp(CVec3d vec)
{

}
