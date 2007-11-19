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

#ifndef _CIvfQuat_h_
#define _CIvfQuat_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>

IvfSmartPointer(CIvfQuat);

/**
 * Quaternion class
 *
 * Interface not finished
 */
class IVFMATH_API CIvfQuat : public CIvfMathBase {
private:
	CIvfVec3d m_q;
	double m_qw;
public:
	/** CIvfQuat constructor */
	CIvfQuat();
	
	/** CIvfQuat copy constructor */
	CIvfQuat(CIvfQuat& v);

	/** CIvfQuat destructor */
	virtual ~CIvfQuat();

	IvfClassInfo("CIvfQuat",CIvfMathBase);

	void getComponents(double &qx, double &qy, double &qz, double &qw);
	void setComponents(double qx, double qy, double qz, double qw);

	CIvfVec3d& getVectorPart();
	double getRealPart();

	void getAxisAngle(double& vx, double& vy, double &vz, double &angle);
	void setFromAxisAngle(double vx, double vy, double vz, double angle);
	void setRealPart(double value);
	CIvfQuat& inv();
	double norm();
	void identity();
	CIvfQuat& conjugate();

	CIvfQuat& operator=(CIvfQuat& a);
	CIvfQuat& operator*(CIvfQuat& a);
	CIvfQuat& operator*(double b);
	CIvfQuat& operator/(double b);
	CIvfQuat& operator+(CIvfQuat& a);
	CIvfQuat& operator-(CIvfQuat& a);
};

CIvfQuat& ivfGetTempQuat();

#endif 
