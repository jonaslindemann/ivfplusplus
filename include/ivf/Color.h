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

#include <ivf/Base.h>

namespace ivf {

IvfSmartPointer(Color);

/**
 * Color class
 *
 * Defines a (r,g,b,a) color class. Use to store colors in the
 * CIvfGLPrimitive derived classes.
 */
class IVF_API Color : public Base {
private:
	float m_color[4];
public:
	/** CIvfColor constructor */
	Color();

	/** CIvfColor destructor */
	~Color();

	IvfClassInfo("Color",Base);
	IvfStdFactory(Color);

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

}