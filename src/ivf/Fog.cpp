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

#include <ivf/Fog.h>

#include <ivf/GL.h>

using namespace ivf;

CFog* CFog::m_instance = 0;
CSingletonDestroyer<CFog> CFog::m_destroyer;

CFog* CFog::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CFog(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CFog::CFog()
{

}

void CFog::enable()
{
	glEnable(GL_FOG);
}

void CFog::disable()
{
	glDisable(GL_FOG);
}

bool CFog::isEnabled()
{
	GLboolean fogEnabled;
	glGetBooleanv(GL_FOG, &fogEnabled);
	return fogEnabled;
}

void CFog::setType(TFogType type)
{
	switch (type) {
	case FT_LINEAR:
		glFogi(GL_FOG_MODE, GL_LINEAR);
		break;
	case FT_EXP:
		glFogi(GL_FOG_MODE, GL_EXP);
		break;
	case FT_EXP2:
		glFogi(GL_FOG_MODE, GL_EXP2);
		break;
	default:

		break;
	}
}

CFog::TFogType CFog::getType()
{
	GLint fogMode;
	glGetIntegerv(GL_FOG_MODE, &fogMode);
	switch (fogMode) {
	case GL_LINEAR:
		return FT_LINEAR;
		break;
	case GL_EXP:
		return FT_EXP;
		break;
	case GL_EXP2:
		return FT_EXP2;
		break;
	default:
		return FT_LINEAR;
		break;
	}
}

void CFog::setLimits(double start, double end)
{
	glFogf(GL_FOG_START, (GLfloat)start);
	glFogf(GL_FOG_END, (GLfloat)end);
}

void CFog::setStart(double start)
{
	glFogf(GL_FOG_START, (GLfloat)start);
}

void CFog::setEnd(double end)
{
	glFogf(GL_FOG_END, (GLfloat)end);
}

double CFog::getStart()
{
	GLfloat fogStart;
	glGetFloatv(GL_FOG_START, &fogStart);
	return fogStart;
}

double CFog::getEnd()
{
	GLfloat fogEnd;
	glGetFloatv(GL_FOG_END, &fogEnd);
	return fogEnd;
}

void CFog::getLimits(double &start, double &end)
{
	start = getStart();
	end = getEnd();
}

void CFog::setColor(float red, float green, float blue, float alpha)
{
	m_fogColor[0] = red;
	m_fogColor[1] = green;
	m_fogColor[2] = blue;
	m_fogColor[3] = alpha;

	glFogfv(GL_FOG_COLOR, m_fogColor);
}

void CFog::getColor(float &red, float &green, float &blue, float &alpha)
{
	GLfloat fogColor[4];
	glGetFloatv(GL_FOG_COLOR, fogColor);
	red = fogColor[0];
	green = fogColor[1];
	blue = fogColor[2];
	alpha = fogColor[3];
}

void CFog::setDensity(double density)
{
	glFogf(GL_FOG_END, (GLfloat)density);
}

double CFog::getDensity()
{
	GLfloat fogEnd;
	glGetFloatv(GL_FOG_END, &fogEnd);
	return (double)fogEnd;
}
