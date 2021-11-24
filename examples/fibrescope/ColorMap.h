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

#ifndef _CIvfColorMap_h_
#define _CIvfColorMap_h_

#include <ivf/config.h>

#include <ivf/Base.h>
#include <ivf/GLPrimitive.h>
#include <ivf/Color.h>

IvfSmartPointer(ColorMap);

class ColorMap : public ivf::Base {
private:
	std::vector<ivf::ColorPtr> m_colors;
public:
	ColorMap();
	virtual ~ColorMap();

	IvfStdFactory(ColorMap);

	void open(const std::string& fname);
	void clear();

	void getColor(double value, float &r, float &g, float &b);
};

#endif
