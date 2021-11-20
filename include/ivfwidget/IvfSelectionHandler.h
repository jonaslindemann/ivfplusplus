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
class IVFWIDGET_API CSingleSelectionEvent : public CEventBase {
public:
	/**
	 * Single select method
	 *
	 * This method is called when a selection has been made
	 * i.e. the user has has clicked in the viewport..
	 */
	virtual void onSelect(CShape* shape) {};
};

/** Multiple selection event class */
class IVFWIDGET_API CMultipleSelectionEvent : public CEventBase {
public:
	/**
	 * Multiple selection method
	 *
	 * This method is called when a multiple selection has
	 * been made, i.e. a selection containing all objects
	 * under the cursor.
	 * @param shapes STL vector<CShape*> containing the
	 * objects located under the cursor position.
	 */
	virtual void onSelectMultiple(CIvfSelectedShapesVector& shapes) {};
};

/** Highlight event class */
class IVFWIDGET_API CHighlightEvent : public CEventBase {
public:
	/**
	 * Highlight event method
	 *
	 * This method is called when the mouse is over
	 * an object, to handle highlighting.
	 * @param shape Reference to CShape derived class
	 * currently under the cursor.
	 */
	virtual void onHighlight(CShape* shape) {};
};

IvfSmartPointer(CSelectionHandler);

/**
 * Selection handler
 *
 * This class automatically handles OpenGL buffer selection.
 * It can fire 3 kinds of events: onSelect which can be
 * assigned to handle a simple selection of a single CShape
 * instance. onMultipleSelect which handles a selection with all
 * objects located under the cursor. onHighlight for implementing
 * highlighting of object under the cursor.
 */
class IVFWIDGET_API CSelectionHandler : public CHandlerBase,
	CMouseDownEvent,
	CMouseMoveEvent,
	CMouseUpEvent
{
private:
	CWidgetBase* m_widget;
	CScenePtr m_scene;
	CShapePtr m_oldShape;
	CBufferSelectionPtr m_selection;
	CSingleSelectionEvent* m_singleSelectionEvent;
	CMultipleSelectionEvent* m_multipleSelectionEvent;
	CHighlightEvent* m_highlightEvent;

	void dispatchSingleSelectionEvent(CShape* shape);
	void dispatchMultipleSelectionEvent(CIvfSelectedShapesVector& selectedShapes);
	void dispatchHighlightEvent(CShape* shape);
public:
	/**
	 * Class constructor
	 *
	 * @param widget Reference to a CWidgetBase instance.
	 * @param scene Reference to a CScene instance containing
	 * the complete scene graph setup.
	 */
	CSelectionHandler(CWidgetBase* widget, CScene* scene);
	virtual ~CSelectionHandler();

	IvfClassInfo("CSelectionHandler",CHandlerBase);

	/** Assign single selection event */
	void setSingleSelectionEvent(CSingleSelectionEvent* event);

	/** Assign multiple selection event */
	void setMultipleSelectionEvent(CMultipleSelectionEvent* event);

	/** Assign highlight event */
	void setHighlightEvent(CHighlightEvent* event);

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
