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

#ifndef _CIvfGleExtrusion_h_
#define _CIvfGleExtrusion_h_

#include <ivf/IvfShape.h>

#include <GL/gle.h>

#include <ivfgle/IvfGleExtrudedBase.h>
#include <ivfgle/IvfGleCoordArray.h>
#include <ivfgle/IvfGleColorArray.h>
#include <ivfgle/IvfGleContour.h>

IvfSmartPointer(CIvfGleExtrusion);

class IVFGLE_API CIvfGleExtrusion: public CIvfGleExtrudedBase {
private:
	CIvfGleContourPtr	 m_contour;
	gleDouble			 m_up[3];
public:
	CIvfGleExtrusion();

	IvfClassInfo("CIvfGleExtrusion", CIvfShape);

	void setContour(CIvfGleContour* contour);
	void setContourUp(double vx, double vy, double vz);
	void setContourUp(CIvfVec3d& vec);
protected:
	virtual void doCreateGeometry();
};

#endif
