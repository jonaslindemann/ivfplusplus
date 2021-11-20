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

#include <ivf/Blending.h>

#include <ivf/GL.h>

using namespace ivf;

CBlending* CBlending::m_instance = 0;
CSingletonDestroyer<CBlending> CBlending::m_destroyer;

CBlending* CBlending::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CBlending(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CBlending::CBlending()
{

}

void CBlending::enable()
{
	glEnable(GL_BLEND);
}

void CBlending::disable()
{
	glDisable(GL_BLEND);
}

bool CBlending::isEnabled()
{
	GLboolean blending;
	glGetBooleanv(GL_BLEND, &blending);
	return (bool)blending;
}



void CBlending::setFunction(TBlendFactor src, TBlendFactor dst)
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

void CBlending::saveState()
{
	glPushAttrib(GL_BLEND);
}

void CBlending::restoreState()
{
	glPopAttrib();
}

void CBlending::defineAddFunction()
{
	this->setFunction(
		CBlending::BF_ONE,
		CBlending::BF_ONE
	);
}

void CBlending::defineAlphaBlendFunction()
{
	this->setFunction(
		CBlending::BF_SRC_ALPHA,
		CBlending::BF_ONE_MINUS_SRC_ALPHA
	);
}
