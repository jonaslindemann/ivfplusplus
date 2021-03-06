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

// Implementation of: public class CIvfOldLight

#include <ivf/ivfconfig.h>
#include <ivf/IvfOldLight.h>

long CIvfOldLight::lightmask = 0;

// ------------------------------------------------------------
CIvfOldLight::CIvfOldLight() 
: CIvfGLBase()
{
	m_lightn = -1;
	
	for ( int i = 0 ; i < (int)IVF_MAX_LIGHTS ; ++i ) {
		if ( !(lightmask & 1 << i) ) {
			//bit i is empty i.e. light i is available
			m_lightn = i;
			lightmask = lightmask | 1 << i;
			break;
		}
	}

	m_active = true;
	
	setAmbient(0.0, 0.0, 0.0, 1.0 );
	setDiffuse(1.0, 1.0, 1.0, 1.0);
	setSpecular(1.0, 1.0, 1.0, 1.0);
	
	/*else {
		setDiffuse(0.0, 0.0, 0.0, 0.0);
		setSpecular(0.0, 0.0, 0.0, 0.0);
	}*/
	setPosition( 0, 0, 1, 0 );
	setConstAtt( 1 );
	setLinAtt( 0 );
	setQuadAtt( 0 );

	m_lightType = LT_DIRECTIONAL;

	m_spotCutoff = 45.0f;
	m_spotDirection[0] = 0.0f;
	m_spotDirection[1] = 0.0f;
	m_spotDirection[2] = -1.0f;
	m_spotExponent = 0.0f;
}

// ------------------------------------------------------------
CIvfOldLight::~CIvfOldLight()
{
	if ( m_lightn != -1 ) 
	{
		deactivate();
		//lightmask = lightmask ^ (1 << m_lightn); 
	}
}

CIvfOldLight& CIvfOldLight::operator = ( const CIvfOldLight &arg )
{
	GLfloat r,g,b,a;
	
	arg.getAmbient(r,g,b,a);
	setAmbient(r,g,b,a);
	
	arg.getDiffuse(r,g,b,a);
	setDiffuse(r,g,b,a);
	
	arg.getSpecular(r,g,b,a);
	setSpecular(r,g,b,a);
	
	arg.getPosition(r,g,b,a);
	setPosition(r,g,b,a);
	
	GLint att;
	
	arg.getConstAtt(att);
	setConstAtt(att);
	
	arg.getLinAtt(att);
	setLinAtt(att);
	
	arg.getQuadAtt(att);
	setQuadAtt(att);
	
	return *this;
}

CIvfOldLight::operator void* ()
{
	if ( m_lightn == -1 ) 
		return 0;
	return this;
}

void CIvfOldLight::setAmbient( const GLfloat r, const GLfloat g, 
						   const GLfloat b, const GLfloat a )
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_ambient[3] = a;
}

void CIvfOldLight::setDiffuse( const GLfloat r, const GLfloat g, 
						   const GLfloat b, const GLfloat a )
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;
}

void CIvfOldLight::setSpecular( const GLfloat r, const GLfloat g, 
							const GLfloat b, const GLfloat a )
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
	m_specular[3] = a;
}

void CIvfOldLight::setPosition( const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w )
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

void CIvfOldLight::setPosition( const GLfloat x, const GLfloat y, const GLfloat z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void CIvfOldLight::setDirection( const GLfloat x, const GLfloat y, const GLfloat z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void CIvfOldLight::setConstAtt( const GLint a )
{
	m_constatt = a;
}

void CIvfOldLight::setLinAtt( const GLint a )
{
	m_linatt = a;
}

void CIvfOldLight::setQuadAtt( const GLint a )
{
	m_quadatt = a;
}


void CIvfOldLight::getAmbient( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const
{
	r = m_ambient[0];
	g = m_ambient[1];
	b = m_ambient[2];
	a = m_ambient[3];
}

void CIvfOldLight::getDiffuse( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const
{
	r = m_diffuse[0];
	g = m_diffuse[1];
	b = m_diffuse[2];
	a = m_diffuse[3];
}

void CIvfOldLight::getSpecular( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const
{
	r = m_specular[0];
	g = m_specular[1];
	b = m_specular[2];
	a = m_specular[3];
}

void CIvfOldLight::getPosition( GLfloat &x, GLfloat &y, GLfloat &z, GLfloat &w ) const
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
	w = m_position[3];
}

void CIvfOldLight::getConstAtt( GLint &a ) const
{
	a = m_constatt;
}

void CIvfOldLight::getLinAtt( GLint &a ) const
{
	a = m_linatt;
}

void CIvfOldLight::getQuadAtt( GLint &a ) const
{
	a = m_quadatt;
}

void CIvfOldLight::render()
{
	if (m_active)
	{
		if ( CIvfGLBase::getState() == OS_OFF || m_lightn == -1 ) 
			return;
		
		GLenum light = GL_LIGHT0 + m_lightn;
		
		glEnable( light );
		
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

void CIvfOldLight::setType(TLightType lightType)
{
	m_lightType = lightType;
}

void CIvfOldLight::setSpotCutoff(float cutoff)
{
	m_spotCutoff = cutoff;
}

void CIvfOldLight::setSpotDirection(float ex, float ey, float ez)
{
	m_spotDirection[0] = ex;
	m_spotDirection[1] = ey;
	m_spotDirection[2] = ez;
}

void CIvfOldLight::setSpotExponent(float exponent)
{
	m_spotExponent = exponent;
}

CIvfOldLight::TLightType CIvfOldLight::getType()
{
	return m_lightType;
}

void CIvfOldLight::getSpotDirection(float &ex, float &ey, float &ez)
{
	ex = m_spotDirection[0];
	ey = m_spotDirection[1];
	ez = m_spotDirection[2];
}

float CIvfOldLight::getSpotCutoff()
{
	return m_spotCutoff;
}

float CIvfOldLight::getSpotExponent()
{
	return m_spotExponent;
}

void CIvfOldLight::activate()
{
	if (!m_active)
	{
		for ( int i = 0 ; i < (int)IVF_MAX_LIGHTS ; ++i ) {
			if ( !(lightmask & 1 << i) ) {
				//bit i is empty i.e. light i is available
				m_lightn = i;
				lightmask = lightmask | 1 << i;
				break;
			}
		}
		GLenum light = GL_LIGHT0 + m_lightn;
		glEnable( light );
		m_active = true;
	}
}

void CIvfOldLight::deactivate()
{
	if ( m_lightn != -1 ) 
		lightmask = lightmask ^ (1 << m_lightn); 

	GLenum light = GL_LIGHT0 + m_lightn;
	glDisable( light );

	m_lightn = -1;

	m_active = false;
}
