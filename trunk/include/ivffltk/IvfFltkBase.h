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

#ifndef _CIvfFltkBase_h_
#define _CIvfFltkBase_h_

#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <FL/Fl_Gl_Window.H>

#include <ivfwidget/IvfWidgetBase.h>

IvfSmartPointer(CIvfFltkBase);

/**
 * Basic fltk widget class
 */
class IVFFLTK_API CIvfFltkBase : public Fl_Gl_Window, public CIvfWidgetBase {
private:
	/** 
	 * Implements the FL_Gl_Window draw method
	 * 
	 * This method is called when the widget is to be drawn.
	 */
	void draw();

#ifdef WIN32
	LARGE_INTEGER m_countsPerSec;
#endif
	double		m_elapsedTime;
	double		m_zeroTime;

public:
	/**
	 * Implements the FL_Gl_Window handle method
	 * 
	 * This method is called in response to GUI events.
	 */
	int handle(int event);

	double doElapsedTime();
	/**
	 * CIvfFltkWidget constructor
	 * 
	 * Initializes the widget position and size and an optional title
	 * @param X Widget x position.
	 * @param Y Widget y position.
	 * @param W Widget width.
	 * @param H Widget height.
	 * @param L Widget title (optional)
	 */
	CIvfFltkBase(int X, int Y, int W, int H, const char *L=0);

	IvfClassInfo("CIvfFltkBase",CIvfWidgetBase);

	void redraw();

	virtual void doDisableIdleProcessing();
	virtual void doEnableIdleProcessing();
	virtual void doEnableTimeout(float time, int nbr);
	virtual void doDisableTimeout(int nbr);
	virtual void doRedraw();
};

#endif 
