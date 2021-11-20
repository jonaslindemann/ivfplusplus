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

#include <ivf/Base.h>

#include <ivfwidget/Events.h>

namespace ivf {

IvfSmartPointer(CWidgetBase);

/**
 * Base class for all Ivf++ widgets
 * 
 * This class implements the standard functionality for handling
 * keyboard, mouse and drawing of a widget. The class should be used 
 * in combination with some kind of window/view class. The 
 * ivffltk library derives a widget class by combining
 * Fl_Gl_Window with CWidgetBase.
 *
 * Ivf++ 0.9.x introduces a new event model in the CWidgetBase
 * class. This model uses assignable event classes in addition of
 * the virtual methods previously used. This enables the use
 * of "handler" classes for supporting pluggable user interface
 * functionality. See the IvfEvent.h header file for a description
 * of the supported events in the CWidgetBase class. Events is
 * preferred way of implementing UI functionality in Ivf++ 0.9.x.
 *
 * When implementing new classes mouse, keyboard and drawing
 * events should be interfaced with the doXXX() methods. These
 * Will in turn call onXXX() methods and asssigned events, from
 * which the user can override the default behavior.
 *
 * The methods doEnableIdleProcessing(), doDisableIdleProcessing(),
 * doEnableTimeout() and doRedraw() should be overidden and
 * functionality for these implemented.
 *
 * @author Jonas Lindemann
 */
class IVFWIDGET_API CWidgetBase : public CBase {
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

	/** Enumerations for modifier status */
	enum TModifierType {
		MT_SHIFT,
		MT_CTRL,
		MT_ALT,
		MT_NONE
	};

private:
	bool m_initDone;
	bool m_doOverlay;
	int m_size[2];
	bool m_leftButtonDown;
	bool m_middleButtonDown;
	bool m_rightButtonDown;
	TModifierType m_modifierKey;

	bool m_idleProcessing;

	float m_timeouts[10];

	std::vector<CRenderEvent*>			m_renderEvents;
	std::vector<CClearEvent*>			m_clearEvents;
	CAppLoopEvent*						m_appLoopEvent;
	std::vector<CDestroyEvent*>			m_destroyEvents;
	std::vector<CFocusEvent*>			m_focusEvents;
	std::vector<CFunctionKeyEvent*>		m_functionKeyEvents;
	std::vector<CIdleEvent*>				m_idleEvents;
	std::vector<CInitContextEvent*>		m_initContextEvents;
	std::vector<CInitEvent*>				m_initEvents;
	std::vector<CInitOverlayEvent*>		m_initOverlayEvents;
	std::vector<CKeyboardEvent*>			m_keyboardEvents;
	std::vector<CModifierDownEvent*>		m_modifierDownEvents;
	std::vector<CModifierUpEvent*>		m_modifierUpEvents;
	std::vector<CMouseDownEvent*>		m_mouseDownEvents;
	std::vector<CMouseEnterEvent*>		m_mouseEnterEvents;
	std::vector<CMouseEvent*>			m_mouseEvents;
	std::vector<CMouseLeaveEvent*>		m_mouseLeaveEvents;
	std::vector<CMouseMoveEvent*>		m_mouseMoveEvents;
	std::vector<CMouseUpEvent*>			m_mouseUpEvents;
	std::vector<COverlayEvent*>			m_overlayEvents;
	std::vector<CResizeEvent*>			m_resizeEvents;
	std::vector<CUnFocusEvent*>			m_unFocusEvents;
	std::vector<CMenuItemEvent*>			m_menuItemEvents;

	CTimeoutEvent*						m_timeoutEvents[10];
public:
	/** Add menu item event */
	void addMenuItemEvent(CMenuItemEvent* event);

	/** Add render event */
	void addRenderEvent(CRenderEvent* event);

	/** Add clear event */
	void addClearEvent(CClearEvent* event);

	/** Assign application loop event */
	void assignAppLoopEvent(CAppLoopEvent* event);

	/** Add destroy event */
	void addDestroyEvent(CDestroyEvent* event);

	/** Add focus event */
	void addFocusEvent(CFocusEvent* event);

	/** Add function key event */
	void addFunctionKeyEvent(CFunctionKeyEvent* event);

	/** Add idle event */
	void addIdleEvent(CIdleEvent* event);

	/** Add init context event */
	void addInitContextEvent(CInitContextEvent* event);

	/** Add init event */
	void addInitEvent(CInitEvent* event);

	/** Add init overlay event */
	void addInitOverlayEvent(CInitOverlayEvent* event);

	/** Add keyboard event */
	void addKeyboardEvent(CKeyboardEvent* event);

	/** Add modifier down event */
	void addModifierDownEvent(CModifierDownEvent* event);

