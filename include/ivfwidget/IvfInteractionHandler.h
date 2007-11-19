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

#ifndef _CIvfInteractionHandler_h_
#define _CIvfInteractionHandler_h_

#include <ivf/IvfScene.h>
#include <ivf/IvfBufferSelection.h>

#include <ivfwidget/IvfHandlerBase.h>
#include <ivfwidget/IvfWidgetBase.h>

#include <ivf3dui/IvfUIInteractiveBase.h>

/** Shape down event class */
class IVFWIDGET_API CIvfShapeDownEvent : public CIvfEventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDown(CIvfShape* shape) {};
};

/** Shape click event class */
class IVFWIDGET_API CIvfShapeClickEvent : public CIvfEventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeClick(CIvfShape* shape) {};
};

/** Shape up event class */
class IVFWIDGET_API CIvfShapeUpEvent : public CIvfEventBase {
public:
	/**
	 * Shape up method
	 *
	 * This event method is called when the user
	 * releases the mouse button.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeUp(CIvfShape* shape) {};
};

/** Shape enter event class */
class IVFWIDGET_API CIvfShapeEnterEvent : public CIvfEventBase {
public:
	/**
	 * Shape enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a shape.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeEnter(CIvfShape* shape) {};
};

/** Shape over event class */
class IVFWIDGET_API CIvfShapeOverEvent : public CIvfEventBase {
public:
	/**
	 * Shape over method
	 *
	 * This method is called when the mouse is over
	 * a specific shape.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeOver(CIvfShape* shape) {};
};

/** Shape leave event class */
class IVFWIDGET_API CIvfShapeLeaveEvent : public CIvfEventBase {
public:
	/**
	 * Shape leave method
	 *
	 * This event method is called when the mouse
	 * leaves a shape.
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeLeave(CIvfShape* shape) {};
};

/** Shape drag event class */
class IVFWIDGET_API CIvfShapeDragEvent : public CIvfEventBase {
public:
	/**
	 * Shape drag method
	 *
	 * This event method is called when the user drags
	 * the mouse over the shape (holds the mouse button
	 * down and moves the mouse).
	 * @param shape CIvfShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDrag(CIvfShape* shape) {};
};

/** Control enter event class */
class IVFWIDGET_API CIvfControlEnterEvent : public CIvfEventBase {
public:
	/**
	 * Control enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlEnter(CIvfUIInteractiveBase* uiControl) {};
};

/** Control over event class */
class IVFWIDGET_API CIvfControlOverEvent : public CIvfEventBase {
public:
	/**
	 * Control over method
	 *
	 * This method is called when the mouse is over
	 * a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlOver(CIvfUIInteractiveBase* uiControl) {};
};

/** Control leave event class */
class IVFWIDGET_API CIvfControlLeaveEvent : public CIvfEventBase {
public:
	/**
	 * Control over method
	 *
	 * This event method is called when the mouse
	 * leaves a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlLeave(CIvfUIInteractiveBase* uiControl) {};
};

/** Control drag event class */
class IVFWIDGET_API CIvfControlDragEvent : public CIvfEventBase {
public:
	/**
	 * Control over method
	 *
	 * This event method is called when the user drags
	 * the mouse over the control (holds the mouse button
	 * down and moves the mouse).
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlDrag(CIvfUIInteractiveBase* uiControl) {};
};

/** Control up event class */
class IVFWIDGET_API CIvfControlUpEvent : public CIvfEventBase {
public:
	/**
	 * Control up method
	 *
	 * This event method is called when the user
	 * has releases the mouse button from a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlUp(CIvfUIInteractiveBase* uiControl) {};
};

/** Control click event class */
class IVFWIDGET_API CIvfControlClickEvent : public CIvfEventBase {
public:
	/**
	 * Control click method
	 *
	 * This event method is called when the user
	 * has clicked on a contro.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlClick(CIvfUIInteractiveBase* uiControl) {};
};

/** Control down event class */
class IVFWIDGET_API CIvfControlDownEvent : public CIvfEventBase {
public:
	/**
	 * Control down method
	 *
	 * This event method is called when the user
	 * has pressed the mouse button on a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlDown(CIvfUIInteractiveBase* uiControl) {};
};

IvfSmartPointer(CIvfInteractionHandler);

/**
 * 3D user interface interaction handler
 *
 * This handler implements 3D user interface interaction
 * for CIvfWidgetBased classes using the ivf3dui library.
 * The handler supports two kinds of interaction, CIvfShape
 * based interaction and interaction with CIvfControl derived
 * controls. CIvfControl classes are specialised 3D user
 * interface controls, which support visual responses to
 * user interaction.
 */
class IVFWIDGET_API CIvfInteractionHandler : public CIvfHandlerBase,
	CIvfMouseDownEvent,
	CIvfMouseMoveEvent,
	CIvfMouseUpEvent
{
private:
	CIvfWidgetBase* m_widget;
	CIvfScenePtr m_scene;
	CIvfShapePtr m_oldShape;
	CIvfBufferSelectionPtr m_selection;

	CIvfShapePtr m_lastShape;
	CIvfShapePtr m_currentShape;

	CIvfUIInteractiveBasePtr m_lastControl;
	CIvfUIInteractiveBasePtr m_currentControl;

	int	m_lastPos[2];

	CIvfShapeDownEvent*		m_shapeDownEvent;
	CIvfShapeClickEvent*	m_shapeClickEvent;
	CIvfShapeUpEvent*		m_shapeUpEvent;
	CIvfShapeEnterEvent*	m_shapeEnterEvent;
	CIvfShapeOverEvent*		m_shapeOverEvent;
	CIvfShapeLeaveEvent*	m_shapeLeaveEvent;
	CIvfShapeDragEvent*		m_shapeDragEvent;

	CIvfControlEnterEvent*	m_controlEnterEvent;
	CIvfControlOverEvent*	m_controlOverEvent;
	CIvfControlLeaveEvent*	m_controlLeaveEvent;
	CIvfControlDragEvent*	m_controlDragEvent;
	CIvfControlUpEvent*		m_controlUpEvent;
	CIvfControlClickEvent*	m_controlClickEvent;
	CIvfControlDownEvent*	m_controlDownEvent;
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to CIvfWidgetBase derived class.
	 * @param scene refercne to a CIvfScene derived class containing
	 * the user interface elements, both CIvfShape and CIvfControl
	 * derived.
	 */
	CIvfInteractionHandler(CIvfWidgetBase* widget, CIvfScene* scene);

	/** Class destructor */
	virtual ~CIvfInteractionHandler();

	IvfClassInfo("CIvfInteractionHandler",CIvfHandlerBase);

	/** Assign control drag event to handler */
	void setControlDragEvent(CIvfControlDragEvent* event);

	/** Assign control enter event to handler */
	void setControlEnterEvent(CIvfControlEnterEvent* event);

	/** Assign control leave event to handler */
	void setControlLeaveEvent(CIvfControlLeaveEvent* event);

	/** Assign control over event to handler */
	void setControlOverEvent(CIvfControlOverEvent* event);

	/** Assign control click event to handler */
	void setControlClickEvent(CIvfControlClickEvent* event);

	/** Assign control up event to handler */
	void setControlUpEvent(CIvfControlUpEvent* event);

	/** Assign control down event to handler */
	void setControlDownEvent(CIvfControlDownEvent* event);

	/** Assign shape drag event to handler */
	void setShapeDragEvent(CIvfShapeDragEvent* event);

	/** Assign shape enter event to handler */
	void setShapeEnterEvent(CIvfShapeEnterEvent* event);

	/** Assign shape over event to handler */
	void setShapeOverEvent(CIvfShapeOverEvent* event);

	/** Assign shape leave event to handler */
	void setShapeLeaveEvent(CIvfShapeLeaveEvent* event);

	/** Assign shape click event to handler */
	void setShapeClickEvent(CIvfShapeClickEvent* event);

	/** Assign shape up event to handler */
	void setShapeUpEvent(CIvfShapeUpEvent* event);

	/** Assign shape down event to handler */
	void setShapeDownEvent(CIvfShapeDownEvent* event);

	virtual void doMouseUp(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseDown(int x, int y);

	// Implemeted events

	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
};
/** \example manip.cpp
 * This example illustrates how to use the CIvfInteractionHandler
 * class.
 */

#endif
