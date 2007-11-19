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

#ifndef _CIvfWindow_h_
#define _CIvfWindow_h_

#include <ivffltk/IvfFltkBase.h>
#include <ivfui/IvfPopupMenu.h>

IvfSmartPointer(CIvfWindow);

/**
 * Window class for a general Ivf++ application
 *
 * This class implements a generic Ivf++ window with 
 * standard event methods based on CIvfWidget. The CIvfWindow
 * class should not be used directly. A user defined window
 * class should be derived and the neccesary event methods
 * overridden.
 *
 * @author Jonas Lindemann
 */
class IVFUI_API CIvfWindow : public CIvfFltkBase, public CIvfPopupMenu {
private:
	bool m_popupButtonDown;
public:
	/** 
	 * CIvfWindow constructor
	 *
	 * Window will not be visible until the 
	 * show() method is called.
	 *
	 * @param X Window x position
	 * @param Y Window y position
	 * @param W Window width
	 * @param H Window height
	 */
	CIvfWindow(int X, int Y, int W, int H);
	virtual ~CIvfWindow();

	IvfClassInfo("CIvfWindow",CIvfFltkBase);

	/** Sets window title */
	void setWindowTitle(const char* title);

	/** Shows window on screen */
	void show();

	virtual void doMouseDown(int x, int y);
	virtual void doMouseUp(int x, int y);
};

/** \example ac3dreader.cpp  */
/** \example advgeom.cpp  */
/** \example culling.cpp */
/** \example extrusion.cpp */
/** \example lod.cpp */
/** \example math.cpp */
/** \example placement.cpp */
/** \example robot.cpp */
/** \example selection.cpp */
/** \example textures.cpp */
/** \example lighting.cpp */

#endif 
