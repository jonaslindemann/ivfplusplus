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

namespace ivf {

IvfSmartPointer(GleColorArray);

class IVFGLE_API GleColorArray: public Base {
private:
	int m_size;
	float (*m_colors)[3];
public:
	GleColorArray();
	GleColorArray(int size);
	virtual ~GleColorArray();

	void setSize(int size);
	int getSize();

	void setColor(int idx, float r, float g, float b);
	void getColor(int idx, float& r, float& g, float& b);

	void* getData();

	IvfClassInfo("GleColorArray", Base);

	static GleColorArrayPtr create(int size) { return GleColorArrayPtr(new GleColorArray(size)); };
	static GleColorArrayPtr create() { return GleColorArrayPtr(new GleColorArray()); };
};

}