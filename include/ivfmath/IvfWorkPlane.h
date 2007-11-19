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

#ifndef _CIvfWorkPlane_h_
#define _CIvfWorkPlane_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>
#include <ivfmath/IvfVec4d.h>
#include <ivfmath/IvfMat4d.h>
#include <ivfmath/IvfPoint3d.h>

/**
 * User coordinate class.
 *
 * CIvfUcs3d implements a user defined coordinate system and
 * routines to convert to and from this. Coordinate snap is 
 * also implemented.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfWorkPlane : public CIvfMathBase {
private:
	double m_snapUnit;

	CIvfVec3d m_zAxis;
	CIvfVec3d m_yAxis;
	CIvfVec3d m_xAxis;

	double m_theta;
	double m_vz;
	double m_vy;
	double m_vx;
	double m_tz;
	double m_tx;
	double m_ty;

	CIvfMat4d m_transformationMatrix;
	CIvfMat4d m_inversionMatrix;
	CIvfVec4d m_pointWorld;
	CIvfVec4d m_pointTransformed;

public:
	CIvfWorkPlane();
	virtual ~CIvfWorkPlane();

	bool isClass(char* name);
	void getClassName(char* name);

	/** Transform point to world coordinate system */
	void transformWorld(CIvfPoint3d* point);

	/** Transform point to local coordinate system */
	void transform(CIvfPoint3d* point);

	/** Transform (sx, sy, sz) to world coordinate system */
	void transformWorld(double sx, double sy, double sz, double &wx, double &wy,  double &wz);

	/** Transform (wx, wy, wz) to local coordinate system */
	void transform(double wx, double wy, double wz, double &sx, double &sy, double &sz);

	/** Snap (x, y, z) to grid */
	void snap(double &x, double &y, double &z);

	/** Set snap unit */
	void setSnapUnit(double unit);

	/** Return snap unit */
	double getSnapUnit();

	/** Determine plane intersection */
	void intersect(double x0, double y0, double z0, double i, double j, double k, double &x, double &y, double &z);

	/** Set coordinate system translation */
	void setTranslation(double tx, double ty, double tz);

	/** Return coordinate system translation */
	void getTranslation(double &tx, double &ty, double &tz);

	/** Set coordinate system rotation */
	void setRotation(double vx, double vy, double vz, double theta);
	
	/** Return coordinate system rotation */
	void getRotation(double & vx, double & vy, double & vz, double & theta);

protected:
	void calcMatrix();
};

#endif 
