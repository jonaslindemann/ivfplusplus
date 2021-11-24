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

#include <ivfmath/MathBase.h>

namespace ivf {

/**
 * Geometric plane class
 *
 * Handles operation on a geometric plane. The class is used to
 * determine visibility of objects when culling the scene.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Plane : public MathBase {
private:
	double m_normal[3];
	double m_distance;
public:
	/** Plane constructor */
	Plane();

	/** Plane destructor */
	virtual ~Plane();

	/** Set plane normal */
	void setNormal(double vx, double vy, double vz);

	/** Return plane normal */
	void getNormal(double &vx, double &vy, double &vz);

	/** Determine plane from vectors \em v1 and \em v2 */
	void normalFromVectors(double* v1, double* v2);

	/** Set plane distance */
	void setDistance(double d);

	/** Return plane distance */
	double getDistance();

	/** Calculate plane distance from given point */
	void distanceFromPoint(double x, double y, double z);

	/** Returns true if the given sphere is in front of the plane */
	bool isSphereInFront(double x, double y, double z, double radius);

	/** Returns true if the given point is in fron of the plane */
	bool isInFront(double x, double y, double z);

	/** 
	 * Calculate plane intersection
	 *
	 * \param x0,y0,z0 starting point
	 * \param vx,vy,vz intersecting vector
	 * \param x,y,z intersection point 
	 */
	void intersect(
		double x0, double y0, double z0, 
		double vx, double vy, double vz, 
		double &x, double &y, double &z);
};

}