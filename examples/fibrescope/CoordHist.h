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

#ifndef _CIvfCoordHist_h_
#define _CIvfCoordHist_h_

#include <ivf/config.h>

#include <ivf/Base.h>

#include "CoordList.h"
#include "DiscreteTime.h"

IvfSmartPointer(CoordHist);

class CoordHist : public ivf::Base {
private:
	std::vector<CoordListPtr> m_lists;
	DiscreteTimePtr m_discreteTime;
public:
	CoordHist();
	virtual ~CoordHist();

	IvfStdFactory(CoordHist);

	void readFromStream(std::istream &in);
	void saveToStream(std::ostream &out);

	void addList(CoordList* list);
	CoordList* getList(long idx);
	CoordList* getList();
	long getSize();
	void clear();

	void setDiscreteTime(DiscreteTime* discreteTime);
};

#endif 
