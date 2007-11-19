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

#include <ivf/ivfconfig.h>

// Implementation of: public class CIvfCoordinateSystem

#include <ivf/IvfCoordinateSystem.h>

// ------------------------------------------------------------
CIvfCoordinateSystem::CIvfCoordinateSystem ()
  : CIvfComposite()
{
	//setClassName("CIvfCoordinateSystem");
	m_vx = 0.0;
	m_vy = 1.0;
	m_vz = 0.0;
	m_theta = 0.0;
	m_tx = 0.0;
	m_ty = 0.0;
	m_tz = 0.0;
	m_tempYTranslation = 0.0;
	//  	m_transformationMatrix.SetSize(4,4);
	//  	m_pointWorld.SetSize(4,1);
	//  	m_pointTransformed.SetSize(4,1);

	/*
	m_grid = new CIvfOldGrid();
	m_axis = new CIvfAxis();
	*/

	m_grid = new CIvfGrid();

	setSnapUnit(1.0);
	addChild(m_grid);
	//addChild(m_axis);
	calcMatrix();
}

// ------------------------------------------------------------
CIvfCoordinateSystem::~CIvfCoordinateSystem ()
{
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setRotation(double vx, double vy, double vz, double theta)
{
	m_vx = vx;
	m_vy = vy;
	m_vz = vz;
	m_theta = theta;
	m_grid->setRotationQuat(m_vx, m_vy, m_vz, m_theta);
	//m_axis->setRotationQuat(m_vx, m_vy, m_vz, m_theta);
	calcMatrix();
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setTranslation(double tx, double ty, double tz)
{
	m_tx = tx;
	m_ty = ty;
	m_tz = tz;
	m_grid->setPosition(tx,ty,tz);
	//m_axis->setPosition(tx,ty,tz);
	calcMatrix();
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::getRotation(double & vx, double & vy, double & vz, double & theta)
{
	vx = m_vx;
	vy = m_vy;
	vz = m_vz;
	theta = m_theta;
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::getTranslation(double & tx, double & ty, double & tz)
{
	tx = m_tx;
	ty = m_ty;
	tz = m_tz;
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::transform(double wx, double wy, double wz, double & sx, double & sy, double & sz)
{
	m_pointWorld.setComponents(wx-m_tx, wy-m_ty, wz-m_tz, 1.0);
	m_pointTransformed = m_transformationMatrix * m_pointWorld;
	m_pointTransformed.getComponents(sx, sy, sz);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::calcMatrix()
{
	double x, y, z;

	m_transformationMatrix.rotate(m_vx, m_vy, m_vz, 0.0); //m_theta*2*M_PI/360.0
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
void CIvfCoordinateSystem::transformWorld(double sx, double sy, double sz, double & wx, double & wy, double & wz)
{
	m_pointTransformed.setComponents(sx, sy, sz, 1.0);
	m_pointWorld = m_inversionMatrix * m_pointTransformed;
	m_pointWorld.getComponents(wx, wy, wz);
	wx = wx + m_tx;
	wy = wy + m_ty;
	wz = wz + m_tz;
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::intersect(double x0, double y0, double z0, double i, double j, double k, double &x, double &y, double &z)
{
	double a, b, c, d;
	double t;

	m_yAxis.getComponents(a, b, c);
	//a = 0.0;
	//b = 1.0;
	//c = 0.0;
	d = -m_ty - m_tempYTranslation;

	t = - (a*x0 + b*y0 + c*z0 + d)/(a*i + b*j + c*k);

    x = x0 + i*t;
    y = y0 + j*t;
    z = z0 + k*t;
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::alignShape(CIvfShape * shape)
{
	shape->setRotationQuat(m_vx, m_vy, m_vz, m_theta);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setSnapUnit(double unit)
{
	m_snapUnit = unit;

	m_grid->setGridSpacing(unit);

	//m_grid->setUnits(m_snapUnit, m_snapUnit);
	//m_grid->setMark(10, 10);
	//m_grid->setSize(200,200);
}

// ------------------------------------------------------------
double CIvfCoordinateSystem::getSnapUnit()
{
	return m_snapUnit;
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::snap(double & x, double & y, double & z)
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
void CIvfCoordinateSystem::transform(CIvfPoint3d * point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	transform(x, y, z, x, y, z);
	point->setComponents(x, y, z);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::transformWorld(CIvfPoint3d * point)
{
	double x, y, z;
	point->getComponents(x, y, z);
	transformWorld(x, y, z, x, y, z);
	point->setComponents(x, y, z);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setDrawSubDivision(bool flag)
{

}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setAxisType(int type)
{
	//m_axis->setAxisType(type);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setWorkspace(double size)
{
	m_snapUnit = size/200.0;

	m_grid->setSize(size, size);
	m_grid->setGridSpacing(m_snapUnit);
	m_grid->setAxisSize(m_snapUnit*10.0);
	m_grid->refresh();

	//m_grid->setUnits(m_snapUnit, m_snapUnit);
	//m_grid->setMark(10, 10);
	//m_grid->setSize(200,200);
	//m_axis->setSize(m_snapUnit*10.0);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setGridState(TObjectState state)
{
	m_grid->setState(state);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setAxisState(TObjectState state)
{
	//m_axis->setState(state);
}

// ------------------------------------------------------------
void CIvfCoordinateSystem::setTempYTranslation(double value)
{
	m_tempYTranslation = value;	
}

// ------------------------------------------------------------
CIvfAxis* CIvfCoordinateSystem::getAxisShape()
{
	return m_grid->getAxisShape();
}

CIvfGrid* CIvfCoordinateSystem::getGrid()
{
	return m_grid;
}
