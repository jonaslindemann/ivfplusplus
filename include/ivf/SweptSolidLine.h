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

#include <ivf/LineRefresh.h>
#include <ivf/Node.h>
#include <ivf/SweptExtrusion.h>

namespace ivf {

IvfSmartPointer(SweptSolidLine);

/**
 * Solid line class without the gle library
 *
 * SweptSolidLine draws a solid line between two Node instances, with a
 * configurable radius and number of sides. It is the SolidLine class built on
 * SweptExtrusion, so it renders from a vertex buffer and works in an OpenGL
 * compatibility profile with or without a shader.
 *
 * The public interface matches SolidLine, so the two are interchangeable at
 * the call site. Note that the class name reported by isClass() differs --
 * code testing for "SolidLine" needs to test for "SweptSolidLine" as well.
 *
 * @author Jonas Lindemann
 */
class IVF_API SweptSolidLine : public SweptExtrusion {
private:
    double m_radius;
    long m_nSides;
    double m_offsets[2];
    NodePtr m_node1;
    NodePtr m_node2;
    LineRefreshMode m_refreshMode;
    bool m_offsetConeCap;
    double m_offsetConeRadius;
    double m_startOffset[2];

    void initNodes();
    void initSection();

public:
    /** SweptSolidLine constructor. */
    SweptSolidLine();

    /** SweptSolidLine destructor. */
    virtual ~SweptSolidLine();

    IvfClassInfo("SweptSolidLine", SweptExtrusion);
    IvfStdFactory(SweptSolidLine);

    /**
     * Refreshes object data
     *
     * Recomputes the section and the spine according to the refresh mode.
     */
    virtual void refresh() override;

    /**
     * Set section sides
     *
     * The section is a circular polyline with the given radius and number of
     * sides.
     *
     * @param n Number of sides in the polygon.
     */
    void setSides(long n);

    /** Returns the number of section sides. */
    long getSides();

    /** Sets the section radius. */
    void setRadius(double radius);

    /** Returns the section radius. */
    double getRadius();

    /**
     * Set nodes
     *
     * The solid line is drawn between two Node instances. The reference count
     * of each node is increased while it is assigned to the line.
     */
    void setNodes(Node *n1, Node *n2);

    /**
     * Returns a node
     *
     * @param idx Index of the node to retrieve (0, 1)
     */
    Node *getNode(int idx);

    /**
     * Set refresh mode
     *
     * rmAll     - refresh the section and the spine
     * rmNodes   - refresh only the spine
     * rmSection - refresh only the section
     */
    void setRefresh(LineRefreshMode mode);

    /**
     * Enables conical end caps
     *
     * Adds a station at each end whose section is collapsed to a point, which
     * closes the line off with a cone rather than an open tube.
     */
    void setUseOffsetConeCap(bool flag);

    /** Sets the radius of the section at the base of the conical end caps. */
    void setOffsetConeCapRadius(double r);

    /** Returns the distance between the two nodes. */
    double getLength();

    /** Shortens the line by the given amount at each end. */
    void setOffsets(double offset1, double offset2);

    /** Sets where the conical end caps start, measured from each node. */
    void setStartOffsets(double offset1, double offset2);

protected:
    /** Recomputes the section and the spine. */
    virtual void initExtrusion();

    /** Called after the nodes have been assigned. */
    virtual void onSetNodes(Node *n1, Node *n2);
};

} // namespace ivf
