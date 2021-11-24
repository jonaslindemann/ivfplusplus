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

#include <ivf/Composite.h>
#include <ivf/Arrow.h>	

namespace ivf {

IvfSmartPointer(Axis);

#define IVF_AXIS_STD    0
#define IVF_AXIS_FLIPX  1

/**
 * Axis shape class
 *
 * CIvfAxis implements a simple axis shape that can
 * be used to display the X,Y and Z axis for a coordinate
 * system. The axis shape is the size of the unit cube.
 * To change the size of the axis shape use the setScale
 * method.
 *
 * @author Jonas Lindemann
 */
class IVF_API Axis : public Composite {
public:
	/** CIvfAxis constructor. */
	Axis ();
	
	/** CIvfAxis destructor. */
	virtual ~Axis ();

	IvfClassInfo("Axis",Composite);
	IvfStdFactory(Axis);

	/** Set axis type */
	void setAxisType(int type);

	/** Set size of axis */
	void setSize(double size);

	/** Set or change material used for arrows */
	void setArrowMaterial(Material* material);
private:
	double m_size;
	int m_axisType;
	ArrowPtr m_zArrow;
	ArrowPtr m_yArrow;
	ArrowPtr m_xArrow;
	MaterialPtr m_zMaterial;
	MaterialPtr m_yMaterial;
	MaterialPtr m_xMaterial;
};

}

