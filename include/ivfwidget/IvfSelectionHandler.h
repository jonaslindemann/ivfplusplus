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

#ifndef _CIvfSelectionHandler_h_
#define _CIvfSelectionHandler_h_

#include <ivfwidget/IvfHandlerBase.h>
#include <ivfwidget/IvfWidgetBase.h>

#include <ivf/IvfScene.h>
#include <ivf/IvfBufferSelection.h>

/** Single selection event class */
class IVFWIDGET_API CIvfSingleSelectionEvent : public CIvfEventBase {
public:
	/**
	 * Single select method
	 *
	 * This method is called when a selection has been made
	 * i.e. the user has has clicked in the viewport..
	 */
	virtual void onSelect(CIvfShape* shape) {};
};

/** Multiple selection event class */
class IVFWIDGET_API CIvfMultipleSelectionEvent : public CIvfEventBase {
public:
	/**
	 * Multiple selection method
	 *
	 * This method is called when a multiple selection has
	 * been made, i.e. a selection containing all objects
	 * under the cursor.
	 * @param shapes STL vector<CIvfShape*> containing the
	 * objects located under the cursor position.
	 */
	virtual void onSelectMultiple(CIvfSelectedShapesVector& shapes) {};
};

/** Highlight event class */
class IVFWIDGET_API CIvfHighlightEvent : public CIvfEventBase {
public:
	/**
	 * Highlight event method
	 *
	 * This method is called when the mouse is over
	 * an object, to handle highlighting.
	 * @param shape Reference to CIvfShape derived class
	 * currently under the cursor.
	 */
	virtual void onHighlight(CIvfShape* shape) {};
};

IvfSmartPointer(CIvfSelectionHandler);

/**
 * Selection handler
 *
 * This class automatically handles OpenGL buffer selection.
 * It can fire 3 kinds of events: onSelect which can be
 * assigned to handle a simple selection of a single CIvfShape
 * instance. onMultipleSelect which handles a selection with all
 * objects located under the cursor. onHighlight for implementing
 * highlighting of object under the cursor.
 */
class IVFWIDGET_API CIvfSelectionHandler : public CIvfHandlerBase,
	CIvfMouseDownEvent,
	CIvfMouseMoveEvent,
	CIvfMouseUpEvent
{
private:
	CIvfWidgetBase* m_widget;
	CIvfScenePtr m_scene;
	CIvfShapePtr m_oldShape;
	CIvfBufferSelectionPtr m_selection;
	CIvfSingleSelectionEvent* m_singleSelectionEvent;
	CIvfMultipleSelectionEvent* m_multipleSelectionEvent;
	CIvfHighlightEvent* m_highlightEvent;

	void dispatchSingleSelectionEvent(CIvfShape* shape);
	void dispatchMultipleSelectionEvent(CIvfSelectedShapesVector& selectedShapes);
	void dispatchHighlightEvent(CIvfShape* shape);
public:
	/**
	 * Class constructor
	 *
	 * @param widget Reference to a CIvfWidgetBase instance.
	 * @param scene Reference to a CIvfScene instance containing
	 * the complete scene graph setup.
	 */
	CIvfSelectionHandler(CIvfWidgetBase* widget, CIvfScene* scene);
	virtual ~CIvfSelectionHandler();

	IvfClassInfo("CIvfSelectionHandler",CIvfHandlerBase);

	/** Assign single selection event */
	void setSingleSelectionEvent(CIvfSingleSelectionEvent* event);

	/** Assign multiple selection event */
	void setMultipleSelectionEvent(CIvfMultipleSelectionEvent* event);

	/** Assign highlight event */
	void setHighlightEvent(CIvfHighlightEvent* event);

	virtual void doMouseUp(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseDown(int x, int y);

	// Implemeted events

	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
};
/** \example handlers.cpp */

#endif