	/** Add modifier up event */
	void addModifierUpEvent(CModifierUpEvent* event);

	/** Add mouse down event */
	void addMouseDownEvent(CMouseDownEvent* event);

	/** Add mouse enter event */
	void addMouseEnterEvent(CMouseEnterEvent* event);

	/** Add mouse event */
	void addMouseEvent(CMouseEvent* event);

	/** Add mouse leave event */
	void addMouseLeaveEvent(CMouseLeaveEvent* event);

	/** Add mouse move event */
	void addMouseMoveEvent(CMouseMoveEvent* event);

	/** Add mouse up event */
	void addMouseUpEvent(CMouseUpEvent* event);

	/** Add overlay event */
	void addOverlayEvent(COverlayEvent* event);

	/** Add resize event */
	void addResizeEvent(CResizeEvent* event);

	/** Add unfocus event */
	void addUnFocusEvent(CUnFocusEvent* event);

	/** Assign timeout event */
	void assignTimeoutEvent(int nbr, CTimeoutEvent* event);

	/** Remove menu item event */
	void removeMenuItemEvent(CMenuItemEvent* event);

	/** Remove render event */
	void removeRenderEvent(CRenderEvent* event);

	/** Remove clear event */
	void removeClearEvent(CClearEvent* event);

	/** Remove destroy event */
	void removeDestroyEvent(CDestroyEvent* event);

	/** Remove focus event */
	void removeFocusEvent(CFocusEvent* event);

	/** Remove function key event */
	void removeFunctionKeyEvent(CFunctionKeyEvent* event);

	/** Remove idle event */
	void removeIdleEvent(CIdleEvent* event);

	/** Remove init context event */
	void removeInitContextEvent(CInitContextEvent* event);

	/** Remove init event */
	void removeInitEvent(CInitEvent* event);

	/** Remove init overlay event */
	void removeInitOverlayEvent(CInitOverlayEvent* event);

	/** Remove keyboard event */
	void removeKeyboardEvent(CKeyboardEvent* event);

	/** Remove modifier down event */
	void removeModifierDownEvent(CModifierDownEvent* event);

	/** Remove modifier up event */
	void removeModifierUpEvent(CModifierUpEvent* event);

	/** Remove mouse down event */
	void removeMouseDownEvent(CMouseDownEvent* event);

	/** Remove mouse enter event */
	void removeMouseEnterEvent(CMouseEnterEvent* event);

	/** Remove mouse event */
	void removeMouseEvent(CMouseEvent* event);

	/** Remove mouse leave event */
	void removeMouseLeaveEvent(CMouseLeaveEvent* event);

	/** Remove mouse move event */
	void removeMouseMoveEvent(CMouseMoveEvent* event);

	/** Remove mouse up event */
	void removeMouseUpEvent(CMouseUpEvent* event);

	/** Remove overlay event */
	void removeOverlayEvent(COverlayEvent* event);

	/** Remove resize event */
	void removeResizeEvent(CResizeEvent* event);

	/** Remove unfocus event */
	void removeUnFocusEvent(CUnFocusEvent* event);

	/** Remove timeout event */
	void removeTimeoutEvent(int nbr);

	/**
	 * CWidgetBase constructor
	 */
	CWidgetBase();

	/** CWidgetBase destructor */
	virtual ~CWidgetBase();

	IvfClassInfo("CWidgetBase", CBase);

	void redraw();

	/** Returns true if onInit() has been called */
	bool isInitDone();

	/** Returns true if middle button is down */
	bool isMiddleButtonDown();

	/** Returns true if right button is down */
	bool isRightButtonDown();

	/** Returns true if left button is down */
	bool isLeftButtonDown();

	/** Returns true if any button is down */
	bool isAnyButtonDown();

	/** Disables idle processing */
	void disableIdleProcessing();

	/** Enables idle processing */
	void enableIdleProcessing();

	/** Returns true if idle processing is enabled */
	bool isIdleProcessing();
	
	/** 
	 * Enables timeout 
	 *
	 * @param time interval in seconds when timeout should occur.
	 * @param nbr what timeout to enable.
	 */
	void enableTimeout(float time, int nbr);

	/**
	 * Enables timeout 
	 *
	 * @param time interval in seconds when timeout should occur.
	 * @param nbr what timeout to enable.
	 */
	void disableTimeout(int nbr);

	/** Returns true if timeout nbr is enabled */
	bool isTimeoutEnabled(int nbr);

	/** Return interval of specified timeout */
	float getTimeout(int number);

	/** Return what modifier key is down */
	TModifierType getModifierKey();

	/** Returns elasped time since onInit event */
	double elapsedTime();

	/** Set position of widget */
	void setPosition(int x, int y);

	/** Set size of widget */
	void setSize(int w, int h);

