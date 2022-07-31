//
// Copyright 1999-2022 by Structural Mechanics, Lund University.
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

#include <ivf/GLPrimitive.h>

namespace ivf {

IvfSmartPointer(LineSet);

/**
 * LineSet class
 *
 * Implements a OpenGL GL_LINES primitive
 *
 * @author Jonas Lindemann
 */
class IVF_API LineSet : public GLPrimitive {
private:
	bool m_useColor;
	bool m_useAlpha;
	float m_lineWidth;
	std::vector<float> m_idxLineWidth;
protected:
	virtual void doCreateGeometry() override;
public:
	/** LineSet constructor */
	LineSet();

	/** LineSet destructor */
	virtual ~LineSet();

	IvfClassInfo("LineSet",GLPrimitive);
	IvfStdFactory(LineSet);

	/** Enable usage of color set */
	void setUseColor(bool flag);

	/** Return color set usage */
	bool getUseColor();

	/** Enable usage of alpha channel */
	void setUseAlpha(bool flag);

	/** Return usage of alpha channel */
	bool getUseAlpha();

	/** 
	 * Set line width
	 *
	 * Default line width is initially set to 1.0.
	 */
	void setLineWidth(float width);

	/** Return line width */
	float getLineWidth();

	void addIndexLineWidth(float w);

	void clearIndexWidths();

};
/** \example advgeom.cpp */
}