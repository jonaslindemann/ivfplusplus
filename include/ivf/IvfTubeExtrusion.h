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

#ifndef IVFTUBEEXTRUSION_H
#define IVFTUBEEXTRUSION_H

#include <ivf/IvfExtrusion.h>

IvfSmartPointer(CIvfTubeExtrusion);

/**
 * Tube extrusion primitive
 *
 * Sweeps a circular section along a spine.
 * @author Jonas Lindemann
 */
class IVF_API CIvfTubeExtrusion : public CIvfExtrusion {
public:
	/** Constructor */
	CIvfTubeExtrusion ();

	/** Destructor */
	virtual ~CIvfTubeExtrusion ();

	IvfClassInfo("CIvfTubeExtrusion",CIvfExtrusion);

	/**
	 * Set section sides
	 *
	 * The section of the solid line is implemented as a 
	 * circular polyline with specified radius and number of sides
	 * \param n Number of sides in polygon.
	 */
	void setSides(long n);

	/** Set polygon radius */
	void setRadius(double radius);

	/** Refresh object representation */
	virtual void refresh();

	/** Not supported */
	void setCalcFirstLast(int flag);
protected:
	void initExtrusion();
private:
	long m_nSides;
	double m_radius;
	int m_calcFirstLast;
};
#endif
