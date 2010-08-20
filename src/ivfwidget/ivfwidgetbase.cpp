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


#include <ivfwidget/IvfWidgetBase.h>

#include <ivf/IvfGL.h>

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
	if (m_timeoutEvents[nbr]!=NULL) \
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
		events[i]->eventMethod((CIvfEventBase::TFunctionKey)key, x, y); \
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

CIvfWidgetBase::CIvfWidgetBase()
{
	m_initDone = false;
	m_doOverlay = false;

	m_leftButtonDown = false;
	m_rightButtonDown = false;
	m_middleButtonDown = false;

	m_modifierKey = MT_NONE;

	m_idleProcessing = false;
	m_appLoopEvent = NULL;

	int i;

	for (i=0; i<10; i++)
	{
		m_timeouts[i] = -1.0f;
		m_timeoutEvents[i] = NULL;
	}
}

CIvfWidgetBase::~CIvfWidgetBase()
{
	doDestroy();
}

bool CIvfWidgetBase::isLeftButtonDown()
{
	return m_leftButtonDown;
}

bool CIvfWidgetBase::isRightButtonDown()
{
	return m_rightButtonDown;
}

bool CIvfWidgetBase::isMiddleButtonDown()
{
	return m_middleButtonDown;
}

void CIvfWidgetBase::setLeftButtonStatus(bool down)
{
	m_leftButtonDown = down;
}

void CIvfWidgetBase::setRightButtonStatus(bool down)
{
	m_rightButtonDown = down;
}

void CIvfWidgetBase::setMiddleButtonStatus(bool down)
{
	m_middleButtonDown = down;
}

void CIvfWidgetBase::setModifierKey(TModifierType key)
{
	m_modifierKey = key;
}

void CIvfWidgetBase::clearMouseStatus()
{
	m_leftButtonDown = false;
	m_rightButtonDown = false;
	m_middleButtonDown = false;
}

int CIvfWidgetBase::getWidth()
{
	return m_size[0];
}

int CIvfWidgetBase::getHeight()
{
	return m_size[1];
}

void CIvfWidgetBase::fullscreen()
{
	doFullscreen();
}

//
// Implementation specific interface routines
//

void CIvfWidgetBase::doFullscreen()
{

}

void CIvfWidgetBase::doDraw()
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

void CIvfWidgetBase::doResize(int width, int height)
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

void CIvfWidgetBase::doMouseDown(int x, int y)
{
	IvfDispatchPositional(m_mouseDownEvents,onMouseDown,x,y);
}

void CIvfWidgetBase::doMouseUp(int x, int y)
{
	IvfDispatchPositional(m_mouseUpEvents,onMouseUp,x,y);
}

void CIvfWidgetBase::doMouseMove(int x, int y)
{
	IvfDispatchPositional(m_mouseMoveEvents,onMouseMove,x,y);
}

void CIvfWidgetBase::doFocus()
{
	IvfDispatchNotify(m_focusEvents,onFocus);
}

void CIvfWidgetBase::doUnFocus()
{
	IvfDispatchNotify(m_unFocusEvents,onUnFocus);
}

void CIvfWidgetBase::doMouseEnter(int x, int y)
{
	IvfDispatchPositional(m_mouseEnterEvents,onMouseEnter,x,y);
}

void CIvfWidgetBase::doMouseLeave(int x, int y)
{
	IvfDispatchPositional(m_mouseLeaveEvents,onMouseLeave,x,y);
}

void CIvfWidgetBase::doKeyboard(int key, int x, int y)
{
	IvfDispatchKeyboard(m_keyboardEvents,onKeyboard,key,x,y);
}

void CIvfWidgetBase::doModifierDown()
{
	IvfDispatchNotify(m_modifierDownEvents,onModifierDown);
}

void CIvfWidgetBase::doModifierUp()
{
	IvfDispatchNotify(m_modifierUpEvents,onModifierUp);
}

void CIvfWidgetBase::doFunctionKey(TFunctionKey key, int x, int y)
{
	IvfDispatchKeyboard(m_functionKeyEvents,onFunctionKey,key,x,y);
}

//
// Event methods
//

void CIvfWidgetBase::onInit(int width, int height)
{

}

void CIvfWidgetBase::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CIvfWidgetBase::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CIvfWidgetBase::onResize(int width, int height)
{

}

void CIvfWidgetBase::onRender()
{

}

void CIvfWidgetBase::onInitOverlay(int width, int height)
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

void CIvfWidgetBase::onDestroy()
{

}


void CIvfWidgetBase::onOverlay()
{

}

void CIvfWidgetBase::onMouse(int x, int y)
{

}

void CIvfWidgetBase::onMouseDown(int x, int y)
{

}

void CIvfWidgetBase::onMouseUp(int x, int y)
{

}

void CIvfWidgetBase::onFocus()
{

}

