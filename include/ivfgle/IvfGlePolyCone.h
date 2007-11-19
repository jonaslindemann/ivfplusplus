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

#ifndef _CIvfGlePolyCone_h_
#define _CIvfGlePolyCone_h_

#include <ivf/IvfShape.h>

#include <ivfgle/IvfGleCoordArray.h>
#include <ivfgle/IvfGleColorArray.h>
#include <ivfgle/IvfGleScalarArray.h>
#include <ivfgle/IvfGleExtrudedBase.h>

IvfSmartPointer(CIvfGlePolyCone);

class IVFGLE_API CIvfGlePolyCone: public CIvfGleExtrudedBase {
private:
	CIvfGleScalarArrayPtr m_radius;
public:
	CIvfGlePolyCone();

	void setRadius(CIvfGleScalarArray* array);

	IvfClassInfo("CIvfGlePolyCone", CIvfShape);
protected:
	virtual void createGeometry();
};

#endif
