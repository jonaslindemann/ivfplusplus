//
// Copyright 1999-2024 by Structural Mechanics, Lund University.
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

#include <ivfgle/GleSpiral.h>

using namespace ivf;

GleSpiral::GleSpiral()
    : Shape()
    , m_contour(nullptr)
    , m_up { 0.0, 0.0, 1.0 }
    , m_startXfm { 0.0 }
    , m_xfmTheta { 0.0 }
    , m_startAngle(0.0)
    , m_startRadius(1.0)
    , m_startZ(0.0)
    , m_radiusChangePerRev(-0.1)
    , m_zChangePerRev(1.0)
    , m_totalSpiralAngle(360.0 * 10)
    , m_sides(32)
{
    int i, j;

    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
        {
            m_startXfm[i][j] = 0.0;
            m_xfmTheta[i][j] = 0.0;
        }

    m_startXfm[0][0] = 1.0;
    m_startXfm[1][1] = 1.0;
}

void GleSpiral::doCreateGeometry()
{
    if (m_contour == nullptr)
        return;

    gleDouble(*temp2)[2];
    temp2 = (double(*)[2])m_contour->getCoordData();

    gleDouble(*temp3)[2];
    temp3 = (double(*)[2])m_contour->getNormalData();

    auto sides = gleGetNumSides();
    gleSetNumSides(m_sides);

    gleSpiral(
        m_contour->getSize(),
        temp2,
        temp3,
        m_up,
        m_startRadius, // Start radius
        m_radiusChangePerRev, // Radius change per rev.
        m_startZ, // Start z value
        m_zChangePerRev, // Change in z per rev
        m_startXfm,
        m_xfmTheta,
        m_startAngle, // Start angle in x-y plane
        m_totalSpiralAngle // Degrees to spiral around
    );

    gleSetNumSides(sides);
}

void GleSpiral::setContour(GleContour* contour)
{
    m_contour = contour;
}

void GleSpiral::setContourUp(double vx, double vy, double vz)
{
    m_up[0] = vx;
    m_up[1] = vy;
    m_up[2] = vz;
}

void GleSpiral::setContourUp(Vec3d& vec)
{
    double vx, vy, vz;
    vec.getComponents(vx, vy, vz);
    m_up[0] = vx;
    m_up[1] = vy;
    m_up[2] = vz;
}

void ivf::GleSpiral::setStartRadius(double valkue)
{
    m_startRadius = valkue;
}

void ivf::GleSpiral::setRadiusChangePerRev(double value)
{
    m_radiusChangePerRev = value;
}

void ivf::GleSpiral::setStartZ(double value)
{
    m_startZ = value;
}

void ivf::GleSpiral::setZChangePerRev(double value)
{
    m_zChangePerRev = value;
}

void ivf::GleSpiral::setStartAngle(double value)
{
    m_startAngle = value;
}

void ivf::GleSpiral::setTotalSpiralAngle(double value)
{
    m_totalSpiralAngle = value;
}

void ivf::GleSpiral::setSides(int value)
{
    m_sides = value;
}

double ivf::GleSpiral::startRadius() const
{
    return m_startRadius;
}

double ivf::GleSpiral::radiusChangePerRev() const
{
    return m_radiusChangePerRev;
}

double ivf::GleSpiral::startZ() const
{
    return m_startZ;
}

double ivf::GleSpiral::zChangePerRev() const
{
    return m_zChangePerRev;
}

double ivf::GleSpiral::startAngle() const
{
    return m_startAngle;
}

double ivf::GleSpiral::totalSpiralAngle() const
{
    return m_totalSpiralAngle;
}

int ivf::GleSpiral::sides() const
{
    return m_sides;
}
