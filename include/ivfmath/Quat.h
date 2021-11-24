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
#include <ivfmath/Vec3d.h>

namespace ivf {

IvfSmartPointer(Quat);

/**
 * Quaternion class
 *
 * Interface not finished
 */
class IVFMATH_API Quat : public MathBase {
private:
	Vec3d m_q;
	double m_qw;
public:
	/** Quat constructor */
	Quat();
	
	/** Quat copy constructor */
	Quat(Quat& v);

	/** Quat destructor */
	virtual ~Quat();

	IvfClassInfo("Quat",MathBase);
	IvfStdFactory(Quat);

	void getComponents(double &qx, double &qy, double &qz, double &qw);
	void setComponents(double qx, double qy, double qz, double qw);

	Vec3d& getVectorPart();
	double getRealPart();

	void getAxisAngle(double& vx, double& vy, double &vz, double &angle);
	void setFromAxisAngle(double vx, double vy, double vz, double angle);
	void setRealPart(double value);
	Quat& inv();
	double norm();
	void identity();
	Quat& conjugate();

	Quat& operator=(Quat& a);
	Quat& operator*(Quat& a);
	Quat& operator*(double b);
	Quat& operator/(double b);
	Quat& operator+(Quat& a);
	Quat& operator-(Quat& a);
};

Quat& ivfGetTempQuat();

}