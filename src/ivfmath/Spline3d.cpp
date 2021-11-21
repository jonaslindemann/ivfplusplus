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

#include <ivfmath/Spline3d.h>

using namespace ivf;

CSpline3d::CSpline3d()
{
 	m_start = new CVec3d();
 	m_end = new CVec3d();
	m_size = -1;
}

CSpline3d::~CSpline3d()
{
	delete m_start;
	delete m_end;
}

void CSpline3d::setSize(int size)
{
	if (size>1)
		m_size = size;
	else
		m_size = -1;

	initSpline();
	updateSpline();
}

int CSpline3d::getSize()
{
	return m_points.size();
}

void CSpline3d::clear()
{
	unsigned int i;

	for (i=0; i<m_segments.size(); i++)
		delete m_segments[i];

	m_segments.clear();
	m_points.clear();
	m_tau.clear();
	m_gamma.clear();
	m_beta.clear();
}

void CSpline3d::initSpline()
{
	if (m_size>1)
	{
		this->clear();
		
		int i;

		// Create list of points for spline

		for (i=0; i<m_size; i++)
		{
			CVec3d* point = new CVec3d();
			m_points.push_back(point);
			m_tau.push_back(0.0);
			m_gamma.push_back(0.0);
			m_beta.push_back(0.0);
		}

		// Create spline segments
		
		for (i=0; i<m_size-1 ; i++)
		{
			CSplineSegment3d* spline = new CSplineSegment3d();
			m_segments.push_back(spline);
			spline->setStartPoint(m_points[i]);
			spline->setEndPoint(m_points[i+1]);
		}
	}
}

/** Retrieve specified point on spline. */
CVec3d* CSpline3d::getPoint(int idx)
{
	if ((idx>=0)&&(idx<(int)m_points.size()))
		return m_points[idx];
	else
		return nullptr;
}
/** No descriptions */
void CSpline3d::updateSpline()
{
	int i;

	if (m_size>1)
	{

		// Handle first point

		CSplineSegment3d* splineLeft = m_segments[0];

		CVec3d* tangentLeft = splineLeft->getStartDirection();

		CVec3d* point = splineLeft->getStartPoint();
		CVec3d* pointRight = splineLeft->getEndPoint();

		*m_start = *point;

		double t, g, b;

		t = m_tau[0];
		g = m_gamma[0];
		b = m_beta[0];

		*tangentLeft =	(1.0-t)*(1.0-g)*(1.0-b)*( (*pointRight) - (*point) );

		// Handle middle points
		
		for (i=1; i<m_size-1; i++)
		{
			CSplineSegment3d* splineLeft = m_segments[i-1];
			CSplineSegment3d* splineRight = m_segments[i];

			CVec3d* tangentLeft = splineLeft->getEndDirection();
			CVec3d* tangentRight = splineRight->getStartDirection();

			CVec3d* pointLeft = splineLeft->getStartPoint();
			CVec3d* point = splineLeft->getEndPoint();
			CVec3d* pointRight = splineRight->getEndPoint();

			double t, g, b;

			t = m_tau[i];
			g = m_gamma[i];
			b = m_beta[i];

			*tangentLeft =	0.5*(1.0-t)*(1.0-g)*(1.0-b)*( (*pointRight) - (*point) ) +
							0.5*(1.0-t)*(1.0+g)*(1.0+b)*( (*point) - (*pointLeft) );

			*tangentRight =	0.5*(1.0-t)*(1.0+g)*(1.0-b)*( (*pointRight) - (*point) ) +
							0.5*(1.0-t)*(1.0-g)*(1.0+b)*( (*point) - (*pointLeft) );
		}

		// Handle last point

		CSplineSegment3d* splineRight = m_segments[m_segments.size()-1];

		CVec3d* tangentRight = splineRight->getEndDirection();

		CVec3d* pointLeft = splineRight->getStartPoint();
		point = splineRight->getEndPoint();

		*m_end = *point;

		t = m_tau[i];
		g = m_gamma[i];
		b = m_beta[i];

		*tangentRight =	(1.0-t)*(1.0-g)*(1.0+b)*( (*point) - (*pointLeft) );

	}
}

/** No descriptions */
CVec3d& CSpline3d::getPosition(double t)
{
	int segment = (int)t;

	if (t>0)
	{
		if (segment<(int)m_segments.size())
			return *m_segments[segment]->getPosition(t-(double)segment);
		else
			return *m_end;
	}
	else
		return *m_start;
}

/** No descriptions */
void CSpline3d::update()
{
	updateSpline();
}

/** No descriptions */
void CSpline3d::setTension(int idx, double value)
{
	if ((idx>=0)&&(idx<(int)m_points.size()))
	{
		m_tau[idx] = value;
		updateSpline();
	}
}

/** No descriptions */
void CSpline3d::setContinuity(int idx, double value)
{
	if ((idx>=0)&&(idx<(int)m_points.size()))
	{
		m_gamma[idx] = value;
		updateSpline();
	}
}

/** No descriptions */
void CSpline3d::setTangentWeight(int idx, double value)
{
	if ((idx>=0)&&(idx<(int)m_points.size()))
	{
		m_beta[idx] = value;
		updateSpline();
	}
}

CVec3d& CSpline3d::getTangent(double t)
{
	int segment = (int)t;

	if (t>0)
	{
		if (segment<(int)m_segments.size())
			return *m_segments[segment]->getTangent(t-(double)segment);
		else
			return *m_segments[m_segments.size()-1]->getTangent(1.0);
	}
	else
		return *m_segments[0]->getTangent(0.0);
}

double CSpline3d::getSpeed(double t)
{
	int segment = (int)t;

	if (t>0)
	{
		if (segment<(int)m_segments.size())
			return m_segments[segment]->getSpeed(t-(double)segment);
		else
			return m_segments[segment]->getSpeed(t-(double)m_segments.size()-1);
	}
	else
		return m_segments[0]->getSpeed(0.0);

}
