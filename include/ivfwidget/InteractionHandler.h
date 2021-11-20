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

#pragma once

#include <ivf/Scene.h>
#include <ivf/BufferSelection.h>

#include <ivfwidget/HandlerBase.h>
#include <ivfwidget/WidgetBase.h>

#include <ivf3dui/UIInteractiveBase.h>

namespace ivf {

/** Shape down event class */
class IVFWIDGET_API CShapeDownEvent : public CEventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDown(CShape* shape) {};
};

/** Shape click event class */
class IVFWIDGET_API CShapeClickEvent : public CEventBase {
public:
	/**
	 * Shape click method
	 *
	 * This event method is called when the user
	 * has clicked on a shape.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeClick(CShape* shape) {};
};

/** Shape up event class */
class IVFWIDGET_API CShapeUpEvent : public CEventBase {
public:
	/**
	 * Shape up method
	 *
	 * This event method is called when the user
	 * releases the mouse button.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeUp(CShape* shape) {};
};

/** Shape enter event class */
class IVFWIDGET_API CShapeEnterEvent : public CEventBase {
public:
	/**
	 * Shape enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a shape.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeEnter(CShape* shape) {};
};

/** Shape over event class */
class IVFWIDGET_API CShapeOverEvent : public CEventBase {
public:
	/**
	 * Shape over method
	 *
	 * This method is called when the mouse is over
	 * a specific shape.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeOver(CShape* shape) {};
};

/** Shape leave event class */
class IVFWIDGET_API CShapeLeaveEvent : public CEventBase {
public:
	/**
	 * Shape leave method
	 *
	 * This event method is called when the mouse
	 * leaves a shape.
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeLeave(CShape* shape) {};
};

/** Shape drag event class */
class IVFWIDGET_API CShapeDragEvent : public CEventBase {
public:
	/**
	 * Shape drag method
	 *
	 * This event method is called when the user drags
	 * the mouse over the shape (holds the mouse button
	 * down and moves the mouse).
	 * @param shape CShape reference to the shape
	 * under the mouse.
	 */
	virtual void onShapeDrag(CShape* shape) {};
};

/** Control enter event class */
class IVFWIDGET_API CControlEnterEvent : public CEventBase {
public:
	/**
	 * Control enter method
	 *
	 * This event method is called when the first
	 * time the mouse enters over a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlEnter(CUIInteractiveBase* uiControl) {};
};

/** Control over event class */
class IVFWIDGET_API CControlOverEvent : public CEventBase {
public:
	/**
	 * Control over method
	 *
	 * This method is called when the mouse is over
	 * a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlOver(CUIInteractiveBase* uiControl) {};
};

/** Control leave event class */
class IVFWIDGET_API CControlLeaveEvent : public CEventBase {
public:
	/**
	 * Control over method
	 *
	 * This event method is called when the mouse
	 * leaves a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlLeave(CUIInteractiveBase* uiControl) {};
};

/** Control drag event class */
class IVFWIDGET_API CControlDragEvent : public CEventBase {
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
	virtual void onControlDrag(CUIInteractiveBase* uiControl) {};
};

/** Control up event class */
class IVFWIDGET_API CControlUpEvent : public CEventBase {
public:
	/**
	 * Control up method
	 *
	 * This event method is called when the user
	 * has releases the mouse button from a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	 virtual void onControlUp(CUIInteractiveBase* uiControl) {};
};

/** Control click event class */
class IVFWIDGET_API CControlClickEvent : public CEventBase {
public:
	/**
	 * Control click method
	 *
	 * This event method is called when the user
	 * has clicked on a contro.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlClick(CUIInteractiveBase* uiControl) {};
};

/** Control down event class */
class IVFWIDGET_API CControlDownEvent : public CEventBase {
public:
	/**
	 * Control down method
	 *
	 * This event method is called when the user
	 * has pressed the mouse button on a control.
	 * @param shape CIvfControl reference to the control
	 * under the mouse.
	 */
	virtual void onControlDown(CUIInteractiveBase* uiControl) {};
};

IvfSmartPointer(CInteractionHandler);

/**
 * 3D user interface interaction handler
 *
 * This handler implements 3D user interface interaction
 * for CIvfWidgetBased classes using the ivf3dui library.
 * The handler supports two kinds of interaction, CShape
 * based interaction and interaction with CIvfControl derived
 * controls. CIvfControl classes are specialised 3D user
 * interface controls, which support visual responses to
 * user interaction.
 */
class IVFWIDGET_API CInteractionHandler : public CHandlerBase,
	CMouseDownEvent,
	CMouseMoveEvent,
	CMouseUpEvent
{
private:
	CWidgetBase* m_widget;
	CScenePtr m_scene;
	CShapePtr m_oldShape;
	CBufferSelectionPtr m_selection;

	CShapePtr m_lastShape;
	CShapePtr m_currentShape;

	CUIInteractiveBasePtr m_lastControl;
	CUIInteractiveBasePtr m_currentControl;

	int	m_lastPos[2];

	CShapeDownEvent*		m_shapeDownEvent;
	CShapeClickEvent*	m_shapeClickEvent;
	CShapeUpEvent*		m_shapeUpEvent;
	CShapeEnterEvent*	m_shapeEnterEvent;
	CShapeOverEvent*		m_shapeOverEvent;
	CShapeLeaveEvent*	m_shapeLeaveEvent;
	CShapeDragEvent*		m_shapeDragEvent;

	CControlEnterEvent*	m_controlEnterEvent;
	CControlOverEvent*	m_controlOverEvent;
	CControlLeaveEvent*	m_controlLeaveEvent;
	CControlDragEvent*	m_controlDragEvent;
	CControlUpEvent*		m_controlUpEvent;
	CControlClickEvent*	m_controlClickEvent;
	CControlDownEvent*	m_controlDownEvent;
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to CWidgetBase derived class.
	 * @param scene refercne to a CScene derived class containing
	 * the user interface elements, both CShape and CIvfControl
	 * derived.
	 */
	CInteractionHandler(CWidgetBase* widget, CScene* scene);

	/** Class destructor */
	virtual ~CInteractionHandler();

	IvfClassInfo("CInteractionHandler",CHandlerBase);

	/** Assign control drag event to handler */
	void setControlDragEvent(CControlDragEvent* event);

	/** Assign control enter event to handler */
	void setControlEnterEvent(CControlEnterEvent* event);

	/** Assign control leave event to handler */
	void setControlLeaveEvent(CControlLeaveEvent* event);

	/** Assign control over event to handler */
	void setControlOverEvent(CControlOverEvent* event);

	/** Assign control click event to handler */
	void setControlClickEvent(CControlClickEvent* event);

	/** Assign control up event to handler */
	void setControlUpEvent(CControlUpEvent* event);

	/** Assign control down event to handler */
	void setControlDownEvent(CControlDownEvent* event);

	/** Assign shape drag event to handler */
	void setShapeDragEvent(CShapeDragEvent* event);

	/** Assign shape enter event to handler */
	void setShapeEnterEvent(CShapeEnterEvent* event);

	/** Assign shape over event to handler */
	void setShapeOverEvent(CShapeOverEvent* event);

	/** Assign shape leave event to handler */
	void setShapeLeaveEvent(CShapeLeaveEvent* event);

	/** Assign shape click event to handler */
	void setShapeClickEvent(CShapeClickEvent* event);

	/** Assign shape up event to handler */
	void setShapeUpEvent(CShapeUpEvent* event);

	/** Assign shape down event to handler */
	void setShapeDownEvent(CShapeDownEvent* event);

	virtual void doMouseUp(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseDown(int x, int y);

	// Implemeted events

	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
};
/** \example manip.cpp
 * This example illustrates how to use the CInteractionHandler
 * class.
 */

}