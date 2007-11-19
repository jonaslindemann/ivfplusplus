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

#include <ivfmath/IvfFunctions.h>

#include <algorithm>
using namespace std;

CIvfLine2d::CIvfLine2d()
{
	m_k = 1.0;
	m_m = 0.0;
}

CIvfLine2d::CIvfLine2d(double x1, double y1, double x2, double y2)
{
	m_k = (y2 - y1)/(x2 - x1);
	m_m = y1 - m_k*x1;
}

CIvfLine2d::CIvfLine2d(double k, double m)
{
	m_k = k;
	m_m = m;
}

double CIvfLine2d::f(double x)
{
	return m_k*x + m_m;
}

double CIvfTheta2d::f(double x)
{
	if (x<0)
		return 0.0;
	else
		return 1.0;
}

CIvfPulse2d::CIvfPulse2d(double delta)
{
	m_delta = 1.0;
}

double CIvfPulse2d::f(double x)
{
	return (m_theta.f(x)-m_theta.f(x-m_delta))/m_delta;
}

CIvfEnvPos2d::CIvfEnvPos2d()
{
	m_pos[0] = 0.0;
	m_pos[1] = 0.0;
}

CIvfEnvPos2d::CIvfEnvPos2d(double x, double y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

double CIvfEnvPos2d::getX()
{
	return m_pos[0];
}

double CIvfEnvPos2d::getY()
{
	return m_pos[1];
}

CIvfEnvelope2d::CIvfEnvelope2d()
{
	m_range[0] = 1e300;
	m_range[1] = -1e300;
	m_finished = false;
}

CIvfEnvelope2d::~CIvfEnvelope2d()
{
	this->clear();
}

void CIvfEnvelope2d::addPoint(double x, double y)
{
	CIvfEnvPos2d* envPos = new CIvfEnvPos2d(x, y);
	m_envelopeSet.insert(envPos);
}


void CIvfEnvelope2d::clear()
{
	CIvfEnvelopePosSetIter ei;

	for (ei=m_envelopeSet.begin(); ei!=m_envelopeSet.end(); ei++)
		delete (*ei);

	CIvfEnvelopePosVectorIter vi;

	for (vi=m_envelope.begin(); vi!=m_envelope.end(); vi++)
		delete (*vi);

	m_envelope.clear();
}

double CIvfEnvelope2d::f(double x)
{
	if (m_finished)
	{
		if (x<=m_range[0])
			return m_envelope[0]->getY();
		
		if (x>=m_range[1])
			return m_envelope[m_envelope.size()-1]->getY();
		
		if ((x>m_range[0])&&(x<m_range[1]))
		{
			if (m_envelope.size()==2)
				return this->interpolate(m_envelope[0], m_envelope[1], x);
			else
			{
				int leftPos = 0;
				int rightPos = m_envelope.size()-1;
				int midPoint = m_envelope.size()/2;		

				while (leftPos!=rightPos-1) {
					
					if (x>m_envelope[midPoint]->getX())
						leftPos = midPoint;
					else
						rightPos = midPoint;
					
					midPoint = (rightPos-leftPos)/2;
				}
				return this->interpolate(m_envelope[leftPos], m_envelope[leftPos+1], x);
			}
		}

		return 0.0;
	}
	else
		return 0.0;
}

void CIvfEnvelope2d::finish()
{
	CIvfEnvelopePosVectorIter vi;
	CIvfEnvelopePosSetIter ei;

	if (m_envelope.size()!=0)
		for (vi=m_envelope.begin(); vi!=m_envelope.end(); vi++)
			delete (*vi);

	m_envelope.clear();

	m_envelope.reserve(m_envelopeSet.size());

	m_range[0] = 1e300;
	m_range[1] = -1e300;

	for (ei=m_envelopeSet.begin(); ei!=m_envelopeSet.end(); ei++)
	{
		CIvfEnvPos2d* pos = (*ei);
		m_envelope.push_back(*ei);

		if (pos->getX()>m_range[1])
			m_range[1] = pos->getX();

		if (pos->getX()<m_range[0])
			m_range[0] = pos->getX();
	}

	m_envelopeSet.clear();

	m_finished = true;
}


double CIvfEnvelope2d::getMaxX()
{
	return m_range[1];
}

double CIvfEnvelope2d::getMinX()
{
	return m_range[0];
}

double CIvfEnvelope2d::interpolate(CIvfEnvPos2d *pos1, CIvfEnvPos2d *pos2, double x)
{
	double x1 = pos1->getX();
	double x2 = pos2->getX();
	double y1 = pos1->getY();
	double y2 = pos2->getY();

	double k = (y2 - y1)/(x2 - x1);
	double m = y1 - k*x1;

	return k*x + m;

}
