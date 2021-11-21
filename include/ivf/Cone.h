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
#include <ivf/SelectionBox.h>

namespace ivf {

IvfSmartPointer(CCone);

/**
 * Cone shape class 
 *
 * CIvfCone implements a class for representing cone
 * shaped objects. CIvfCone has a top radius and a 
 * bottom radius. It is also possible to specify how
 * many slices and stacks the cone should consist of.
 * @author Jonas Lindemann
 */
class IVF_API CCone : public CShape {
public:
	/** CIvfCone constructor. */
	CCone ();

	/** CIvfCone destructor. */
	virtual ~CCone ();

	IvfClassInfo("CCone",CShape);
	IvfStdFactory(CCone);

	/** Set top radius */
	void setTopRadius (const double radius);

	/** Return top radius */
	double getTopRadius ();

	/** Set bottom radius */
	void setBottomRadius (const double radius);

	/** Return bottom radius */
	double getBottomRadius ();

	/** Set height */
	void setHeight (const double height);

	/** Return height */
	double getHeight ();

	/** 
	 * Set stacks
	 * 
	 * The stacks property determines how many slices the cone
	 * should have along its axis.
	 * @param stacks Number of stacks (default = 8).
	 */
	void setStacks(int stacks);

	/** Return stacks */
	int getStacks();

	/**
	 * Set slices 
	 *
	 * The slices property determines how many slices the cone
	 * should have around its axis.
	 * @param slices Number of slices (default = 8).
	 */
	void setSlices(int slices);

	/** Return slices */
	int getSlices();

	/** 
	 * Updates bounding sphere
	 *
	 * Upates the radius of the assigned bounding sphere, if any.
	 */
	void doUpdateBoundingSphere();
private:
	void updateSelectBox();
	CSelectionBox* m_selectionBox;
	int m_stacks;
	int m_slices;
	double	m_height;
	double	m_topRadius;
	double	m_bottomRadius;
protected:
	virtual void doCreateSelect();
	virtual void doCreateGeometry();
};

}