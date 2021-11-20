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

#include <ivf/Switch.h>
#include <ivf/Camera.h>

namespace ivf {

IvfSmartPointer(CLOD);

/**
 * Level of detail class
 * 
 * Handles the selection of multiple representations
 * depending on distance to the viewer.
 * @author Jonas Lindemann
 */
class IVF_API CLOD : public CSwitch {
public:
	/** Constructor */
	CLOD ();

	/** Destructor */
	virtual ~CLOD ();

	IvfClassInfo("CLOD",CSwitch);

	/** Set camera used to determine viewer distance */
	void setCamera(CCamera* camera);

	/** Return camera */
	CCamera* getCamera();

	/** 
	 * Set near and far limits
	 *
	 * @param nearLimit Determines the distance
	 * where the first shape is used
	 * @param farLimit Determines the distance
	 * where the last shape is used
	 */
	void setLimits(double nearLimit, double farLimit);

	/** Returns near and far limits */
	void getLimits(double &nearLimit, double &farLimit);
private:
	double m_nearLimit;
	double m_farLimit;

	CCamera* m_camera;
	virtual void doCreateGeometry();
protected:
	double theta(double x);
	virtual double lodFunction(double dist);
};

/** \example lod.cpp
 * This is an example of how to use the CLOD/CSwitch classes.
 */

}