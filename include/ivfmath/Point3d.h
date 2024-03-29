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

#pragma once

#include <ivfmath/Vec3d.h>

namespace ivf {

IvfSmartPointer(Point3d);

/**
 * Simple point class.
 *
 * CIvfPojnt3d is a simple point class. Defining a set of methods for
 * managing a point in 3D space.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Point3d : public Vec3d {
private:
public:
	IvfClassInfo("Point3d",Vec3d);
	IvfStdFactory(Point3d);
};

//class IVFMATH_API CIvfPojnt3d : public MathBase {
//public:
//	/** CIvfPojnt3d constructor. */
//	CIvfPojnt3d ();
//	
//	/** CIvfPojnt3d destructor. */
//	virtual ~CIvfPojnt3d ();
//
//	/**
//	 * Set point
//	 * @param x X coordinate
//	 * @param y Y coordinate
//	 * @param z Z coordinate
//	 */
//	void setPoint (const double x, const double y, const double z);
//
//	/** Return point */
//	void getPoint (double &x, double &y, double &z);
//
//	/** Return pointer to vector */
//	double* getPoint();
//
//	/** Add a displacement to the current point */
//	void add(double dx, double dy, double dz);
//protected:
//	double m_coord[3];
//private:
//};
}