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

#ifndef _CIvfCursor_h_
#define _CIvfCursor_h_

#include <ivf/IvfComposite.h>
#include <ivf/IvfBrick.h>

IvfSmartPointer(CIvfCursor);

/**
 * Cursor shape class
 *
 * CIvfCursor implements a cursor shape. The shape 
 * consists of three brick shapes i creating a 
 * three dimensional cross.
 * @author Jonas Lindemann
 */
class IVF_API CIvfCursor : public CIvfComposite {
public:
	enum TCursorType {
		CT_LINE_CURSOR,
		CT_SOLID_CURSOR,
		CT_INVISIBLE
	};
public:
	/** CIvfCursor constructor */
	CIvfCursor ();

	/** CIvfCursor destructor */
	virtual ~CIvfCursor ();

	IvfClassInfo("CIvfCursor",CIvfComposite);

	/** 
	 * Set cursor size
	 *
	 * The size set the length of the individual brick shapes.
	 * The thickness of the brick shapes are set to 5 % of the
	 * length.
	 */
	void setSize(double size);
    
    void setThickness(double thickness);

	/** 
	 * Set cursor type
	 *
	 * The cursor can be represented by lines or as solid
	 * shapes.
	 * @param type CT_LINE_CURSOR cursor made of lines.
	 * CT_SOLID_CURSOR (default).
	 */
	void setCursorType(TCursorType type);

	/** Return cursor type */
	int getCursorType();
private:
	int m_cursorType;
	CIvfMaterial* m_zMaterial;
	CIvfMaterial* m_yMaterial;
	CIvfMaterial* m_xMaterial;
	CIvfBrick* m_zBrick;
	CIvfBrick* m_yBrick;
	CIvfBrick* m_xBrick;
	double m_size;
    double m_thickness;
protected:
	virtual void doCreateGeometry();
};
#endif
