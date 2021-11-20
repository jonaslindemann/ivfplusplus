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

#include <ivf3dui/IvfUIInteractiveBase.h>

CUIInteractiveBase::CUIInteractiveBase()
:CUIBase()
{
	setUseName(true);
}

CUIInteractiveBase::~CUIInteractiveBase()
{

}

void CUIInteractiveBase::addChild(CShape *shape)
{
	shape->setUseName(false);
	CComposite::addChild(shape);
}

void CUIInteractiveBase::doControlDown(CVec3d vec, int button)
{

}

void CUIInteractiveBase::doControlOver(CVec3d vec)
{

}

void CUIInteractiveBase::doControlUp(CVec3d vec)
{

}

void CUIInteractiveBase::doControlDrag(CVec3d vec, int button)
{

}

void CUIInteractiveBase::doControlClick(CVec3d vec, int button)
{

}

void CUIInteractiveBase::doControlLeave(CVec3d vec)
{

}
