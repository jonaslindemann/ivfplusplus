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

#include <ivfmath/IvfSlerpSegment.h>

CSlerpSegment::CSlerpSegment()
{
	m_q1 = new CQuat();
	m_q1->addReference();
	m_q2 = new CQuat();
	m_q2->addReference();
}

CSlerpSegment::~CSlerpSegment()
{
	if (m_q1!=NULL)
	{
		m_q1->deleteReference();
		if (!m_q1->referenced())
			delete m_q1;
	}
	
	if (m_q2!=NULL)
	{
		m_q2->deleteReference();
		if (!m_q2->referenced())
			delete m_q2;
	}
}

CQuat& CSlerpSegment::getQuat(double t)
{
	CQuat& s = ivfGetTempQuat();
	double k1, k2;

	if ((t>=0.0)&&(t<=1.0))
	{
		k1 = sin(m_theta*(1.0-t))/sin(m_theta);
		k2 = sin(m_theta*t)/sin(m_theta);

		//cout << k1 << ", " << k2 << endl;

		s = (*m_q1)*k1 + (*m_q2)*k2;

		return s;
	}
	else
	{
		if (t>1.0)
			return *m_q2;
		else
			return *m_q1;
	}
}

void CSlerpSegment::initSlerp()
{
	double qx, qy, qz, qw;
	double rx, ry, rz, rw;

	m_q1->getComponents(qx, qy, qz, qw);
	m_q2->getComponents(rx, ry, rz, rw);
	m_theta = acos(qx*rx + qy*ry + qz*rz + qw*rw);
}

const void CSlerpSegment::setStartQuat(CQuat* q)
{
	if (m_q1!=NULL)
	{
		m_q1->deleteReference();
		if (!m_q1->referenced())
			delete m_q1;
	}
	m_q1 = q;
	m_q1->addReference();

	initSlerp();
}

const void CSlerpSegment::setEndQuat(CQuat* q)
{
	if (m_q2!=NULL)
	{
		m_q2->deleteReference();
		if (!m_q2->referenced())
			delete m_q2;
	}
	m_q2 = q;
	m_q2->addReference();
	
	initSlerp();
}

void CSlerpSegment::update()
{
	initSlerp();
}
