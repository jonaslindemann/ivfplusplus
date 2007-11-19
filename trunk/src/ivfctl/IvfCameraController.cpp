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
//
// Written by Jonas Lindemann
//

#include <ivfctl/IvfCameraController.h>

CIvfCameraController::CIvfCameraController()
{
	m_path = new CIvfSpline3d();
	m_targetPath = new CIvfSpline3d();
	
	m_t = 0.0;
	m_v = 0.0;

	m_t0 = 0.0;
	m_v0 = 0.0;
	m_a0 = 0.0;

	m_target_t = 0.0;
	m_target_v = 0.0;

	m_target_t0 = 0.0;
	m_target_v0 = 0.0;
	m_target_a0 = 0.0;

	m_useTargetPath = false;
	m_followPath = false;

	m_endAction = EA_STOP;
	m_startAction = SA_STOP;

	m_targetEndAction = EA_STOP;
	m_targetStartAction = SA_STOP;

	m_camera = NULL;
}

CIvfCameraController::~CIvfCameraController()
{
	/*
	if (m_path!=NULL)
	{
		m_path->deleteReference();
		if (!m_path->referenced())
			delete m_path;
	}

	if (m_targetPath!=NULL)
	{
		m_targetPath->deleteReference();
		if (!m_path->referenced())
			delete m_targetPath;
	}
	*/
}

void CIvfCameraController::setPath(CIvfSpline3d *spline)
{
	m_path = spline;
	/*
	if (m_path!=NULL)
	{
		m_path->deleteReference();
		if (!m_path->referenced())
			delete m_path;
	}

	m_path = spline;
	m_path->addReference();
	*/
}

CIvfSpline3d* CIvfCameraController::getPath()
{
	return m_path;
}

void CIvfCameraController::doUpdate(double dt)
{
	if (m_camera!=NULL)
	{
		double x, y, z;

		// Update camera position path

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
			case SA_DEACTIVATE:
				this->deactivate();
				break;
			default:
				m_v = 0.0;
				m_a0 = 0.0;
				break;
			}
		}

		m_camera->setPosition(x, y, z);

		// Update target position

		if (m_useTargetPath)
		{
			double x, y, z;
			
			// Update camera position path
			
			m_target_v += m_target_a0*dt;
			m_target_t += m_target_v*dt + pow(m_target_a0,2)*dt*0.5;
			
			if (m_targetPath!=NULL)
				m_targetPath->getPosition(m_target_t).getComponents(x, y, z);

			if (m_target_t>(double)m_targetPath->getSize()-1.0)
			{
				m_target_t -= m_target_v*dt + pow(m_target_a0,2)*dt*0.5;

				switch (m_targetEndAction) {
				case EA_STOP:
					m_target_v = 0.0;
					m_target_a0 = 0.0;
					break;
				case EA_REVERSE:
					m_target_v = -m_target_v;
					break;
				case EA_RESET:
					this->reset();
					break;
				default:
					m_target_v = 0.0;
					m_target_a0 = 0.0;
					break;
				}
			}
			if (m_target_t<0)
			{
				m_target_t = 0.0;
				switch (m_targetStartAction) {
				case SA_STOP:
					m_target_v = 0.0;
					m_target_a0 = 0.0;
					break;
				case SA_REVERSE:
					m_target_v = -m_target_v;
					break;
				case SA_RESET:
					this->reset();
					break;
				default:
					m_target_v = 0.0;
					m_target_a0 = 0.0;
					break;
				}
			}
			
			m_camera->setTarget(x, y, z);
			
		}
		else
		{
			if (m_followPath)
			{
				CIvfVec3d p0;
				CIvfVec3d p1;
				CIvfVec3d forward;
				p0 = m_path->getPosition(m_t);
				//p1 = m_path->getPosition(m_t + m_v*dt + pow(m_a0,2)*dt*0.5);
				p1 = m_path->getPosition(m_t+0.3);
				forward = p1-p0;

				forward.print(std::cout);

				m_camera->setForwardVector(forward);
			}
		}
	}
}

void CIvfCameraController::doReset()
{
	m_t = m_t0;
	m_v = m_v0;

	m_target_t = m_target_t0;
	m_target_v = m_target_v0;

	if (m_camera!=NULL)
	{
		double x, y, z;

		if (m_path!=NULL)
			m_path->getPosition(m_t).getComponents(x, y, z);

		m_camera->setPosition(x, y, z);

		if (m_targetPath!=NULL)
		{
			m_targetPath->getPosition(m_target_t).getComponents(x, y, z);
			if (m_useTargetPath)
				m_camera->setTarget(x, y, z);
		}
	}
}

void CIvfCameraController::setInitialPos(double t)
{
	m_t0 = t;
	doReset();
}

void CIvfCameraController::setInitialSpeed(double v)
{
	m_v0 = v;
	doReset();
}

void CIvfCameraController::setInitialAcceleration(double a)
{
	m_a0 = a;
	doReset();
}

void CIvfCameraController::setEndActionType(TEndAction action)
{
	m_endAction = action;
}

void CIvfCameraController::setStartActionType(TStartAction action)
{
	m_startAction = action;
}

void CIvfCameraController::setCamera(CIvfCamera *camera)
{
	m_camera = camera;
}

CIvfCamera* CIvfCameraController::getCamera()
{
	return m_camera;
}

void CIvfCameraController::setTargetPath(CIvfSpline3d *spline)
{
	m_targetPath = spline;
	/*
	if (m_targetPath!=NULL)
	{
		m_targetPath->deleteReference();
		if (!m_targetPath->referenced())
			delete m_targetPath;
	}

	m_targetPath = spline;
	m_targetPath->addReference();
	*/

	m_useTargetPath = true;
}

void CIvfCameraController::setUseTargetPath(bool flag)
{
	m_useTargetPath = true;
}

bool CIvfCameraController::getUseTargetPath()
{
	return m_useTargetPath;
}

CIvfSpline3d* CIvfCameraController::getTargetPath()
{
	return m_targetPath;
}

void CIvfCameraController::setInitialTargetPos(double t)
{
	m_target_t0 = t;
	doReset();
}

void CIvfCameraController::setInitialTargetSpeed(double v)
{
	m_target_v0 = v;
	doReset();
}

void CIvfCameraController::setInitialTargetAcceleration(double a)
{
	m_target_a0 = a;
	doReset();
}

void CIvfCameraController::setTargetStartActionType(TStartAction action)
{
	m_targetStartAction = action;
}

void CIvfCameraController::setTargetEndActionType(TEndAction action)
{
	m_targetEndAction = action;
}

void CIvfCameraController::setFollowPath(bool flag)
{
	m_followPath = flag;
	if (m_camera!=NULL)
		if (m_followPath)
			m_camera->setType(CIvfCamera::CT_FREE_ORIENTATION);
		else
			m_camera->setType(CIvfCamera::CT_NORMAL);
}
