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

#include <ivf3dui/UIBase.h>

using namespace ivf;

CUIBase::CUIBase()
{
	m_id = -1;
	m_enabled = true;
	m_parent = nullptr;
	setUseName(false);
}

CUIBase::~CUIBase()
{

}

void CUIBase::setId(int id)
{
	m_id = id;
}

int CUIBase::getId()
{
	return m_id;
}



void CUIBase::enable()
{
	m_enabled = true;
}

void CUIBase::disable()
{
	m_enabled = false;
}

bool CUIBase::isEnabled()
{
	return m_enabled;
}


void CUIBase::doChangeState(bool enabled)
{
	
}

void CUIBase::setParentControl(CUIBase *parent)
{
	m_parent = parent;
}

CUIBase* CUIBase::getParentControl()
{
	return m_parent;
}

bool CUIBase::haveParent()
{
	return m_parent!=nullptr;
}

