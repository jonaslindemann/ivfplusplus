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

#ifndef _CIvfCoordList_h_
#define _CIvfCoordList_h_

#include <ivf/IvfBase.h>
#include <ivfmath/IvfVec3d.h>

#include <ivf/ivfconfig.h>

IvfSmartPointer(CCoordList);

class CCoordList : public CBase {
private:

public:
	CCoordList();
	virtual ~CCoordList();

	IvfClassInfo("CCoordList",CBase);

	virtual void readFromStream(std::istream &in);
	virtual void saveToStream(std::ostream &out);

	virtual void addCoord(double x, double y, double z);
	virtual void addCoord(double x, double y, double z, float r, float g, float b);
	virtual void addCoord(CVec3d* coord);

	virtual void setColor(long idx, float r, float g, float b);
	virtual void getColor(long idx, float &r, float &g, float &b);
	
	virtual void setCoord(int idx, double x, double y, double z);
	virtual void getCoord(long idx, double &x, double &y, double &z);
	virtual CVec3d* getCoord(long idx);

	virtual long getSize();
	virtual void clear();
};

#endif 
