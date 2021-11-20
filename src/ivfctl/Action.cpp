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

// IvfAction.cpp: implementation of the CAction class.
//
//////////////////////////////////////////////////////////////////////

#include <ivfctl/Action.h>

#include <ivfctl/Controller.h>

using namespace ivf;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAction::CAction()
{
	m_time = 0.0;
	m_actionType = 0;
	m_dParam = 0.0;
	m_iParam = 0;
	m_target = NULL;
}

CAction::~CAction()
{

}

void CAction::setTime(double time)
{
	m_time = time;
}

void CAction::setDoubleParam(double param)
{
	m_dParam = param;
}

void CAction::setIntParam(int param)
{
	m_iParam = param;
}

double CAction::getTime()
{
	return m_time;
}

double CAction::getDoubleParam()
{
	return m_dParam;
}

int CAction::getIntParam()
{
	return m_iParam;
}

void CAction::setActionType(int type)
{
	m_actionType = type;
}

int CAction::getActionType()
{
	return m_actionType;
}

void CAction::setTarget(CController *controller)
{
	m_target = controller;
}

CController* CAction::getTarget()
{
	return m_target;
}
