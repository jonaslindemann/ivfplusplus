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

#include <ivfdef/Def.h>

namespace ivf {

/**
 * Base class of events
 *
 * Events in Ivf++ are simple classes with a
 * single empty public virtual method defining
 * the event method. @see WidgetBase.
 */
class IVFWIDGET_API EventBase {
public:
	/** Function key enumerations */
	enum TFunctionKey {
		FK_BUTTON,
		FK_BACKSPACE,
		FK_TAB,
		FK_ENTER,
		FK_PAUSE,
		FK_SCROLL_LOCK,
		FK_ESCAPE,
		FK_HOME,
		FK_LEFT,
		FK_UP,
		FK_RIGHT,
		FK_DOWN,
		FK_PAGE_UP,
		FK_PAGE_DOWN,
		FK_END,
		FK_PRINT,
		FK_INSERT,
		FK_MENU,
		FK_NUM_LOCK,
		FK_F1,
		FK_F2,
		FK_F3,
		FK_F4,
		FK_F5,
		FK_F6,
		FK_F7,
		FK_F8,
		FK_F9,
		FK_F10,
		FK_F11,
		FK_F12,
		FK_KP0,
		FK_KP1,
		FK_KP2,
		FK_KP3,
		FK_KP4,
		FK_KP5,
		FK_KP6,
		FK_KP7,
		FK_KP8,
		FK_KP9,
		FK_KP_ENTER,
		FK_SHIFT_L,
		FK_SHIFT_R,
		FK_CONTROL_L,
		FK_CONTROL_R,
		FK_CAPS_LOCK,
		FK_META_L,
		FK_META_R,
		FK_ALT_L,
		FK_ALT_R,
		FK_DELETE
	};
};

/**
 * Render event class
 */
class IVFWIDGET_API RenderEvent : public EventBase {
public:
	/**
	 * onRender method
	 *
	 * The onRender method is called whenever the widget
	 * needs to be redrawn.
	 */
	virtual void onRender() {};
};

/**
 * Init event class
 */
class IVFWIDGET_API InitEvent : public EventBase {
public:
	/**
	 * onInit method
	 * 
	 * onInit is called the first time an OpenGL context is 
	 * activated. All OpenGL dependent intialization code
	 * should be placed here, including Ivf++ code.
	 */
	virtual void onInit(int width, int height) {};
};

/**
 * Destroy event class
 */
class IVFWIDGET_API DestroyEvent : public EventBase {
public:
	/** 
	 * onDestroy event
	 *
	 * This method is called when the widget is destroyed.
	 */
	virtual void onDestroy() {};
};

/**
 * Init context event class
 */
class IVFWIDGET_API InitContextEvent : public EventBase {
public:
	/**
	 * onInitContext method
	 *
	 * This method is called when an OpenGL context is initialized.
	 * OpenGL state initialization code should be placed here.
	 */
	virtual void onInitContext(int width, int height) {};
};

/**
 * Overlay event class
 */
class IVFWIDGET_API OverlayEvent : public EventBase {
public:
	/**
	 * onOverlay method
	 *
	 * This method is called when an overlay is to be drawn. For this
	 * method to be called, the UseOverlay property must be set using 
	 * the setUseOverlay method. The view port is configured for
	 * 2D drawing when this method is called.
	 */
	virtual void onOverlay() {};
};

/**
 * Mouse event class
 */
class IVFWIDGET_API MouseEvent : public EventBase {
public:
	/**
	 * onMouseDown method
	 *
	 * The onMouseDown method is called when any of the mouse
	 * buttons are pressed.
	 */
	virtual void onMouse(int x, int y) {};
};

/**
 * Mouse down event class
 */
class IVFWIDGET_API MouseDownEvent : public EventBase {
public:
	/**
	 * onMouseUp method
	 *
	 * The onMouse method is called when the mouse button is 
	 * released. 
	 */
	virtual void onMouseDown(int x, int y) {};
};

/**
 * Mouse up event class
 */
class IVFWIDGET_API MouseUpEvent : public EventBase {
public:
	/**
	 * onMouseMove method
	 * 
	 * The onMouseMove method is called whenever the mouse is moved in 
	 * the window.
	 */
	virtual void onMouseUp(int x, int y) {};
};

/**
 * Mouse move event class
 */
class IVFWIDGET_API MouseMoveEvent : public EventBase {
public:
	/**
	 * onMouseMove method
	 * 
	 * The onMouseMove method is called whenever the mouse is moved in 
	 * the window.
	 */
	virtual void onMouseMove(int x, int y) {};
};

/**
 * Mouse leave event class
 */
class IVFWIDGET_API MouseLeaveEvent : public EventBase {
public:
	/**
	 * onMouseLeave method
	 * 
	 * The onMouseLeave event is called when the mouse leaves 
	 * the window.
	 */
	virtual void onMouseLeave(int x, int y) {};
};

/**
 * Mouse leave event class
 */
class IVFWIDGET_API MouseEnterEvent : public EventBase {
public:
	/**
	 * onMouseEnter method
	 * 
	 * The onMouseEnter method is called when the mouse enters 
	 * the window.
	 */
	virtual void onMouseEnter(int x, int y) {};
};

/**
 * Keyboard event class
 */
class IVFWIDGET_API KeyboardEvent : public EventBase {
public:
	/**
	 * onKeyboard method
	 * 
	 * The onKeyboard method is called when a key on the
	 * keyboard is pressed 
	 */
	virtual void onKeyboard(int key, int x, int y) {};
};

/**
 * Menu event class
 */
class IVFWIDGET_API MenuItemEvent : public EventBase {
public:
	virtual void onMenuItem(int id) {};
};

/**
 * Modifier event class
 */
class IVFWIDGET_API ModifierDownEvent : public EventBase {
public:
	/**
	 * onModifierDown method
	 *
	 * The onModifier method is called when a modifier key
	 * is pressed (only supported on WIN32 for now)
	 */
	virtual void onModifierDown() {};
};

/**
 * Modifier event class
 */
class IVFWIDGET_API ModifierUpEvent : public EventBase {
public:
	/**
	 * onModifierUp method
	 *
	 * The onModifier method is called when a modifier key
	 * is released (only supported on WIN32 for now)
	 */
	virtual void onModifierUp() {};
};

/**
 * Init overlay event
 */
class IVFWIDGET_API InitOverlayEvent : public EventBase {
public:
	/**
	 * onInitOverlay method
	 * 
	 * The onInitOverlay method is called before the
	 * onOverlay event to setup the overlay projection 
	 * matrices.
	 */
	virtual void onInitOverlay(int width, int height) {};
};

/**
 * Clear event class
 */
class IVFWIDGET_API ClearEvent : public EventBase {
public:
	/**
	 * onClear method
	 *
	 * The onClear method is called when the widget needs to 
	 * be cleared. Typically a glClear() function should be
	 * called to clear the widget if overidden. If not 
	 * overidden, the widget is clear with the color black.
	 */
	virtual void onClear() {};
};

/**
 * Resize event class
 */
class IVFWIDGET_API ResizeEvent : public EventBase {
public:
	/**
	 * onResize method
	 *
	 * The onResize method occurs when the widget has changed size.
	 * Typically code for initialising the viewport and perspective
	 * transformation goes here.
	 */
	virtual void onResize(int width, int height) {};
};

/**
 * Unfocus event class
 */
class IVFWIDGET_API UnFocusEvent : public EventBase {
public:
	/**
	 * onUnFocus method
	 *
	 * The onUnFocus method occurs when the widget has lost focus.
	 */
	virtual void onUnFocus() {};
};

/**
 * Focus event class
 */
class IVFWIDGET_API FocusEvent : public EventBase {
public:
	/**
	 * onFocus method
	 *
	 * The onFocus method occurs when the widget has received focus.
	 */
	virtual void onFocus() {};
};

/**
 * Function key event class
 */
class IVFWIDGET_API FunctionKeyEvent : public EventBase {
public:
	/**
	 * onFunctionKey method
	 *
	 * The onFunctionKey method is called when a function key is 
	 * pressed. See the TFunctionKey enumeration.
	 */
	virtual void onFunctionKey(EventBase::TFunctionKey key, int x, int y) {};
};

/**
 * Idle event class
 */
class IVFWIDGET_API IdleEvent : public EventBase {
public:
	/**
	 * onIdle method 
	 *
	 * This method handles idle processing if it is turned on using
	 * enableIdleProcessing().
	 */
	virtual void onIdle() {};
};

/**
 * Timeout event class
 */
class IVFWIDGET_API TimeoutEvent : public EventBase {
public:
	/**
	 * onTimeout method
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout() { return true; };
};

/**
 * Apploop event class
 */
class IVFWIDGET_API AppLoopEvent : public EventBase {
private:
	bool m_finished;
public:
	/**
	 * onAppLoop method
	 *
	 * This method is called repeatedly to implement a real-time
	 * rendering application. To terminate the loop the method
	 * should return false. 
	 */
	virtual bool onAppLoop() { m_finished = false; return true; };

	void setFinished(bool flag) { m_finished = flag; };
	bool isFinished() { return m_finished; };
};

}