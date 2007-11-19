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

#ifndef _CIvfTriStripSet_h_
#define _CIvfTriStripSet_h_

#include <ivf/IvfGLPrimitive.h>

IvfSmartPointer(CIvfTriStripSet);

/**
 * Tri strip set class
 *
 * Implements an OpenGL GL_TRIANGLE_STRIP primitive
 *
 * @autor Jonas Lindemann
 */
class IVF_API CIvfTriStripSet : public CIvfGLPrimitive {
private:
	bool m_useColor;
public:
	/** CIvfTriStripSet constructor */
	CIvfTriStripSet();

	/** CIvfTriStripSet destructor */
	virtual ~CIvfTriStripSet();

	IvfClassInfo("CIvfTriStripSet",CIvfGLPrimitive);

	/** Enable/disable usage of color values when rendering */
	void setUseColor(bool flag);

	/** Return color usage */
	bool getUseColor();
protected:
	virtual void updateVertexNormals();
	virtual void calcNormal(CIvfIndex* idx);
	virtual void createGeometry();
};
/** \example advgeom.cpp */
#endif 
