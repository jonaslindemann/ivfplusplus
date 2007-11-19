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

#include <ivf/ivfconfig.h>
#include <ivf/IvfLOD.h>

// ------------------------------------------------------------
CIvfLOD::CIvfLOD ()
		:CIvfSwitch()
		//TODO: check and complete member initialisation list!
{
	m_camera = NULL;
	m_nearLimit = 10.0;
	m_farLimit = 100.0;
}

// ------------------------------------------------------------
CIvfLOD::~CIvfLOD ()
{
}

// ------------------------------------------------------------
void CIvfLOD::createGeometry()
{
	if (m_camera!=NULL)
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
	CIvfSwitch::createGeometry();
}

// ------------------------------------------------------------
double CIvfLOD::lodFunction(double dist)
{
	double g = (1/(m_farLimit-m_nearLimit))*dist - m_nearLimit/(m_farLimit - m_nearLimit);
	double f = (theta(dist - m_nearLimit) - theta(dist - m_farLimit))*g + theta(dist - m_farLimit);;
	return f;
}

// ------------------------------------------------------------
double CIvfLOD::theta(double x)
{
	return 1.0*(double)(x>=0);
}

// ------------------------------------------------------------
void CIvfLOD::setCamera(CIvfCamera *camera)
{
	m_camera = camera;
}

// ------------------------------------------------------------
CIvfCamera* CIvfLOD::getCamera()
{
	return m_camera;
}

// ------------------------------------------------------------
void CIvfLOD::setLimits(double nearLimit, double farLimit)
{
	m_nearLimit = nearLimit;
	m_farLimit = farLimit;
}

// ------------------------------------------------------------
void CIvfLOD::getLimits(double &nearLimit, double &farLimit)
{
	nearLimit = m_nearLimit;
	farLimit = m_farLimit;
}
