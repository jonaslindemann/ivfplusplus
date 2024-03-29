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

#include <ivf/config.h>
#include <ivf/LOD.h>

using namespace ivf;

// ------------------------------------------------------------
LOD::LOD ()
		:Switch()
		//TODO: check and complete member initialisation list!
{
	m_camera = nullptr;
	m_nearLimit = 10.0;
	m_farLimit = 100.0;
}

// ------------------------------------------------------------
LOD::~LOD ()
{
}

// ------------------------------------------------------------
void LOD::doCreateGeometry()
{
	if (m_camera!=nullptr)
	{
		double x1, y1, z1;
		double x2, y2, z2;
		double distance, lodValue;
		int childIndex;

		this->getPosition(x2, y2, z2);
		m_camera->getPosition(x1, y1, z1);
		distance = sqrt( pow(x2-x1,2) + pow(y2-y1,2) + pow(z2-z1,2));
		lodValue = lodFunction(distance);
		childIndex = (int)((this->getSize()-1)*lodValue);
		this->setCurrentChild(childIndex);
	}
	else
	{
		this->setCurrentChild(0);
	}
	Switch::doCreateGeometry();
}

// ------------------------------------------------------------
double LOD::lodFunction(double dist)
{
	double g = (1/(m_farLimit-m_nearLimit))*dist - m_nearLimit/(m_farLimit - m_nearLimit);
	double f = (theta(dist - m_nearLimit) - theta(dist - m_farLimit))*g + theta(dist - m_farLimit);;
	return f;
}

// ------------------------------------------------------------
double LOD::theta(double x)
{
	return 1.0*(double)(x>=0);
}

// ------------------------------------------------------------
void LOD::setCamera(Camera *camera)
{
	m_camera = camera;
}

// ------------------------------------------------------------
Camera* LOD::getCamera()
{
	return m_camera;
}

// ------------------------------------------------------------
void LOD::setLimits(double nearLimit, double farLimit)
{
	m_nearLimit = nearLimit;
	m_farLimit = farLimit;
}

// ------------------------------------------------------------
void LOD::getLimits(double &nearLimit, double &farLimit)
{
	nearLimit = m_nearLimit;
	farLimit = m_farLimit;
}
