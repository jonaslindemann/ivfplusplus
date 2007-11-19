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

#ifndef _IvfFunctions_h_
#define _IvfFunctions_h_

#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>
#include <ivf/ivfconfig.h>

IvfSmartPointer(CIvfFunction2d);

/**
 * Abstract function class
 *
 * Defines an interface for a math function.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfFunction2d : public CIvfMathBase {
private:

public:
	IvfClassInfo("CIvfFunction2d",CIvfMathBase);
	/** 
	 * function method 
	 * 
	 * This method should be overridden in 
	 * derived function classes
	 */
	virtual double f(double x) = 0;
};

class IVFMATH_API CIvfScalarVectorFunction : public CIvfMathBase {
private:

public:
	IvfClassInfo("CIvfScalarVectorFunction",CIvfMathBase);

	virtual CIvfVec3d& f(double x) = 0;
};

IvfSmartPointer(CIvfLine2d);

/**
 * 2D line function
 *
 * Implements a 2d line, based on f(x) = kx + m.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfLine2d : public CIvfFunction2d {
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
	CIvfLine2d(double x1, double y1, double x2, double y2);

	/**
	 * CIvfLine2d constructor
	 *
	 * Inititialises line to f(x) = x
	 */
	CIvfLine2d();

	IvfClassInfo("CIvfLine2d",CIvfFunction2d);

	/**
	 * CIvfLine2d constructor
	 *
	 * Initialises line using k and m 
	 */
	CIvfLine2d(double k, double m);

	/** Returns line function value */
	virtual double f(double x);
};

IvfSmartPointer(CIvfTheta2d);

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
class IVFMATH_API CIvfTheta2d : public CIvfFunction2d {
private:

public:
	IvfClassInfo("CIvfTheta2d",CIvfFunction2d);
	/** Theta function value */
	virtual double f(double x);
};

IvfSmartPointer(CIvfPulse2d);

/**
 * Pulse function
 *
 * Implements a pulse function 
 *
 * f(x) = theta(x)-theta(x-delta))/delta
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfPulse2d : public CIvfFunction2d {
private:
	double m_delta;
	CIvfTheta2d m_theta;
public:
	/**
	 * CIvfPulse2d constructor
	 *
	 * Initialises pulse function with a width
	 * of @param delta.
	 */
	CIvfPulse2d(double delta);

	IvfClassInfo("CIvfPulse2d",CIvfFunction2d);

	/** Pulse function value */
	virtual double f(double x);
};

IvfSmartPointer(CIvfEnvPos2d);

/**
 * Envelope position class
 *
 * Implements a position on an envelope function 
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfEnvPos2d : public CIvfMathBase {
private:
	double m_pos[2];
public:
	/** 
	 * CIvfEnvPos2d constructor
	 *
	 * Default values are (0,0)
	 */
	CIvfEnvPos2d();

	/**
	 * CIvfEnvPos2d constructor
	 *
	 * @param x x-position on envelope
	 * @param y y-position on envelope 
	 */
	CIvfEnvPos2d(double x, double y);

	IvfClassInfo("CIvfEnvPos2d",CIvfMathBase);

	/** Return x position */
	double getY();

	/** Return y position */
	double getX();
};

/**
 * STL IvfEnvPos2d compare function
 */
struct SIvfEnvPos2dCompare {
    bool operator() (CIvfEnvPos2d* p1, CIvfEnvPos2d* p2) const {
        return (p1->getX()<p2->getX());
    }
};

namespace std {};

typedef std::set<CIvfEnvPos2d*, SIvfEnvPos2dCompare> CIvfEnvelopePosSet;
typedef std::set<CIvfEnvPos2d*, SIvfEnvPos2dCompare>::iterator CIvfEnvelopePosSetIter;
typedef std::vector<CIvfEnvPos2d*> CIvfEnvelopePosVector;
typedef std::vector<CIvfEnvPos2d*>::iterator CIvfEnvelopePosVectorIter;

IvfSmartPointer(CIvfEnvelope2d);

/**
 * 2d envelope function
 *
 * Implements a linear 2d envelope function defined using a set of
 * 2d points.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfEnvelope2d : public CIvfFunction2d {
private:
	double interpolate(CIvfEnvPos2d* pos1, CIvfEnvPos2d* pos2, double x);
	CIvfEnvelopePosSet m_envelopeSet;
	CIvfEnvelopePosVector m_envelope;
	double m_range[2];
	bool m_finished;
public:
	/** CIvfEnvelope2d constructor */
	CIvfEnvelope2d();

	/** CIvfEnvelope2d destructor */
	virtual ~CIvfEnvelope2d();

	IvfClassInfo("CIvfEnvelope2d",CIvfFunction2d);

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

#endif
