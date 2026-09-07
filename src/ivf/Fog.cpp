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
#include <ivf/LegacyGL.h>
#include <ivf/rc.h>

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
	m_fogColor[0] = 0.0f;
	m_fogColor[1] = 0.0f;
	m_fogColor[2] = 0.0f;
	m_fogColor[3] = 1.0f;
	m_fogStart = 0.0;
	m_fogEnd = 1.0;
	m_fogDensity = 1.0;
	m_enabled = false;
	m_type = FT_LINEAR;
}

void Fog::syncToRenderContext()
{
	// The shader reads a single mode value, with zero meaning no fog, so
	// enable/disable and the fog equation collapse into one number.

	int mode = 0;

	if (m_enabled)
	{
		switch (m_type)
		{
		case FT_LINEAR: mode = 1; break;
		case FT_EXP:    mode = 2; break;
		case FT_EXP2:   mode = 3; break;
		default:        mode = 1; break;
		}
	}

	rcSetFogMode(mode);
	rcSetFogColor(m_fogColor[0], m_fogColor[1], m_fogColor[2], m_fogColor[3]);
	rcSetFogDensity((float)m_fogDensity);
	rcSetFogRange((float)m_fogStart, (float)m_fogEnd);
}

void Fog::enable()
{
	m_enabled = true;
	lgEnableLegacy(GL_FOG);
	syncToRenderContext();
}

void Fog::disable()
{
	m_enabled = false;
	lgDisableLegacy(GL_FOG);
	syncToRenderContext();
}

bool Fog::isEnabled()
{
	return m_enabled;
}

void Fog::setType(TFogType type)
{
	m_type = type;

	switch (type) {
	case FT_LINEAR:
		lgFogi(GL_FOG_MODE, GL_LINEAR);
		break;
	case FT_EXP:
		lgFogi(GL_FOG_MODE, GL_EXP);
		break;
	case FT_EXP2:
		lgFogi(GL_FOG_MODE, GL_EXP2);
		break;
	default:

		break;
	}

	syncToRenderContext();
}

Fog::TFogType Fog::getType()
{
	return m_type;
}

void Fog::setLimits(double start, double end)
{
	m_fogStart = start;
	m_fogEnd = end;
	lgFogf(GL_FOG_START, (GLfloat)start);
	lgFogf(GL_FOG_END, (GLfloat)end);
	syncToRenderContext();
}

void Fog::setStart(double start)
{
	m_fogStart = start;
	lgFogf(GL_FOG_START, (GLfloat)start);
	syncToRenderContext();
}

void Fog::setEnd(double end)
{
	m_fogEnd = end;
	lgFogf(GL_FOG_END, (GLfloat)end);
	syncToRenderContext();
}

double Fog::getStart()
{
	return m_fogStart;
}

double Fog::getEnd()
{
	return m_fogEnd;
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

	lgFogfv(GL_FOG_COLOR, m_fogColor);
	syncToRenderContext();
}

void Fog::getColor(float &red, float &green, float &blue, float &alpha)
{
	red = m_fogColor[0];
	green = m_fogColor[1];
	blue = m_fogColor[2];
	alpha = m_fogColor[3];
}

void Fog::setDensity(double density)
{
	// This wrote GL_FOG_END rather than GL_FOG_DENSITY, so setting a density had
	// no effect on the exponential fog modes and quietly moved the far plane of
	// the linear one instead.

	m_fogDensity = density;
	lgFogf(GL_FOG_DENSITY, (GLfloat)density);
	syncToRenderContext();
}

double Fog::getDensity()
{
	return m_fogDensity;
}
