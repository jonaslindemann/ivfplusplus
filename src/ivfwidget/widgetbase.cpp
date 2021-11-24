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


#include <ivfwidget/WidgetBase.h>

#include <ivf/GL.h>

using namespace ivf;

#define IvfDispatchNotify(events,eventMethod) \
	if (events.size()>0) \
	{ \
		unsigned int i; \
		for (i=0; i<events.size(); i++) \
			events[i]->eventMethod(); \
	} \
	else \
	{ \
		eventMethod(); \
	}

#define IvfDispatchTimeout(nbr) \
	if (m_timeoutEvents[nbr]!=nullptr) \
	{ \
		return m_timeoutEvents[nbr]->onTimeout(); \
	} \
	else \
	{ \
		return onTimeout##nbr(); \
	}

#define IvfDispatchPositional(events,eventMethod,x,y) \
	if (events.size()>0) \
	{ \
		unsigned int i; \
		for (i=0; i<events.size(); i++) \
			events[i]->eventMethod(x, y); \
	} \
	else \
	{ \
		eventMethod(x, y); \
	}

#define IvfDispatchDimensional(events,eventMethod,width,height) \
	if (events.size()>0) \
	{ \
		unsigned int i; \
		for (i=0; i<events.size(); i++) \
			events[i]->eventMethod(width, height); \
	} \
	else \
	{ \
		eventMethod(width, height); \
	}

#define IvfDispatchKeyboard(events,eventMethod,key,x,y) \
	if (events.size()>0) \
	{ \
		unsigned int i; \
		for (i=0; i<events.size(); i++) \
		events[i]->eventMethod((EventBase::TFunctionKey)key, x, y); \
	} \
	else \
	{ \
		eventMethod(key, x, y); \
	}

#define IvfDispatchMenuItem(events,eventMethod,id) \
	if (events.size()>0) \
	{ \
		unsigned int i; \
		for (i=0; i<events.size(); i++) \
		events[i]->eventMethod(id); \
	} \
	else \
	{ \
		eventMethod(id); \
	}

#define IvfRemoveEvent(eventClass,eventVector,event) \
	std::vector<eventClass*>::iterator vi; \
	for (vi=eventVector.begin(); vi<eventVector.end(); vi++) \
	{ \
		eventClass* e = *vi; \
		if (event==e) \
		{ \
			eventVector.erase(vi); \
			break; \
		} \
	}

WidgetBase::WidgetBase()
{
	m_initDone = false;
	m_doOverlay = false;

	m_leftButtonDown = false;
	m_rightButtonDown = false;
	m_middleButtonDown = false;

	m_modifierKey = MT_NONE;

	m_idleProcessing = false;
	m_appLoopEvent = nullptr;

	int i;

	for (i=0; i<10; i++)
	{
		m_timeouts[i] = -1.0f;
		m_timeoutEvents[i] = nullptr;
	}
}

WidgetBase::~WidgetBase()
{
	doDestroy();
}

bool WidgetBase::isLeftButtonDown()
{
	return m_leftButtonDown;
}

bool WidgetBase::isRightButtonDown()
{
	return m_rightButtonDown;
}

bool WidgetBase::isMiddleButtonDown()
{
	return m_middleButtonDown;
}

void WidgetBase::setLeftButtonStatus(bool down)
{
	m_leftButtonDown = down;
}

void WidgetBase::setRightButtonStatus(bool down)
{
	m_rightButtonDown = down;
}

void WidgetBase::setMiddleButtonStatus(bool down)
{
	m_middleButtonDown = down;
}

void WidgetBase::setModifierKey(TModifierType key)
{
	m_modifierKey = key;
}

void WidgetBase::clearMouseStatus()
{
	m_leftButtonDown = false;
	m_rightButtonDown = false;
	m_middleButtonDown = false;
}

int WidgetBase::getWidth()
{
	return m_size[0];
}

int WidgetBase::getHeight()
{
	return m_size[1];
}

void WidgetBase::fullscreen()
{
	doFullscreen();
}

