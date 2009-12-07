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

#ifndef _CIvfLineStripSet_h_
#define _CIvfLineStripSet_h_

#include <ivf/IvfGLPrimitive.h>

IvfSmartPointer(CIvfLineStripSet);

/**
 * LineStripSet class
 *
 * Implements an OpenGL GL_LINE_STRIP primitive
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfLineStripSet : public CIvfGLPrimitive {
private:
	bool m_useColor;
	float m_lineWidth;
protected:
	virtual void doCreateGeometry();
public:
	/** CIvfLineStripSet constructor */
	CIvfLineStripSet();

	/** CIvfLineStripSet destructor */
	virtual ~CIvfLineStripSet();

	IvfClassInfo("CIvfLineStripSet",CIvfGLPrimitive);

	/** Enables or disables the use of color values when rendering */
	void setUseColor(bool flag);

	/** Returns color usages state */
	bool getUseColor();

	/** 
	 * Set line width
	 *
	 * Default line width is initially set to 1.0.
	 */
	void setLineWidth(float width);

	/** Return current line width */
	float getLineWidth();
};
/** \example advgeom.cpp */
#endif 
