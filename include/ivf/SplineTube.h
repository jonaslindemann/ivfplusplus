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

#include <ivfmath/Spline3d.h>

#include <ivf/TubeExtrusion.h>

namespace ivf {

IvfSmartPointer(CSplineTube);

/**
 * Spline extrusion class
 * 
 * Implements a extrusion using a CSpline3d spine
 */
class IVF_API CSplineTube : public CTubeExtrusion {
private:
	double m_resolution;
	CSpline3dPtr m_spline;
public:
	/** CSplineTube constructor */
	CSplineTube();

	/** CSplineTube destructor */
	virtual ~CSplineTube();

	IvfClassInfo("CSplineTube",CTubeExtrusion);

	/** Set the number of sides in the circular section */
	void setSides(int sides);

	/** Sets the spline used as a spine */
	void setSpline(CSpline3d* spline);
	CSpline3d* getSpline();

	/** Set the precision when creating the polyline representation */
	void setPrecision(double resolution);

	/** Updates the extrusion when the spline geometry changes */
	virtual void refresh();
};

}