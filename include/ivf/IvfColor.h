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

#ifndef _CIvfColor_h_
#define _CIvfColor_h_

#include <ivf/IvfBase.h>

IvfSmartPointer(CIvfColor);

/**
 * Color class
 *
 * Defines a (r,g,b,a) color class. Use to store colors in the
 * CIvfGLPrimitive derived classes.
 */
class IVF_API CIvfColor : public CIvfBase {
private:
	float m_color[4];
public:
	/** CIvfColor constructor */
	CIvfColor();

	/** CIvfColor destructor */
	~CIvfColor();

	IvfClassInfo("CIvfColor",CIvfBase);

	/** Set color value */
	void setColor(float red, float green, float blue);

	/** Return color value*/
	void getColor(float &red, float &green, float &blue);

	/** Returns a pointer to internal color vector */
	const float* getColor();

	/** Set color alfa value */
	void setAlfa(float alfa);

	/** Return color alfa value */
	float getAlfa();
};

#endif