//
// Implementation specific interface routines
//

void WidgetBase::doFullscreen()
{

}

void WidgetBase::doDraw()
{
	// Drawing code

	doClear();
	
	glPushMatrix();

	doRender();
	
	if (m_doOverlay)
	{
		glPushAttrib(GL_ENABLE_BIT);

		doInitOverlay(m_size[0], m_size[1]);
		doOverlay();

		glPopAttrib();

		if (m_initDone)
			doResize(m_size[0], m_size[1]);
	}
	
	glPopMatrix();
}

void WidgetBase::doResize(int width, int height)
{
	// Call initialisation callback
	
	m_size[0] = width;
	m_size[1] = height;

	if (!m_initDone)
	{
		m_initDone = true;
		doInit(width, height);
	}

	doInitContext(width, height);

	IvfDispatchDimensional(m_resizeEvents,onResize,width,height);
}

void WidgetBase::doMouseDown(int x, int y)
{
	IvfDispatchPositional(m_mouseDownEvents,onMouseDown,x,y);
}

void WidgetBase::doMouseUp(int x, int y)
{
	IvfDispatchPositional(m_mouseUpEvents,onMouseUp,x,y);
}

void WidgetBase::doMouseMove(int x, int y)
{
	IvfDispatchPositional(m_mouseMoveEvents,onMouseMove,x,y);
}

void WidgetBase::doFocus()
{
	IvfDispatchNotify(m_focusEvents,onFocus);
}

void WidgetBase::doUnFocus()
{
	IvfDispatchNotify(m_unFocusEvents,onUnFocus);
}

void WidgetBase::doMouseEnter(int x, int y)
{
	IvfDispatchPositional(m_mouseEnterEvents,onMouseEnter,x,y);
}

void WidgetBase::doMouseLeave(int x, int y)
{
	IvfDispatchPositional(m_mouseLeaveEvents,onMouseLeave,x,y);
}

void WidgetBase::doKeyboard(int key, int x, int y)
{
	IvfDispatchKeyboard(m_keyboardEvents,onKeyboard,key,x,y);
}

void WidgetBase::doModifierDown()
{
	IvfDispatchNotify(m_modifierDownEvents,onModifierDown);
}

void WidgetBase::doModifierUp()
{
	IvfDispatchNotify(m_modifierUpEvents,onModifierUp);
}

void WidgetBase::doFunctionKey(TFunctionKey key, int x, int y)
{
	IvfDispatchKeyboard(m_functionKeyEvents,onFunctionKey,key,x,y);
}

//
// Event methods
//

void WidgetBase::onInit(int width, int height)
{

}

void WidgetBase::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void WidgetBase::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WidgetBase::onResize(int width, int height)
{

}

void WidgetBase::onRender()
{

}

void WidgetBase::onInitOverlay(int width, int height)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)m_size[0], (double)m_size[1], 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WidgetBase::onDestroy()
{

}


void WidgetBase::onOverlay()
{

}

void WidgetBase::onMouse(int x, int y)
{

}

void WidgetBase::onMouseDown(int x, int y)
{

}

void WidgetBase::onMouseUp(int x, int y)
{

}

void WidgetBase::onFocus()
{

}

void WidgetBase::onUnFocus()
{

}

void WidgetBase::onMouseEnter(int x, int y)
{

}

void WidgetBase::onMouseLeave(int x, int y)
{

}

void WidgetBase::onMouseMove(int x, int y)
{

}

void WidgetBase::onKeyboard(int key, int x, int y)
{

}

void WidgetBase::onMenuItem(int id)
{

}

void WidgetBase::onModifierDown()
{

}

void WidgetBase::onModifierUp()
{

}

void WidgetBase::onFunctionKey(TFunctionKey key, int x, int y)
{

}


WidgetBase::TModifierType WidgetBase::getModifierKey()
{
	return m_modifierKey;
}

bool WidgetBase::isAnyButtonDown()
{
	return (isLeftButtonDown())||(isRightButtonDown())||(isMiddleButtonDown());
}

