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

#include <ivfmath/MathBase.h>
#include <ivfmath/Plane.h>
#include <ivfmath/BoundingSphere.h>

namespace ivf {

IvfSmartPointer(ViewFrustum);

/**
 * View frustum class
 * 
 * The ViewFrustum implements a view frustum, which can 
 * be used when culling a scene graph.
 */
class IVFMATH_API ViewFrustum : public MathBase {
private:
	Plane* m_near;
	Plane* m_far;
	Plane* m_left;
	Plane* m_right;
	Plane* m_top;
	Plane* m_bottom;
public:
	/** ViewFrustum constructor */
	ViewFrustum();

	/** CIvfViewFrustom destructor */
	virtual ~ViewFrustum();

	IvfClassInfo("ViewFrustum",MathBase);
	IvfStdFactory(ViewFrustum);

	/** Returns true if bounding sphere is inside frustum */
	bool isInside(BoundingSphere* bSphere);

	/** Returns bottom plane */
	Plane* getBottomPlane();

	/** Returns top plane */
	Plane* getTopPlane();

	/** Returns right plane */
	Plane* getRightPlane();

	/** Returns left plane */
	Plane* getLeftPlane();

	/** Returns far plane */
	Plane* getFarPlane();

	/** Returns near plane */
	Plane* getNearPlane();
};

}