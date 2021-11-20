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

#ifndef _CIvfVec3d_h_
#define _CIvfVec3d_h_

#include <ivf/ivfconfig.h>
#include <ivfmath/IvfMathBase.h>

IvfSmartPointer(CVec3d);

/**
 * 3d vector class.
 *
 * CVec3d implements a 3d vector and associated
 * operations and arithmetic.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CVec3d : public CMathBase {
private:
	double m_vector[3];
public:
	/** CVec3d constructor */
	CVec3d();

	/** CVec3d constructor */
	CVec3d(double vx, double vy, double vz);

	/** CVec3d copy constructor */
	CVec3d(CVec3d& v);

	/** CVec3d destructor */
	virtual ~CVec3d();

	IvfClassInfo("CVec3d",CMathBase);

	/** Rotate vector angle about axis */
	void rotate(CVec3d& axis, double angle);

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

	/** Set vector from two points */
	void setFromPoints(CVec3d& pos, CVec3d& target);

	/** Add components to vector (obsolete) */
	void add(double dx, double dy, double dz);

	/** Calculate euler angles */
	void getEulerAngles(double &pitch, double &heading);

	/** Set vector components */
	void setComponents(double vx, double vy, double vz);

	/** Get vector components */
	void getComponents(double &vx, double &vy, double &vz);

	/** Set vector components */
	void setComponents(const double *v);

	/** Get vector components */
	void getComponents(double* v);

	/** Get pointer to vector components */
	double* getComponents();

	/** Set z component */
	void setZ(double value);

	/** Set y component */
	void setY(double value);

	/** Set x component */
	void setX(double value);

	CVec3d& operator+=(CVec3d& a);
	CVec3d& operator-=(CVec3d& a);
	//CVec3d& operator=(CVec3d& a);
	CVec3d& operator=(CVec3d& a);
	double operator[](const int idx);

	bool operator==(CVec3d& a);
	bool operator!=(CVec3d& a);

};

IVFMATH_API CVec3d& operator*(CVec3d& a, CVec3d& b);
IVFMATH_API CVec3d& operator*(CVec3d& a, double b); 
IVFMATH_API CVec3d& operator*(double a, CVec3d& b);
IVFMATH_API CVec3d& operator+(CVec3d& a, CVec3d& b);
IVFMATH_API CVec3d& operator-(CVec3d& a, CVec3d& b);

IVFMATH_API CVec3d& ivfGetTempVec3d();

/** \example math.cpp
 * This is an example of how to use some of the math classes.
 */

#endif 
