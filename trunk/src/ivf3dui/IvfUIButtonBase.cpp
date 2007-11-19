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

#include <ivf3dui/IvfUIButtonBase.h>

#include <ivf3dui/IvfUIButtonGroup.h>

CIvfUIButtonBase::CIvfUIButtonBase()
{
	m_type = BT_NORMAL;
	m_state = BS_UP;
}

CIvfUIButtonBase::~CIvfUIButtonBase()
{
	
}

void CIvfUIButtonBase::setType(TButtonType type)
{
	m_type = type;
}

CIvfUIButtonBase::TButtonType CIvfUIButtonBase::getType()
{
	return m_type;
}

void CIvfUIButtonBase::setState(TButtonState state)
{
	if (this->getParentControl()!=NULL)
	{
		if (state==BS_DOWN)
		{
			CIvfUIBase* parent = this->getParentControl();
			CIvfUIButtonGroup* group = (CIvfUIButtonGroup*)parent;
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

CIvfUIButtonBase::TButtonState CIvfUIButtonBase::getState()
{
	return m_state;
}

void CIvfUIButtonBase::doStateChange()
{
	
}

void CIvfUIButtonBase::groupUpdateState(TButtonState state)
{
	m_state = state;
	doStateChange();
}
