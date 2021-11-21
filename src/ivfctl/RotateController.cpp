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

#include <ivfctl/RotateController.h>

using namespace ivf;

CRotateController::CRotateController()
{
	m_rotationSpeed[0] = 0;
	m_rotationSpeed[1] = 0;
	m_rotationSpeed[2] = 0;

	m_useAxis = false;
	m_axis.setComponents(0.0, 0.0, 1.0);
}

CRotateController::~CRotateController()
{

}

void CRotateController::setRotationSpeed(double vx, double vy, double vz)
{
	m_rotationSpeed[0] = vx;
	m_rotationSpeed[1] = vy;
	m_rotationSpeed[2] = vz;
	m_useAxis = false;
}

void CRotateController::getRotationSpeed(double &vx, double &vy, double &vz)
{
	vx = m_rotationSpeed[0];
	vy = m_rotationSpeed[1];
	vz = m_rotationSpeed[2];
}

void CRotateController::doUpdate(double dt)
{
	CShape* shape = this->getShape();

	if (shape!=nullptr)
	{	
		if (!m_useAxis)
		{
			double rx, ry, rz;
			shape->getRotation(rx, ry, rz);
			rx+=m_rotationSpeed[0]*dt;
			ry+=m_rotationSpeed[1]*dt;
			rz+=m_rotationSpeed[2]*dt;
			shape->setRotation(rx, ry, rz);
		}
		else
		{
			double vx, vy, vz, theta;
			shape->getRotationQuat(vx, vy, vz, theta);
			theta+=m_rotationSpeed[0]*dt;
			shape->setRotationQuat(vx, vy, vz, theta);
		}
	}
}

void CRotateController::setRotationSpeed(double v)
{
	m_rotationSpeed[0] = v;
	m_useAxis = true;
}

