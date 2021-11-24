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

IvfSmartPointer(Fibre);

class Fibre : public ivf::Shape {
private:
	CoordHistPtr m_coords;
	DiscreteTimePtr m_discreteTime;
	ivf::CameraPtr m_camera;

	double (*m_sectionCoords)[2];
	double (*m_sectionNormals)[2];

	double m_fibreRadius;

	void initSection();
public:
	Fibre();
	virtual ~Fibre();

	IvfStdFactory(Fibre);

	void refresh();

	CoordHist* getCoordHist();
	void setDiscreteTime(DiscreteTime* discreteTime);
	void setFibreRadius(double radius);

	void readFromStream(std::istream &in);
	void saveToStream(std::ostream &out);
protected:
	void doCreateGeometry();
};

#endif