void WidgetBase::onIdle()
{

}

void WidgetBase::doIdle()
{
	onIdle();
}

void WidgetBase::enableIdleProcessing()
{
	doEnableIdleProcessing();
	m_idleProcessing = true;
}

void WidgetBase::disableIdleProcessing()
{
	doDisableIdleProcessing();
	m_idleProcessing = false;
}

void WidgetBase::doEnableIdleProcessing()
{

}

void WidgetBase::doDisableIdleProcessing()
{

}

bool WidgetBase::isIdleProcessing()
{
	return m_idleProcessing;
}

void WidgetBase::enableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = time;
		doEnableTimeout(time, nbr);
	}
}

void WidgetBase::disableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = -1.0f;
		doDisableTimeout(nbr);
	}
}

void WidgetBase::doEnableTimeout(float time, int nbr)
{

}

bool WidgetBase::onTimeout0()
{
	return false;
}

bool WidgetBase::onTimeout1()
{
	return false;
}

bool WidgetBase::onTimeout2()
{
	return false;
}

bool WidgetBase::onTimeout3()
{
	return false;
}

bool WidgetBase::onTimeout4()
{
	return false;
}

bool WidgetBase::onTimeout5()
{
	return false;
}

bool WidgetBase::onTimeout6()
{
	return false;
}

bool WidgetBase::onTimeout7()
{
	return false;
}

bool WidgetBase::onTimeout8()
{
	return false;
}

bool WidgetBase::onTimeout9()
{
	return false;
}

bool WidgetBase::doTimeout0()
{
	IvfDispatchTimeout(0);
}

bool WidgetBase::doTimeout1()
{
	IvfDispatchTimeout(1);
}

bool WidgetBase::doTimeout2()
{
	IvfDispatchTimeout(2);
}

bool WidgetBase::doTimeout3()
{
	IvfDispatchTimeout(3);
}

bool WidgetBase::doTimeout4()
{
	IvfDispatchTimeout(4);
}

bool WidgetBase::doTimeout5()
{
	IvfDispatchTimeout(5);
}

bool WidgetBase::doTimeout6()
{
	IvfDispatchTimeout(6);
}

bool WidgetBase::doTimeout7()
{
	IvfDispatchTimeout(7);
}

bool WidgetBase::doTimeout8()
{
	IvfDispatchTimeout(8);
}

bool WidgetBase::doTimeout9()
{
	IvfDispatchTimeout(9);
}

float WidgetBase::getTimeout(int number)
{
	if ((number>=0)&&(number<10))
		return m_timeouts[number];
	else
		return -1.0;
}

void WidgetBase::doRedraw()
{

}

void WidgetBase::doInit(int width, int height)
{
	IvfDispatchDimensional(m_initEvents,onInit,width,height);
}

void WidgetBase::doInitContext(int width, int height)
{
	IvfDispatchDimensional(m_initContextEvents,onInitContext,width,height);
}

void WidgetBase::doRender()
{
	IvfDispatchNotify(m_renderEvents,onRender);
}

void WidgetBase::doClear()
{
	IvfDispatchNotify(m_clearEvents,onClear);
}

void WidgetBase::doInitOverlay(int width, int height)
{
	IvfDispatchDimensional(m_initOverlayEvents,onInitOverlay,width,height);
}

void WidgetBase::doOverlay()
{
	IvfDispatchNotify(m_overlayEvents,onOverlay);
}

void WidgetBase::doDestroy()
{
	IvfDispatchNotify(m_destroyEvents,onDestroy);
}

void WidgetBase::doMenuItem(int id)
{
	IvfDispatchMenuItem(m_menuItemEvents,onMenuItem,id)	
}

bool WidgetBase::isInitDone()
{
	return m_initDone;
}

void WidgetBase::doDisableTimeout(int nbr)
{

}


bool WidgetBase::isTimeoutEnabled(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		return m_timeouts[nbr]>=0.0f;
	else
		return false;
}

