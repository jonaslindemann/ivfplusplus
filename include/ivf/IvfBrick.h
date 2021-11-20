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

#ifndef _CIvfBrick_h_
#define _CIvfBrick_h_

#include <ivf/IvfQuadSet.h>

IvfSmartPointer(CBrick);

/**
 * Brick shape class
 *
 * CIvfBrick implements a shape consisting of 6 sides forming
 * a brick.
 * 
 * @author Jonas Lindemann
 */
class IVF_API CBrick : public CQuadSet {
public:
	/** CIvfBrick constructor */
	CBrick ();

	/** CIvfBrick destructor */
	virtual ~CBrick ();

	IvfClassInfo("CIvfBrick",CQuadSet);

	/**
	 * Set brick size (2 points)
	 *
	 * The brick size is specified by supplying to
	 * point references.
	 * @param p1 CIvfPoint instance specifying the first corner of the brick.
	 * @param p2 CIvfPoint instance specifying the opposite corner of the brick.
	 */
	void setSize(CPoint3d* p1, CPoint3d* p2);

	/**
	 * Set brick size
	 *
	 * The brick size is specified by supplying 3 dimensions.
	 * @param width Width of brick (X-dir)
	 * @param height Height of brick (Y-dir)
	 * @param depth Depth of brick (Z-dir)
	 */
	void setSize (const double width, const double height, const double depth);

	/** Return brick size */
	void getSize (double &width, double &height, double &depth);
private:
	void updateBrick();
protected:
	double	m_size[3];
	virtual void doCreateSelect();
	virtual void initBrick();
};
#endif
