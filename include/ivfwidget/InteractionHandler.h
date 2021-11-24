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

#include <ivf/Scene.h>
#include <ivf/BufferSelection.h>

#include <ivfwidget/HandlerBase.h>
#include <ivfwidget/WidgetBase.h>

#include <ivf3dui/UIInteractiveBase.h>

namespace ivf {

/** Shape down event class */
class IVFWIDGET_API ShapeDownEvent : public EventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDown(Shape* shape) {};
};

/** Shape click event class */
class IVFWIDGET_API ShapeClickEvent : public EventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeClick(Shape* shape) {};
};

/** Shape up event class */
class IVFWIDGET_API ShapeUpEvent : public EventBase {
public:
	/**
	 * Shape up method
	 *
	 * This event method is called when the user
	 * releases the mouse button.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeUp(Shape* shape) {};
};

/** Shape enter event class */
class IVFWIDGET_API ShapeEnterEvent : public EventBase {
public:
	/**
	 * Shape enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a shape.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeEnter(Shape* shape) {};
};

/** Shape over event class */
class IVFWIDGET_API ShapeOverEvent : public EventBase {
public:
	/**
	 * Shape over method
	 *
	 * This method is called when the mouse is over
	 * a specific shape.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeOver(Shape* shape) {};
};

/** Shape leave event class */
class IVFWIDGET_API ShapeLeaveEvent : public EventBase {
public:
	/**
	 * Shape leave method
	 *
	 * This event method is called when the mouse
	 * leaves a shape.
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeLeave(Shape* shape) {};
};

/** Shape drag event class */
class IVFWIDGET_API ShapeDragEvent : public EventBase {
public:
	/**
	 * Shape drag method
	 *
	 * This event method is called when the user drags
	 * the mouse over the shape (holds the mouse button
	 * down and moves the mouse).
	 * @param shape Shape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDrag(Shape* shape) {};
};

/** Control enter event class */
class IVFWIDGET_API ControlEnterEvent : public EventBase {
public:
	/**
	 * Control enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlEnter(UIInteractiveBase* uiControl) {};
};

/** Control over event class */
class IVFWIDGET_API ControlOverEvent : public EventBase {
public:
	/**
	 * Control over method
	 *
	 * This method is called when the mouse is over
	 * a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlOver(UIInteractiveBase* uiControl) {};
};

/** Control leave event class */
class IVFWIDGET_API ControlLeaveEvent : public EventBase {
public:
	/**
	 * Control over method
	 *
	 * This event method is called when the mouse
	 * leaves a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlLeave(UIInteractiveBase* uiControl) {};
};

/** Control drag event class */
class IVFWIDGET_API ControlDragEvent : public EventBase {
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
	virtual void onControlDrag(UIInteractiveBase* uiControl) {};
};

/** Control up event class */
class IVFWIDGET_API ControlUpEvent : public EventBase {
public:
	/**
	 * Control up method
	 *
	 * This event method is called when the user
	 * has releases the mouse button from a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlUp(UIInteractiveBase* uiControl) {};
};

/** Control click event class */
class IVFWIDGET_API ControlClickEvent : public EventBase {
public:
	/**
	 * Control click method
	 *
	 * This event method is called when the user
	 * has clicked on a contro.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlClick(UIInteractiveBase* uiControl) {};
};

/** Control down event class */
class IVFWIDGET_API ControlDownEvent : public EventBase {
public:
	/**
	 * Control down method
	 *
	 * This event method is called when the user
	 * has pressed the mouse button on a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlDown(UIInteractiveBase* uiControl) {};
};

IvfSmartPointer(InteractionHandler);

/**
 * 3D user interface interaction handler
 *
 * This handler implements 3D user interface interaction
 * for CIvfWidgetBased classes using the ivf3dui library.
 * The handler supports two kinds of interaction, Shape
 * based interaction and interaction with CIvfControl derived
 * controls. CIvfControl classes are specialised 3D user
 * interface controls, which support visual responses to
 * user interaction.
 */
class IVFWIDGET_API InteractionHandler : public HandlerBase,
	MouseDownEvent,
	MouseMoveEvent,
	MouseUpEvent
{
private:
	WidgetBase* m_widget;
	ScenePtr m_scene;
	ShapePtr m_oldShape;
	BufferSelectionPtr m_selection;

	ShapePtr m_lastShape;
	ShapePtr m_currentShape;

	UIInteractiveBasePtr m_lastControl;
	UIInteractiveBasePtr m_currentControl;

	int	m_lastPos[2];

	ShapeDownEvent*		m_shapeDownEvent;
	ShapeClickEvent*	m_shapeClickEvent;
	ShapeUpEvent*		m_shapeUpEvent;
	ShapeEnterEvent*	m_shapeEnterEvent;
	ShapeOverEvent*		m_shapeOverEvent;
	ShapeLeaveEvent*	m_shapeLeaveEvent;
	ShapeDragEvent*		m_shapeDragEvent;

	ControlEnterEvent*	m_controlEnterEvent;
	ControlOverEvent*	m_controlOverEvent;
	ControlLeaveEvent*	m_controlLeaveEvent;
	ControlDragEvent*	m_controlDragEvent;
	ControlUpEvent*		m_controlUpEvent;
	ControlClickEvent*	m_controlClickEvent;
	ControlDownEvent*	m_controlDownEvent;
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to WidgetBase derived class.
	 * @param scene refercne to a Scene derived class containing
	 * the user interface elements, both Shape and CIvfControl
	 * derived.
	 */
	InteractionHandler(WidgetBase* widget, Scene* scene);

	/** Class destructor */
	virtual ~InteractionHandler();

	IvfClassInfo("InteractionHandler",HandlerBase);

	static InteractionHandlerPtr create(WidgetBase* widget, Scene* scene)
	{
		return InteractionHandlerPtr(new InteractionHandler(widget, scene));
	}

	/** Assign control drag event to handler */
	void setControlDragEvent(ControlDragEvent* event);

	/** Assign control enter event to handler */
	void setControlEnterEvent(ControlEnterEvent* event);

	/** Assign control leave event to handler */
	void setControlLeaveEvent(ControlLeaveEvent* event);

	/** Assign control over event to handler */
	void setControlOverEvent(ControlOverEvent* event);

	/** Assign control click event to handler */
	void setControlClickEvent(ControlClickEvent* event);

	/** Assign control up event to handler */
	void setControlUpEvent(ControlUpEvent* event);

	/** Assign control down event to handler */
	void setControlDownEvent(ControlDownEvent* event);

	/** Assign shape drag event to handler */
	void setShapeDragEvent(ShapeDragEvent* event);

	/** Assign shape enter event to handler */
	void setShapeEnterEvent(ShapeEnterEvent* event);

	/** Assign shape over event to handler */
	void setShapeOverEvent(ShapeOverEvent* event);

	/** Assign shape leave event to handler */
	void setShapeLeaveEvent(ShapeLeaveEvent* event);

	/** Assign shape click event to handler */
	void setShapeClickEvent(ShapeClickEvent* event);

	/** Assign shape up event to handler */
	void setShapeUpEvent(ShapeUpEvent* event);

	/** Assign shape down event to handler */
	void setShapeDownEvent(ShapeDownEvent* event);

	virtual void doMouseUp(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseDown(int x, int y);

	// Implemeted events

	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
};
/** \example manip.cpp
 * This example illustrates how to use the InteractionHandler
 * class.
 */

}