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

#include <ivf/IvfCompositeState.h>

#include <ivf/IvfGL.h>

CIvfCompositeState::CIvfCompositeState()
{

}

CIvfCompositeState::~CIvfCompositeState()
{
	clear();
}

void CIvfCompositeState::doSaveState()
{
	/*
	int i;

	for (i=0;i<m_children.size();i++)
	{
		CIvfRenderState* state = m_children[i];
		state->doSaveState();
	}
	*/
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void CIvfCompositeState::doState()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		CIvfRenderState* state = m_children[i];
		state->doState();
	}
}

void CIvfCompositeState::doRestoreState()
{
	/*
	int i;

	for (i=0;i<m_children.size();i++)
	{
		CIvfRenderState* state = m_children[i];
		state->doRestoreState();
	}
	*/
	glPopAttrib();
}

void CIvfCompositeState::addChild(CIvfRenderState *state)
{
	state->addReference();
	m_children.push_back(state);
}

void CIvfCompositeState::clear()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		CIvfRenderState* state = m_children[i];
		state->deleteReference();
		if (!state->referenced())
			delete state;
	}
	m_children.clear();	
}
