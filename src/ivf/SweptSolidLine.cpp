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

// Implementation of: public class SweptSolidLine

#include <ivf/SweptSolidLine.h>

#include <ivf/config.h>
#include <ivfmath/Vec3d.h>

#include <cmath>

using namespace ivf;

namespace {

// Length of the lead-in and lead-out segments. The spine vertices at the very
// ends are there to orient the section and are not swept, so they have to sit
// clear of the drawn part of the line.

constexpr double kOrientationLead = 0.1;

} // namespace

// ------------------------------------------------------------
SweptSolidLine::SweptSolidLine()
    : SweptExtrusion(), m_radius(0.25), m_nSides(6), m_refreshMode(rmAll), m_offsetConeCap(false),
      m_offsetConeRadius(0.0)
{
    m_offsets[0] = 0.0;
    m_offsets[1] = 0.0;
    m_startOffset[0] = 0.0;
    m_startOffset[1] = 0.0;

    this->setSpineSize(4);
    this->setUseColor(false);
}

// ------------------------------------------------------------
SweptSolidLine::~SweptSolidLine()
{
}

// ------------------------------------------------------------
void SweptSolidLine::initExtrusion()
{
    initSection();
    initNodes();
}

// ------------------------------------------------------------
void SweptSolidLine::setRadius(double radius)
{
    m_radius = radius;
    initExtrusion();
}

// ------------------------------------------------------------
double SweptSolidLine::getRadius()
{
    return m_radius;
}

// ------------------------------------------------------------
void SweptSolidLine::setNodes(Node *n1, Node *n2)
{
    m_node1 = n1;
    m_node2 = n2;

    initExtrusion();
    doUpdateBoundingSphere();

    onSetNodes(n1, n2);
}

// ------------------------------------------------------------
Node *SweptSolidLine::getNode(int idx)
{
    switch (idx)
    {
    case 0:
        return m_node1;
    case 1:
        return m_node2;
    default:
        return nullptr;
    }
}

// ------------------------------------------------------------
void SweptSolidLine::setSides(long n)
{
    m_nSides = n;
    initExtrusion();
}

// ------------------------------------------------------------
long SweptSolidLine::getSides()
{
    return m_nSides;
}

// ------------------------------------------------------------
void SweptSolidLine::refresh()
{
    switch (m_refreshMode)
    {
    case rmNodes:
        initNodes();
        break;
    case rmSection:
        initSection();
        break;
    case rmAll:
    default:
        initExtrusion();
        break;
    }
}

// ------------------------------------------------------------
void SweptSolidLine::setRefresh(LineRefreshMode mode)
{
    m_refreshMode = mode;
}

// ------------------------------------------------------------
void SweptSolidLine::setUseOffsetConeCap(bool flag)
{
    m_offsetConeCap = flag;
    this->initNodes();
}

// ------------------------------------------------------------
void SweptSolidLine::setOffsetConeCapRadius(double r)
{
    m_offsetConeRadius = r;
    this->initNodes();
    this->setSpineScale(1, r, r);
    this->setSpineScale(4, r, r);
}

// ------------------------------------------------------------
void SweptSolidLine::initSection()
{
    // The section is a circle, given with its first point repeated at the end
    // so it reads as a closed ring.

    int nSides = static_cast<int>(m_nSides);

    if (nSides < 3)
        nSides = 3;

    double r = m_radius;

    setSectionSize(nSides + 1);

    for (int i = 0; i <= nSides; i++)
    {
        double angle = 2.0 * M_PI * (static_cast<double>(i) / static_cast<double>(nSides));
        double x = r * std::cos(angle);
        double y = r * std::sin(angle);

        setSectionCoord(i, x, y);

        if (r > 0.0)
            setSectionNormal(i, x / r, y / r);
    }
}