void CIvfWidgetBase::onUnFocus()
{

}

void CIvfWidgetBase::onMouseEnter(int x, int y)
{

}

void CIvfWidgetBase::onMouseLeave(int x, int y)
{

}

void CIvfWidgetBase::onMouseMove(int x, int y)
{

}

void CIvfWidgetBase::onKeyboard(int key, int x, int y)
{

}

void CIvfWidgetBase::onMenuItem(int id)
{

}

void CIvfWidgetBase::onModifierDown()
{

}

void CIvfWidgetBase::onModifierUp()
{

}

void CIvfWidgetBase::onFunctionKey(TFunctionKey key, int x, int y)
{

}


CIvfWidgetBase::TModifierType CIvfWidgetBase::getModifierKey()
{
	return m_modifierKey;
}

bool CIvfWidgetBase::isAnyButtonDown()
{
	return (isLeftButtonDown())||(isRightButtonDown())||(isMiddleButtonDown());
}

void CIvfWidgetBase::onIdle()
{

}

void CIvfWidgetBase::doIdle()
{
	onIdle();
}

void CIvfWidgetBase::enableIdleProcessing()
{
	doEnableIdleProcessing();
	m_idleProcessing = true;
}

void CIvfWidgetBase::disableIdleProcessing()
{
	doDisableIdleProcessing();
	m_idleProcessing = false;
}

void CIvfWidgetBase::doEnableIdleProcessing()
{

}

void CIvfWidgetBase::doDisableIdleProcessing()
{

}

bool CIvfWidgetBase::isIdleProcessing()
{
	return m_idleProcessing;
}

void CIvfWidgetBase::enableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = time;
		doEnableTimeout(time, nbr);
	}
}

void CIvfWidgetBase::disableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = -1.0f;
		doDisableTimeout(nbr);
	}
}

void CIvfWidgetBase::doEnableTimeout(float time, int nbr)
{

}

bool CIvfWidgetBase::onTimeout0()
{
	return false;
}

bool CIvfWidgetBase::onTimeout1()
{
	return false;
}

bool CIvfWidgetBase::onTimeout2()
{
	return false;
}

bool CIvfWidgetBase::onTimeout3()
{
	return false;
}

bool CIvfWidgetBase::onTimeout4()
{
	return false;
}

bool CIvfWidgetBase::onTimeout5()
{
	return false;
}

bool CIvfWidgetBase::onTimeout6()
{
	return false;
}

bool CIvfWidgetBase::onTimeout7()
{
	return false;
}

bool CIvfWidgetBase::onTimeout8()
{
	return false;
}

bool CIvfWidgetBase::onTimeout9()
{
	return false;
}

bool CIvfWidgetBase::doTimeout0()
{
	IvfDispatchTimeout(0);
}

bool CIvfWidgetBase::doTimeout1()
{
	IvfDispatchTimeout(1);
}

bool CIvfWidgetBase::doTimeout2()
{
	IvfDispatchTimeout(2);
}

bool CIvfWidgetBase::doTimeout3()
{
	IvfDispatchTimeout(3);
}

bool CIvfWidgetBase::doTimeout4()
{
	IvfDispatchTimeout(4);
}

bool CIvfWidgetBase::doTimeout5()
{
	IvfDispatchTimeout(5);
}

bool CIvfWidgetBase::doTimeout6()
{
	IvfDispatchTimeout(6);
}

bool CIvfWidgetBase::doTimeout7()
{
	IvfDispatchTimeout(7);
}

bool CIvfWidgetBase::doTimeout8()
{
	IvfDispatchTimeout(8);
}

bool CIvfWidgetBase::doTimeout9()
{
	IvfDispatchTimeout(9);
}

float CIvfWidgetBase::getTimeout(int number)
{
	if ((number>=0)&&(number<10))
		return m_timeouts[number];
	else
		return -1.0;
}

void CIvfWidgetBase::doRedraw()
{

}

void CIvfWidgetBase::doInit(int width, int height)
{
	IvfDispatchDimensional(m_initEvents,onInit,width,height);
}

void CIvfWidgetBase::doInitContext(int width, int height)
{
	IvfDispatchDimensional(m_initContextEvents,onInitContext,width,height);
}

void CIvfWidgetBase::doRender()
{
	IvfDispatchNotify(m_renderEvents,onRender);
}

void CIvfWidgetBase::doClear()
{
	IvfDispatchNotify(m_clearEvents,onClear);
}

void CIvfWidgetBase::doInitOverlay(int width, int height)
{
	IvfDispatchDimensional(m_initOverlayEvents,onInitOverlay,width,height);
}

void CIvfWidgetBase::doOverlay()
{
	IvfDispatchNotify(m_overlayEvents,onOverlay);
}

void CIvfWidgetBase::doDestroy()
{
	IvfDispatchNotify(m_destroyEvents,onDestroy);
}

