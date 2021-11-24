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

IvfSmartPointer(Viewport);

/**
 * View port definition class
 *
 * This class defines a viewport in OpenGL. Used
 * together with View derived classes.
 */
class IVF_API Viewport : public Base {
private:
	int m_pos[2];
	int m_size[2];
public:
	/** Class constructor */
	Viewport();

	/** Class destructor */
	virtual ~Viewport();

	IvfClassInfo("Viewport",Base);
	IvfStdFactory(Viewport);

	/** Applies the current viewport */
	void apply();

	/** Set position of viewport */
	void setPosition(int x, int y);

	/** Return position of viewport */
	void getPosition(int &x, int &y);

	/** Set viewport dimensions */
	void setSize(int width, int height);

	/** Return viewport dimensions */
	void getSize(int &width, int &height);

	/** Return viewport aspect ratio */
	double getAspect();
};

}