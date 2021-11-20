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

#include <ivf/GenericButton.h>

using namespace ivf;

CGenericButton::CGenericButton()
{
	m_state = BS_NORMAL;
	m_id = -1;
}

CGenericButton::~CGenericButton()
{

}

void CGenericButton::setButtonState(TButtonState state)
{
	m_state = state;

	switch (m_state) {
	case BS_NORMAL:
		this->setCurrentChild(0);
		break;
	case BS_PRESSED:
		this->setCurrentChild(1);
		break;
	case BS_DISABLED:
		this->setCurrentChild(2);
		break;
	case BS_CHECKED:
		this->setCurrentChild(3);
		break;
	case BS_HIDDEN:
		this->setCurrentChild(4);
		break;
	default:
		this->setCurrentChild(0);
		break;
	}
}

void CGenericButton::setId(int id)
{
	m_id = id;
}

int CGenericButton::getId()
{
	return m_id;
}