	int getWidth();
	int getHeight();
	
	void fullscreen();

	/** 
	 * Enable/disable use of overlay 
	 *
	 * When flag is set to true the widget will call the
	 * onOverlay event, for drawing the overlay.
	 */
	void setUseOverlay(bool flag);

	/** Return overlay status */
	bool getUseOverlay();

	/** Clears mouse status */
	void clearMouseStatus();

	/** Sets the modifier key */
	void setModifierKey(TModifierType key);

	/** Set middle mouse status */
	void setMiddleButtonStatus(bool down);

	/** Set right mouse status */
	void setRightButtonStatus(bool down);

	/** Set left mouse status */
	void setLeftButtonStatus(bool down);

	/**
	 * onInit event
	 * 
	 * onInit is called the first time an OpenGL context is 
	 * activated. All OpenGL dependent intialization code
	 * should be placed here, including Ivf++ code.
	 */
	virtual void onInit(int width, int height);

	/** 
	 * onDestroy event
	 *
	 * This method is called when the widget is destroyed.
	 */
	virtual void onDestroy();

	/**
	 * onInitContext event
	 *
	 * This event is called when an OpenGL context is initialized.
	 * OpenGL state initialization code should be placed here.
	 */
	virtual void onInitContext(int width, int height);

	/**
	 * onOverlay event
	 *
	 * This event is called when an overlay is to be drawn. For this
	 * method to be called, the UseOverlay property must be set using 
	 * the setUseOverlay method. The view port is configured for
	 * 2D drawing when this method is called.
	 */
	virtual void onOverlay();

	virtual void onMouse(int x, int y);

	/**
	 * onMouseDown event
	 *
	 * The onMouseDown event is called when any of the mouse
	 * buttons are pressed.
	 */
	virtual void onMouseDown(int x, int y);

	/**
	 * onMouseUp event
	 *
	 * The onMouse event is called when the mouse button is 
	 * released. 
	 */
	virtual void onMouseUp(int x, int y);

	/**
	 * onMouseMove event
	 * 
	 * The onMouseMove event is called whenever the mouse is moved in 
	 * the window.
	 */
	virtual void onMouseMove(int x, int y);

	/**
	 * onMouseLeave event
	 * 
	 * The onMouseLeave event is called when the mouse leaves 
	 * the window.
	 */
	virtual void onMouseLeave(int x, int y);

	/**
	 * onMouseEnter event
	 * 
	 * The onMouseEnter event is called when the mouse enters 
	 * the window.
	 */
	virtual void onMouseEnter(int x, int y);

	/**
	 * onKeyboard event
	 * 
	 * The onKeyboard event is called when a key on the
	 * keyboard is pressed 
	 */
	virtual void onKeyboard(int key, int x, int y);

	/**
	 * onModifierDown event
	 *
	 * The onModifier event is called when a modifier key
	 * is pressed (only supported on WIN32 for now)
	 */
	virtual void onModifierDown();

	/**
	 * onModifierUp event
	 *
	 * The onModifier event is called when a modifier key
	 * is released (only supported on WIN32 for now)
	 */
	virtual void onModifierUp();

	/**
	 * onInitOverlay event
	 * 
	 * The onInitOverlay event is called before the
	 * onOverlay event to setup the overlay projection 
	 * matrices.
	 */
	virtual void onInitOverlay(int width, int height);

	/**
	 * onRender event
	 *
	 * The onRender event is called whenever the widget
	 * needs to be redrawn.
	 */
	virtual void onRender();

	virtual void onMenuItem(int id);

	/**
	 * onClear event
	 *
	 * The onClear event is called when the widget needs to 
	 * be cleared. Typically a glClear() function should be
	 * called to clear the widget if overidden. If not 
	 * overidden, the widget is clear with the color black.
	 */
	virtual void onClear();

	/**
	 * onResize event
	 *
	 * The onResize event occurs when the widget has changed size.
	 * Typically code for initialising the viewport and perspective
	 * transformation goes here.
	 */
	virtual void onResize(int width, int height);

	/**
	 * onUnFocus event
	 *
	 * The onUnFocus event occurs when the widget has lost focus.
	 */
	virtual void onUnFocus();

	/**
	 * onFocus event
	 *
	 * The onFocus event occurs when the widget has received focus.
	 */
	virtual void onFocus();

	/**
	 * onFunctionKey event
	 *
	 * The onFunctionKey event is called when a function key is 
	 * pressed. See the TFunctionKey enumeration.
	 */
	virtual void onFunctionKey(TFunctionKey key, int x, int y);

	/**
	 * onIdle event 
	 *
	 * This event handles idle processing if it is turned on using
	 * enableIdleProcessing().
	 */
	virtual void onIdle();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout0();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout1();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout2();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout3();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout4();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout5();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout6();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout7();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout8();