void WidgetBase::setUseOverlay(bool flag)
{
	m_doOverlay = flag;
}

bool WidgetBase::getUseOverlay()
{
	return m_doOverlay;
}

void WidgetBase::redraw()
{
	doRedraw();
}

double WidgetBase::elapsedTime()
{
	return doElapsedTime();
}

double WidgetBase::doElapsedTime()
{
	return 0.0;
}

void WidgetBase::setSize(int w, int h)
{
	doSetSize(w, h);
	doResize(w, h);
}

void WidgetBase::setPosition(int x, int y)
{
	doSetPosition(x, y);
}

void WidgetBase::doSetSize(int w, int h)
{

}

void WidgetBase::doSetPosition(int x, int y)
{

}

bool WidgetBase::doAppLoop()
{
	if (m_initDone)
	{
		if (m_appLoopEvent!=nullptr)
			return m_appLoopEvent->onAppLoop();
		else	
			return onAppLoop();
	}
	else
		return true;
}

bool WidgetBase::onAppLoop()
{
	return true;
}

void WidgetBase::addRenderEvent(RenderEvent *event)
{
	m_renderEvents.push_back(event);
}

void WidgetBase::addClearEvent(ClearEvent* event)
{
	m_clearEvents.push_back(event);
}

void WidgetBase::assignAppLoopEvent(AppLoopEvent* event)
{
	m_appLoopEvent = event;
}

void WidgetBase::addDestroyEvent(DestroyEvent* event)
{
	m_destroyEvents.push_back(event);
}

void WidgetBase::addFocusEvent(FocusEvent* event)
{
	m_focusEvents.push_back(event);
}

void WidgetBase::addFunctionKeyEvent(FunctionKeyEvent* event)
{
	m_functionKeyEvents.push_back(event);
}

void WidgetBase::addIdleEvent(IdleEvent* event)
{
	m_idleEvents.push_back(event);
}

void WidgetBase::addInitContextEvent(InitContextEvent* event)
{
	m_initContextEvents.push_back(event);
}

void WidgetBase::addInitEvent(InitEvent* event)
{
	m_initEvents.push_back(event);
}

void WidgetBase::addInitOverlayEvent(InitOverlayEvent* event)
{
	m_initOverlayEvents.push_back(event);
}

void WidgetBase::addKeyboardEvent(KeyboardEvent* event)
{
	m_keyboardEvents.push_back(event);
}

void WidgetBase::addMouseDownEvent(MouseDownEvent* event)
{
	m_mouseDownEvents.push_back(event);
}

void WidgetBase::addMouseEnterEvent(MouseEnterEvent* event)
{
	m_mouseEnterEvents.push_back(event);
}

void WidgetBase::addMouseEvent(MouseEvent* event)
{
	m_mouseEvents.push_back(event);
}

void WidgetBase::addMouseLeaveEvent(MouseLeaveEvent* event)
{
	m_mouseLeaveEvents.push_back(event);
}

void WidgetBase::addMouseMoveEvent(MouseMoveEvent* event)
{
	m_mouseMoveEvents.push_back(event);
}

void WidgetBase::addMouseUpEvent(MouseUpEvent* event)
{
	m_mouseUpEvents.push_back(event);
}

void WidgetBase::addOverlayEvent(OverlayEvent* event)
{
	m_overlayEvents.push_back(event);
}

void WidgetBase::addResizeEvent(ResizeEvent* event)
{
	m_resizeEvents.push_back(event);
}

void WidgetBase::addUnFocusEvent(UnFocusEvent* event)
{
	m_unFocusEvents.push_back(event);
}

void WidgetBase::removeRenderEvent(RenderEvent* event)
{
	IvfRemoveEvent(RenderEvent,m_renderEvents,event);
}

void WidgetBase::removeClearEvent(ClearEvent* event)
{
	IvfRemoveEvent(ClearEvent,m_clearEvents,event);
}

