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

#ifndef _CIvfSplineSegment3d_h_
#define _CIvfSplineSegment3d_h_

#include <ivfmath/IvfCurveSegment3d.h>

/**
 * 3d spline segment class
 *
 * Implements a 3d spline segment, used in conjunction with
 * the CIvfSpline3d class.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfSplineSegment3d : public CIvfCurveSegment3d  {
private:
	CIvfVec3d* m_p1;
	CIvfVec3d* m_p2;
	CIvfVec3d* m_p;
	CIvfVec3d* m_t;
	CIvfVec3d* m_t1;
	CIvfVec3d* m_t2;
	double m_bias[2];
	
	double h0(double t);
	double h1(double t);
	double h2(double t);
	double h3(double t);

	double dh3(double t);
	double dh2(double t);
	double dh1(double t);
	double dh0(double t);
public:
	/** CIvfSplineSegment3d constructor */
	CIvfSplineSegment3d();

	/** CIvfSplineSegment3d destructor */
	virtual ~CIvfSplineSegment3d();
	
	/** 
	 * Get position on curve 
	 *
	 * @param t distance from curve start 0 <= t <= 1
	 */
	virtual CIvfVec3d* getPosition(double t);

	/** Get curve start point */
	virtual CIvfVec3d* getEndPoint();

	/** Get curve starting point */
	virtual CIvfVec3d* getStartPoint();
	
	/** Get curve end direction */
	virtual CIvfVec3d* getEndDirection();

	/** Get curve start direction */
	virtual CIvfVec3d* getStartDirection();

	/** Get curve end bias*/
	double getEndBias();

	/** Get curve start bias*/
	double getStartBias();

	/** Set curve end bias*/
	void setEndBias(double bias);

	/** Set curve start bias*/
	void setStartBias(double bias);

	/** Set curve end point */
  	void setEndPoint(CIvfVec3d* point);
	
	/** Set curve start point */
	void setStartPoint(CIvfVec3d* point);

	/** 
	 * Get curve speed
	 *
	 * @param t distance from curve start 0.0 <= t <= 1.0
	 */
	double getSpeed(double t);

	/** 
	 * Get curve tangent
	 *
	 * @param t distance from curve start 0.0 <= t <= 1.0
	 */
	virtual CIvfVec3d* getTangent(double t);
};

#endif
