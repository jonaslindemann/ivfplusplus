//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivf/Composite.h>
#include <ivf/Brick.h>

namespace ivf {

IvfSmartPointer(Cursor);

/**
 * Cursor shape class
 *
 * CIvfCursor implements a cursor shape. The shape 
 * consists of three brick shapes i creating a 
 * three dimensional cross.
 * @author Jonas Lindemann
 */
class IVF_API Cursor : public Composite {
public:
	enum TCursorType {
		CT_LINE_CURSOR,
		CT_SOLID_CURSOR,
		CT_INVISIBLE
	};
public:
	/** CIvfCursor constructor */
	Cursor ();

	/** CIvfCursor destructor */
	virtual ~Cursor ();

	IvfClassInfo("Cursor",Composite);
	IvfStdFactory(Cursor);

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
	Material* m_zMaterial;
	Material* m_yMaterial;
	Material* m_xMaterial;
	Brick* m_zBrick;
	Brick* m_yBrick;
	Brick* m_xBrick;
	double m_size;
    double m_thickness;
protected:
	virtual void doCreateGeometry() override;
};

}