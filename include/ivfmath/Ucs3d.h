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
#include <ivfmath/Vec4d.h>
#include <ivfmath/Mat4d.h>
#include <ivfmath/Point3d.h>

namespace ivf {

IvfSmartPointer(Ucs3d);

/**
 * User coordinate class.
 *
 * Ucs3d implements a user defined coordinate system and
 * routines to convert to and from this. Coordinate snap is 
 * also implemented.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Ucs3d : public MathBase {
private:
	double m_snapUnit;

	Vec3d m_zAxis;
	Vec3d m_yAxis;
	Vec3d m_xAxis;

	double m_theta;
	double m_vz;
	double m_vy;
	double m_vx;
	double m_tz;
	double m_tx;
	double m_ty;

	Mat4d m_transformationMatrix;
	Mat4d m_inversionMatrix;
	Vec4d m_pointWorld;
	Vec4d m_pointTransformed;

public:
	/** Ucs3d constructor */
	Ucs3d();

	/** Ucs3d destructor */
	virtual ~Ucs3d();

	IvfClassInfo("Ucs3d",MathBase);
	IvfStdFactory(Ucs3d);

	/** Transform point to world coordinate system */
	void transformWorld(Point3d* point);

	/** Transform point to local coordinate system */
	void transform(Point3d* point);

	/** Transform (sx, sy, sz) to world coordinate system */
	void transformWorld(double sx, double sy, double sz, double &wx, double &wy,  double &wz);

	/** Transform (wx, wy, wz) to local coordinate system */
	void transform(double wx, double wy, double wz, double &sx, double &sy, double &sz);

	/** Transform vector to world coordinates */
	Vec3d& transformWorld(Vec3d& vector);

	/** Transform vector to local coordinate system */
	Vec3d& transform(Vec3d& vector);

	/** Snap (x, y, z) to grid */
	void snap(double &x, double &y, double &z);

	/** Snap vector to grid */
	Vec3d& snap(Vec3d& vector);

	/** Set snap unit */
	void setSnapUnit(double unit);

	/** Return snap unit */
	double getSnapUnit();

	/** Determine plane intersection */
	void intersect(double x0, double y0, double z0, double i, double j, double k, double &x, double &y, double &z);

	/** Determine plane intersection */
	Vec3d& intersect(Vec3d &origin, Vec3d &vector);

	/** Set coordinate system translation */
	void setTranslation(double tx, double ty, double tz);

	/** Return coordinate system translation */
	void getTranslation(double &tx, double &ty, double &tz);

	/** Set coordinate system rotation */
	void setRotation(double vx, double vy, double vz, double theta);
	
	/** Return coordinate system rotation */
	void getRotation(double & vx, double & vy, double & vz, double & theta);

	/** Return y-axis orientation */
	Vec3d& getYAxis();
protected:
	void calcMatrix();
};

}