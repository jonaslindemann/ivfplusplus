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

#include <ivfmath/MathBase.h>
#include <ivfmath/Vec3d.h>
#include <set>

namespace ivf {

IvfSmartPointer(CFunction2d);

/**
 * Abstract function class
 *
 * Defines an interface for a math function.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CFunction2d : public CMathBase {
private:

public:
	IvfClassInfo("CFunction2d",CMathBase);
	/** 
	 * function method 
	 * 
	 * This method should be overridden in 
	 * derived function classes
	 */
	virtual double f(double x) = 0;
};

class IVFMATH_API CScalarVectorFunction : public CMathBase {
private:

public:
	IvfClassInfo("CScalarVectorFunction",CMathBase);

	virtual CVec3d& f(double x) = 0;
};

IvfSmartPointer(CLine2d);

/**
 * 2D line function
 *
 * Implements a 2d line, based on f(x) = kx + m.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CLine2d : public CFunction2d {
private:
	double m_k;
	double m_m;
public:
	/** 
	 * CIvfLine constructor 
	 *
	 * Initialises line by to points on line (x1, y1) and
	 * (x2, y2)
	 */
	CLine2d(double x1, double y1, double x2, double y2);

	/**
	 * CLine2d constructor
	 *
	 * Inititialises line to f(x) = x
	 */
	CLine2d();

	IvfClassInfo("CLine2d",CFunction2d);

	/**
	 * CLine2d constructor
	 *
	 * Initialises line using k and m 
	 */
	CLine2d(double k, double m);

	/** Returns line function value */
	virtual double f(double x);
};

IvfSmartPointer(CTheta2d);

/**
 * Theta function
 *
 * Implements the theta function 
 *
 * f(x) = 
 *     1, x >= 0
 *     0, x <  0
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CTheta2d : public CFunction2d {
private:

public:
	IvfClassInfo("CTheta2d",CFunction2d);
	/** Theta function value */
	virtual double f(double x);
};

IvfSmartPointer(CPulse2d);

/**
 * Pulse function
 *
 * Implements a pulse function 
 *
 * f(x) = theta(x)-theta(x-delta))/delta
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CPulse2d : public CFunction2d {
private:
	double m_delta;
	CTheta2d m_theta;
public:
	/**
	 * CPulse2d constructor
	 *
	 * Initialises pulse function with a width
	 * of @param delta.
	 */
	CPulse2d(double delta);

	IvfClassInfo("CPulse2d",CFunction2d);

	/** Pulse function value */
	virtual double f(double x);
};

IvfSmartPointer(CEnvPos2d);

/**
 * Envelope position class
 *
 * Implements a position on an envelope function 
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CEnvPos2d : public CMathBase {
private:
	double m_pos[2];
public:
	/** 
	 * CEnvPos2d constructor
	 *
	 * Default values are (0,0)
	 */
	CEnvPos2d();

	/**
	 * CEnvPos2d constructor
	 *
	 * @param x x-position on envelope
	 * @param y y-position on envelope 
	 */
	CEnvPos2d(double x, double y);

	IvfClassInfo("CEnvPos2d",CMathBase);

	/** Return x position */
	double getY();

	/** Return y position */
	double getX();
};

/**
 * STL IvfEnvPos2d compare function
 */
struct SIvfEnvPos2dCompare {
    bool operator() (CEnvPos2d* p1, CEnvPos2d* p2) const {
        return (p1->getX()<p2->getX());
    }
};

typedef std::set<CEnvPos2d*, SIvfEnvPos2dCompare> CIvfEnvelopePosSet;
typedef std::set<CEnvPos2d*, SIvfEnvPos2dCompare>::iterator CIvfEnvelopePosSetIter;
typedef std::vector<CEnvPos2d*> CIvfEnvelopePosVector;
typedef std::vector<CEnvPos2d*>::iterator CIvfEnvelopePosVectorIter;

IvfSmartPointer(CEnvelope2d);

/**
 * 2d envelope function
 *
 * Implements a linear 2d envelope function defined using a set of
 * 2d points.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CEnvelope2d : public CFunction2d {
private:
	double interpolate(CEnvPos2d* pos1, CEnvPos2d* pos2, double x);
	CIvfEnvelopePosSet m_envelopeSet;
	CIvfEnvelopePosVector m_envelope;
	double m_range[2];
	bool m_finished;
public:
	/** CEnvelope2d constructor */
	CEnvelope2d();

	/** CEnvelope2d destructor */
	virtual ~CEnvelope2d();

	IvfClassInfo("CEnvelope2d",CFunction2d);

	/** Adds a point to envelope (required) */
	void addPoint(double x, double y);

	/** 
	 * Finish envelope definition (required) 
	 *
	 * When all points have been added to the envelope
	 * function the finish() method must be called to 
	 * sort the envelope points and enable fast lookup
	 * of envelope break points.
	 */
	void finish();

	/** Clears envelope function */
	void clear();

	/** Returns minimum x position of envelope function */
	double getMinX();

	/** Returns maximum x position of envelope function */
	double getMaxX();

	/** Envelope function value */
	virtual double f(double x);
};

}