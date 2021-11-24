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
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivf/Composite.h>
#include <ivf/Camera.h>

namespace ivf {

IvfSmartPointer(BillBoard);

#define IVF_BILLBOARD_Y  0
#define IVF_BILLBOARD_X  1
#define IVF_BILLBOARD_XY 2

#define IVF_ALIGN_CAMERA 0
#define IVF_ALIGN_VECTOR 1

/**
 * Billboard object
 * 
 * CIvfBase handles an object that is pointed at the viewer or
 * against a specified vector
 *
 * @author Jonas Lindemann
 */
class IVF_API BillBoard : public Composite {
private:
	Camera* m_camera;
	double m_angle1;
	double m_angle2;
	int m_billboardType;
	void updateRotation();
	double m_forward[3];
	int m_alignObject;
public:
	/** CIvfBillboard constructor */
	BillBoard();
	
	/** CIvfBillboard destructor */
	virtual ~BillBoard();

	IvfClassInfo("CBillboard",Composite);

	/** Assign the camera used to align the object. */
	void setCamera(Camera* camera);

	/** 
	 * Set billboard type
	 *
	 * IVF_BILLBOARD_Y  - Rotate around global Y-axis 
	 * IVF_BILLBOARD_X  - Rotate around global X-axis
	 * IVF_BILLBOARD_XY - Rotate around X- and Y-axis
	 */
	void setBillboardType(int type);

	/** Return current billboard type */
	int getBillboardType();

	/** 
	 * Set alignment object type
	 *
	 * IVF_ALIGN_CAMERA - Object is aligned against camera
	 * IVF_ALIGN_VECTOR - Object is aligned against a vector
	 */
	void setAlignObject(int objectType);

	/** Set alignment vector */
	void setVector(double *v);

	/** Set alignment vector */
	void setVector(double vx, double vy, double vz);

protected:
	virtual void doCreateGeometry();
};

}

