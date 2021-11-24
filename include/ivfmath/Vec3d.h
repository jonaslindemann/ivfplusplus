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

#include <ivfmath/MathBase.h>

namespace ivf {

IvfSmartPointer(Vec3d);

/**
 * 3d vector class.
 *
 * Vec3d implements a 3d vector and associated
 * operations and arithmetic.
 *
 * @author Jonas Lindemann
 */
class Vec3d : public MathBase {
private:
	double m_vector[3];
public:
	/** Vec3d constructor */
	Vec3d();

	/** Vec3d constructor */
	Vec3d(double vx, double vy, double vz);

	/** Vec3d copy constructor */
	Vec3d(Vec3d& v);

	/** Vec3d destructor */
	virtual ~Vec3d();

	IvfClassInfo("Vec3d",MathBase);
	IvfStdFactory(Vec3d);

	/** Rotate vector angle about axis */
	void rotate(Vec3d& axis, double angle);

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
	void setFromPoints(Vec3d& pos, Vec3d& target);

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

	Vec3d& operator+=(Vec3d& a);
	Vec3d& operator-=(Vec3d& a);
	//Vec3d& operator=(Vec3d& a);
	Vec3d& operator=(Vec3d& a);
	double operator[](const int idx);

	bool operator==(Vec3d& a);
	bool operator!=(Vec3d& a);

};

ivf::Vec3d& operator*(Vec3d& a, Vec3d& b);
ivf::Vec3d& operator*(Vec3d& a, double b);
ivf::Vec3d& operator*(double a, Vec3d& b);
ivf::Vec3d& operator+(Vec3d& a, Vec3d& b);
ivf::Vec3d& operator-(Vec3d& a, Vec3d& b);

Vec3d& ivfGetTempVec3d();

/** \example math.cpp
 * This is an example of how to use some of the math classes.
 */

}