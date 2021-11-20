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

#ifndef _CIvfBoundingSphere_h_
#define _CIvfBoundingSphere_h_

#include <ivfmath/IvfMathBase.h>

IvfSmartPointer(CBoundingSphere);

/**
 * Bounding sphere class
 * 
 * CBoundingSphere defines a bounding sphere used together 
 * with the CIvfCulledScene class.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CBoundingSphere : public CMathBase {
private:
	double m_radius;
	double m_globalPos[3];
public:
	/** CIvfBounding sphere constructor */
	CBoundingSphere();

	/** CIvfBounding sphere destructor */
	virtual ~CBoundingSphere();

	IvfClassInfo("CBoundingSphere",CMathBase);

	/** Set the bounding sphere radius. */
	void setRadius(double radius);

	/** 
	 * Returns the bounding sphere radius. 
	 * 
	 * The bounding sphere radius is initially set to 0.0.
	 */
	double getRadius();

	/** Sets the global position of the bounding sphere. */
	void getGlobalPosition(double &x, double &y, double &z);
	
	/** 
	 * Returns the global position of the bounding sphere. 
	 * 
	 * Global position is initially set to (0, 0, 0)
	 */
	void setGlobalPosition(double x, double y, double z);
};

#endif 
