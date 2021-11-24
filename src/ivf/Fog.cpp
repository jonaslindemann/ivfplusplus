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

#include <ivf/Fog.h>

#include <ivf/GL.h>

using namespace ivf;

Fog* Fog::m_instance = 0;
SingletonDestroyer<Fog> Fog::m_destroyer;

Fog* Fog::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new Fog(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

Fog::Fog()
{

}

void Fog::enable()
{
	glEnable(GL_FOG);
}

void Fog::disable()
{
	glDisable(GL_FOG);
}

bool Fog::isEnabled()
{
	GLboolean fogEnabled;
	glGetBooleanv(GL_FOG, &fogEnabled);
	return fogEnabled;
}

void Fog::setType(TFogType type)
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

Fog::TFogType Fog::getType()
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

void Fog::setLimits(double start, double end)
{
	glFogf(GL_FOG_START, (GLfloat)start);
	glFogf(GL_FOG_END, (GLfloat)end);
}

void Fog::setStart(double start)
{
	glFogf(GL_FOG_START, (GLfloat)start);
}

void Fog::setEnd(double end)
{
	glFogf(GL_FOG_END, (GLfloat)end);
}

double Fog::getStart()
{
	GLfloat fogStart;
	glGetFloatv(GL_FOG_START, &fogStart);
	return fogStart;
}

double Fog::getEnd()
{
	GLfloat fogEnd;
	glGetFloatv(GL_FOG_END, &fogEnd);
	return fogEnd;
}

void Fog::getLimits(double &start, double &end)
{
	start = getStart();
	end = getEnd();
}

void Fog::setColor(float red, float green, float blue, float alpha)
{
	m_fogColor[0] = red;
	m_fogColor[1] = green;
	m_fogColor[2] = blue;
	m_fogColor[3] = alpha;

	glFogfv(GL_FOG_COLOR, m_fogColor);
}

void Fog::getColor(float &red, float &green, float &blue, float &alpha)
{
	GLfloat fogColor[4];
	glGetFloatv(GL_FOG_COLOR, fogColor);
	red = fogColor[0];
	green = fogColor[1];
	blue = fogColor[2];
	alpha = fogColor[3];
}

void Fog::setDensity(double density)
{
	glFogf(GL_FOG_END, (GLfloat)density);
}

double Fog::getDensity()
{
	GLfloat fogEnd;
	glGetFloatv(GL_FOG_END, &fogEnd);
	return (double)fogEnd;
}
