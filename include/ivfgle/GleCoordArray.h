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

#include <ivfmath/Vec3d.h>

#include <GL/gle.h>

namespace ivf {

IvfSmartPointer(GleCoordArray);

class IVFGLE_API GleCoordArray: public Base {
private:
	int m_size;
	gleDouble (*m_coords)[3];
public:
	GleCoordArray();
	GleCoordArray(int size);
	virtual ~GleCoordArray();

	IvfClassInfo("GleCoordArray", Base);

	static GleCoordArrayPtr create(int size) { return GleCoordArrayPtr(new GleCoordArray(size)); };
	static GleCoordArrayPtr create() { return GleCoordArrayPtr(new GleCoordArray()); };

	void calcFirstAndLast();

	void setSize(int size);
	int getSize();

	void setCoord(int idx, double x, double y, double z);
	void getCoord(int idx, double& x, double& y, double& z);

	void setCoord(int idx, Vec3d& vec);
	Vec3d& getCoord(int idx);

	void* getData();

};

}