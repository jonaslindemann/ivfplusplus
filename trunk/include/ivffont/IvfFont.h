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

#ifndef _CIvfFont_h_
#define _CIvfFont_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(CIvfFont);

/**
 * Font base class
 *
 * CIvfFont is the abstract base class for a Ivf++ font.
 * @author Jonas Lindemann
 */
class IVFFONT_API CIvfFont: public CIvfShape {
private:
	std::string m_filename;
	std::string m_text;
	unsigned int m_faceSize;
	double m_depth;
public:
	/** 
	 * CIvfFont constructor. 
	 * 
	 * param filename full path to truetype font (.ttf)
	 */
	CIvfFont(const char* filename);

	/** Class destructor */
	virtual ~CIvfFont();

	/** Returns font filename of currently loaded font. */
	const char* getFilename(void);

	IvfClassInfo("CIvfFont", CIvfShape);
	
	/** Set the text to be rendered. */
	void setText(const char* text);

	/** Return the current assigned text. */
	const char* getText(void);
	
	/** Set the size of the face in points. */
	void setFaceSize(const unsigned int faceSize);

	/** Return the face size */
	unsigned int getFaceSize(void);

	/** Set the font extrusion depth, if supported.  */
	void setDepth(const double depth);

	/** Return the font extrusion depth, if supported. */
	double getDepth(void);

	/** Return real font size. **/
	virtual void getSize(double & xsize, double & ysize, double & zsize);

protected:
	virtual void doSetText(const char* text);
	virtual void doSetFaceSize(const unsigned int faceSize);
	virtual void doSetDepth(const double depth);
};

#endif
