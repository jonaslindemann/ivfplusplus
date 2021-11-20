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

#ifndef _CIvfFog_h_
#define _CIvfFog_h_

#include <ivf/IvfBase.h>
#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(CFog);

/**
 * Fog singleton class
 *
 * The CIvfFog singleton encapsulates OpenGL fog handling. 
 * An instance of the CIvfFog class is retrieved using the
 * getInstance() method. See the following code:
 *
 * \code
 * CIvfFogPtr fog = CIvfFog::getInstance();
 * fog->enable();
 * fog->setType(CIvfFog::FT_LINEAR);
 * fog->setLimits(5.0, 20.0);
 * ...
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CFog : public CBase {
public:
	enum TFogType {
		FT_LINEAR,
		FT_EXP,
		FT_EXP2
	};	
private:
	static CFog* m_instance;
	static CSingletonDestroyer<CFog> m_destroyer;
	float m_fogColor[4];
	double m_fogStart;
	double m_fogEnd;
	TFogType m_type;
public:
	/** Returns the blending singleton */
	static CFog* getInstance();

	IvfClassInfo("CIvfFog",CBase);

	/** Enable fog */
	void enable();

	/** Disable fog */
	void disable();

	/** Returns true if fog is enabled */
	bool isEnabled();

	/** 
	 * Set fog type 
	 * 
	 * param type FT_LINEAR, FT_EXP or FT_EXP2 see also OpenGL documentation
	 */
	void setType(TFogType type);

	/** Return fog type */
	TFogType getType();

	/** 
	 * Set fog limits
	 *
	 * Sets the limits used for FT_LINEAR fog type.
	 *
	 * param start eye distance to fog starting
	 * param end eye distance to fog end
	 */
	void setLimits(double start, double end);

	/** Sets eye distance to fog start */
	void setStart(double start);

	/** Sets eye distance to fog end */
	void setEnd(double end);

	/** Return fog limits */
	void getLimits(double &start, double &end);

	/** Return eye distance to fog start */
	double getStart();

	/** Return eye distance to fog end */
	double getEnd();

	/** Set fog density */
	void setDensity(double density);

	/** Return fog density */
	double getDensity();

	/** Sets fog color */
	void setColor(float red, float green, float blue, float alpha);

	/** Returns fog color */
	void getColor(float &red, float &green, float &blue, float &alpha);
protected:
	CFog();
	friend class CSingletonDestroyer<CFog>;
};

#endif 
