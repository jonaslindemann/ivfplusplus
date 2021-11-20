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

#include <ivf3dui/UIButtonGroup.h>

using namespace ivf;

CUIButtonGroup::CUIButtonGroup()
{
	this->setUseName(false);
}

CUIButtonGroup::~CUIButtonGroup()
{

}

void CUIButtonGroup::addChild(CUIButtonBase* button)
{
	button->setParentControl(this);
	button->setType(CUIButtonBase::BT_TOGGLE);
	CComposite::addChild(button);
}

void CUIButtonGroup::updateState(CUIButtonBase* button)
{
	int i;

	for (i=0; i<this->getSize(); i++)
	{
		CShape* shape = this->getChild(i);
		CUIButtonBasePtr b = (CUIButtonBase*)shape;

		if (b!=button)
			b->groupUpdateState(CUIButtonBase::BS_UP);
	}	
}