void CIvfWidgetBase::doMenuItem(int id)
{
	IvfDispatchMenuItem(m_menuItemEvents,onMenuItem,id)	
}

bool CIvfWidgetBase::isInitDone()
{
	return m_initDone;
}

void CIvfWidgetBase::doDisableTimeout(int nbr)
{

}


bool CIvfWidgetBase::isTimeoutEnabled(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		return m_timeouts[nbr]>=0.0f;
	else
		return false;
}

void CIvfWidgetBase::setUseOverlay(bool flag)
{
	m_doOverlay = flag;
}

bool CIvfWidgetBase::getUseOverlay()
{
	return m_doOverlay;
}

void CIvfWidgetBase::redraw()
{
	doRedraw();
}

double CIvfWidgetBase::elapsedTime()
{
	return doElapsedTime();
}

double CIvfWidgetBase::doElapsedTime()
{
	return 0.0;
}

void CIvfWidgetBase::setSize(int w, int h)
{
	doSetSize(w, h);
	doResize(w, h);
}

void CIvfWidgetBase::setPosition(int x, int y)
{
	doSetPosition(x, y);
}

void CIvfWidgetBase::doSetSize(int w, int h)
{

}

void CIvfWidgetBase::doSetPosition(int x, int y)
{

}

bool CIvfWidgetBase::doAppLoop()
{
	if (m_initDone)
	{
		if (m_appLoopEvent!=NULL)
			return m_appLoopEvent->onAppLoop();
		else	
			return onAppLoop();
	}
	else
		return true;
}

bool CIvfWidgetBase::onAppLoop()
{
	return true;
}

void CIvfWidgetBase::addRenderEvent(CIvfRenderEvent *event)
{
	m_renderEvents.push_back(event);
}

void CIvfWidgetBase::addClearEvent(CIvfClearEvent* event)
{
	m_clearEvents.push_back(event);
}

void CIvfWidgetBase::assignAppLoopEvent(CIvfAppLoopEvent* event)
{
	m_appLoopEvent = event;
}

void CIvfWidgetBase::addDestroyEvent(CIvfDestroyEvent* event)
{
	m_destroyEvents.push_back(event);
}

void CIvfWidgetBase::addFocusEvent(CIvfFocusEvent* event)
{
	m_focusEvents.push_back(event);
}

void CIvfWidgetBase::addFunctionKeyEvent(CIvfFunctionKeyEvent* event)
{
	m_functionKeyEvents.push_back(event);
}

void CIvfWidgetBase::addIdleEvent(CIvfIdleEvent* event)
{
	m_idleEvents.push_back(event);
}

void CIvfWidgetBase::addInitContextEvent(CIvfInitContextEvent* event)
{
	m_initContextEvents.push_back(event);
}

void CIvfWidgetBase::addInitEvent(CIvfInitEvent* event)
{
	m_initEvents.push_back(event);
}

void CIvfWidgetBase::addInitOverlayEvent(CIvfInitOverlayEvent* event)
{
	m_initOverlayEvents.push_back(event);
}

void CIvfWidgetBase::addKeyboardEvent(CIvfKeyboardEvent* event)
{
	m_keyboardEvents.push_back(event);
}

void CIvfWidgetBase::addMouseDownEvent(CIvfMouseDownEvent* event)
{
	m_mouseDownEvents.push_back(event);
}

void CIvfWidgetBase::addMouseEnterEvent(CIvfMouseEnterEvent* event)
{
	m_mouseEnterEvents.push_back(event);
}

void CIvfWidgetBase::addMouseEvent(CIvfMouseEvent* event)
{
	m_mouseEvents.push_back(event);
}

void CIvfWidgetBase::addMouseLeaveEvent(CIvfMouseLeaveEvent* event)
{
	m_mouseLeaveEvents.push_back(event);
}

void CIvfWidgetBase::addMouseMoveEvent(CIvfMouseMoveEvent* event)
{
	m_mouseMoveEvents.push_back(event);
}

void CIvfWidgetBase::addMouseUpEvent(CIvfMouseUpEvent* event)
{
	m_mouseUpEvents.push_back(event);
}

void CIvfWidgetBase::addOverlayEvent(CIvfOverlayEvent* event)
{
	m_overlayEvents.push_back(event);
}

void CIvfWidgetBase::addResizeEvent(CIvfResizeEvent* event)
{
	m_resizeEvents.push_back(event);
}

void CIvfWidgetBase::addUnFocusEvent(CIvfUnFocusEvent* event)
{
	m_unFocusEvents.push_back(event);
}

void CIvfWidgetBase::removeRenderEvent(CIvfRenderEvent* event)
{
	IvfRemoveEvent(CIvfRenderEvent,m_renderEvents,event);
}

