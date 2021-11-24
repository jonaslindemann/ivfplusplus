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

#ifndef _CIvfPolygonalFont_h_
#define _CIvfPolygonalFont_h_

#include <ivf/IvfShape.h>

#include <ivffont/IvfVectorFont.h>

#include <FTGL/ftgl.h>

IvfSmartPointer(CIvfPolygonalFont);

/**
 * Polygonal font class
 *
 * The CIvfPolygonalFont represents a font rendered with a flat surface.
 */
class IVFFONT_API CIvfPolygonalFont: public CIvfVectorFont {
private:
	FTGLPolygonFont* m_ftglPolygonFont;
	double m_scaleFactor;
public:
	/** Class constructor. @see CIvfFont. */
	CIvfPolygonalFont(const std::string& filename);

	/** Class destructor. */
	virtual ~CIvfPolygonalFont();

	IvfClassInfo("CIvfPolygonalFont", CIvfVectorFont);
protected:
	virtual void doCreateGeometry();
	virtual void doSetRealFaceSize(const double size);
	virtual void doSetFaceSize(const unsigned int size);
	virtual void doGetSize(double & xsize, double & ysize, double & zsize);
};

#endif
