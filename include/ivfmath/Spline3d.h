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

#include <ivf/config.h>

#include <ivfmath/MathBase.h>
#include <ivfmath/SplineSegment3d.h>

namespace ivf {

IvfSmartPointer(Spline3d);

/**
 * 3d spline class
 *
 * Implements a 3d spline class consisting of a number
 * of SplineSegment3d segments.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Spline3d : public MathBase {
private:
	int m_size;
	std::vector<SplineSegment3d*> m_segments;	
	std::vector<Vec3d*> m_points;

	std::vector<double> m_tau;    // Slack/tightening
	std::vector<double> m_gamma;  // Continuity
	std::vector<double> m_beta;   // Bias

	Vec3d* m_start;
	Vec3d* m_end;

	void initSpline();
	void updateSpline();
public:
	/** Spline3d constructor */
	Spline3d();

	/** Spline3d destructor */
	virtual ~Spline3d();

	IvfClassInfo("Spline3d",MathBase);
	IvfStdFactory(Spline3d);

	/** Removes all spline points. */
	void clear();

	/** Returns the number of points in the spline */
	int getSize();

	/** 
	 * Set number of points in spline 
	 *
	 * This will effectively remove all previous points.
	 */
	void setSize(int size);
	
	/** Get specified point on spline. */
	Vec3d* getPoint(int idx);
	
	/** 
	 * Get position on curve 
	 *
	 * @param t distance from curve start 0 <= t <= getSize()-1
	 */
	Vec3d& getPosition(double t);

	/** Updates the spline if any points are changed */
	void update();

	/** Set tangent weight of specified point */
	void setTangentWeight(int idx, double value);

	/** Set tangent continuity of specified point */
	void setContinuity(int idx, double value);
	
	/** Set tangen tension of specified point */
	void setTension(int idx, double value);

	/** 
	 * Get curve speed 
	 *
	 * @param t distance from curve start 0 <= t <= getSize()-1
	 */
	double getSpeed(double t);

	/** 
	 * Get curve tangent 
	 *
	 * @param t distance from curve start 0 <= t <= getSize()-1
	 */
	Vec3d& getTangent(double t);
};

}