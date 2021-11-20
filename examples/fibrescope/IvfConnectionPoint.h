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

#ifndef _CConnectionPoint_h_
#define _CConnectionPoint_h_

#include <ivf/ivfconfig.h>

namespace std {};
using namespace std;

#include <ivf/IvfShape.h>
#include <ivf/IvfCamera.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfSphere.h>

#include "IvfCoordHist.h"
#include "IvfDiscreteTime.h"
#include "IvfUserSettings.h"

IvfSmartPointer(CConnectionPoint);

class CConnectionPoint : public CShape {
private:
	CCoordHistPtr m_coords;
	CDiscreteTimePtr m_discreteTime;
	CCameraPtr m_camera;
	bool m_useColor;
	double m_connectionRadius;

	static GLUquadricObj* m_qobj;
	static CMaterial* m_material;
public:
	CConnectionPoint();
	virtual ~CConnectionPoint();

	void setCamera(CCamera* camera);
	void setRadius(double radius);
	void setUseColor(bool flag);
	void setDiscreteTime(CDiscreteTime* discreteTime);

	void colorScale(double t, float &r, float &g, float &b);

	CCoordHist* getCoordHist();

	void readFromStream(istream &in);
	void saveToStream(ostream &out);

protected:
	void doCreateGeometry();
};

#endif
