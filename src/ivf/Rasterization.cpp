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

#include <ivf/Rasterization.h>

#include <ivf/GL.h>

using namespace ivf;

Rasterization* Rasterization::m_instance = 0;
SingletonDestroyer<Rasterization> Rasterization::m_destroyer;

Rasterization* Rasterization::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new Rasterization(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

Rasterization::Rasterization()
{

}


void Rasterization::enableCullFace()
{
	glEnable(GL_CULL_FACE);
}

void Rasterization::disableCullFace()
{
	glDisable(GL_CULL_FACE);
}

bool Rasterization::isCullFaceEnabled()
{
	GLboolean depthTest;
	glGetBooleanv(GL_CULL_FACE, &depthTest);
	return (bool)depthTest;
}

void Rasterization::setCullFace(TCullFace face)
{
	switch (face) {
	case CF_FRONT:
		glCullFace(GL_FRONT);
		break;
	case CF_BACK:
		glCullFace(GL_BACK);
		break;
	case CF_FRONT_AND_BACK:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	default:
		glCullFace(GL_BACK);
		break;
	}
}

Rasterization::TCullFace Rasterization::getCullFace()
{
	GLint cullFace;
	glGetIntegerv(GL_CULL_FACE, &cullFace);

	switch (cullFace) {
	case GL_FRONT:
		return CF_FRONT;
		break;
	case GL_BACK:
		return CF_BACK;
		break;
	case GL_FRONT_AND_BACK:
		return CF_FRONT_AND_BACK;
		break;
	default:
		return CF_BACK;
		break;
	}
}

void Rasterization::setPolygonMode(TTargetFace face, TPolygonMode mode)
{
	GLenum glFace, glMode;

	switch (face) {
	case TF_FRONT:
		glFace = GL_FRONT;
		break;
	case TF_BACK:
		glFace = GL_BACK;
		break;
	case TF_FRONT_AND_BACK:
		glFace = GL_FRONT_AND_BACK;
		break;
	default:
		glFace = GL_FRONT_AND_BACK;
		break;
	}

	switch (mode) {
	case PM_POINT:
		glMode = GL_POINT;
		break;
	case PM_LINE:
		glMode = GL_LINE;
		break;
	case PM_FILL:
		glMode = GL_FILL;
		break;
	default:
		glMode = GL_FILL;
		break;
	}

	glPolygonMode(glFace, glMode);
}

void Rasterization::setLineWidth(float width)
{
	glLineWidth(width);
}

void Rasterization::enableLineSmooth()
{
	glEnable(GL_LINE_SMOOTH);
}

void Rasterization::disableLineSmooth()
{
	glDisable(GL_LINE_SMOOTH);
}

void Rasterization::enablePolygonSmooth()
{
	glEnable(GL_POLYGON_SMOOTH);
}

void Rasterization::disablePolygonSmooth()
{
	glDisable(GL_POLYGON_SMOOTH);
}

void Rasterization::defineWireFrameMode()
{
	this->setPolygonMode(
		Rasterization::TF_FRONT_AND_BACK,
		Rasterization::PM_LINE
	);
}

void Rasterization::defineSmoothShadeMode()
{
	this->setPolygonMode(
		Rasterization::TF_FRONT_AND_BACK,
		Rasterization::PM_FILL
	);
}
