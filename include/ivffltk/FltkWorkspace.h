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

#pragma once

#include <FL/Fl.H>

#if defined(__APPLE__)
#  include <OpenGL/gl3.h> // defines OpenGL 3.0+ functions
#else
#  if defined(WIN32)
#    define GLEW_STATIC 1
#    include <Windows.h>
#  endif
#  include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <FL/Fl_Gl_Window.H>

#include <ivf/Scene.h>
#include <ivf/View.h>
#include <ivf/OldLightModel.h>
#include <ivf/OldLight.h>

#include <ivfwidget/WorkspaceWidget.h>

namespace ivf {

IvfSmartPointer(CFltkWorkspace);

/**
 * Fltk Workspace widget class
 */
class IVFFLTK_API CFltkWorkspace : public Fl_Gl_Window, public CWorkspaceWidget {
private:
protected:
	virtual void doRedraw();
	/** 
	 * Implements the FL_Gl_Window draw method
	 * 
	 * This method is called when the widget is to be drawn.
	 */
	void draw();

	/**
	 * Implements the FL_Gl_Window handle method
	 * 
	 * This method is called in response to GUI events.
	 */
	int handle(int event);

	virtual void doDisableIdleProcessing();
	virtual void doEnableIdleProcessing();
	virtual void doEnableTimeout(float time, int nbr);
public:
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
	CFltkWorkspace(int X, int Y, int W, int H, const char *L=0);

	IvfClassInfo("CFltkWorkspace",CWorkspaceWidget);
};

}