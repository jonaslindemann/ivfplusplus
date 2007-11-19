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

#ifndef _CIvfQuadPlane_h_
#define _CIvfQuadPlane_h_

#include <ivf/IvfQuadSet.h>

#include <ivfmath/IvfPlane.h>

IvfSmartPointer(CIvfQuadPlane);

/**
 * Quad plane class
 *
 * Implements a simple plane defined by a width and a height. When
 * using textures the class will consider the aspect ratio of the
 * texture.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfQuadPlane : public CIvfQuadSet {
public:
	enum TPlaneOrientation {
		PO_XY,
		PO_XZ,
		PO_YZ
	};
private:
	void initPlane();
	double m_width;
	double m_height;
	double m_ratio;
	double m_textureRepeat[2];
	TPlaneOrientation m_planeOrientation;
	void updateTextureCoords();
public:
	/** CIvfQuadPlane constructor */
	CIvfQuadPlane();

	/** CIvfQuadPlane destructor */
	virtual ~CIvfQuadPlane();

	IvfClassInfo("CIvfQuadPlane",CIvfQuadSet);

	/** Flips texture coordinates */
	void flipVert();

	/** 
	 * Set width of plane
	 *
	 * Automatically calculates height if a texture has been given.
	 * If no texture has been set \em height = \em width.
	 */
	void setWidth(double width);

	/**
	 * Set texture
	 *
	 * Calculates ratio and modifies the plane size
	 */
	void setTexture(CIvfTexture* texture);

	/**
	 * Set size of plane
	 *
	 * Overrides any settings set using the methods
	 * \em setWidth and \em setTexture.
	 */
	void setSize(double width, double height);

	/** Set orientation of plane. */
	void setOrientation(TPlaneOrientation orientation);

	/** Set texture repeat in x and y direction. */
	void setTextureRepeat(double xrepeat, double yrepeat);
};

#endif 
