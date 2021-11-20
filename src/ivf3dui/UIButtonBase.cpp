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

#include <ivf3dui/UIButtonBase.h>

#include <ivf3dui/UIButtonGroup.h>

using namespace ivf;

CUIButtonBase::CUIButtonBase()
{
	m_type = BT_NORMAL;
	m_state = BS_UP;
}

CUIButtonBase::~CUIButtonBase()
{
	
}

void CUIButtonBase::setType(TButtonType type)
{
	m_type = type;
}

CUIButtonBase::TButtonType CUIButtonBase::getType()
{
	return m_type;
}

void CUIButtonBase::setState(TButtonState state)
{
	if (this->getParentControl()!=NULL)
	{
		if (state==BS_DOWN)
		{
			CUIBase* parent = this->getParentControl();
			CUIButtonGroup* group = (CUIButtonGroup*)parent;
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

CUIButtonBase::TButtonState CUIButtonBase::getState()
{
	return m_state;
}

void CUIButtonBase::doStateChange()
{
	
}

void CUIButtonBase::groupUpdateState(TButtonState state)
{
	m_state = state;
	doStateChange();
}
