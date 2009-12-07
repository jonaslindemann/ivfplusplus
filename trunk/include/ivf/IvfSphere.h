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

#ifndef _CIvfSphere_h_
#define _CIvfSphere_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfSelectionBox.h>

IvfSmartPointer(CIvfSphere);

/**
 * Sphere primitive
 * @author Jonas Lindemann
 */
class IVF_API CIvfSphere : public CIvfShape {
public:
	/** Constructor */
	CIvfSphere ();

	/** Destructor */
	virtual ~CIvfSphere ();

	IvfClassInfo("CIvfSphere",CIvfShape);

	/** Set sphere radius */
	void setRadius (double radius);

	/** Return sphere radius */
	double getRadius ();

	/** Set number of stacks in sphere representation */
	void setStacks(int stacks);

	/** Return number of stacks */
	int getStacks();

	/** Set number of slices in sphere representation */
	void setSlices(int slices);

	/** Return number of slices */
	int getSlices();

	/** Update bounding sphere size */
	void doUpdateBoundingSphere();

private:
	void updateSelectBox();
	CIvfSelectionBox* m_selectionBox;
	int m_stacks;
	int m_slices;
	double	m_radius;
	GLUquadricObj* m_qobj;
protected:
	virtual void doCreateSelect();
	virtual void doCreateGeometry();

};
#endif
