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

#include <ivfmath/IvfVec3d.h>

//
// From Stroustrup 11.6
//

const int ivfMaxTempVec3d = 32;

CIvfVec3d::CIvfVec3d()
{
	setComponents(0.0, 0.0, 0.0);
}

CIvfVec3d& ivfGetTempVec3d()
{
	static int nBuf = 0;
	static CIvfVec3d buf[ivfMaxTempVec3d];
	
	if (nBuf==ivfMaxTempVec3d) nBuf = 0;

	buf[nBuf].setComponents(0.0, 0.0, 0.0);

	return buf[nBuf++];
}

CIvfVec3d::CIvfVec3d(double vx, double vy, double vz)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
}

CIvfVec3d::CIvfVec3d(CIvfVec3d& v)
{
	v.getComponents(m_vector);
}

CIvfVec3d::~CIvfVec3d()
{

}

void CIvfVec3d::setComponents(double vx, double vy, double vz)
{
	m_vector[0] = vx;
	m_vector[1] = vy;
	m_vector[2] = vz;
}

void CIvfVec3d::setComponents(const double *v)
{
	m_vector[0] = v[0];
	m_vector[1] = v[1];
	m_vector[2] = v[2];
}

void CIvfVec3d::getComponents(double &vx, double &vy, double &vz)
{
	vx = m_vector[0];
	vy = m_vector[1];
	vz = m_vector[2];
}

CIvfVec3d& CIvfVec3d::operator+=(CIvfVec3d& a)
{
	m_vector[0] += a.m_vector[0];
	m_vector[1] += a.m_vector[1];
	m_vector[2] += a.m_vector[2];
	return *this;
}

bool CIvfVec3d::operator==(CIvfVec3d& a)
{
	return ((m_vector[0] == a.m_vector[0])&&
		(m_vector[1] == a.m_vector[1])&&
		(m_vector[2] == a.m_vector[2]));
}

/*
CIvfVec3d& CIvfVec3d::operator=(CIvfVec3d& a)
{
	m_vector[0] = a.m_vector[0];
	m_vector[1] = a.m_vector[1];
	m_vector[2] = a.m_vector[2];
	return *this;
}
*/

CIvfVec3d& CIvfVec3d::operator=(CIvfVec3d& a)
{
	m_vector[0] = a.m_vector[0];
	m_vector[1] = a.m_vector[1];
	m_vector[2] = a.m_vector[2];
	return *this;
}

CIvfVec3d& CIvfVec3d::operator-=(CIvfVec3d& a)
{
	m_vector[0] -= a.m_vector[0];
	m_vector[1] -= a.m_vector[1];
	m_vector[2] -= a.m_vector[2];
	return *this;
}

CIvfVec3d& operator+(CIvfVec3d& a, CIvfVec3d& b)
{
	CIvfVec3d& r = ivfGetTempVec3d();
	r = a;
	return r += b;
}

CIvfVec3d& operator-(CIvfVec3d& a, CIvfVec3d& b)
{
	CIvfVec3d& r = ivfGetTempVec3d();
	r = a;
	return r -= b;
}

double CIvfVec3d::operator[](const int idx)
{
	if ((idx>=0)&&(idx<3))
		return m_vector[idx];
	else
		return 0.0;
}

CIvfVec3d& operator*(CIvfVec3d& a, CIvfVec3d& b)
{
	CIvfVec3d& r = ivfGetTempVec3d();

	double c1, c2, c3;

	c1 = a[1] * b[2] - a[2] * b[1];
	c2 = a[2] * b[0] - a[0] * b[2];
	c3 = a[0] * b[1] - a[1] * b[0];
	
	r.setComponents(c1, c2, c3);

	return r;
}

CIvfVec3d& operator*(CIvfVec3d& a, double b)
{
	CIvfVec3d& r = ivfGetTempVec3d();
	r.setComponents(a[0]*b, a[1]*b, a[2]*b);
	return r;
}

CIvfVec3d& operator*(double a, CIvfVec3d& b)
{
	CIvfVec3d& r = ivfGetTempVec3d();
	r.setComponents(b[0]*a, b[1]*a, b[2]*a);
	return r;
}

void CIvfVec3d::getComponents(double *v)
{
	v[0] = m_vector[0];
	v[1] = m_vector[1];
	v[2] = m_vector[2];
}

double CIvfVec3d::length()
{
	return sqrt(pow(m_vector[0],2) + pow(m_vector[1],2) + pow(m_vector[2],2));
}

void CIvfVec3d::normalize()
{
	double quote = 1.0/length();

	m_vector[0] = m_vector[0] * quote;
	m_vector[1] = m_vector[1] * quote;
	m_vector[2] = m_vector[2] * quote;
}


void CIvfVec3d::rotate(CIvfVec3d &axis, double angle)
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

void CIvfVec3d::getEulerAngles(double &pitch, double &heading)
{
	heading = atan2(m_vector[0], m_vector[2])*180.0/M_PI;
	double t = sqrt(pow(m_vector[0],2)+pow(m_vector[2],2));
	pitch = atan2(m_vector[1], t)*180.0/M_PI;
}

void CIvfVec3d::negate()
{
	m_vector[0] = - m_vector[0];
	m_vector[1] = - m_vector[1];
	m_vector[2] = - m_vector[2];
}


void CIvfVec3d::setX(double value)
{
	m_vector[0] = value;
}

void CIvfVec3d::setY(double value)
{
	m_vector[1] = value;
}

void CIvfVec3d::setZ(double value)
{
	m_vector[2] = value;
}

#ifdef IVF_HAVE_IOSTREAM
void CIvfVec3d::print(std::ostream &out)
{
	using namespace std;

	out << m_vector[0] << " " << m_vector[1] << " " << m_vector[2] << endl;
}
#else
void CIvfVec3d::print(ostream &out)
{
	out << m_vector[0] << " " << m_vector[1] << " " << m_vector[2] << endl;
}
#endif

void CIvfVec3d::setFromPoints(CIvfVec3d &pos, CIvfVec3d &target)
{
	CIvfVec3d r;
	r = target - pos;
	r.getComponents(m_vector);
}

void CIvfVec3d::add(double dx, double dy, double dz)
{
	m_vector[0] += dx;
	m_vector[1] += dy;
	m_vector[2] += dz;
}

double* CIvfVec3d::getComponents()
{
	return &m_vector[0];
}
