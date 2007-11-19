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

#ifndef _CIvfViewFrustum_h_
#define _CIvfViewFrustum_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfPlane.h>
#include <ivfmath/IvfBoundingSphere.h>

IvfSmartPointer(CIvfViewFrustum);

/**
 * View frustum class
 * 
 * The CIvfViewFrustum implements a view frustum, which can 
 * be used when culling a scene graph.
 */
class IVFMATH_API CIvfViewFrustum : public CIvfMathBase {
private:
	CIvfPlane* m_near;
	CIvfPlane* m_far;
	CIvfPlane* m_left;
	CIvfPlane* m_right;
	CIvfPlane* m_top;
	CIvfPlane* m_bottom;
public:
	/** CIvfViewFrustum constructor */
	CIvfViewFrustum();

	/** CIvfViewFrustom destructor */
	virtual ~CIvfViewFrustum();

	IvfClassInfo("CIvfViewFrustum",CIvfMathBase);

	/** Returns true if bounding sphere is inside frustum */
	bool isInside(CIvfBoundingSphere* bSphere);

	/** Returns bottom plane */
	CIvfPlane* getBottomPlane();

	/** Returns top plane */
	CIvfPlane* getTopPlane();

	/** Returns right plane */
	CIvfPlane* getRightPlane();

	/** Returns left plane */
	CIvfPlane* getLeftPlane();

	/** Returns far plane */
	CIvfPlane* getFarPlane();

	/** Returns near plane */
	CIvfPlane* getNearPlane();
};

#endif 
