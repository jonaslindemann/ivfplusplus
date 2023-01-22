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

#include <ivf/Extrusion.h>
#include <ivf/Node.h>

namespace ivf {

IvfSmartPointer(SolidLine);

enum LineRefreshMode
{
	rmAll,
	rmNodes,
	rmSection
};


/**
 * Solid line class
 *
 * SolidLine implements a solid line between
 * two Node instances. The radius and number
 * of sides can be defined.
 * @author Jonas Lindemann
 */
class IVF_API SolidLine : public Extrusion {
public:
	/** SolidLine constructor */
	SolidLine ();

	/** SolidLine destructor */
	virtual ~SolidLine ();

	IvfClassInfo("SolidLine",Extrusion);
	IvfStdFactory(SolidLine);

	/** 
	 * Refreshes object data
	 * 
	 * To avoid calculations when drawing the object,
	 * data for the extrusion is updated by calling
	 * this method.
	 */
	virtual void refresh() override;

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
	 * The solid line is constructed between two Node instances.
	 * The reference count of the node is increased when assigned to 
	 * the solid line. If the node are not referenced they are deleted
	 * when the solid line is destroyed.
	 */
	void setNodes(Node* n1, Node* n2);

	/** 
	 * Returns a node
	 *
	 * @param idx Idx of node to be retrieved. (0, 1)
	 */
	Node* getNode(int idx);

	/**
	 * Set refresh mode
	 *
	 * IVF_REFRESH_ALL     - Refresh all 
     * IVF_REFRESH_NODES   - Only nodes
	 * IVF_REFRESH_SECTION - Only section
	 */
    void setRefresh(LineRefreshMode mode);

	void setUseOffsetConeCap(bool flag);
    void setOffsetConeCapRadius(double r);

	double getRadius();
	double getLength();
protected:
	virtual void initExtrusion();
	virtual void onSetNodes(Node* n1, Node* n2);
private:
	void initNodes();
	void initSection();
	double m_radius;
	long m_nSides;
	double m_offsets[2];
	NodePtr m_node2;
	NodePtr m_node1;
    LineRefreshMode m_refreshMode;
    bool m_offsetConeCap;
    double m_offsetConeRadius;
    double m_startOffset[2];

public:
	void setOffsets(double offset1, double offset2);
    void setStartOffsets(double offset1, double offset2);
};

}