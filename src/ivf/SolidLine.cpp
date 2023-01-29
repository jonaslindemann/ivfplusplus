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

// Implementation of: public class SolidLine

#include <ivf/SolidLine.h>
#include <ivf/config.h>
#include <ivfmath/Vec3d.h>

using namespace ivf;

SolidLine::SolidLine()
    : Extrusion()
{
    m_radius = 0.25;
    m_nSides = 6;
    m_refreshMode = rmAll;
    m_offsets[0] = 0.0;
    m_offsets[1] = 0.0;
    m_offsetConeCap = false;
    m_offsetConeRadius = 0.0;
    m_startOffset[0] = 0.0;
    m_startOffset[1] = 0.0;

    this->setSpineSize(4);
    this->setUseColor(false);
}

SolidLine::~SolidLine()
{
}

void SolidLine::initExtrusion()
{
    initSection();
    initNodes();
}

void SolidLine::setRadius(double radius)
{
    m_radius = radius;
    initExtrusion();
}

void SolidLine::setNodes(Node* n1, Node* n2)
{
    // Set nodes

    m_node1 = n1;
    m_node2 = n2;

    // Update extrusion parameters

    initExtrusion();
    doUpdateBoundingSphere();

    onSetNodes(n1, n2);
}

Node* SolidLine::getNode(int idx)
{
    switch (idx)
    {
    case 0:
        return m_node1;
        break;
    case 1:
        return m_node2;
        break;
    default:
        return nullptr;
        break;
    }
}

void SolidLine::setSides(long n)
{
    m_nSides = n;
    initExtrusion();
}

long ivf::SolidLine::getSides()
{
    return m_nSides;
}

void SolidLine::refresh()
{
    switch (m_refreshMode)
    {
    case rmAll:
        initExtrusion();
        break;
    case rmNodes:
        initNodes();
        break;
    case rmSection:
        initSection();
        break;
    default:
        initExtrusion();
        break;
    }
}

void SolidLine::setRefresh(LineRefreshMode mode)
{
    m_refreshMode = mode;
}

void ivf::SolidLine::setUseOffsetConeCap(bool flag)
{
    m_offsetConeCap = flag;
    this->initNodes();
}

void ivf::SolidLine::setOffsetConeCapRadius(double r)
{
    m_offsetConeRadius = r;
    this->initNodes();
    this->setSpineScale(1, r, r);
    this->setSpineScale(4, r, r);
}

void SolidLine::initSection()
{
    int i, nSides;
    double r, angle, x, y;

    // Create section

    r = m_radius;

    nSides = m_nSides;

    setSectionSize(nSides + 1);

    for (i = 0; i <= nSides; i++)
    {
        angle = 2.0 * M_PI * (((double)i) / ((double)nSides));
        x = r * cos(angle);
        y = r * sin(angle);
        setSectionCoord(i, x, y);
        setSectionNormal(i, x / r, y / r);
    }
}

void SolidLine::initNodes()
{
    if ((m_node1 != nullptr) && (m_node2 != nullptr))
    {

        // Create spine

        Vec3d p1, p2;
        Vec3d v1;
        double x, y, z, ex, ey, ez;

        m_node1->getPosition(x, y, z);
        p1.setComponents(x, y, z);
        m_node2->getPosition(x, y, z);
        p2.setComponents(x, y, z);
        v1.setFromPoints(p1, p2);
        v1.normalize();
        v1.getComponents(ex, ey, ez);

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
            //   offset[1]          offset[2]

            // First point

            p1.getComponents(x, y, z);
            x = -0.1 * ex + x;
            y = -0.1 * ey + y;
            z = -0.1 * ez + z;
            setSpineCoord(0, x, y, z);

            p1.getComponents(x, y, z);
            x = -m_startOffset[0] * ex + x;
            y = -m_startOffset[0] * ey + y;
            z = -m_startOffset[0] * ez + z;
            setSpineCoord(1, x, y, z);
            setSpineScale(1, 0.0, 0.0);

            // Second point

            p1.getComponents(x, y, z);
            x = -m_offsets[0] * ex + x;
            y = -m_offsets[0] * ey + y;
            z = -m_offsets[0] * ez + z;
            setSpineCoord(2, x, y, z);
            setSpineScale(2, 1.0, 1.0);

            // Third point

            p2.getComponents(x, y, z);
            x = m_offsets[1] * ex + x;
            y = m_offsets[1] * ey + y;
            z = m_offsets[1] * ez + z;
            setSpineCoord(3, x, y, z);
            setSpineScale(3, 1.0, 1.0);

            // Fourth point

            p2.getComponents(x, y, z);
            x = m_startOffset[0] * ex + x;
            y = m_startOffset[0] * ey + y;
            z = m_startOffset[0] * ez + z;
            setSpineCoord(4, x, y, z);
            setSpineScale(4, 0.0, 0.0);

            p2.getComponents(x, y, z);
            x = 0.1 * ex + x;
            y = 0.1 * ey + y;
            z = 0.1 * ez + z;
            setSpineCoord(5, x, y, z);        

        }
        else
        {
            this->setSpineSize(4);

            // First point

            p1.getComponents(x, y, z);
            x = (-0.1 - m_offsets[0]) * ex + x;
            y = (-0.1 - m_offsets[0]) * ey + y;
            z = (-0.1 - m_offsets[0]) * ez + z;
            setSpineCoord(0, x, y, z);

            // Second point

            p1.getComponents(x, y, z);
            x = -m_offsets[0] * ex + x;
            y = -m_offsets[0] * ey + y;
            z = -m_offsets[0] * ez + z;
            setSpineCoord(1, x, y, z);

            // Third point

            p2.getComponents(x, y, z);
            x = m_offsets[1] * ex + x;
            y = m_offsets[1] * ey + y;
            z = m_offsets[1] * ez + z;
            setSpineCoord(2, x, y, z);

            // Fourth point

            p2.getComponents(x, y, z);
            x = (0.1 + m_offsets[1]) * ex + x;
            y = (0.1 + m_offsets[1]) * ey + y;
            z = (0.1 + m_offsets[1]) * ez + z;
            setSpineCoord(3, x, y, z);        
        }
    }
}

double SolidLine::getLength()
{
    if ((m_node1 != nullptr) && (m_node2 != nullptr))
    {
        Vec3d p1 = m_node1->getPosition();
        Vec3d p2 = m_node2->getPosition();
        Vec3d v = p2 - p1;
        return v.length();
    }
    else
        return 0.0;
}

double SolidLine::getRadius()
{
    return m_radius;
}

void SolidLine::onSetNodes(Node* n1, Node* n2)
{
}

void SolidLine::setOffsets(double offset1, double offset2)
{
    m_offsets[0] = offset1;
    m_offsets[1] = offset2;
    this->initExtrusion();
}

void ivf::SolidLine::setStartOffsets(double offset1, double offset2)
{
    m_startOffset[0] = offset1;
    m_startOffset[1] = offset2;
}
