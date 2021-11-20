//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#ifndef _CIvfGleCoordArray_h_
#define _CIvfGleCoordArray_h_

#include <ivf/IvfBase.h>

#include <ivfmath/IvfVec3d.h>

#include <GL/gle.h>

IvfSmartPointer(CGleCoordArray);

class IVFGLE_API CGleCoordArray: public CBase {
private:
	int m_size;
	gleDouble (*m_coords)[3];
public:
	CGleCoordArray();
	CGleCoordArray(int size);
	virtual ~CGleCoordArray();

	IvfClassInfo("CGleCoordArray", CBase);

	void calcFirstAndLast();

	void setSize(int size);
	int getSize();

	void setCoord(int idx, double x, double y, double z);
	void getCoord(int idx, double& x, double& y, double& z);

	void setCoord(int idx, CVec3d& vec);
	CVec3d& getCoord(int idx);

	void* getData();

};

#endif
