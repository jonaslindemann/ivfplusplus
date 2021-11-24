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

UISwitchBase::UISwitchBase()
{
	m_state = false;
}

UISwitchBase::~UISwitchBase()
{

}

void UISwitchBase::doControlClick(Vec3d vec, int button)
{

}

void UISwitchBase::doControlOver(Vec3d vec)
{
	this->setHighlight(Shape::HS_ON);
}

void UISwitchBase::doControlLeave(Vec3d vec)
{
	this->setHighlight(Shape::HS_OFF);
}

void UISwitchBase::on()
{
	if (!m_state)
	{
		m_state = true;
		doStateChange(m_state);
	}
}

void UISwitchBase::off()
{
	if (m_state)
	{
		m_state = false;
		doStateChange(m_state);
	}
}

bool UISwitchBase::isOn()
{
	return m_state;
}

bool UISwitchBase::isOff()
{
	return !m_state;
}

bool UISwitchBase::getState()
{
	return m_state;
}

void UISwitchBase::setState(bool state)
{
	if (m_state!=state)
	{
		m_state = state;
		doStateChange(m_state);
	}
}

void UISwitchBase::doStateChange(bool state)
{

}

void UISwitchBase::doControlDown(Vec3d vec, int button)
{
	setState(!m_state);
	this->setHighlight(Shape::HS_OFF);
}

void UISwitchBase::doControlUp(Vec3d vec)
{

}
