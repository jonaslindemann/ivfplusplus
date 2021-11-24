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

#include <ivf/Shape.h>

namespace ivf {

IvfSmartPointer(Ruler);

/**
 * Ruler class
 *
 * Implements a simple ruler, which draws a line
 * between two points with tickmarks.
 */
class IVF_API Ruler : public Shape {
private:
	Vec3d m_startPoint;
	Vec3d m_endPoint;
	Vec3d m_direction;
	double m_tickStep;
	double m_length;

	void initRuler();
public:
	/** Class constructor */
	Ruler();

	/** Class destructor */
	virtual ~Ruler();

	/** Set the distance between each tick on the ruler */
	void setTickStep(double tickStep);

	/** Set ruler end point */
	void setEndPoint(double x, double y, double z);

	/** Set ruler start point */
	void setStartPoint(double x, double y, double z);

	/** Set ruler end point */
	void setEndPoint(Vec3d& endPoint);

	/** Set ruler start point */
	void setStartPoint(Vec3d& startPoint);

	IvfClassInfo("Ruler",Shape);
protected:
	virtual void doCreateGeometry();
};

}