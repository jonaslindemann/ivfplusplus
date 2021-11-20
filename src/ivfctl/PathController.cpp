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

#include <ivfctl/PathController.h>

using namespace ivf;

CPathController::CPathController()
{
	m_path = new CSpline3d();
	
	m_t = 0.0;
	m_v = 0.0;

	m_t0 = 0.0;
	m_v0 = 0.0;
	m_a0 = 0.0;

	m_endAction = EA_STOP;
	m_startAction = SA_STOP;
}

CPathController::~CPathController()
{
}

void CPathController::setPath(CSpline3d *spline)
{
	m_path = spline;
}

CSpline3d* CPathController::getPath()
{
	return m_path;
}

void CPathController::doUpdate(double dt)
{
	CShape* shape = this->getShape();
	if (shape!=NULL)
	{
		double x, y, z;

		m_v += m_a0*dt;
		m_t += m_v*dt + pow(m_a0,2)*dt*0.5;

		if (m_path!=NULL)
			m_path->getPosition(m_t).getComponents(x, y, z);

		if (m_t>(double)m_path->getSize()-1.0)
		{
			m_t -= m_v*dt + pow(m_a0,2)*dt*0.5;

			doEndAction();

			switch (m_endAction) {
			case EA_STOP:
				m_v = 0.0;
				m_a0 = 0.0;
				break;
			case EA_REVERSE:
				m_v = -m_v;
				break;
			case EA_RESET:
				this->reset();
				break;
			case EA_DEACTIVATE:
				this->deactivate();
				break;
			default:
				m_v = 0.0;
				m_a0 = 0.0;
				break;
			}
		}
		if (m_t<0)
		{
			m_t = 0.0;

			doStartAction();

			switch (m_startAction) {
			case SA_STOP:
				m_v = 0.0;
				m_a0 = 0.0;
				break;
			case SA_REVERSE:
				m_v = -m_v;
				break;
			case SA_RESET:
				this->reset();
				break;
			default:
				m_v = 0.0;
				m_a0 = 0.0;
				break;
			}
		}

		shape->setPosition(x, y, z);
		if (getLight()!=NULL)
			getLight()->setPosition(x, y, z);
	}
}

void CPathController::doReset()
{
	m_t = m_t0;
	m_v = m_v0;

	CShape* shape = this->getShape();
	if (shape!=NULL)
	{
		double x, y, z;

		if (m_path!=NULL)
			m_path->getPosition(m_t).getComponents(x, y, z);

		shape->setPosition(x, y, z);
	}
}

void CPathController::setInitialPos(double t)
{
	m_t0 = t;
	doReset();
}

void CPathController::setInitialSpeed(double v)
{
	m_v0 = v;
	doReset();
}

void CPathController::setInitialAcceleration(double a)
{
	m_a0 = a;
	doReset();
}

void CPathController::setEndActionType(TEndAction action)
{
	m_endAction = action;
}

void CPathController::setStartActionType(TStartAction action)
{
	m_startAction = action;
}
