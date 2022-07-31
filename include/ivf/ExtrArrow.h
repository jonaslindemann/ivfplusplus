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

IvfSmartPointer(ExtrArrow);

/**
 * Extruded arrow class
 *
 * CIvfExtrArrow implements an arrow using an extrusion.
 *
 * @author Jonas Lindemann
 */
class IVF_API ExtrArrow : public Shape {
private:
	void initExtrusion();
	int arrowType;
	double m_extraHead;
	double m_headRadius;
	double m_head;
	double m_length;
	double m_tailRadius;
	double m_coords[6][3];
	double m_radius[6];
	double m_direction[3];
	double m_position[3];
	double m_offset;
public:
	void setDirection(Vec3d& vec);
	/** CIvfExtrArrow constructor */
	ExtrArrow();

	/** CIvfExtrArrow destructor */
	virtual ~ExtrArrow();

	IvfClassInfo("ExtrArrow",Shape);
	IvfStdFactory(ExtrArrow);

	/** Set the arrow radiuses */
	void setRadius(double head, double tail);

	/** Return arrow radiuses */
	void getRadius(double &head, double &tail);

	/** Set the arrow sizes */
	void setSize(double length, double head);

	/** Return arrow sizes */
	void getSize(double &length, double &head);

	/** Set the arrow direction in vector form */
	void setDirection(double ex, double ey, double ez);

	/** Return arrow direction */
	void getDirection(double &ex, double &ey, double &ez);

	/**
	 * Set arrow offset
	 *
	 * Default, the arrow head is placed at (0,0,0)
	 * the @param offset moves the arrow in the arrow
	 * direction
	 */
	void setOffset(double offset);

	/** Return arrow offset */
	double getOffset();

	/** 
	 * Updates bounding sphere
	 *
	 * Upates the radius of the assigned bounding sphere, if any.
	 */
    virtual void doUpdateBoundingSphere() override;

protected:
	virtual void doCreateGeometry() override;
};

}