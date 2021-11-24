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

#include <ivf3dui/UIButtonBase.h>

#include <ivf3dui/UIButtonGroup.h>

using namespace ivf;

UIButtonBase::UIButtonBase()
{
	m_type = BT_NORMAL;
	m_state = BS_UP;
}

UIButtonBase::~UIButtonBase()
{
	
}

void UIButtonBase::setType(TButtonType type)
{
	m_type = type;
}

UIButtonBase::TButtonType UIButtonBase::getType()
{
	return m_type;
}

void UIButtonBase::setState(TButtonState state)
{
	if (this->getParentControl()!=nullptr)
	{
		if (state==BS_DOWN)
		{
			UIBase* parent = this->getParentControl();
			UIButtonGroup* group = (UIButtonGroup*)parent;
			group->updateState(this);
			m_state = state;
			doStateChange();
		}
	}
	else
	{
		m_state = state;
		doStateChange();
	}
}

UIButtonBase::TButtonState UIButtonBase::getState()
{
	return m_state;
}

void UIButtonBase::doStateChange()
{
	
}

void UIButtonBase::groupUpdateState(TButtonState state)
{
	m_state = state;
	doStateChange();
}
