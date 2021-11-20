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

#pragma once

#include <ivf/Composite.h>
#include <ivf/Cylinder.h>
#include <ivf/Cone.h>

#include <ivfmath/Point3d.h>
#include <ivfmath/Vec3d.h>

namespace ivf {
	
IvfSmartPointer(CArrow);

#define IVF_ARROW_STANDARD	0
#define IVF_ARROW_DOUBLE	1

/**
 * Arrow shape class
 *
 * CIvfArrow implements a simple arrow shape consisting
 * of a cone placed on top of a cylinder.
 * @author Jonas Lindemann
 */
class IVF_API CArrow : public CComposite {
public:
	/** CIvfArrow constructor */
	CArrow ();
	
	/** CIvfArrow destructor */
	virtual ~CArrow ();

	IvfClassInfo("CIvfArrow",CComposite);
	
	/**
	 * Set arrow type
	 *
	 * Two arrow type can be set. One standard arrow type with
	 * one head and a arrow with a double head.
	 * @param type IVF_ARROW_STANDARD standard arrow type (default).
	 * IVF_ARROW_DOUBLE arrow with two heads.
	 */
	void setArrowType(int type);
	
	/** Returns arrow type */
	int getArrowType();
	
	/** 
	 * Set sizes of arrow components.
	 *
	 * The arrow size is specified by a total length and and a
	 * the length of the head. The length of the cylinder is
	 * length - head.
	 * @param length Total length of arrow.
	 * @prarm head Size of arrow head.
	 */
	void setSize(double length, double head);

	/** Returns sizes of arrow components. */
	void getSize(double &length, double &head);

	/** 
	 * Set radius of arrow components.
	 *
	 * Two radius values are given to specify the thickness of
	 * the arrow. 
	 * @param head Bottom radius of the cone.
	 * @param tail Radius of cylinder.
	 */
	void setRadius(double head, double tail);

	/** 
	 * Set offset of arrow center.
	 *
	 * Default the arrow has its origin in the middle of the 
	 * arrow. By specifying an offset the origion can be moved 
	 * along the axis of the arrow.
	 * @param offset Origion offset value.
	 */
	void setOffset(double offset);

	/** Returns the offset value. */
	double getOffset();

	/** Returns the total length of the arrow. (calculated) */
	double getLength();
	
	/**
	 * Align arrow to a vector.
	 *
	 * The arrow can be aligned to a specific vector by 
	 * providing a reference to a CIvfVect3d instance.
	 * @param ivfVector Reference to a CVec3d instance to which
	 * the arrow is aligned.
	 */
	void alignVector(CVec3d *vec);
private:
	double m_offset;
	int m_arrowType;
	CCylinderPtr m_tail;
	CConePtr m_head;
	CConePtr m_extraHead;
	double m_tailRadius;
	double m_headRadius;
	double m_headSize;
	double m_length;
};

}