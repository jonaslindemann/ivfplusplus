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

#ifndef _CIvfPointSet_h_
#define _CIvfPointSet_h_

#include <ivf/IvfGLPrimitive.h>

IvfSmartPointer(CIvfPointSet);

/**
 * Point set class
 *
 * Implements a OpenGL GL_POINTS primitive
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfPointSet : public CIvfGLPrimitive {
private:
	bool m_useColor;
	int m_pointSize;
public:
	/** CIvfPointSet constructor */
	CIvfPointSet();

	/** CIvfPointSet destructor */
	virtual ~CIvfPointSet();

	IvfClassInfo("CIvfPointSet",CIvfGLPrimitive);

	/** Enable or disable the use of color values when rendering */
	void setUseColor(bool flag);

	/** Return color usage */
	bool getUseColor();

	/** Set the size of drawn points in pixels */
	void setPointSize(int size);

	/** Get the size of drawn points in pixels */
	int getPointSize();


protected:
	void createGeometry();
};
/** \example advgeom.cpp */
#endif 
