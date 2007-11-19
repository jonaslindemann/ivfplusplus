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

#ifndef _CIvfSolidLine_h_
#define _CIvfSolidLine_h_

#include <ivf/IvfExtrusion.h>
#include <ivf/IvfNode.h>

IvfSmartPointer(CIvfSolidLine);

#define IVF_LINE_SIMPLE 0
#define IVF_LINE_SOLID	1

#define IVF_REFRESH_ALL     0
#define IVF_REFRESH_NODES   1
#define IVF_REFRESH_SECTION 2

/**
 * Solid line class
 *
 * CIvfSolidLine implements a solid line between
 * two CIvfNode instances. The radius and number
 * of sides can be defined.
 * @author Jonas Lindemann
 */
class IVF_API CIvfSolidLine : public CIvfExtrusion {
public:
	/** CIvfSolidLine constructor */
	CIvfSolidLine ();

	/** CIvfSolidLine destructor */
	virtual ~CIvfSolidLine ();

	IvfClassInfo("CIvfSolidLine",CIvfExtrusion);

	/** 
	 * Refreshes object data
	 * 
	 * To avoid calculations when drawing the object,
	 * data for the extrusion is updated by calling
	 * this method.
	 */
	virtual void refresh();

	/**
	 * Set section sides
	 *
	 * The section of the solid line is implemented as a 
	 * circular polyline with specified radius and number of sides
	 * @param n Number of sides in polygon.
	 */
	void setSides(long n);

	/** Set polygon radius */
	void setRadius(double radius);

	/**
	 * Set nodes
	 *
	 * The solid line is constructed between two CIvfNode instances.
	 * The reference count of the node is increased when assigned to 
	 * the solid line. If the node are not referenced they are deleted
	 * when the solid line is destroyed.
	 */
	void setNodes(CIvfNode* n1, CIvfNode* n2);

	/** 
	 * Returns a node
	 *
	 * @param idx Idx of node to be retrieved. (0, 1)
	 */
	CIvfNode* getNode(int idx);

	/**
	 * Set refresh mode
	 *
	 * IVF_REFRESH_ALL     - Refresh all 
     * IVF_REFRESH_NODES   - Only nodes
	 * IVF_REFRESH_SECTION - Only section
	 */
	void setRefresh(int mode);

	double getRadius();
	double getLength();
protected:
	virtual void initExtrusion();
	virtual void onSetNodes(CIvfNode* n1, CIvfNode* n2);
private:
	void initNodes();
	void initSection();
	double m_radius;
	long m_nSides;
	double m_offsets[2];
	CIvfNodePtr m_node2;
	CIvfNodePtr m_node1;
	int m_refreshMode;
public:
	void setOffsets(double offset1, double offset2);
};
#endif