void WidgetBase::removeDestroyEvent(DestroyEvent* event)
{
	IvfRemoveEvent(DestroyEvent,m_destroyEvents,event);
}

void WidgetBase::removeFocusEvent(FocusEvent* event)
{
	IvfRemoveEvent(FocusEvent,m_focusEvents,event);
}

void WidgetBase::removeFunctionKeyEvent(FunctionKeyEvent* event)
{
	IvfRemoveEvent(FunctionKeyEvent,m_functionKeyEvents,event);
}

void WidgetBase::removeIdleEvent(IdleEvent* event)
{
	IvfRemoveEvent(IdleEvent,m_idleEvents,event);
}

void WidgetBase::removeInitContextEvent(InitContextEvent* event)
{
	IvfRemoveEvent(InitContextEvent,m_initContextEvents,event);
}

void WidgetBase::removeInitEvent(InitEvent* event)
{
	IvfRemoveEvent(InitEvent,m_initEvents,event);
}

void WidgetBase::removeInitOverlayEvent(InitOverlayEvent* event)
{
	IvfRemoveEvent(InitOverlayEvent,m_initOverlayEvents,event);
}

void WidgetBase::removeKeyboardEvent(KeyboardEvent* event)
{
	IvfRemoveEvent(KeyboardEvent,m_keyboardEvents,event);
}

void WidgetBase::removeMouseDownEvent(MouseDownEvent* event)
{
	IvfRemoveEvent(MouseDownEvent,m_mouseDownEvents,event);
}

void WidgetBase::removeMouseEnterEvent(MouseEnterEvent* event)
{
	IvfRemoveEvent(MouseEnterEvent,m_mouseEnterEvents,event);
}

void WidgetBase::removeMouseEvent(MouseEvent* event)
{
	IvfRemoveEvent(MouseEvent,m_mouseEvents,event);
}

void WidgetBase::removeMouseLeaveEvent(MouseLeaveEvent* event)
{
	IvfRemoveEvent(MouseLeaveEvent,m_mouseLeaveEvents,event);
}

void WidgetBase::removeMouseMoveEvent(MouseMoveEvent* event)
{
	IvfRemoveEvent(MouseMoveEvent,m_mouseMoveEvents,event);
}

void WidgetBase::removeMouseUpEvent(MouseUpEvent* event)
{
	IvfRemoveEvent(MouseUpEvent,m_mouseUpEvents,event);
}

void WidgetBase::removeOverlayEvent(OverlayEvent* event)
{
	IvfRemoveEvent(OverlayEvent,m_overlayEvents,event);
}

void WidgetBase::removeResizeEvent(ResizeEvent* event)
{
	IvfRemoveEvent(ResizeEvent,m_resizeEvents,event);
}

void WidgetBase::removeUnFocusEvent(UnFocusEvent* event)
{
	IvfRemoveEvent(UnFocusEvent,m_unFocusEvents,event);
}

void WidgetBase::assignTimeoutEvent(int nbr, TimeoutEvent* event)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = event;
}

void WidgetBase::removeTimeoutEvent(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = nullptr;
}

void WidgetBase::addModifierDownEvent(ModifierDownEvent *event)
{
	m_modifierDownEvents.push_back(event);
}

void WidgetBase::removeModifierDownEvent(ModifierDownEvent *event)
{
	IvfRemoveEvent(ModifierDownEvent,m_modifierDownEvents,event);
}

void WidgetBase::addModifierUpEvent(ModifierUpEvent *event)
{
	m_modifierUpEvents.push_back(event);
}

void WidgetBase::removeModifierUpEvent(ModifierUpEvent *event)
{
	IvfRemoveEvent(ModifierUpEvent,m_modifierUpEvents,event);
}

void WidgetBase::addMenuItemEvent(MenuItemEvent *event)
{
	m_menuItemEvents.push_back(event);
}

void WidgetBase::removeMenuItemEvent(MenuItemEvent *event)
{
	IvfRemoveEvent(MenuItemEvent,m_menuItemEvents,event);
}
