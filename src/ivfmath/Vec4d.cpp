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

#include <ivfmath/Vec4d.h>

using namespace ivf;

//
// From Stroustrup 11.6
//

const int ivfMaxTempVec4d = 32;

Vec4d& ivf::ivfGetTempVec4d()
{
	static int nBuf = 0;
	static Vec4d buf[ivfMaxTempVec4d];
	
	if (nBuf==ivfMaxTempVec4d) nBuf = 0;
	buf[nBuf].setComponents(0.0, 0.0, 0.0, 0.0);

	return buf[nBuf++];
}

Vec4d::Vec4d()
{
	setComponents(0.0, 0.0, 0.0, 0.0);
}

Vec4d::Vec4d(double vx, double vy, double vz)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
	m_vector[3] = 0.0;
}

Vec4d::Vec4d(double vx, double vy, double vz, double vw)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
	m_vector[3] = vw;
}

Vec4d::Vec4d(Vec4d& v)
{
	v.getComponents(m_vector);
}

Vec4d::~Vec4d()
{

}

void Vec4d::setComponents(double vx, double vy, double vz)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
	m_vector[3] = 0.0;
}

void Vec4d::setComponents(double vx, double vy, double vz, double vw)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
	m_vector[3] = vw;
}

void Vec4d::getComponents(double &vx, double &vy, double &vz)
{
	vx = m_vector[0];
	vy = m_vector[1];
	vz = m_vector[2];
}

void Vec4d::getComponents(double &vx, double &vy, double &vz, double &vw)
{
	vx = m_vector[0];
	vy = m_vector[1];
	vz = m_vector[2];
	vw = m_vector[3];
}

void Vec4d::setComponents(const double *v)
{
	m_vector[0] = v[0];
	m_vector[1] = v[1];
	m_vector[2] = v[2];
	m_vector[3] = v[3];
}

Vec4d& Vec4d::operator+=(Vec4d& a)
{
	m_vector[0] += a.m_vector[0];
	m_vector[1] += a.m_vector[1];
	m_vector[2] += a.m_vector[2];
	m_vector[3] = a.m_vector[3];
	return *this;
}

Vec4d& Vec4d::operator=(Vec4d& a)
{
	m_vector[0] = a.m_vector[0];
	m_vector[1] = a.m_vector[1];
	m_vector[2] = a.m_vector[2];
	m_vector[3] = a.m_vector[3];
	return *this;
}

/*
Vec4d Vec4d::operator=(Vec4d& a)
{
	Vec4d r;
	r.m_vector[0] = a.m_vector[0];
	r.m_vector[1] = a.m_vector[1];
	r.m_vector[2] = a.m_vector[2];
	r.m_vector[3] = a.m_vector[3];
	return r;
}

Vec4d Vec4d::operator=(Vec4d a)
{
	Vec4d r;
	r.m_vector[0] = a.m_vector[0];
	r.m_vector[1] = a.m_vector[1];
	r.m_vector[2] = a.m_vector[2];
	r.m_vector[3] = a.m_vector[3];
	return r;
}
*/

Vec4d& Vec4d::operator-=(Vec4d& a)
{
	m_vector[0] -= a.m_vector[0];
	m_vector[1] -= a.m_vector[1];
	m_vector[2] -= a.m_vector[2];
	m_vector[3] = a.m_vector[3];
	return *this;
}

Vec4d& operator+(Vec4d& a, Vec4d& b)
{
	Vec4d& r = ivfGetTempVec4d();
	r = a;	
	return r += b;
}

Vec4d& operator-(Vec4d& a, Vec4d& b)
{
	Vec4d& r = ivfGetTempVec4d();
	r = a;
	return r -= b;
}

double Vec4d::operator[](const int idx)
{
	if ((idx>=0)&&(idx<4))
		return m_vector[idx];
	else
		return 0.0;
}

Vec4d& operator*(Vec4d& a, Vec4d& b)
{
	Vec4d& r = ivfGetTempVec4d();

	double c1, c2, c3;

	c1 = a[1] * b[2] - a[2] * b[1];
	c2 = a[2] * b[0] - a[0] * b[2];
	c3 = a[0] * b[1] - a[1] * b[0];
	
	r.setComponents(c1, c2, c3);

	return r;
}

Vec4d& operator*(Vec4d& a, double b)
{
	Vec4d& r = ivfGetTempVec4d();
	r.setComponents(a[0]*b, a[1]*b, a[2]*b);
	return r;
}

Vec4d& operator*(double a, Vec4d& b)
{
	Vec4d& r = ivfGetTempVec4d();
	r.setComponents(b[0]*a, b[1]*a, b[2]*a);
	return r;
}

void Vec4d::getComponents(double *v)
{
	v[0] = m_vector[0];
	v[1] = m_vector[1];
	v[2] = m_vector[2];
	v[3] = m_vector[3];
}

double Vec4d::length()
{
	return sqrt(pow(m_vector[0],2) + pow(m_vector[1],2) + pow(m_vector[2],2));
}

void Vec4d::normalize()
{
	double quote = 1.0/length();

	m_vector[0] = m_vector[0] * quote;
	m_vector[1] = m_vector[1] * quote;
	m_vector[2] = m_vector[2] * quote;
}


void Vec4d::rotate(Vec4d &axis, double angle)
{
	double cost = cos(angle*2*M_PI/360.0);
	double sint = sin(angle*2*M_PI/360.0);

	double rv[3];

	rv[0] = 
		(cost + (1.0-cost)*pow(axis[0],2))        * m_vector[0] + 
		((1.0-cost)*axis[0]*axis[1]-axis[2]*sint) * m_vector[1] + 
		((1.0-cost)*axis[0]*axis[2]+axis[1]*sint) * m_vector[2];

	rv[1] = 
		((1.0-cost)*axis[0]*axis[1]+axis[2]*sint) * m_vector[0] +
		(cost + (1.0-cost)*pow(axis[1],2))        * m_vector[1] + 
		((1.0-cost)*axis[1]*axis[2]-axis[0]*sint) * m_vector[2];

	rv[2] = 
		((1.0-cost)*axis[0]*axis[2]-axis[1]*sint) * m_vector[0] + 
		((1.0-cost)*axis[1]*axis[2]+axis[0]*sint) * m_vector[1] +
		(cost+(1.0-cost)*pow(axis[2],2))          * m_vector[2];

	m_vector[0] = rv[0];
	m_vector[1] = rv[1];
	m_vector[2] = rv[2];
}

void Vec4d::getEulerAngles(double &pitch, double &heading)
{
	heading = atan2(m_vector[0], m_vector[2])*180.0/M_PI;
	double t = sqrt(pow(m_vector[0],2)+pow(m_vector[2],2));
	pitch = atan2(m_vector[1], t)*180.0/M_PI;
}

void Vec4d::negate()
{
	m_vector[0] = - m_vector[0];
	m_vector[1] = - m_vector[1];
	m_vector[2] = - m_vector[2];
}


void Vec4d::setX(double value)
{
	m_vector[0] = value;
}

void Vec4d::setY(double value)
{
	m_vector[1] = value;
}

void Vec4d::setZ(double value)
{
	m_vector[2] = value;
}

#ifdef IVF_HAVE_IOSTREAM
void Vec4d::print(std::ostream &out)
{
	using namespace std;

	out << m_vector[0] << " " << m_vector[1] << " " << m_vector[2] << " " << m_vector[3] << endl;
}
#else
void CIvfVec4d::print(ostream &out)
{
	out << m_vector[0] << " " << m_vector[1] << " " << m_vector[2] << " " << m_vector[3] << endl;
}
#endif

