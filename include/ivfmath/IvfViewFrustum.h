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

IvfSmartPointer(CViewFrustum);

/**
 * View frustum class
 * 
 * The CViewFrustum implements a view frustum, which can 
 * be used when culling a scene graph.
 */
class IVFMATH_API CViewFrustum : public CMathBase {
private:
	CPlane* m_near;
	CPlane* m_far;
	CPlane* m_left;
	CPlane* m_right;
	CPlane* m_top;
	CPlane* m_bottom;
public:
	/** CViewFrustum constructor */
	CViewFrustum();

	/** CIvfViewFrustom destructor */
	virtual ~CViewFrustum();

	IvfClassInfo("CViewFrustum",CMathBase);

	/** Returns true if bounding sphere is inside frustum */
	bool isInside(CBoundingSphere* bSphere);

	/** Returns bottom plane */
	CPlane* getBottomPlane();

	/** Returns top plane */
	CPlane* getTopPlane();

	/** Returns right plane */
	CPlane* getRightPlane();

	/** Returns left plane */
	CPlane* getLeftPlane();

	/** Returns far plane */
	CPlane* getFarPlane();

	/** Returns near plane */
	CPlane* getNearPlane();
};

#endif 
