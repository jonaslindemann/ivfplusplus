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

#include <ivfmath/IvfUcs3d.h>

#include <ivf/ivfconfig.h>

// ------------------------------------------------------------
CUcs3d::CUcs3d()
{
	m_vx = 0.0;
	m_vy = 1.0;
	m_vz = 0.0;
	m_theta = 0.0;
	m_tx = 0.0;
	m_ty = 0.0;
	m_tz = 0.0;
	setSnapUnit(1.0);
	calcMatrix();
}

// ------------------------------------------------------------
CUcs3d::~CUcs3d()
{

}

// ------------------------------------------------------------
void CUcs3d::setRotation(double vx, double vy, double vz, double theta)
{
	m_vx = vx;
	m_vy = vy;
	m_vz = vz;
	m_theta = theta;
	calcMatrix();
}

// ------------------------------------------------------------
void CUcs3d::setTranslation(double tx, double ty, double tz)
{
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	calcMatrix();
}

// ------------------------------------------------------------
void CUcs3d::getRotation(double & vx, double & vy, double & vz, double & theta)
{
	vx = m_vx;
	vy = m_vy;
	vz = m_vz;
	theta = m_theta;
}

// ------------------------------------------------------------
void CUcs3d::getTranslation(double & tx, double & ty, double & tz)
{
	tx = m_tx;
	ty = m_ty;
	tz = m_tz;
}

// ------------------------------------------------------------
void CUcs3d::transform(double wx, double wy, double wz, double & sx, double & sy, double & sz)
{
	m_pointWorld.setComponents(wx-m_tx, wy-m_ty, wz-m_tz, 1.0);
	m_pointTransformed = m_transformationMatrix * m_pointWorld;
	m_pointTransformed.getComponents(sx, sy, sz);
}

// ------------------------------------------------------------
void CUcs3d::calcMatrix()
{
	double x, y, z;

	m_transformationMatrix.rotate(m_vx, m_vy, m_vz, m_theta*2*M_PI/360.0); //m_theta*2*M_PI/360.0
	m_transformationMatrix = m_transformationMatrix.t();
	m_inversionMatrix = m_transformationMatrix.inv();

	// Calculate unit vectors

	transformWorld(1.0, 0.0, 0.0, x, y, z);
	m_xAxis.setComponents(x-m_tx, y-m_ty, z-m_tz);
	transformWorld(0.0, 1.0, 0.0, x, y, z);
	m_yAxis.setComponents(x-m_tx, y-m_ty, z-m_tz);
	transformWorld(0.0, 0.0, 1.0, x, y, z);
	m_zAxis.setComponents(x-m_tx, y-m_ty, z-m_tz);
}

// ------------------------------------------------------------
void CUcs3d::transformWorld(double sx, double sy, double sz, double & wx, double & wy, double & wz)
{
	m_pointTransformed.setComponents(sx, sy, sz, 1.0);

	m_pointWorld = m_inversionMatrix * m_pointTransformed;

	m_pointWorld.getComponents(wx, wy, wz);
	wx = wx + m_tx;
	wy = wy + m_ty;
	wz = wz + m_tz;
}

// ------------------------------------------------------------
void CUcs3d::intersect(double x0, double y0, double z0, double i, double j, double k, double &x, double &y, double &z)
{
	double a, b, c, d;
	double t;

	m_yAxis.getComponents(a, b, c);

	d = -a*m_tx - b*m_ty - c*m_tz;

	t = - (a*x0 + b*y0 + c*z0 + d)/(a*i + b*j + c*k);

    x = x0 + i*t;
    y = y0 + j*t;
    z = z0 + k*t;
}

// ------------------------------------------------------------
CVec3d& CUcs3d::intersect(CVec3d &origin, CVec3d &vec)
{
	double x, y, z;
	double vx, vy, vz;
	double ix, iy, iz;
	CVec3d& intersection = ivfGetTempVec3d();

	origin.getComponents(x, y, z);
	vec.getComponents(vx, vy, vz);
	intersect(x, y, z, vx, vy, vz, ix, iy, iz);
	intersection.setComponents(ix, iy, iz);

	return intersection;
}

// ------------------------------------------------------------
void CUcs3d::setSnapUnit(double unit)
{
	m_snapUnit = unit;
}

// ------------------------------------------------------------
double CUcs3d::getSnapUnit()
{
	return m_snapUnit;
}

// ------------------------------------------------------------
void CUcs3d::snap(double & x, double & y, double & z)
{
	if (x>0)
		x = (double)((long)((x + (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);
	else
		x = (double)((long)((x - (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);
	
	if (y>0) 
		y = (double)((long)((y + (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);
	else
		y = (double)((long)((y - (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);

	if (z>0)
		z = (double)((long)((z + (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);
	else
		z = (double)((long)((z - (m_snapUnit/2)) / m_snapUnit) * m_snapUnit);
}

// ------------------------------------------------------------
void CUcs3d::transform(CPoint3d * point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	transform(x, y, z, x, y, z);
	point->setComponents(x, y, z);
}

// ------------------------------------------------------------
void CUcs3d::transformWorld(CPoint3d * point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	transformWorld(x, y, z, x, y, z);
	point->setComponents(x, y, z);
}

// ------------------------------------------------------------
CVec3d& CUcs3d::getYAxis()
{
	return m_yAxis;
}

// ------------------------------------------------------------
CVec3d& CUcs3d::transformWorld(CVec3d& vec)
{
	CVec3d& r = ivfGetTempVec3d();
	double x, y, z;
	vec.getComponents(x, y, z);
	transformWorld(x, y, z, x, y, z);
	r.setComponents(x, y, z);
	return r;
}

// ------------------------------------------------------------
CVec3d& CUcs3d::transform(CVec3d& vec)
{
	CVec3d& r = ivfGetTempVec3d();
	double x, y, z;
	vec.getComponents(x, y, z);
	transform(x, y, z, x, y, z);
	r.setComponents(x, y, z);
	return r;
}

// ------------------------------------------------------------
CVec3d& CUcs3d::snap(CVec3d& vec)
{
	CVec3d& r = ivfGetTempVec3d();
	double x, y, z;
	vec.getComponents(x, y, z);
	snap(x, y, z);
	r.setComponents(x, y, z);
	return r;
}
