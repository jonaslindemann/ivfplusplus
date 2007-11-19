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

#include <ivf/IvfRasterization.h>

#include <ivf/IvfGL.h>

CIvfRasterization* CIvfRasterization::m_instance = 0;
CIvfSingletonDestroyer<CIvfRasterization> CIvfRasterization::m_destroyer;

CIvfRasterization* CIvfRasterization::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CIvfRasterization(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CIvfRasterization::CIvfRasterization()
{

}


void CIvfRasterization::enableCullFace()
{
	glEnable(GL_CULL_FACE);
}

void CIvfRasterization::disableCullFace()
{
	glDisable(GL_CULL_FACE);
}

bool CIvfRasterization::isCullFaceEnabled()
{
	GLboolean depthTest;
	glGetBooleanv(GL_CULL_FACE, &depthTest);
	return (bool)depthTest;
}

void CIvfRasterization::setCullFace(TCullFace face)
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

CIvfRasterization::TCullFace CIvfRasterization::getCullFace()
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

void CIvfRasterization::setPolygonMode(TTargetFace face, TPolygonMode mode)
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

void CIvfRasterization::setLineWidth(float width)
{
	glLineWidth(width);
}

void CIvfRasterization::enableLineSmooth()
{
	glEnable(GL_LINE_SMOOTH);
}

void CIvfRasterization::disableLineSmooth()
{
	glDisable(GL_LINE_SMOOTH);
}

void CIvfRasterization::enablePolygonSmooth()
{
	glEnable(GL_POLYGON_SMOOTH);
}

void CIvfRasterization::disablePolygonSmooth()
{
	glDisable(GL_POLYGON_SMOOTH);
}

void CIvfRasterization::defineWireFrameMode()
{
	this->setPolygonMode(
		CIvfRasterization::TF_FRONT_AND_BACK,
		CIvfRasterization::PM_LINE
	);
}

void CIvfRasterization::defineSmoothShadeMode()
{
	this->setPolygonMode(
		CIvfRasterization::TF_FRONT_AND_BACK,
		CIvfRasterization::PM_FILL
	);
}
