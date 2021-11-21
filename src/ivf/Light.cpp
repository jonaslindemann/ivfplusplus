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
// Written by Jonas Lindemann and Carl-Johan Lejdfors
//

#include <ivf/Light.h>

#include <ivf/GL.h>

using namespace ivf;

// ------------------------------------------------------------
CLight::CLight()
{
	m_enabled = false;

	setAmbientColor(0.0, 0.0, 0.0, 1.0 );
	setDiffuseColor(1.0, 1.0, 1.0, 1.0);
	setSpecularColor(1.0, 1.0, 1.0, 1.0);

	/*else {
		setDiffuse(0.0, 0.0, 0.0, 0.0);
		setSpecular(0.0, 0.0, 0.0, 0.0);
	}*/
	setLightPosition( 0, 0, 1, 0 );
	setConstAtt( 1 );
	setLinAtt( 0 );
	setQuadAtt( 0 );

	m_lightType = LT_DIRECTIONAL;

	m_spotCutoff = 45.0f;
	m_spotDirection[0] = 0.0f;
	m_spotDirection[1] = 0.0f;
	m_spotDirection[2] = -1.0f;
	m_spotExponent = 0.0f;

	m_lightn = 0;
}

// ------------------------------------------------------------
CLight::~CLight()
{

}

void CLight::enable()
{
	glEnable(GL_LIGHT0 + m_lightn);
	m_enabled = true;
}

void CLight::disable()
{
	glDisable(GL_LIGHT0 + m_lightn);
	m_enabled = false;
}

bool CLight::isEnabled()
{
	/*
	GLboolean lightEnabled;
	glGetBooleanv(GL_LIGHT0 + m_lightn, &lightEnabled);
	return (bool)lightEnabled;
	*/
	return m_enabled;
}

void CLight::doCreateGeometry()
{
	if (m_enabled)
	{
		if ( CGLBase::getState() == CGLBase::OS_OFF )
			return;

		GLenum light = GL_LIGHT0 + m_lightn;

		//glEnable( light );

		glLightfv( light, GL_AMBIENT, m_ambient );
		glLightfv( light, GL_DIFFUSE, m_diffuse );
		glLightfv( light, GL_SPECULAR, m_specular );

		switch (m_lightType) {
		case LT_POINT:
			m_position[3] = 1.0;
			glLightfv( light, GL_POSITION, m_position );
			glLightf(light, GL_SPOT_CUTOFF, 180.0f);
			break;
		case LT_DIRECTIONAL:
			m_position[3] = 0.0;
			glLightfv( light, GL_POSITION, m_position );
			glLightf(light, GL_SPOT_CUTOFF, 180.0f);
			break;
		case LT_SPOT:
			m_position[3] = 1.0;
			glLightfv( light, GL_POSITION, m_position );
			glLightf(light, GL_SPOT_CUTOFF, m_spotCutoff);
			glLightfv(light, GL_SPOT_DIRECTION, m_spotDirection);
			glLightf(light, GL_SPOT_EXPONENT, m_spotExponent);
			break;
		default:

			break;
		}

		glLighti( light, GL_CONSTANT_ATTENUATION, m_constatt );
		glLighti( light, GL_LINEAR_ATTENUATION, m_linatt );
		glLighti( light, GL_QUADRATIC_ATTENUATION, m_quadatt );
	}
}

void CLight::setAmbientColor( const float r, const float g,
						   const float b, const float a )
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_ambient[3] = a;
}

void CLight::setDiffuseColor( const float r, const float g,
						   const float b, const float a )
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;
}

void CLight::setSpecularColor( const float r, const float g,
							const float b, const float a )
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
	m_specular[3] = a;
}

void CLight::setLightPosition( const float x, const float y, const float z, const float w )
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
	m_position[3] = w;

	if (w>=0.9)
		m_lightType = LT_POINT;
	else
		m_lightType = LT_DIRECTIONAL;
}

void CLight::setLightPosition( const float x, const float y, const float z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void CLight::setDirection( const float x, const float y, const float z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void CLight::setConstAtt( const int a )
{
	m_constatt = a;
}

void CLight::setLinAtt( const int a )
{
	m_linatt = a;
}

void CLight::setQuadAtt( const int a )
{
	m_quadatt = a;
}


void CLight::getAmbientColor( float &r, float &g, float &b, float &a ) const
{
	r = m_ambient[0];
	g = m_ambient[1];
	b = m_ambient[2];
	a = m_ambient[3];
}

void CLight::getDiffuseColor( float &r, float &g, float &b, float &a ) const
{
	r = m_diffuse[0];
	g = m_diffuse[1];
	b = m_diffuse[2];
	a = m_diffuse[3];
}

void CLight::getSpecularColor( float &r, float &g, float &b, float &a ) const
{
	r = m_specular[0];
	g = m_specular[1];
	b = m_specular[2];
	a = m_specular[3];
}

void CLight::getLightPosition( float &x, float &y, float &z, float &w ) const
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
	w = m_position[3];
}

void CLight::getConstAtt( int &a ) const
{
	a = m_constatt;
}

void CLight::getLinAtt( int &a ) const
{
	a = m_linatt;
}

void CLight::getQuadAtt( int &a ) const
{
	a = m_quadatt;
}

void CLight::setType(TLightType lightType)
{
	m_lightType = lightType;
}

void CLight::setSpotCutoff(float cutoff)
{
	m_spotCutoff = cutoff;
}

void CLight::setSpotDirection(float ex, float ey, float ez)
{
	m_spotDirection[0] = ex;
	m_spotDirection[1] = ey;
	m_spotDirection[2] = ez;
}

void CLight::setSpotExponent(float exponent)
{
	m_spotExponent = exponent;
}

CLight::TLightType CLight::getType()
{
	return m_lightType;
}

void CLight::getSpotDirection(float &ex, float &ey, float &ez)
{
	ex = m_spotDirection[0];
	ey = m_spotDirection[1];
	ez = m_spotDirection[2];
}

float CLight::getSpotCutoff()
{
	return m_spotCutoff;
}

float CLight::getSpotExponent()
{
	return m_spotExponent;
}


void CLight::setNumber(int number)
{
	m_lightn = number;
}
