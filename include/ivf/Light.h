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

#pragma once

#include <ivf/Shape.h>

namespace ivf {

IvfSmartPointer(CLight);

/**
 * Light class
 *
 * Implements a single OpenGL light source. Instances of the CLight
 * class should be retrieved from the CLighting singleton.
 *
 * @author Carl-Johan Lejdfors and Jonas Lindemann
 */
class IVF_API CLight : public CShape {
public:
	enum TLightType {
		LT_POINT,
		LT_DIRECTIONAL,
		LT_SPOT
	};
private:
	int m_lightn; 
	
	GLfloat m_ambient[4];
	GLfloat m_diffuse[4];
	GLfloat m_specular[4];
	GLfloat m_position[4];
	GLint m_constatt;
	GLint m_linatt;
	GLint m_quadatt;

	float m_spotCutoff;
	float m_spotDirection[3];
	float m_spotExponent;

	TLightType m_lightType;
	bool m_enabled;
public:
	CLight();
	virtual ~CLight();

	/** Enable light */
	void enable();

	/** Disable light */
	void disable();

	/** Returns true if light enabled */
	bool isEnabled();

	/** Set the number of the light = GL_LIGHT0 + number */
	void setNumber(int number);

	IvfClassInfo("CLight",CShape);

	/** Set ambient light components */
	void setAmbientColor( const float r, const float g, 
		const float b, const float a );
	/** Set diffuse light components */
	void setDiffuseColor( const float r, const float g, 
		const float b, const float a );
	/** Set specular light components */
	void setSpecularColor( const float r, const float g, 
		const float b, const float a );
	
	/** 
	 * Set light position
	 *
	 * @param x light x coordinate
	 * @param y light y coordinate
	 * @param z light z coordinate
	 * @param w light type (w = 0 directional light, w=1 positional light)
	 */
	void setLightPosition( const float x, const float y, const float z, const float w );
	void setLightPosition( const float x, const float y, const float z);
	void setDirection( const float x, const float y, const float z);

	/** Set constant attenuation */
	void setConstAtt( const int a );
	
	/** Set linear attenuation */
	void setLinAtt( const int a );

	/** Set quadratic attenutation */
	void setQuadAtt( const int a );

	/** Return ambient light components */
	void getAmbientColor( float &r, float &g, float &b, float &a ) const;

	/** return diffuse light components */
	void getDiffuseColor( float &r, float &g, float &b, float &a ) const;

	/** return specular light components */
	void getSpecularColor( float &r, float &g, float &b, float &a ) const;
	
	/** return light position */
	void getLightPosition( float &x, float &y, float &z, float &w ) const;

	/** return constant attenuation */
	void getConstAtt( int &a ) const;

	/** return linear attenuation */
	void getLinAtt( int &a ) const;

	/** return quadratic attenuation */
	void getQuadAtt( int &a ) const;

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
	 * param lightType can be LT_POINT for a point source,
	 * LT_DIRECTIONAL for a directional light source and 
	 * LT_SPOT for a spotlight
	 */
	void setType(TLightType lightType);

	/** Return current type of light */
	TLightType getType();

protected:
	virtual void doCreateGeometry();
};

}