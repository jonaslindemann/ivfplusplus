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

#ifndef _CIvfSimpleLineSet_h_
#define _CIvfSimpleLineSet_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(CIvfSimpleLineSet);

/**
 * Line set shape class (obsolete)
 * 
 * CIvfSimpleLineSet implements a OpenGL line strip.
 * @author Jonas Lindemann
 */
class IVF_API CIvfSimpleLineSet : public CIvfShape {
public:
	/** CIvfSimpleLineSet constructor */
	CIvfSimpleLineSet ();

	/** CIvfSimpleLineSet destructor */
	virtual ~CIvfSimpleLineSet ();

	IvfClassInfo("CIvfSimpleLineSet",CIvfShape);

	/** Set size of color list */
	void setColorSize(int n);

	/**
	 * Set color
	 *
	 * colors can be specified at each coordinate.
	 * @param n Color index
	 * @param red Red color component
	 * @param green Green color component
	 * @param blue Blue color component
	 */
	void setColor(int n, double red, double green, double blue);

	/** Set size of coordinate list */
	void setCoordSize(int n);

	/**
	 * Set coordinate
	 *
	 * Set a coordinate in coordinate list
	 * @param n Coordinate index (0 to getCoordSize()-1)
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	void setCoord(int n, double x, double y, double z);
	
	/** Set size of line index list */
	void setLineSize(int n);

	/**
	 * Set line index
	 *
	 * Sets an index in the line index list. Individual
	 * lines are separated by placing -1 in the index list.
	 * The index list must end with -1.
	 * @param n Index position
	 * @param idx index of coordinate
	 */
	void setLineIndex(int n, int idx);
private:
	int m_nColors;
	double (*m_coords)[3];	
	int *m_coordIndex;
	double (*m_colors)[3];
	int m_nCoords;
	int m_nCoordIndex;
protected:
	virtual void createGeometry();
};
#endif
