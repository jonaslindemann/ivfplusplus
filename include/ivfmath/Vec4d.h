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

#include <ivf/config.h>
#include <ivfmath/MathBase.h>

namespace ivf {

IvfSmartPointer(Vec4d);

/**
 * 4d vector class.
 *
 * Vec3d implements a 4d vector and associated
 * operations and arithmetic.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Vec4d : public MathBase {
private:
	double m_vector[4];
public:
	/** Vec3d constructor */
	Vec4d();

	/** Vec3d constructor */
	Vec4d(double vx, double vy, double vz);

	/** Vec3d constructor */
	Vec4d(double vx, double vy, double vz, double vw);

	/** Vec3d copy constructor */
	Vec4d(Vec4d& v);

	/** Vec3d destructor */
	virtual ~Vec4d();

	IvfClassInfo("Vec4d",MathBase);
	IvfStdFactory(Vec4d);

	/** Rotate vector angle about axis */
	void rotate(Vec4d& axis, double angle);

	/** Normalize vector */
	void normalize();

	/** Return vector length */
	double length();

	/** Negate vector */
	void negate();

#ifdef IVF_HAVE_IOSTREAM
	/** Print vector to output stream */
	void print(std::ostream &out);
#else
	/** Print vector to output stream */
	void print(ostream &out);
#endif

	/** Return euler angles */
	void getEulerAngles(double &pitch, double &heading);

	/** Set vector components (vw = 0.0) */
	void setComponents(double vx, double vy, double vz);

	/** Set vector components */
	void setComponents(double vx, double vy, double vz, double vw);

	/** Get vector components */
	void getComponents(double &vx, double &vy, double &vz, double &vw);

	/** Get vector components */
	void getComponents(double &vx, double &vy, double &vz);

	/** Get vector components */
	void setComponents(const double *v);

	/** Get pointer to vector components */
	void getComponents(double* v);

	/** Set vector z value */
	void setZ(double value);

	/** Set vector y value */
	void setY(double value);

	/** Set vector x value */
	void setX(double value);

	/** Vector/Vector += operator */
	Vec4d& operator+=(Vec4d& a);

	/** Vector/Vector -= operator */
	Vec4d& operator-=(Vec4d& a);

	
	/** Vector/scalar = operator */
	Vec4d& operator=(Vec4d& a);

	/** Index operator */
	double operator[](const int idx);

};

/** Vector/Vector cross product operator */
IVFMATH_API Vec4d& operator*(Vec4d& a, Vec4d& b);

/** Vector/Scalar elemental multiplication operator */
IVFMATH_API Vec4d& operator*(Vec4d& a, double b); 

/** Scalar/Vector elemental multiplication operator */
IVFMATH_API Vec4d& operator*(double a, Vec4d& b);

/** Vector/Vector + operator */
IVFMATH_API Vec4d& operator+(Vec4d& a, Vec4d& b);

/** Vector/Vector - operator */
IVFMATH_API Vec4d& operator-(Vec4d& a, Vec4d& b);

IVFMATH_API Vec4d& ivfGetTempVec4d();

/** \example math.cpp */

}