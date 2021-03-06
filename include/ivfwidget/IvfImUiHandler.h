//
// Copyright 1999-2010 by Structural Mechanics, Lund University.
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

#pragma once

#include <ivf/IvfBase.h>
#include <ivf/IvfCamera.h>

#include <ivfwidget/IvfWidgetBase.h>
#include <ivfwidget/IvfHandlerBase.h>

/** Single selection event class */
class IVFWIDGET_API CIvfGuiEvent : public CIvfEventBase {
public:
	/**
	 * Single select method
	 *
	 * This method is called when a selection has been made
	 * i.e. the user has has clicked in the viewport..
	 */
	virtual void onGui() {};
};

IvfSmartPointer(CIvfImUiHandler);

class IVFWIDGET_API CIvfImUiHandler : public CIvfHandlerBase,
	CIvfMouseDownEvent,
	CIvfMouseMoveEvent,
	CIvfMouseUpEvent,
	CIvfResizeEvent,
	CIvfOverlayEvent
{
public:

private:
	CIvfWidgetBase* m_widget;
	CIvfGuiEvent* m_guiEvent;
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to CIvfWidgetBase class.
	 * @param camera reference to a CIvfCamera to be manipulated.
	 */
	CIvfImUiHandler(CIvfWidgetBase* widget);

	/** Class destructor */
	virtual ~CIvfImUiHandler();
	
	void setGuiEvent(CIvfGuiEvent* event);
	void dispatchGuiEvent();

	IvfClassInfo("CIvfImUiHandler",CIvfBase);
	virtual void doResize(int width, int height);
	virtual void doMouseDown(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseUp(int x, int y);
	virtual void doOverlay();

	// Implemented events

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onResize(int width, int height);
	virtual void onOverlay();

};

