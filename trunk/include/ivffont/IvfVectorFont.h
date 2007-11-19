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

#ifndef _CIvfVectorFont_h_
#define _CIvfVectorFont_h_

#include <ivffont/IvfFont.h>

IvfSmartPointer(CIvfVectorFont);

/**
 * Abstract base class for 3D renderable fonts
 * 
 * The CIvfVectorFont defines a base class for all
 * fonts that can be renderable in the 3D scenegraph.
 * Enables the use of real world coordinates for 
 * specifying font sizes.
 */
class IVFFONT_API CIvfVectorFont: public CIvfFont {
private:
	double m_realFaceSize;
public:
	/** Constructor. @see CIvfFont. */
	CIvfVectorFont(const char* filename);

	IvfClassInfo("CIvfVectorFont", CIvfVectorFont);
	
	/** Set real world font size. (default = 1.0) */
	void setRealFaceSize(const double size);
	
	/** Return real world font size. */
	double getRealFaceSize(void);
protected:
	virtual void doSetRealFaceSize(const double size);
	virtual void doGetSize(double & xsize, double & ysize, double & zsize);
public:
	/** Return the actual size of the font. (Depends on the actual text specified)*/
	void getSize(double & xsize, double & ysize, double & zsize);
};

#endif
