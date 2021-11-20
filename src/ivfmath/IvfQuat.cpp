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

#include <ivfmath/IvfQuat.h>

//
// From Stroustrup 11.6
//

const int ivfMaxTempQuat = 32;

CQuat& ivfGetTempQuat()
{
	static int nBuf = 0;
	static CQuat buf[ivfMaxTempQuat];
	
	if (nBuf==ivfMaxTempQuat) nBuf = 0;

	buf[nBuf].setComponents(0.0, 0.0, 0.0, 1.0);

	return buf[nBuf++];
}

CQuat::CQuat()
{
	m_q.setComponents(0.0, 0.0, 0.0);
	m_qw = 1.0;
}

CQuat::CQuat(CQuat& v)
{
	double qx, qy, qz, qw;

	v.getComponents(qx, qy, qz, qw);
	m_q.setComponents(qx, qy, qz);
	m_qw = qw;
}

CQuat::~CQuat()
{

}

void CQuat::setComponents(double qx, double qy, double qz, double qw)
{
	m_q.setComponents(qx, qy, qz);
	m_qw = qw;
}

void CQuat::getComponents(double &qx, double &qy, double &qz, double &qw)
{
	m_q.getComponents(qx, qy, qz);
	qw = m_qw;
}

CQuat& CQuat::operator=(CQuat& a)
{
	double ax, ay, az, aw;
	a.getComponents(ax, ay, az, aw);
	this->setComponents(ax, ay, az, aw);

	return *this;
}

CQuat& CQuat::operator *(CQuat& a)
{
	CQuat& r = ivfGetTempQuat();

	CVec3d& av = a.getVectorPart();
	double* u = m_q.getComponents();
	double* v = a.getVectorPart().getComponents();

	r.getVectorPart() = m_q*av + a.getRealPart()*m_q + m_qw*a.getVectorPart();
	r.setRealPart(m_qw*a.getRealPart() - u[0]*v[0] - u[1]*v[1] - u[2]*v[2]);
	
	return r;
}

CQuat& CQuat::operator *(double a)
{
	CQuat& r = ivfGetTempQuat();

	r.getVectorPart() = m_q * a;
	r.setRealPart(m_qw*a);
	return r;
}

CQuat& CQuat::operator/(double b)
{
	CQuat& r = ivfGetTempQuat();

	double k = 1.0/b;
	r.getVectorPart() = m_q * k;
	r.setRealPart(m_qw*k);
	return r;
}

CQuat& CQuat::operator +(CQuat& a)
{
	CQuat& r = ivfGetTempQuat();

	CVec3d& av = a.getVectorPart();

	r.getVectorPart() = m_q + av;
	r.setRealPart(m_qw + a.getRealPart());

	return r;
}

CQuat& CQuat::operator -(CQuat& a)
{
	CQuat& r = ivfGetTempQuat();

	CVec3d& av = a.getVectorPart();

	r.getVectorPart() = m_q - av;
	r.setRealPart(m_qw - a.getRealPart());

	 return r;
}

CVec3d& CQuat::getVectorPart()
{
	return m_q;
}

double CQuat::getRealPart()
{
	return m_qw;
}

CQuat& CQuat::conjugate()
{
	CQuat& r = ivfGetTempQuat();

	double qx, qy, qz;

	m_q.getComponents(qx, qy, qz);
	r.getVectorPart().setComponents(-qx, -qy, -qz);

	return r;
}

void CQuat::identity()
{
	m_q.setComponents(0.0, 0.0, 0.0);
	m_qw = 1.0;
}

double CQuat::norm()
{
	double qx, qy, qz;
	m_q.getComponents(qx, qy, qz);
	return pow(qx,2) + pow(qy,2) + pow(qz,2) + pow(m_qw,2);
}

CQuat& CQuat::inv()
{
	CQuat& r = ivfGetTempQuat();

	r = this->conjugate()/this->norm();

	return r;
}

void CQuat::setRealPart(double value)
{
	m_qw = value;
}

void CQuat::setFromAxisAngle(double vx, double vy, double vz, double angle)
{
	double x, y, z;
	/*
    s := sin(aa.Angle/2);
    w := cos(aa.Angle/2);
    x := aa.Axis.x * s;
    y := aa.Axis.y * s;
    z := aa.Axis.z * s;
    norm := sqrt(x*x + y*y + z*z + w*w);
    w := w / norm;
    x := x / norm;
    y := y / norm;
    z := z / norm;
	*/

	double s = sin(angle/2.0);
	m_qw = cos(angle/2.0);

	x = vx*s;
	y = vy*s;
	z = vz*s;

	double norm = sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(m_qw,2));

	m_q.setComponents(x/norm, y/norm, z/norm);
	m_qw = m_qw/norm;
}


void CQuat::getAxisAngle(double &vx, double &vy, double &vz, double &angle)
{
	double x, y, z;
	double s;

	m_q.getComponents(x, y, z);

	s = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

    vx = x / s;
    vy = y / s;
    vz = z / s;

	angle = 2.0*acos(m_qw);
}