void CIvfWidgetBase::removeClearEvent(CIvfClearEvent* event)
{
	IvfRemoveEvent(CIvfClearEvent,m_clearEvents,event);
}

void CIvfWidgetBase::removeDestroyEvent(CIvfDestroyEvent* event)
{
	IvfRemoveEvent(CIvfDestroyEvent,m_destroyEvents,event);
}

void CIvfWidgetBase::removeFocusEvent(CIvfFocusEvent* event)
{
	IvfRemoveEvent(CIvfFocusEvent,m_focusEvents,event);
}

void CIvfWidgetBase::removeFunctionKeyEvent(CIvfFunctionKeyEvent* event)
{
	IvfRemoveEvent(CIvfFunctionKeyEvent,m_functionKeyEvents,event);
}

void CIvfWidgetBase::removeIdleEvent(CIvfIdleEvent* event)
{
	IvfRemoveEvent(CIvfIdleEvent,m_idleEvents,event);
}

void CIvfWidgetBase::removeInitContextEvent(CIvfInitContextEvent* event)
{
	IvfRemoveEvent(CIvfInitContextEvent,m_initContextEvents,event);
}

void CIvfWidgetBase::removeInitEvent(CIvfInitEvent* event)
{
	IvfRemoveEvent(CIvfInitEvent,m_initEvents,event);
}

void CIvfWidgetBase::removeInitOverlayEvent(CIvfInitOverlayEvent* event)
{
	IvfRemoveEvent(CIvfInitOverlayEvent,m_initOverlayEvents,event);
}

void CIvfWidgetBase::removeKeyboardEvent(CIvfKeyboardEvent* event)
{
	IvfRemoveEvent(CIvfKeyboardEvent,m_keyboardEvents,event);
}

void CIvfWidgetBase::removeMouseDownEvent(CIvfMouseDownEvent* event)
{
	IvfRemoveEvent(CIvfMouseDownEvent,m_mouseDownEvents,event);
}

void CIvfWidgetBase::removeMouseEnterEvent(CIvfMouseEnterEvent* event)
{
	IvfRemoveEvent(CIvfMouseEnterEvent,m_mouseEnterEvents,event);
}

void CIvfWidgetBase::removeMouseEvent(CIvfMouseEvent* event)
{
	IvfRemoveEvent(CIvfMouseEvent,m_mouseEvents,event);
}

void CIvfWidgetBase::removeMouseLeaveEvent(CIvfMouseLeaveEvent* event)
{
	IvfRemoveEvent(CIvfMouseLeaveEvent,m_mouseLeaveEvents,event);
}

void CIvfWidgetBase::removeMouseMoveEvent(CIvfMouseMoveEvent* event)
{
	IvfRemoveEvent(CIvfMouseMoveEvent,m_mouseMoveEvents,event);
}

void CIvfWidgetBase::removeMouseUpEvent(CIvfMouseUpEvent* event)
{
	IvfRemoveEvent(CIvfMouseUpEvent,m_mouseUpEvents,event);
}

void CIvfWidgetBase::removeOverlayEvent(CIvfOverlayEvent* event)
{
	IvfRemoveEvent(CIvfOverlayEvent,m_overlayEvents,event);
}

void CIvfWidgetBase::removeResizeEvent(CIvfResizeEvent* event)
{
	IvfRemoveEvent(CIvfResizeEvent,m_resizeEvents,event);
}

void CIvfWidgetBase::removeUnFocusEvent(CIvfUnFocusEvent* event)
{
	IvfRemoveEvent(CIvfUnFocusEvent,m_unFocusEvents,event);
}

void CIvfWidgetBase::assignTimeoutEvent(int nbr, CIvfTimeoutEvent* event)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = event;
}

void CIvfWidgetBase::removeTimeoutEvent(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = NULL;
}

void CIvfWidgetBase::addModifierDownEvent(CIvfModifierDownEvent *event)
{
	m_modifierDownEvents.push_back(event);
}

void CIvfWidgetBase::removeModifierDownEvent(CIvfModifierDownEvent *event)
{
	IvfRemoveEvent(CIvfModifierDownEvent,m_modifierDownEvents,event);
}

void CIvfWidgetBase::addModifierUpEvent(CIvfModifierUpEvent *event)
{
	m_modifierUpEvents.push_back(event);
}

void CIvfWidgetBase::removeModifierUpEvent(CIvfModifierUpEvent *event)
{
	IvfRemoveEvent(CIvfModifierUpEvent,m_modifierUpEvents,event);
}

void CIvfWidgetBase::addMenuItemEvent(CIvfMenuItemEvent *event)
{
	m_menuItemEvents.push_back(event);
}

void CIvfWidgetBase::removeMenuItemEvent(CIvfMenuItemEvent *event)
{
	IvfRemoveEvent(CIvfMenuItemEvent,m_menuItemEvents,event);
}
