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

#include <ivfgle/GleSpiralCylinder.h>

using namespace ivf;

GleSpiralCylinder::GleSpiralCylinder()
    : GleSpiral()
    , m_sides(16)
    , m_contour(nullptr)
    , m_radius(0.1f)
{
    m_contour = GleContour::create(m_sides + 1);
    initContour();
    this->setContour(m_contour);
}

void ivf::GleSpiralCylinder::initContour()
{
    for (int i = 0; i <= m_sides; i++)
    {
        float angle = 2.0f * M_PI * i / m_sides;
        float x = m_radius * cos(angle);
        float y = m_radius * sin(angle);
        m_contour->setCoord(i, x, y);
    }
    m_contour->setClosed(true);
    m_contour->calcNormals();
}

void ivf::GleSpiralCylinder::setSides(int sides)
{
    m_sides = sides;
    m_contour = GleContour::create(m_sides + 1);
    initContour();
    this->setContour(m_contour);
}

int ivf::GleSpiralCylinder::sides() const
{
    return m_sides;
}

void ivf::GleSpiralCylinder::setRadius(double radius)
{
    m_radius = radius;
    initContour();
    this->setContour(m_contour);
}

double ivf::GleSpiralCylinder::radius() const
{
    return m_radius;
}