// ------------------------------------------------------------
void SweptSolidLine::initNodes()
{
    if ((m_node1 == nullptr) || (m_node2 == nullptr))
        return;

    Vec3d p1, p2, v1;
    double x, y, z, ex, ey, ez;

    m_node1->getPosition(x, y, z);
    p1.setComponents(x, y, z);
    m_node2->getPosition(x, y, z);
    p2.setComponents(x, y, z);

    v1.setFromPoints(p1, p2);
    v1.normalize();
    v1.getComponents(ex, ey, ez);

    // A vertical line leaves the default up vector parallel to the spine,
    // which carries no orientation. Pick another axis for that case.

    if ((ex == 0) && (ez == 0) && (ey != 0))
        setUpVector(1.0, 0.0, 0.0);
    else
        setUpVector(0.0, 1.0, 0.0);

    if (m_offsetConeCap)
    {
        this->setSpineSize(6);

        // o--o----o-------------o----o--o
        //    p1                      p2
        //    |    |             |    |
        //   startOffset        offsets
        //
        // Stations 0 and 5 orient the ends without being drawn. Stations 1
        // and 4 carry a section collapsed to a point, which closes each end
        // with a cone.

        p1.getComponents(x, y, z);
        setSpineCoord(0, -kOrientationLead * ex + x, -kOrientationLead * ey + y, -kOrientationLead * ez + z);

        p1.getComponents(x, y, z);
        setSpineCoord(1, -m_startOffset[0] * ex + x, -m_startOffset[0] * ey + y, -m_startOffset[0] * ez + z);
        setSpineScale(1, 0.0, 0.0);

        p1.getComponents(x, y, z);
        setSpineCoord(2, -m_offsets[0] * ex + x, -m_offsets[0] * ey + y, -m_offsets[0] * ez + z);
        setSpineScale(2, 1.0, 1.0);

        p2.getComponents(x, y, z);
        setSpineCoord(3, m_offsets[1] * ex + x, m_offsets[1] * ey + y, m_offsets[1] * ez + z);
        setSpineScale(3, 1.0, 1.0);

        p2.getComponents(x, y, z);
        setSpineCoord(4, m_startOffset[0] * ex + x, m_startOffset[0] * ey + y, m_startOffset[0] * ez + z);
        setSpineScale(4, 0.0, 0.0);

        p2.getComponents(x, y, z);
        setSpineCoord(5, kOrientationLead * ex + x, kOrientationLead * ey + y, kOrientationLead * ez + z);
    }
    else
    {
        this->setSpineSize(4);

        // Stations 0 and 3 orient the ends. The line itself runs between
        // stations 1 and 2.

        double lead0 = kOrientationLead + m_offsets[0];
        double lead1 = kOrientationLead + m_offsets[1];

        p1.getComponents(x, y, z);
        setSpineCoord(0, -lead0 * ex + x, -lead0 * ey + y, -lead0 * ez + z);

        p1.getComponents(x, y, z);
        setSpineCoord(1, -m_offsets[0] * ex + x, -m_offsets[0] * ey + y, -m_offsets[0] * ez + z);

        p2.getComponents(x, y, z);
        setSpineCoord(2, m_offsets[1] * ex + x, m_offsets[1] * ey + y, m_offsets[1] * ez + z);

        p2.getComponents(x, y, z);
        setSpineCoord(3, lead1 * ex + x, lead1 * ey + y, lead1 * ez + z);
    }
}

// ------------------------------------------------------------
double SweptSolidLine::getLength()
{
    if ((m_node1 == nullptr) || (m_node2 == nullptr))
        return 0.0;

    Vec3d p1 = m_node1->getPosition();
    Vec3d p2 = m_node2->getPosition();
    Vec3d v = p2 - p1;

    return v.length();
}

// ------------------------------------------------------------
void SweptSolidLine::onSetNodes(Node *n1, Node *n2)
{
}

// ------------------------------------------------------------
void SweptSolidLine::setOffsets(double offset1, double offset2)
{
    m_offsets[0] = offset1;
    m_offsets[1] = offset2;

    this->initExtrusion();
}

// ------------------------------------------------------------
void SweptSolidLine::setStartOffsets(double offset1, double offset2)
{
    m_startOffset[0] = offset1;
    m_startOffset[1] = offset2;

    // Unlike the other offset setters this one does not rebuild the spine, so
    // the change only lands on the next explicit refresh. Mark the geometry
    // stale anyway -- a stale mesh is a visible bug, an extra rebuild is not.

    markListDirty();
}
