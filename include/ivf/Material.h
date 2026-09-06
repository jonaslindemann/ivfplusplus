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

#pragma once

#include <ivf/GLBase.h>
#include <ivf/ShaderProgram.h>

namespace ivf {

IvfSmartPointer(Material);

/**
 * OpenGL material definition.
 *
 * CIvfMateral contains a material definition as used in OpenGL.
 * The class is used by classes derived from the Shape class.
 *
 * @author Jonas Lindemann
 */
class IVF_API Material : public GLBase {
public:
	bool getColorMaterial();
	void setColorMaterial(bool flag);
	/** Material constructor */
	Material ();

	/** Material destructor */
	virtual ~Material ();

	IvfClassInfo("Material",GLBase);
	IvfStdFactory(Material);

	/**
	 * A state object, not geometry -- and the one that feeds the shader its
	 * material uniforms. It must never have the program unbound around it, or
	 * every object drawn after it renders with the default material.
	 */
	virtual bool hasModernPath() override;


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
	void assignFrom(Material* material);
	void brightnessEmission(double factor);
	void brightnessAmbient(double factor);
	void brightnessDiffuse(double factor);
	void brightnessSpecular(double factor);

	/**
	 * Upload material properties to a shader program.
	 *
	 * Sets uMatAmbient, uMatDiffuse, uMatSpecular, uMatEmission, uMatShininess.
	 * Called automatically from doCreateMaterial() when an active shader is set
	 * in RenderContext. Can also be called directly.
	 */
	void uploadToShader(ShaderProgram* prog);

	/**
	 * Discards the redundant-material-state cache.
	 *
	 * doCreateMaterial() skips the glMaterialfv() calls when the requested
	 * material values are identical to the ones already applied. That shortcut is
	 * only valid while Material is the sole author of GL material state, so any
	 * code that changes it behind the class's back -- glMaterialfv() by hand,
	 * glColorMaterial(), or a glPushAttrib()/glPopAttrib() pair spanning material
	 * state -- must call this afterwards.
	 *
	 * Note the cache compares values rather than Material identity, so distinct
	 * Material instances holding equal values still collapse to a single upload.
	 */
	static void invalidateStateCache();

	/**
	 * Enables or disables the redundant-material-state cache, returning the
	 * previous setting.
	 *
	 * Must be disabled while recording a display list: inside glNewList() the
	 * material calls are captured rather than executed, so what the cache believes
	 * is current says nothing about what the list contains.
	 */
	static bool setStateCacheEnabled(bool flag);

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
	virtual void doCreateMaterial() override;
};
}