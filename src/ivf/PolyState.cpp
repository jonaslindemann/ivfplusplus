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

#include <ivf/PolyState.h>

#include <ivf/GL.h>

using namespace ivf;

PolyState::PolyState()
{
	m_face = TF_FRONT_AND_BACK;
	m_mode = PM_FILL;
}

PolyState::~PolyState()
{

}

void PolyState::setFace(TTargetFace face)
{
	m_face = face;
}

PolyState::TTargetFace PolyState::getFace()
{
	return m_face;
}

void PolyState::setMode(TPolygonMode mode)
{
	m_mode = mode;
}

PolyState::TPolygonMode PolyState::getMode()
{
	return m_mode;
}

void PolyState::doState()
{
	GLenum face, mode;

	switch (m_face) {
	case TF_FRONT:
		face = GL_FRONT;
		break;
	case TF_BACK:
		face = GL_BACK;
		break;
	case TF_FRONT_AND_BACK:
		face = GL_FRONT_AND_BACK;
		break;
	default:
		face = GL_FRONT_AND_BACK;
		break;
	}

	switch (m_mode) {
	case PM_POINT:
		mode = GL_POINT;
		break;
	case PM_LINE:
		mode = GL_LINE;
		break;
	case PM_FILL:
		mode = GL_FILL;
		break;
	default:
		mode = GL_FILL;
		break;
	}

	glPolygonMode(face, mode);
}

void PolyState::doSaveState()
{
	GLint value[2];
	glGetIntegerv(GL_POLYGON_MODE, value);
	m_oldMode = value[0];
}

void PolyState::doRestoreState()
{
	GLenum face;

	switch (m_face) {
	case TF_FRONT:
		face = GL_FRONT;
		break;
	case TF_BACK:
		face = GL_BACK;
		break;
	case TF_FRONT_AND_BACK:
		face = GL_FRONT_AND_BACK;
		break;
	default:
		face = GL_FRONT_AND_BACK;
		break;
	}
	glPolygonMode(face, m_oldMode);
}
