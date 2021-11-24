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

#include <ivfmath/Vec3d.h>

#include <ivf/Shape.h>

#include <ivfgle/GlePolyCone.h>
#include <ivfgle/GlePolyCylinder.h>
#include <ivfgle/GleTwistExtrusion.h>
#include <ivfgle/GleExtrusion.h>

namespace ivf {

IvfSmartPointer(GleLine);

class IVFGLE_API GleLine: public Shape {
public:
	enum TLineRepresentation {
		LR_EXTRUSION,
		LR_POLY_CYLINDER,
		LR_POLY_CONE,
		LR_WIRE
	};
private:
	Vec3d m_startPoint;
	Vec3d m_endPoint;
	TLineRepresentation m_lineRep;
	void updateLine();
public:
	GleLine();
	virtual ~GleLine();

	void setLineRepresentation(TLineRepresentation lineRep);
	void setStartPoint(double x, double y, double z);
	void setEndPoint(double x, double y, double z);
	void setSections(int sections);

	IvfClassInfo("GleLine", Shape);
	IvfStdFactory(GleLine);
};

}