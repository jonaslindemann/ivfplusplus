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

#ifndef _CIvfUcs3d_h_
#define _CIvfUcs3d_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>
#include <ivfmath/IvfVec4d.h>
#include <ivfmath/IvfMat4d.h>
#include <ivfmath/IvfPoint3d.h>

IvfSmartPointer(CIvfUcs3d);

/**
 * User coordinate class.
 *
 * CIvfUcs3d implements a user defined coordinate system and
 * routines to convert to and from this. Coordinate snap is 
 * also implemented.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfUcs3d : public CIvfMathBase {
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
	/** CIvfUcs3d constructor */
	CIvfUcs3d();

	/** CIvfUcs3d destructor */
	virtual ~CIvfUcs3d();

	IvfClassInfo("CIvfUcs3d",CIvfMathBase);

	/** Transform point to world coordinate system */
	void transformWorld(CIvfPoint3d* point);

	/** Transform point to local coordinate system */
	void transform(CIvfPoint3d* point);

	/** Transform (sx, sy, sz) to world coordinate system */
	void transformWorld(double sx, double sy, double sz, double &wx, double &wy,  double &wz);

	/** Transform (wx, wy, wz) to local coordinate system */
	void transform(double wx, double wy, double wz, double &sx, double &sy, double &sz);

	/** Transform vector to world coordinates */
	CIvfVec3d& transformWorld(CIvfVec3d& vector);

	/** Transform vector to local coordinate system */
	CIvfVec3d& transform(CIvfVec3d& vector);

	/** Snap (x, y, z) to grid */
	void snap(double &x, double &y, double &z);

	/** Snap vector to grid */
	CIvfVec3d& snap(CIvfVec3d& vector);

	/** Set snap unit */
	void setSnapUnit(double unit);

	/** Return snap unit */
	double getSnapUnit();

	/** Determine plane intersection */
	void intersect(double x0, double y0, double z0, double i, double j, double k, double &x, double &y, double &z);

	/** Determine plane intersection */
	CIvfVec3d& intersect(CIvfVec3d &origin, CIvfVec3d &vector);

	/** Set coordinate system translation */
	void setTranslation(double tx, double ty, double tz);

	/** Return coordinate system translation */
	void getTranslation(double &tx, double &ty, double &tz);

	/** Set coordinate system rotation */
	void setRotation(double vx, double vy, double vz, double theta);
	
	/** Return coordinate system rotation */
	void getRotation(double & vx, double & vy, double & vz, double & theta);

	/** Return y-axis orientation */
	CIvfVec3d& getYAxis();
protected:
	void calcMatrix();
};

#endif 
