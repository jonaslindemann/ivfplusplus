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

IvfSmartPointer(Function2d);

/**
 * Abstract function class
 *
 * Defines an interface for a math function.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Function2d : public MathBase {
private:

public:
	IvfClassInfo("Function2d",MathBase);
	/** 
	 * function method 
	 * 
	 * This method should be overridden in 
	 * derived function classes
	 */
	virtual double f(double x) = 0;
};

class IVFMATH_API ScalarVectorFunction : public MathBase {
private:

public:
	IvfClassInfo("ScalarVectorFunction",MathBase);

	virtual Vec3d& f(double x) = 0;
};

IvfSmartPointer(Line2d);

/**
 * 2D line function
 *
 * Implements a 2d line, based on f(x) = kx + m.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Line2d : public Function2d {
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
	Line2d(double x1, double y1, double x2, double y2);

	/**
	 * Line2d constructor
	 *
	 * Inititialises line to f(x) = x
	 */
	Line2d();

	IvfClassInfo("Line2d",Function2d);

	/**
	 * Line2d constructor
	 *
	 * Initialises line using k and m 
	 */
	Line2d(double k, double m);

	/** Returns line function value */
	virtual double f(double x);
};

IvfSmartPointer(Theta2d);

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
class IVFMATH_API Theta2d : public Function2d {
private:

public:
	IvfClassInfo("Theta2d",Function2d);
	/** Theta function value */
	virtual double f(double x);
};

IvfSmartPointer(Pulse2d);

/**
 * Pulse function
 *
 * Implements a pulse function 
 *
 * f(x) = theta(x)-theta(x-delta))/delta
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Pulse2d : public Function2d {
private:
	double m_delta;
	Theta2d m_theta;
public:
	/**
	 * Pulse2d constructor
	 *
	 * Initialises pulse function with a width
	 * of @param delta.
	 */
	Pulse2d(double delta);

	IvfClassInfo("Pulse2d",Function2d);

	/** Pulse function value */
	virtual double f(double x);
};

IvfSmartPointer(EnvPos2d);

/**
 * Envelope position class
 *
 * Implements a position on an envelope function 
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API EnvPos2d : public MathBase {
private:
	double m_pos[2];
public:
	/** 
	 * EnvPos2d constructor
	 *
	 * Default values are (0,0)
	 */
	EnvPos2d();

	/**
	 * EnvPos2d constructor
	 *
	 * @param x x-position on envelope
	 * @param y y-position on envelope 
	 */
	EnvPos2d(double x, double y);

	IvfClassInfo("EnvPos2d",MathBase);

	/** Return x position */
	double getY();

	/** Return y position */
	double getX();
};

/**
 * STL IvfEnvPos2d compare function
 */
struct SIvfEnvPos2dCompare {
    bool operator() (EnvPos2d* p1, EnvPos2d* p2) const {
        return (p1->getX()<p2->getX());
    }
};

typedef std::set<EnvPos2d*, SIvfEnvPos2dCompare> CIvfEnvelopePosSet;
typedef std::set<EnvPos2d*, SIvfEnvPos2dCompare>::iterator CIvfEnvelopePosSetIter;
typedef std::vector<EnvPos2d*> CIvfEnvelopePosVector;
typedef std::vector<EnvPos2d*>::iterator CIvfEnvelopePosVectorIter;

IvfSmartPointer(Envelope2d);

/**
 * 2d envelope function
 *
 * Implements a linear 2d envelope function defined using a set of
 * 2d points.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Envelope2d : public Function2d {
private:
	double interpolate(EnvPos2d* pos1, EnvPos2d* pos2, double x);
	CIvfEnvelopePosSet m_envelopeSet;
	CIvfEnvelopePosVector m_envelope;
	double m_range[2];
	bool m_finished;
public:
	/** Envelope2d constructor */
	Envelope2d();

	/** Envelope2d destructor */
	virtual ~Envelope2d();

	IvfClassInfo("Envelope2d",Function2d);

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