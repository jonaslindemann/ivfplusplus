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

IvfSmartPointer(CQuat);

/**
 * Quaternion class
 *
 * Interface not finished
 */
class IVFMATH_API CQuat : public CMathBase {
private:
	CVec3d m_q;
	double m_qw;
public:
	/** CQuat constructor */
	CQuat();
	
	/** CQuat copy constructor */
	CQuat(CQuat& v);

	/** CQuat destructor */
	virtual ~CQuat();

	IvfClassInfo("CQuat",CMathBase);

	void getComponents(double &qx, double &qy, double &qz, double &qw);
	void setComponents(double qx, double qy, double qz, double qw);

	CVec3d& getVectorPart();
	double getRealPart();

	void getAxisAngle(double& vx, double& vy, double &vz, double &angle);
	void setFromAxisAngle(double vx, double vy, double vz, double angle);
	void setRealPart(double value);
	CQuat& inv();
	double norm();
	void identity();
	CQuat& conjugate();

	CQuat& operator=(CQuat& a);
	CQuat& operator*(CQuat& a);
	CQuat& operator*(double b);
	CQuat& operator/(double b);
	CQuat& operator+(CQuat& a);
	CQuat& operator-(CQuat& a);
};

CQuat& ivfGetTempQuat();

#endif 
