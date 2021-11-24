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

#include <ivfwidget/HandlerBase.h>
#include <ivfwidget/WidgetBase.h>

#include <ivf/Scene.h>
#include <ivf/BufferSelection.h>

namespace ivf {

/** Single selection event class */
class IVFWIDGET_API SingleSelectionEvent : public EventBase {
public:
	/**
	 * Single select method
	 *
	 * This method is called when a selection has been made
	 * i.e. the user has has clicked in the viewport..
	 */
	virtual void onSelect(Shape* shape) {};
};

/** Multiple selection event class */
class IVFWIDGET_API MultipleSelectionEvent : public EventBase {
public:
	/**
	 * Multiple selection method
	 *
	 * This method is called when a multiple selection has
	 * been made, i.e. a selection containing all objects
	 * under the cursor.
	 * @param shapes STL vector<Shape*> containing the
	 * objects located under the cursor position.
	 */
	virtual void onSelectMultiple(SelectedShapesVector& shapes) {};
};

/** Highlight event class */
class IVFWIDGET_API HighlightEvent : public EventBase {
public:
	/**
	 * Highlight event method
	 *
	 * This method is called when the mouse is over
	 * an object, to handle highlighting.
	 * @param shape Reference to Shape derived class
	 * currently under the cursor.
	 */
	virtual void onHighlight(Shape* shape) {};
};

IvfSmartPointer(SelectionHandler);

/**
 * Selection handler
 *
 * This class automatically handles OpenGL buffer selection.
 * It can fire 3 kinds of events: onSelect which can be
 * assigned to handle a simple selection of a single Shape
 * instance. onMultipleSelect which handles a selection with all
 * objects located under the cursor. onHighlight for implementing
 * highlighting of object under the cursor.
 */
class IVFWIDGET_API SelectionHandler : public HandlerBase,
	MouseDownEvent,
	MouseMoveEvent,
	MouseUpEvent
{
private:
	WidgetBase* m_widget;
	ScenePtr m_scene;
	ShapePtr m_oldShape;
	BufferSelectionPtr m_selection;
	SingleSelectionEvent* m_singleSelectionEvent;
	MultipleSelectionEvent* m_multipleSelectionEvent;
	HighlightEvent* m_highlightEvent;

	void dispatchSingleSelectionEvent(Shape* shape);
	void dispatchMultipleSelectionEvent(SelectedShapesVector& selectedShapes);
	void dispatchHighlightEvent(Shape* shape);
public:
	/**
	 * Class constructor
	 *
	 * @param widget Reference to a WidgetBase instance.
	 * @param scene Reference to a Scene instance containing
	 * the complete scene graph setup.
	 */
	SelectionHandler(WidgetBase* widget, Scene* scene);
	virtual ~SelectionHandler();

	IvfClassInfo("SelectionHandler",HandlerBase);

	static SelectionHandlerPtr create(WidgetBase* widget, Scene* scene)
	{
		return SelectionHandlerPtr(new SelectionHandler(widget, scene));
	}

	/** Assign single selection event */
	void setSingleSelectionEvent(SingleSelectionEvent* event);

	/** Assign multiple selection event */
	void setMultipleSelectionEvent(MultipleSelectionEvent* event);

	/** Assign highlight event */
	void setHighlightEvent(HighlightEvent* event);

	virtual void doMouseUp(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseDown(int x, int y);

	// Implemeted events

	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
};
/** \example handlers.cpp */

}