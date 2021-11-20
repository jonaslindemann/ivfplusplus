//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#ifndef _CVectorCoordList_h_
#define _CVectorCoordList_h_

#include <ivf/Base.h>
#include <ivf/GLPrimitive.h>

#include <ivfmath/Vec3d.h>

#include "CoordList.h"

IvfSmartPointer(CVectorCoordList);

class CVectorCoordList : public CCoordList {
private:
	std::vector<ivf::CVec3dPtr> m_coords;
	std::vector<ivf::CColorPtr> m_colors;
public:
	CVectorCoordList();
	virtual ~CVectorCoordList();

	IvfClassInfo("CVectorCoordList",CCoordList);
	
	void readFromStream(std::istream &in);
	void saveToStream(std::ostream &out);

	void addCoord(double x, double y, double z);
	void addCoord(double x, double y, double z, float r, float g, float b);
	void addCoord(ivf::CVec3d* coord);

	void setColor(long idx, float r, float g, float b);
	void getColor(long idx, float &r, float &g, float &b);
	
	void getCoord(long idx, double &x, double &y, double &z);
	ivf::CVec3d* getCoord(long idx);

	long getSize();
	void clear();
};

#endif 
