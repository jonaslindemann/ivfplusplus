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

#include <ivf/CompositeState.h>

#include <ivf/GL.h>

using namespace ivf;

CompositeState::CompositeState()
{

}

CompositeState::~CompositeState()
{
	clear();
}

void CompositeState::doSaveState()
{
	/*
	int i;

	for (i=0;i<m_children.size();i++)
	{
		RenderState* state = m_children[i];
		state->doSaveState();
	}
	*/
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void CompositeState::doState()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		RenderState* state = m_children[i];
		state->doState();
	}
}

void CompositeState::doRestoreState()
{
	/*
	int i;

	for (i=0;i<m_children.size();i++)
	{
		RenderState* state = m_children[i];
		state->doRestoreState();
	}
	*/
	glPopAttrib();
}

void CompositeState::addChild(RenderState *state)
{
	state->addReference();
	m_children.push_back(state);
}

void CompositeState::clear()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		RenderState* state = m_children[i];
		state->deleteReference();
		if (!state->referenced())
			delete state;
	}
	m_children.clear();	
}
