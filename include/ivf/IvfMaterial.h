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

#ifndef _CIvfMaterial_h_
#define _CIvfMaterial_h_

#include <ivf/IvfGLBase.h>

IvfSmartPointer(CMaterial);

/**
 * OpenGL material definition.
 *
 * CIvfMateral contains a material definition as used in OpenGL.
 * The class is used by classes derived from the CShape class.
 *
 * @author Jonas Lindemann
 */
class IVF_API CMaterial : public CGLBase {
public:
	bool getColorMaterial();
	void setColorMaterial(bool flag);
	/** CMaterial constructor */
	CMaterial ();

	/** CMaterial destructor */
	virtual ~CMaterial ();

	IvfClassInfo("CMaterial",CGLBase);

	/**
	 * Sets the shininess property.
	 * 
	 * See OpenGL documentation
	 * @param shininess Has a range between 0 and 128
	 */
	void setShininess(float shininess);

	/** Returns the shininess property */
	void getShininess(float &shininess);

	/**
	 * Sets the emsissive color components
	 *
	 * Sets the emissive color components using red, green and blue
	 * components. See OpenGL documentation.
	 * @param red Red color component (0.0f-1.0f)
	 * @param green Green color component (0.0f-1.0f)
	 * @param blue Blue color component (0.0f-1.0f)
	 * @param alfa Opacity component (0.0f-1.0f)
	 */
	void setEmissionColor(const float red, const float green, const float blue, const float alfa);
	/** Returns the emissive color components */
	void getEmissionColor(float &red, float &green, float &blue, float &alfa);
	
	/**
	 * Sets the diffuse color components
	 *
	 * Sets the diffuse color components using red, green and blue
	 * components. See OpenGL documentation.
	 * @param red Red color component (0.0f-1.0f)
	 * @param green Green color component (0.0f-1.0f)
	 * @param blue Blue color component (0.0f-1.0f)
	 * @param alfa Opacity component (0.0f-1.0f)
	 */
	void setDiffuseColor (const float red, const float green, const float blue, const float alfa);
	
	/** Returns the diffuse color components */
	void getDiffuseColor (float &red, float &green, float &blue, float &alfa);
	
	/**
	 * Sets the specular color components
	 *
	 * Sets the specular color components using red, green and blue
	 * components. See OpenGL documentation.
	 * @param red Red color component (0.0f-1.0f)
	 * @param green Green color component (0.0f-1.0f)
	 * @param blue Blue color component (0.0f-1.0f)
	 * @param alfa Opacity component (0.0f-1.0f)
	 */
	void setSpecularColor (const float red, const float green, const float blue, const float alfa);
	
	/** Returns the specular color components */
	void getSpecularColor (float &red, float &green, float &blue, float &alfa);
	
	/**
	 * Sets the ambient color components
	 *
	 * Sets the ambient color components using red, green and blue
	 * components. See OpenGL documentation.
	 * @param red Red color component (0.0f-1.0f)
	 * @param green Green color component (0.0f-1.0f)
	 * @param blue Blue color component (0.0f-1.0f)
	 * @param alfa Opacity component (0.0f-1.0f)
	 */
	void setAmbientColor (const float red, const float green, const float blue, const float alfa);
	
	/** Returns the ambient color components */
	void getAmbientColor (float &red, float &green, float &blue, float &alfa);

	/** Set material alpha value for all material properties */
	void setAlphaValue(float alpha);
	
	void multiplyColor(double factor);
	void multiplyEmission(double factor);
	void multiplyAmbient(double factor);
	void multiplyDiffuse(double factor);
	void multiplySpecular(double factor);
	void assignFrom(CMaterial* material);
	void brightnessEmission(double factor);
	void brightnessAmbient(double factor);
	void brightnessDiffuse(double factor);
	void brightnessSpecular(double factor);

private:
	float	m_shininess;
	float	m_diffuseColor[4];
	float	m_specularColor[4];
	float	m_ambientColor[4];
	float	m_emissionColor[4];

	bool m_colorMaterial;

	void modifyBrightness(float* color, double factor);
	void toGreyscale(float* color, float* grey);
protected:
	virtual void doCreateMaterial();
};
#endif
