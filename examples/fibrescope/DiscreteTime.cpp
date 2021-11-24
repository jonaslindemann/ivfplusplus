//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "DiscreteTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <ivf/config.h>

DiscreteTime::DiscreteTime()
{
	m_time = 0;
	m_startTime = 0;
	m_endTime = -1;
	m_step = 1;
	m_endAction = IVF_DT_REWIND;
	m_loop = false;
}

DiscreteTime::~DiscreteTime()
{

}

long DiscreteTime::getCurrentTime()
{
	return m_time;
}

void DiscreteTime::setInterval(long startTime, long endTime)
{
	m_startTime = startTime;
	m_endTime = endTime;
	m_time = m_startTime;
}

void DiscreteTime::next()
{
	this->checkStep(m_time + m_step);
}

void DiscreteTime::rewind()
{
	m_time = m_startTime;
}

void DiscreteTime::previous()
{
	this->checkStep(m_time - m_step);
}

void DiscreteTime::setTime(long time)
{
	if (time>=m_startTime)
	{
		if (m_endTime>=0)
		{
			if (time<=m_endTime)
				m_time = time;
		}
		else
			m_time =time;
	}
}

void DiscreteTime::setEndAction(int action)
{
	m_endAction = action;
	m_time = 0;
	m_step = 1;
}

void DiscreteTime::last()
{
	m_time = m_endTime;
}

void DiscreteTime::setLoop(bool flag)
{
	m_loop = flag;
}

void DiscreteTime::reset()
{
	m_time = 0;
	m_step = 1;
	m_loop = false;
}

void DiscreteTime::checkStep(int testStep)
{
	if (testStep<m_startTime)
	{
		if (m_loop)
		{
			switch (m_endAction) {
			case IVF_DT_REWIND:
				m_time = m_endTime;
				break;
			case IVF_DT_REVERSE:
				m_step = -m_step;
				m_time += m_step;
				break;
			default:
				m_time = m_endTime;
				break;
			}
		}
	}

	if (testStep>m_endTime)
	{
		if (m_loop)
		{
			switch (m_endAction) {
			case IVF_DT_REWIND:
				m_time = m_startTime;
				break;
			case IVF_DT_REVERSE:
				m_step = -m_step;
				m_time += m_step;
				break;
			default:
				m_time = m_startTime;
				break;
			}
		}
	}

	if ((testStep>=m_startTime)&&(testStep<=m_endTime))
		m_time += m_step;

}