	/**
	 * onTimeout event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onTimeout9();

	/**
	 * onAppLoop event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool onAppLoop();
	
	virtual void doFullscreen();

	/**
	 * Idle method processing 
	 *
	 * This method should be called from the derived widget
	 * when it is in idle processing. doIdle() will call
	 * the onIdle method.
	 */
	virtual void doIdle();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout0();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout1();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout2();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout3();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout4();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout5();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout6();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout7();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout8();

	/**
	 * Timeout processing (called from widget)
	 *
	 * This method should be called from the derived widget
	 * when a timeout event has occured. This method will inturn
	 * call onTimeoutX()
	 */
	virtual bool doTimeout9();

	/**
	 * doAppLoop event
	 *
	 * These events handles timer calls. If timeout should be repeated
	 * the return value should be true.
	 */
	virtual bool doAppLoop();

	virtual double doElapsedTime();

	/** Called by the doDraw() method */
	virtual void doInitOverlay(int width, int height);

	/** Called by the doDraw() method */
	virtual void doDestroy();

	/** Called by the doDraw() method */
	virtual void doOverlay();

	/** Called by doResize() */
	virtual void doInitContext(int width, int height);

	/** Called by doDraw() */
	virtual void doClear();

	/** Called by doDraw() */
	virtual void doRender();

	/** Called by doResize() */
	virtual void doInit(int width, int height);

	void doMenuItem(int id);

	/** 
	 * Enable idle processing (to be overidden)
	 *
	 * Derived widget should override this method to enable
	 * idle processing. Idle processing should then call doIdle().
	 */
	virtual void doDisableIdleProcessing();

	/** 
	 * Disable idle processing (to be overidden)
	 *
	 * Derived widget should override this method to disable
	 * idle processing.
	 */
	virtual void doEnableIdleProcessing();

	/** 
	 * Enable timeout callback (to be overidden)
	 *
	 * Derived widget should override this method to enable
	 * specified callback. 
	 */
	virtual void doEnableTimeout(float time, int nbr);

	/** 
	 * Disable timeout callback (to be overidden)
	 *
	 * Derived widget should override this method to disable
	 * specified callback. 
	 */
	virtual void doDisableTimeout(int nbr);
	
	/**
	 * Redraw operation (to be overidden)
	 *
	 * Derived widget should override this method to do
	 * a redraw of widget 
	 */
	virtual void doRedraw();

	/** 
	 * MouseEnter event handling (called from widget)
     *
	 * Derived widget should call this method when mouse
	 * enters the widget.
	 */
	virtual void doMouseEnter(int x, int y);

	/** 
	 * UnFocus event handling (called from widget)
     *
	 * Derived widget should call this method when widget
	 * looses focus.
	 */
	virtual void doUnFocus();

	/** 
	 * UnFocus event handling (called from widget)
     *
	 * Derived widget should call this method when widget
	 * received focus.
	 */
	virtual void doFocus();

	/** 
	 * MouseMove event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * the mouse is moved.
	 */
	virtual void doMouseMove(int x, int y);

	/** 
	 * MouseUp event handling (called from widget)
     *
	 * Derived widget should call this method when
	 * any of the mouse buttons is released.
	 */
	virtual void doMouseUp(int x, int y);

	/** 
	 * MouseDown event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * any of the mouse buttons is pressed.
	 */
	virtual void doMouseDown(int x, int y);

	/** 
	 * Draw event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * widget is to be redrawn.
	 */
	virtual void doDraw();

	/** 
	 * Resize event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * widget is resized.
	 */
	virtual void doResize(int width, int height);

	/** 
	 * MouseLeave event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * the mouse leaves the widget.
	 */
	virtual void doMouseLeave(int x, int y);

	/** 
	 * Keyboard event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * a key on the keyboard is pressed.
	 */
	virtual void doKeyboard(int key, int x, int y);

	/**
	 * A modifer key event handling
	 *
	 * Derived widgets should call this method when
	 * a modifier key is pressed.
	 */
	virtual void doModifierDown();

	/**
	 * A modifer key event handling
	 *
	 * Derived widgets should call this method when
	 * a modifier key is released.
	 */
	virtual void doModifierUp();

	/** 
	 * FunctionKey event handling (called from widget)
     *
	 * Derived widget should call this method when 
	 * a function key on the keyboard is pressed.
	 */
	virtual void doFunctionKey(TFunctionKey key, int x, int y);

	virtual void doSetPosition(int x, int y);
	virtual void doSetSize(int w, int h);
};
/** \example robot_events.cpp
 * This is an example of how to use CWidgetBase derived
 * classes with the new event model.
 */
}