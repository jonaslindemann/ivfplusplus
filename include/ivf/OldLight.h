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
// Modified by Carl-Johan Lejdfors
//

#pragma once

#include <ivf/GLBase.h>

namespace ivf {

IvfSmartPointer(OldLight);

static size_t IVF_MAX_LIGHTS = 8;

/**
 * Light class
 *
 * Implements a OpenGL lightsource. When used it determines
 * an available light. If all lights are allocated in OpenGL
 * the light will not be rendered.
 *
 * @author Carl Johan Lejdfors
 */
class IVF_API OldLight : public GLBase {
public:
	enum TLightType {
		LT_POINT,
		LT_DIRECTIONAL,
		LT_SPOT
	};
public:
	/** Constructor */
	OldLight();

	/** Destructor */
	virtual ~OldLight();

	/** Assignment operator */
	OldLight& operator = ( const OldLight& ); 
	
	/** void* operator */
	operator void*(); 

	IvfClassInfo("OldLight",GLBase);

	/** Set ambient light components */
	void setAmbient( const GLfloat r, const GLfloat g, 
		const GLfloat b, const GLfloat a );
	/** Set diffuse light components */
	void setDiffuse( const GLfloat r, const GLfloat g, 
		const GLfloat b, const GLfloat a );
	/** Set specular light components */
	void setSpecular( const GLfloat r, const GLfloat g, 
		const GLfloat b, const GLfloat a );
	
	/** 
	 * Set light position
	 *
	 * @param x light x coordinate
	 * @param y light y coordinate
	 * @param z light z coordinate
	 * @param w light type (w = 0 directional light, w=1 positional light)
	 */
	void setPosition( const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w );
	void setPosition( const GLfloat x, const GLfloat y, const GLfloat z);
	void setDirection( const GLfloat x, const GLfloat y, const GLfloat z);

	/** Set constant attenuation */
	void setConstAtt( const GLint a );
	
	/** Set linear attenuation */
	void setLinAtt( const GLint a );

	/** Set quadratic attenutation */
	void setQuadAtt( const GLint a );

	/** Return ambient light components */
	void getAmbient( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const;

	/** return diffuse light components */
	void getDiffuse( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const;

	/** return specular light components */
	void getSpecular( GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a ) const;
	
	/** return light position */
	void getPosition( GLfloat &x, GLfloat &y, GLfloat &z, GLfloat &w ) const;

	/** return constant attenuation */
	void getConstAtt( GLint &a ) const;

	/** return linear attenuation */
	void getLinAtt( GLint &a ) const;

	/** return quadratic attenuation */
	void getQuadAtt( GLint &a ) const;

	/** render light source */
	void render();

	/** Set spotlight exponent value */
	void setSpotExponent(float exponent);

	/** Return spotlight exponent value */
	float getSpotExponent();

	/** Set spotlight cutoff angle */
	void setSpotCutoff(float cutoff);

	/** Return spotlight cutoff angle */
	float getSpotCutoff();

	/** Set direction of spotlight */
	void setSpotDirection(float ex, float ey, float ez);

	/** Return spotlight direction */
	void getSpotDirection(float &ex, float &ey, float &ez);

	/** 
	 * Set light type
	 *
	 * @param lightType can be LT_POINT for a point source,
	 * LT_DIRECTIONAL for a directional light source and 
	 * LT_SPOT for a spotlight
	 */
	void setType(TLightType lightType);

	/** Return current type of light */
	TLightType getType();

	/** 
	 * Activate light 
	 *
	 * When a light is active it will be rendered and enabled 
	 * (glEnable(GL_LIGHTx).
	 */
	void activate();

	/** 
	 * Activate light 
	 *
	 * When a light is active it will not be rendered or enabled 
	 * (glEnable(GL_LIGHTx).
	 */
	void deactivate();
	
protected:
	// disallow copying 
	OldLight( const OldLight& );
	
	static long lightmask; //available lights
	
private:
	int m_lightn; 
	
	GLfloat m_ambient[4];
	GLfloat m_diffuse[4];
	GLfloat m_specular[4];
	GLfloat m_position[4];
	GLint m_constatt;
	GLint m_linatt;
	GLint m_quadatt;

	GLfloat m_spotCutoff;
	GLfloat m_spotDirection[3];
	GLfloat m_spotExponent;

	TLightType m_lightType;
	bool m_active;
};
/** \example lighting.cpp
 * This is an example of how to use the OldLight and 
 * OldLightModel classes.
 */
}