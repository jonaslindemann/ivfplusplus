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

#include <ivfmath/IvfSlerp.h>

CSlerp::CSlerp()
{
	m_size = -1;
}

CSlerp::~CSlerp()
{
}

void CSlerp::setSize(int size)
{
	if (size>1)
		m_size = size;
	else
		m_size = -1;

	initSlerp();
}

int CSlerp::getSize()
{
	return m_quats.size();
}

void CSlerp::clear()
{
	int i;

	for (i=0; i<(int)m_segments.size(); i++)
		delete m_segments[i];

	m_segments.clear();
	m_quats.clear();
}

void CSlerp::initSlerp()
{
	if (m_size>1)
	{
		this->clear();
		
		int i;

		// Create list of points for spline

		for (i=0; i<m_size; i++)
		{
			CQuat* q = new CQuat();
			m_quats.push_back(q);
		}

		// Create spline segments
		
		for (i=0; i<m_size-1 ; i++)
		{
			CSlerpSegment* slerpSegment = new CSlerpSegment();
			m_segments.push_back(slerpSegment);
			slerpSegment->setStartQuat(m_quats[i]);
			slerpSegment->setEndQuat(m_quats[i+1]);
		}
	}
}

/** Retrieve specified point on spline. */
CQuat& CSlerp::getQuat(int idx)
{
	if ((idx>=0)&&(idx<(int)m_quats.size()))
		return *m_quats[idx];
	else
		return ivfGetTempQuat();
}

/** No descriptions */
CQuat& CSlerp::getQuat(double t)
{
	int segment = (int)t;

	if (t>0)
	{
		if (segment<(int)m_segments.size())
			return m_segments[segment]->getQuat(t-(double)segment);
		else
			return m_segments[m_segments.size()-1]->getQuat(1.0);
	}
	else
		return m_segments[0]->getQuat(0.0);
}


void CSlerp::setQuat(int idx, CQuat &q)
{
	if ((idx>=0)&&(idx<(int)m_quats.size()))
		*m_quats[idx] = q;
}

void CSlerp::setQuatAxisAngle(int idx, double vx, double vy, double vz, double angle)
{
	if ((idx>=0)&&(idx<(int)m_quats.size()))
		m_quats[idx]->setFromAxisAngle(vx, vy, vz, angle*2.0*M_PI/360.0);
}

void CSlerp::update()
{
	int i;

	for (i=0; i<(int)m_segments.size(); i++)
		m_segments[i]->update();
}

