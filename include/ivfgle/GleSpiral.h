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

#include <GL/gle.h>

#include <ivfgle/GleColorArray.h>
#include <ivfgle/GleContour.h>
#include <ivfgle/GleCoordArray.h>

namespace ivf
{

IvfSmartPointer(GleSpiral);

class IVFGLE_API GleSpiral : public Shape
{
private:
    GleContourPtr m_contour;
    gleDouble m_up[3];
    gleDouble m_startXfm[2][3];
    gleDouble m_xfmTheta[2][3];

    gleDouble m_startRadius;
    gleDouble m_radiusChangePerRev;
    gleDouble m_startZ;
    gleDouble m_zChangePerRev;
    gleDouble m_startAngle;
    gleDouble m_totalSpiralAngle;
    int m_sides;

public:
    GleSpiral();

    IvfClassInfo("GleSpiral", Shape);
    IvfStdFactory(GleSpiral);

    void setContour(GleContour* contour);
    void setContourUp(double vx, double vy, double vz);
    void setContourUp(Vec3d& vec);

    void setStartRadius(double valkue);
    void setRadiusChangePerRev(double value);
    void setStartZ(double value);
    void setZChangePerRev(double value);
    void setStartAngle(double value);
    void setTotalSpiralAngle(double value);
    void setSides(int value);

    double startRadius() const;
    double radiusChangePerRev() const;
    double startZ() const;
    double zChangePerRev() const;
    double startAngle() const;
    double totalSpiralAngle() const;
    int sides() const;

protected:
    virtual void doCreateGeometry();
};

}
