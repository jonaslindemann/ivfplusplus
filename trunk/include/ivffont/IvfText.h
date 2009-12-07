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

#ifndef _CIvfText_h_
#define _CIvfText_h_

#include <ivf/IvfShape.h>

#include <ivffont/IvfFont.h>

IvfSmartPointer(CIvfText);

/**
 * Generalized text class
 *
 * The CIvfText class renders a given text using a specified vector font.
 */
class IVFFONT_API CIvfText: public CIvfShape {
public:
	enum THorisontalAlignment {
		HA_LEFT,
		HA_CENTER,
		HA_RIGHT
	};
	enum TVerticalAlignment {
		VA_BOTTOM,
		VA_MIDDLE,
		VA_TOP
	};
private:
	CIvfFontPtr m_font;
	std::string m_text;
	THorisontalAlignment m_horisontalAlignment;
	TVerticalAlignment m_verticalAlignment;
public:
	CIvfText();

	IvfClassInfo("CIvfText", CIvfShape);

	/** Set the font to be used for text rendering. */
	void setFont(CIvfFont* font);

	/** Return the currently assigned font. */
	CIvfFont* getFont(void);

	/** Set the text to be rendered. */
	void setText(const std::string& text);

	/** Get real size of text. **/
	void getSize(double & xsize, double & ysize, double & zsize);
protected:
	virtual void doCreateGeometry();
public:
	/** Return the rendered text. */
	const std::string getText(void);
};

#endif
