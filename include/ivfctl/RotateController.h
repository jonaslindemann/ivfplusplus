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

#pragma once

#include <ivfctl/Controller.h>

#include <ivfmath/Vec3d.h>

namespace ivf {

IvfSmartPointer(CRotateController);

/**
 * Rotation controller class
 *
 * This class can control the rotation of an object
 * around a given axis using a specified speed or by specifying
 * a rotation speed around the three main axes
 */
class IVFCTL_API CRotateController : public CController {
private:
	double m_rotationSpeed[3];
	CVec3d m_axis;
	bool m_useAxis;
public:
	/** CRotateController constructor */
	CRotateController();

	/** CRotateController destructor */
	virtual ~CRotateController();

	IvfClassInfo("CRotateController",CController);
	IvfStdFactory(CRotateController);

	/** Set rotation speeds around x, y and z axes */
	void setRotationSpeed(double vx, double vy, double vz);

	/** Get rotation speeds around x, y and z axes */
	void getRotationSpeed(double &vx, double &vy, double &vz);

	/** Set rotation speed around the rotation axis of the object */
	void setRotationSpeed(double v);

	virtual void doUpdate(double dt);

};

}