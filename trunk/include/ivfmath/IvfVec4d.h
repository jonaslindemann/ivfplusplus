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

#ifndef _CIvfVec4d_h_
#define _CIvfVec4d_h_

#include <ivf/ivfconfig.h>
#include <ivfmath/IvfMathBase.h>

IvfSmartPointer(CIvfVec4d);

/**
 * 4d vector class.
 *
 * CIvfVec3d implements a 4d vector and associated
 * operations and arithmetic.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfVec4d : public CIvfMathBase {
private:
	double m_vector[4];
public:
	/** CIvfVec3d constructor */
	CIvfVec4d();

	/** CIvfVec3d constructor */
	CIvfVec4d(double vx, double vy, double vz);

	/** CIvfVec3d constructor */
	CIvfVec4d(double vx, double vy, double vz, double vw);

	/** CIvfVec3d copy constructor */
	CIvfVec4d(CIvfVec4d& v);

	/** CIvfVec3d destructor */
	virtual ~CIvfVec4d();

	IvfClassInfo("CIvfVec4d",CIvfMathBase);

	/** Rotate vector angle about axis */
	void rotate(CIvfVec4d& axis, double angle);

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
	CIvfVec4d& operator+=(CIvfVec4d& a);

	/** Vector/Vector -= operator */
	CIvfVec4d& operator-=(CIvfVec4d& a);

	
	/** Vector/scalar = operator */
	CIvfVec4d& operator=(CIvfVec4d& a);

	/** Index operator */
	double operator[](const int idx);

};

/** Vector/Vector cross product operator */
IVFMATH_API CIvfVec4d& operator*(CIvfVec4d& a, CIvfVec4d& b);

/** Vector/Scalar elemental multiplication operator */
IVFMATH_API CIvfVec4d& operator*(CIvfVec4d& a, double b); 

/** Scalar/Vector elemental multiplication operator */
IVFMATH_API CIvfVec4d& operator*(double a, CIvfVec4d& b);

/** Vector/Vector + operator */
IVFMATH_API CIvfVec4d& operator+(CIvfVec4d& a, CIvfVec4d& b);

/** Vector/Vector - operator */
IVFMATH_API CIvfVec4d& operator-(CIvfVec4d& a, CIvfVec4d& b);

IVFMATH_API CIvfVec4d& ivfGetTempVec4d();

/** \example math.cpp */

#endif 
