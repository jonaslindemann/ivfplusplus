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

#include <ivf/Blending.h>

#include <ivf/GL.h>

using namespace ivf;

Blending* Blending::m_instance = 0;
SingletonDestroyer<Blending> Blending::m_destroyer;

Blending* Blending::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new Blending(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

Blending::Blending()
{

}

void Blending::enable()
{
	glEnable(GL_BLEND);
}

void Blending::disable()
{
	glDisable(GL_BLEND);
}

bool Blending::isEnabled()
{
	GLboolean blending;
	glGetBooleanv(GL_BLEND, &blending);
	return (bool)blending;
}



void Blending::setFunction(TBlendFactor src, TBlendFactor dst)
{
	GLenum glSrc, glDst;
	
	switch (src) {
	case BF_ZERO:
		glSrc = GL_ZERO;
		break;
	case BF_ONE:
		glSrc = GL_ONE;
		break;
	case BF_DST_COLOR:
		glSrc = GL_DST_COLOR;
		break;
	case BF_SRC_COLOR:
		glSrc = GL_SRC_COLOR;
		break;
	case BF_ONE_MINUS_DST_COLOR:
		glSrc = BF_ONE_MINUS_DST_COLOR;
		break;
	case BF_ONE_MINUS_SRC_COLOR:
		glSrc = GL_ONE_MINUS_SRC_COLOR;
		break;
	case BF_SRC_ALPHA:
		glSrc = GL_SRC_ALPHA;
		break;
	case BF_ONE_MINUS_SRC_ALPHA:
		glSrc = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case BF_DST_ALPHA:
		glSrc = GL_DST_ALPHA;
		break;
	case BF_ONE_MINUS_DST_ALPHA:
		glSrc = GL_ONE_MINUS_DST_ALPHA;
		break;
	case BF_SRC_ALPHA_SATURATE:
		glSrc = GL_SRC_ALPHA_SATURATE;
		break;
	default:
		glSrc = GL_ONE;
		break;
	}

	switch (dst) {
	case BF_ZERO:
		glDst = GL_ZERO;
		break;
	case BF_ONE:
		glDst = GL_ONE;
		break;
	case BF_DST_COLOR:
		glDst = GL_DST_COLOR;
		break;
	case BF_SRC_COLOR:
		glDst = GL_SRC_COLOR;
		break;
	case BF_ONE_MINUS_DST_COLOR:
		glDst = BF_ONE_MINUS_DST_COLOR;
		break;
	case BF_ONE_MINUS_SRC_COLOR:
		glDst = GL_ONE_MINUS_SRC_COLOR;
		break;
	case BF_SRC_ALPHA:
		glDst = GL_SRC_ALPHA;
		break;
	case BF_ONE_MINUS_SRC_ALPHA:
		glDst = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case BF_DST_ALPHA:
		glDst = GL_DST_ALPHA;
		break;
	case BF_ONE_MINUS_DST_ALPHA:
		glDst = GL_ONE_MINUS_DST_ALPHA;
		break;
	case BF_SRC_ALPHA_SATURATE:
		glDst = GL_SRC_ALPHA_SATURATE;
		break;
	default:
		glDst = GL_ONE;
		break;
	}

	glBlendFunc(glSrc, glDst);
}

void Blending::saveState()
{
	glPushAttrib(GL_BLEND);
}

void Blending::restoreState()
{
	glPopAttrib();
}

void Blending::defineAddFunction()
{
	this->setFunction(
		Blending::BF_ONE,
		Blending::BF_ONE
	);
}

void Blending::defineAlphaBlendFunction()
{
	this->setFunction(
		Blending::BF_SRC_ALPHA,
		Blending::BF_ONE_MINUS_SRC_ALPHA
	);
}
