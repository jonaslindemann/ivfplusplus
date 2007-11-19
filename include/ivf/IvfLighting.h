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

#ifndef _CIvfLighting_h_
#define _CIvfLighting_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfLight.h>

#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(CIvfLighting);

/**
 * Lighting singleton class
 *
 * The CIvfLighting singleton encapsulates OpenGL lighting. 
 * An instance of the CIvfLighting class is retrieved using the
 * getInstance() method. See the following code:
 *
 * \code
 * CIvfLightingPtr lighting = CIvfLighting::getInstance();
 * lighting->enable();
 * CIvfLightPtr light = lighting->getLight(0);
 * light->enable();
 * ...
 * \endcode
 *
 * @author Carl-Johan Lejdfors and Jonas Lindemann
 */
class IVF_API CIvfLighting : public CIvfBase {
private:
	static CIvfLighting* m_instance;
	static CIvfSingletonDestroyer<CIvfLighting> m_destroyer;
	std::vector<CIvfLight*> m_lights;
	GLfloat m_ambient[4];
	GLint m_local[1], m_twoside[1];
	std::vector<bool> m_enabled;
public:	/** Return instance of CIvfLighting */
	static CIvfLighting* getInstance();

	IvfClassInfo("CIvfLighting",CIvfBase);


	/** Renders all lights */
	void render();

	/** Enables lighting */
	void enable();

	/** Disables lighting */
	void disable();

	/** Returns true if lighting is enabled */
	bool isEnabled();

	/** Disables all lights */
	void disableLights();
	
	/** Enables all lights */
	void enableLights();

	/** Saves lighting state */
	void saveState();

	/** Restores the lighting state */
	void restoreState();

	/** Enables/Disables two-sided lighting */
	void setTwoSide(bool flag);

	/** Returns two-sided lighting flag */
	bool getTwoSide();

	/** Enables/Disables local viewer light model */
	void setLocalViewer(bool flag);

	/** Returns local viewer light model flag */
	bool getLocalViewer();

	/** 
	 * Retrieve an OpenGL light source 
	 *
	 * param idx should be between 0 and getSize()-1
	 */
	CIvfLight* getLight(int idx);

	/** Return the maximum light sources */
	int getSize();

	/** Sets the ambient color level */
	void setAmbientColor(float red, float green, float blue, float alpha);

	/** Save state of enabled lights */
	void restoreEnabledState();

	/** Restore state of enabled lights */
	void saveEnabledState();
protected:
	/** Protected constructor (do not use) */
	CIvfLighting();
	~CIvfLighting();
	friend class CIvfSingletonDestroyer<CIvfLighting>;
};
 
#endif 
