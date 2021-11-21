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

#include <ivf/BlendState.h>

using namespace ivf;

CBlendState::CBlendState()
{
	m_sfactor = GL_ONE;
	m_dfactor = GL_ZERO;
}

CBlendState::CBlendState(GLenum sfactor, GLenum dfactor)
{
	m_sfactor = sfactor;
	m_dfactor = dfactor;
}

CBlendState::~CBlendState()
{

}

void CBlendState::doState()
{
	glEnable(GL_BLEND);
	glBlendFunc(m_sfactor, m_dfactor);
}

void CBlendState::doSaveState()
{
	m_oldState = glIsEnabled(GL_BLEND);
	glPushAttrib(GL_BLEND);
}

void CBlendState::doRestoreState()
{
	if (m_oldState)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glPopAttrib();
}

void CBlendState::setFunction(GLenum sfactor, GLenum dfactor)
{
	m_sfactor = sfactor;
	m_dfactor = dfactor;
}
