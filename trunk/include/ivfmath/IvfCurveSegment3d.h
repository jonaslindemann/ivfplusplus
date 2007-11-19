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

#ifndef _CIvfCurveSegment3d_h_
#define _CIvfCurveSegment3d_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>

IvfSmartPointer(CIvfCurveSegment3d);

/**
 * Abstract curve segment class
 *
 * Defines the basic interface for a curve.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfCurveSegment3d : public CIvfMathBase {
private:
	double m_t0;
public:
	/** CIvfCurveSegment constructor */
	CIvfCurveSegment3d();

	/** CIvfCurveSegment destructor */
	virtual ~CIvfCurveSegment3d();

	IvfClassInfo("CIvfCurveSegment3d",CIvfMathBase);

  	/** Set start position of curve */
	void setStartPosition(double t0);
  	
	/** Get point on curve */
  	virtual CIvfVec3d* getPosition(double t)=0;
	
	/** Get start point on curve */
  	virtual CIvfVec3d* getStartPoint()=0;
	
	/** Get end point on curve */
  	virtual CIvfVec3d* getEndPoint()=0;
};

#endif
