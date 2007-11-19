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

#include <ivf/IvfLightingState.h>

CIvfLightingState::CIvfLightingState()
{
	m_lightingState = true;
	m_lighting = CIvfLighting::getInstance();
}

CIvfLightingState::~CIvfLightingState()
{

}

void CIvfLightingState::doState()
{
	if (m_lightingState)
		m_lighting->enable();
	else
		m_lighting->disable();
}

void CIvfLightingState::doSaveState()
{
	m_lighting->saveState();
}

void CIvfLightingState::doRestoreState()
{
	m_lighting->restoreState();
}


void CIvfLightingState::setLighting(bool flag)
{
	m_lightingState = flag;
}

bool CIvfLightingState::getLighting()
{
	return m_lightingState;
}
