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

#include <ivf/Base.h>

#include <GL/gle.h>

namespace ivf
{

IvfSmartPointer(GleContour);

class IVFGLE_API GleContour : public Base
{
private:
    int m_size;
    gleDouble (*m_coords)[2];
    gleDouble (*m_normals)[2];
    bool m_closed;

public:
    GleContour();
    GleContour(int size);
    virtual ~GleContour();

    IvfClassInfo("GleContour", Base);

    static GleContourPtr create(int size) { return GleContourPtr(new GleContour(size)); };
    static GleContourPtr create() { return GleContourPtr(new GleContour()); };

    void setClosed(bool closed);
    bool closed();

    void calcNormals();

    void setSize(int size);
    int getSize();

    void setCoord(int idx, double x, double y);
    void getCoord(int idx, double& x, double& y);

    void* getCoordData();
    void* getNormalData();
};

}
