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

#include <ivfmath/SplineSegment3d.h>

using namespace ivf;

CSplineSegment3d::CSplineSegment3d()
{
	m_p1 = new CVec3d();
	m_p1->addReference();
	m_p2 = new CVec3d();
	m_p2->addReference();

	m_t1 = new CVec3d();
	m_t2 = new CVec3d();
	m_p = new CVec3d();
	m_t = new CVec3d();
	m_bias[0] = 1.0;
	m_bias[1] = 1.0;
}

CSplineSegment3d::~CSplineSegment3d()
{
	m_p1->deleteReference();	
	m_p2->deleteReference();	

	if (!m_p1->referenced())
		delete m_p1;
		
	if (!m_p2->referenced())
		delete m_p2;

	delete m_t1;
	delete m_t2;
	delete m_p;
	delete m_t;
}

/**  */
CVec3d* CSplineSegment3d::getStartPoint()
{
	return m_p1;
}

/**  */
CVec3d* CSplineSegment3d::getEndPoint()
{
	return m_p2;
}

/**  */
CVec3d* CSplineSegment3d::getPosition(double t)
{
	if ((m_p1!=NULL)&&(m_p2!=NULL))
	{
	*m_p =
		h0(t)*(*m_p1) +
		h1(t)*(*m_p2) +
		m_bias[0]*h2(t)*(*m_t1) +
		m_bias[1]*h3(t)*(*m_t2);
	}
	return m_p;
}

/**  */
CVec3d* CSplineSegment3d::getStartDirection()
{
	return m_t1;
}

/**  */
CVec3d* CSplineSegment3d::getEndDirection()
{
	return m_t2;
}

/**  */
double CSplineSegment3d::h0(double t)
{
	return 2*pow(t,3)-3*pow(t,2)+1.0;
}

/**  */
double CSplineSegment3d::h1(double t)
{
	return -2*pow(t,3)+3*pow(t,2);
}

/**  */
double CSplineSegment3d::h2(double t)
{
	return pow(t,3)-2*pow(t,2)+t;
}

/**  */
double CSplineSegment3d::h3(double t)
{
	return pow(t,3)-pow(t,2);
}

/**  */
void CSplineSegment3d::setStartBias(double bias)
{
	m_bias[0] = bias;
}
/**  */
void CSplineSegment3d::setEndBias(double bias)
{
	m_bias[1] = bias;
}
/**  */
double CSplineSegment3d::getStartBias()
{
	return m_bias[0];
}
/**  */
double CSplineSegment3d::getEndBias()
{
	return m_bias[1];
}

/** Set start point of spline */
void CSplineSegment3d::setStartPoint(CVec3d* point)
{
	m_p1->deleteReference();	
	if (!m_p1->referenced())
		delete m_p1;
		
	m_p1 = point;
	m_p1->addReference();
}

/** Set endpoint of spline */
void CSplineSegment3d::setEndPoint(CVec3d* point)
{
	m_p2->deleteReference();	
	if (!m_p2->referenced())
		delete m_p2;
		
	m_p2 = point;
	m_p2->addReference();
}

double CSplineSegment3d::dh0(double t)
{
	return 6.0*pow(t,2)-6.0*t;
}

double CSplineSegment3d::dh1(double t)
{
	return -6.0*pow(t,2)+6.0*t;
}

double CSplineSegment3d::dh2(double t)
{
	return 3.0*pow(t,2)-4.0*t+1.0;
}

double CSplineSegment3d::dh3(double t)
{
	return 3.0*pow(t,2)-2.0*t;
}

CVec3d* CSplineSegment3d::getTangent(double t)
{
	if ((m_p1!=NULL)&&(m_p2!=NULL))
	{
	*m_t =
		dh0(t)*(*m_p1) +
		dh1(t)*(*m_p2) +
		m_bias[0]*dh2(t)*(*m_t1) +
		m_bias[1]*dh3(t)*(*m_t2);
	}
	return m_t;
}

double CSplineSegment3d::getSpeed(double t)
{
	CVec3d* vec;
	vec = this->getTangent(t);
	return vec->length();
}
