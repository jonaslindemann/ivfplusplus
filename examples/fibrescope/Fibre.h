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

#ifndef _CFibre_h_
#define _CFibre_h_

#include <ivf/Shape.h>
#include <ivf/Camera.h>

#include "CoordHist.h"
#include "DiscreteTime.h"
#include "UserSettings.h"
#include "VectorCoordList.h"
#include "ArrayCoordList.h"

IvfSmartPointer(CFibre);

class CFibre : public ivf::CShape {
private:
	CCoordHistPtr m_coords;
	CDiscreteTimePtr m_discreteTime;
	ivf::CCameraPtr m_camera;

	double (*m_sectionCoords)[2];
	double (*m_sectionNormals)[2];

	double m_fibreRadius;

	void initSection();
public:
	CFibre();
	virtual ~CFibre();

	void refresh();

	CCoordHist* getCoordHist();
	void setDiscreteTime(CDiscreteTime* discreteTime);
	void setFibreRadius(double radius);

	void readFromStream(std::istream &in);
	void saveToStream(std::ostream &out);
protected:
	void doCreateGeometry();
};

#endif
