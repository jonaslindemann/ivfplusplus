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

#include <ivfmath/Quat.h>

using namespace ivf;

//
// From Stroustrup 11.6
//

const int ivfMaxTempQuat = 32;

Quat& ivf::ivfGetTempQuat()
{
	static int nBuf = 0;
	static Quat buf[ivfMaxTempQuat];
	
	if (nBuf==ivfMaxTempQuat) nBuf = 0;

	buf[nBuf].setComponents(0.0, 0.0, 0.0, 1.0);

	return buf[nBuf++];
}

Quat::Quat()
{
	m_q.setComponents(0.0, 0.0, 0.0);
	m_qw = 1.0;
}

Quat::Quat(Quat& v)
{
	double qx, qy, qz, qw;

	v.getComponents(qx, qy, qz, qw);
	m_q.setComponents(qx, qy, qz);
	m_qw = qw;
}

Quat::~Quat()
{

}

void Quat::setComponents(double qx, double qy, double qz, double qw)
{
	m_q.setComponents(qx, qy, qz);
	m_qw = qw;
}

void Quat::getComponents(double &qx, double &qy, double &qz, double &qw)
{
	m_q.getComponents(qx, qy, qz);
	qw = m_qw;
}

Quat& Quat::operator=(Quat& a)
{
	double ax, ay, az, aw;
	a.getComponents(ax, ay, az, aw);
	this->setComponents(ax, ay, az, aw);

	return *this;
}

Quat& Quat::operator *(Quat& a)
{
	Quat& r = ivfGetTempQuat();

	Vec3d& av = a.getVectorPart();
	double* u = m_q.getComponents();
	double* v = a.getVectorPart().getComponents();

	r.getVectorPart() = m_q*av + a.getRealPart()*m_q + m_qw*a.getVectorPart();
	r.setRealPart(m_qw*a.getRealPart() - u[0]*v[0] - u[1]*v[1] - u[2]*v[2]);
	
	return r;
}

Quat& Quat::operator *(double a)
{
	Quat& r = ivfGetTempQuat();

	r.getVectorPart() = m_q * a;
	r.setRealPart(m_qw*a);
	return r;
}

Quat& Quat::operator/(double b)
{
	Quat& r = ivfGetTempQuat();

	double k = 1.0/b;
	r.getVectorPart() = m_q * k;
	r.setRealPart(m_qw*k);
	return r;
}

Quat& Quat::operator +(Quat& a)
{
	Quat& r = ivfGetTempQuat();

	Vec3d& av = a.getVectorPart();

	r.getVectorPart() = m_q + av;
	r.setRealPart(m_qw + a.getRealPart());

	return r;
}

Quat& Quat::operator -(Quat& a)
{
	Quat& r = ivfGetTempQuat();

	Vec3d& av = a.getVectorPart();

	r.getVectorPart() = m_q - av;
	r.setRealPart(m_qw - a.getRealPart());

	 return r;
}

Vec3d& Quat::getVectorPart()
{
	return m_q;
}

double Quat::getRealPart()
{
	return m_qw;
}

Quat& Quat::conjugate()
{
	Quat& r = ivfGetTempQuat();

	double qx, qy, qz;

	m_q.getComponents(qx, qy, qz);
	r.getVectorPart().setComponents(-qx, -qy, -qz);

	return r;
}

void Quat::identity()
{
	m_q.setComponents(0.0, 0.0, 0.0);
	m_qw = 1.0;
}

double Quat::norm()
{
	double qx, qy, qz;
	m_q.getComponents(qx, qy, qz);
	return pow(qx,2) + pow(qy,2) + pow(qz,2) + pow(m_qw,2);
}

Quat& Quat::inv()
{
	Quat& r = ivfGetTempQuat();

	r = this->conjugate()/this->norm();

	return r;
}

void Quat::setRealPart(double value)
{
	m_qw = value;
}

void Quat::setFromAxisAngle(double vx, double vy, double vz, double angle)
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


void Quat::getAxisAngle(double &vx, double &vy, double &vz, double &angle)
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
