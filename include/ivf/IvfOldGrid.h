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

#ifndef _CIvfOldGrid_h_
#define _CIvfOldGrid_h_

#include <ivf/IvfSimpleLineSet.h>

IvfSmartPointer(COldGrid);

/**
 * Grid shape class
 *
 * CIvfGrid implements a grid with specified
 * units, size and subdivision. CIvfGrid is
 * derived from a CLineSet. (Soon obsolete)
 * @author Jonas Lindemann
 */
class IVF_API COldGrid : public CSimpleLineSet {
public:
	/** CIvfGrid constructor. */
	COldGrid ();

	/** CIvfGrid destructor. */
	virtual ~COldGrid ();

	IvfClassInfo("COldGrid",CSimpleLineSet);

	/**
	 * Set subdivision intervals
	 *
	 * The subdivision properties determines at what
	 * interval a subdivision line shoud be drawn.
	 * @param x Mark every x line
	 * @param y Mark every y line
	 */
	void setMark(int x, int y);

	/** 
	 * Set grid units
	 *
	 * @param x Grid spacing in x-direction.
	 * @param y Grid spacing in y-direction.
	 */
	void setUnits(GLdouble x, GLdouble y);

	/**
	 * Set grid size
	 *
	 * Determines how many rows and columns of
	 * the grid that should be drawn.
	 * @param rows Rows in the z-direction
	 * @param cols Columns in the x-direction.
	 */
	void setSize(GLint rows, GLint cols);

	/**
	 * Set axis color
	 *
	 * @param red Red color component
	 * @param green Green color component
	 * @param blue blue color component
	 */
	void setAxisColor(float red, float green, float blue);

	/**
	 * Set grid color
	 *
	 * @param red Red color component
	 * @param green Green color component
	 * @param blue blue color component
	 */
	void setGridColor(float red, float green, float blue);

	/**
	 * Set mark color
	 *
	 * @param red Red color component
	 * @param green Green color component
	 * @param blue blue color component
	 */
	void setMarkColor(float red, float green, float blue);

	/** Updates bounding sphere for grid */
	virtual void doUpdateBoundingSphere();
private:
	bool m_onlySubdivision;
	GLdouble m_yCenter;
	GLdouble m_xCenter;
	GLint m_cols;
	GLint m_rows;
	GLdouble m_yUnit;
	GLdouble m_xUnit;
	GLint m_xMark;
	GLint m_yMark;
	GLfloat m_markColor[3];
	GLfloat m_gridColor[3];
	GLfloat m_axisColor[3];
protected:
	void createGrid();
};
#endif
