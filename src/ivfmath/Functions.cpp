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

#include <ivfmath/Functions.h>


#include <algorithm>

using namespace std;
using namespace ivf;

Line2d::Line2d()
{
	m_k = 1.0;
	m_m = 0.0;
}

Line2d::Line2d(double x1, double y1, double x2, double y2)
{
	m_k = (y2 - y1)/(x2 - x1);
	m_m = y1 - m_k*x1;
}

Line2d::Line2d(double k, double m)
{
	m_k = k;
	m_m = m;
}

double Line2d::f(double x)
{
	return m_k*x + m_m;
}

double Theta2d::f(double x)
{
	if (x<0)
		return 0.0;
	else
		return 1.0;
}

Pulse2d::Pulse2d(double delta)
{
	m_delta = 1.0;
}

double Pulse2d::f(double x)
{
	return (m_theta.f(x)-m_theta.f(x-m_delta))/m_delta;
}

EnvPos2d::EnvPos2d()
{
	m_pos[0] = 0.0;
	m_pos[1] = 0.0;
}

EnvPos2d::EnvPos2d(double x, double y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

double EnvPos2d::getX()
{
	return m_pos[0];
}

double EnvPos2d::getY()
{
	return m_pos[1];
}

Envelope2d::Envelope2d()
{
	m_range[0] = 1e300;
	m_range[1] = -1e300;
	m_finished = false;
}

Envelope2d::~Envelope2d()
{
	this->clear();
}

void Envelope2d::addPoint(double x, double y)
{
	EnvPos2d* envPos = new EnvPos2d(x, y);
	m_envelopeSet.insert(envPos);
}


void Envelope2d::clear()
{
	CIvfEnvelopePosSetIter ei;

	for (ei=m_envelopeSet.begin(); ei!=m_envelopeSet.end(); ei++)
		delete (*ei);

	CIvfEnvelopePosVectorIter vi;

	for (vi=m_envelope.begin(); vi!=m_envelope.end(); vi++)
		delete (*vi);

	m_envelope.clear();
}

double Envelope2d::f(double x)
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
				int rightPos = static_cast<int>(m_envelope.size())-1;
				int midPoint = static_cast<int>(m_envelope.size())/2;

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

void Envelope2d::finish()
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
		EnvPos2d* pos = (*ei);
		m_envelope.push_back(*ei);

		if (pos->getX()>m_range[1])
			m_range[1] = pos->getX();

		if (pos->getX()<m_range[0])
			m_range[0] = pos->getX();
	}

	m_envelopeSet.clear();

	m_finished = true;
}


double Envelope2d::getMaxX()
{
	return m_range[1];
}

double Envelope2d::getMinX()
{
	return m_range[0];
}

double Envelope2d::interpolate(EnvPos2d *pos1, EnvPos2d *pos2, double x)
{
	double x1 = pos1->getX();
	double x2 = pos2->getX();
	double y1 = pos1->getY();
	double y2 = pos2->getY();

	double k = (y2 - y1)/(x2 - x1);
	double m = y1 - k*x1;

	return k*x + m;

}
