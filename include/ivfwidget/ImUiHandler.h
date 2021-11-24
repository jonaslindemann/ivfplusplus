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

#include <ivf/Base.h>
#include <ivf/Camera.h>

#include <ivfwidget/WidgetBase.h>
#include <ivfwidget/HandlerBase.h>

namespace ivf {

/** Single selection event class */
class IVFWIDGET_API GuiEvent : public EventBase {
public:
	/**
	 * Single select method
	 *
	 * This method is called when a selection has been made
	 * i.e. the user has has clicked in the viewport..
	 */
	virtual void onGui() {};
};

IvfSmartPointer(ImUiHandler);

class IVFWIDGET_API ImUiHandler : public HandlerBase,
	MouseDownEvent,
	MouseMoveEvent,
	MouseUpEvent,
	ResizeEvent,
	OverlayEvent
{
public:

private:
	WidgetBase* m_widget;
	GuiEvent* m_guiEvent;
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to WidgetBase class.
	 * @param camera reference to a CIvfCamera to be manipulated.
	 */
	ImUiHandler(WidgetBase* widget);

	/** Class destructor */
	virtual ~ImUiHandler();
	
	void setGuiEvent(GuiEvent* event);
	void dispatchGuiEvent();

	IvfClassInfo("ImUiHandler",Base);

	static ImUiHandlerPtr create(WidgetBase* widget)
	{
		return ImUiHandlerPtr(new ImUiHandler(widget));
	}

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

}