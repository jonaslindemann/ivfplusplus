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

#ifndef _CIvfGleLine_h_
#define _CIvfGleLine_h_

#include <ivfmath/IvfVec3d.h>

#include <ivf/IvfShape.h>

#include <ivfgle/IvfGlePolyCone.h>
#include <ivfgle/IvfGlePolyCylinder.h>
#include <ivfgle/IvfGleTwistExtrusion.h>
#include <ivfgle/IvfGleExtrusion.h>

IvfSmartPointer(CIvfGleLine);

class IVFGLE_API CIvfGleLine: public CIvfShape {
public:
	enum TLineRepresentation {
		LR_EXTRUSION,
		LR_POLY_CYLINDER,
		LR_POLY_CONE,
		LR_WIRE
	};
private:
	CIvfVec3d m_startPoint;
	CIvfVec3d m_endPoint;
	TLineRepresentation m_lineRep;
	void updateLine();
public:
	CIvfGleLine();
	virtual ~CIvfGleLine();

	void setLineRepresentation(TLineRepresentation lineRep);
	void setStartPoint(double x, double y, double z);
	void setEndPoint(double x, double y, double z);
	void setSections(int sections);

	IvfClassInfo("CIvfGleLine", CIvfShape);
};

#endif
