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

#ifndef _CIvfVectorCoordList_h_
#define _CIvfVectorCoordList_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfGLPrimitive.h>

#include <ivfmath/IvfVec3d.h>

#include "IvfCoordList.h"

IvfSmartPointer(CIvfVectorCoordList);

class CIvfVectorCoordList : public CIvfCoordList {
private:
	std::vector<CIvfVec3dPtr> m_coords;
	std::vector<CIvfColorPtr> m_colors;
public:
	CIvfVectorCoordList();
	virtual ~CIvfVectorCoordList();

	IvfClassInfo("CIvfVectorCoordList",CIvfCoordList);
	
	void readFromStream(std::istream &in);
	void saveToStream(std::ostream &out);

	void addCoord(double x, double y, double z);
	void addCoord(double x, double y, double z, float r, float g, float b);
	void addCoord(CIvfVec3d* coord);

	void setColor(long idx, float r, float g, float b);
	void getColor(long idx, float &r, float &g, float &b);
	
	void getCoord(long idx, double &x, double &y, double &z);
	CIvfVec3d* getCoord(long idx);

	long getSize();
	void clear();
};

#endif 
