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
		events[i]->eventMethod((CEventBase::TFunctionKey)key, x, y); \
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

CWidgetBase::CWidgetBase()
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

CWidgetBase::~CWidgetBase()
{
	doDestroy();
}

bool CWidgetBase::isLeftButtonDown()
{
	return m_leftButtonDown;
}

bool CWidgetBase::isRightButtonDown()
{
	return m_rightButtonDown;
}

bool CWidgetBase::isMiddleButtonDown()
{
	return m_middleButtonDown;
}

void CWidgetBase::setLeftButtonStatus(bool down)
{
	m_leftButtonDown = down;
}

void CWidgetBase::setRightButtonStatus(bool down)
{
	m_rightButtonDown = down;
}

void CWidgetBase::setMiddleButtonStatus(bool down)
{
	m_middleButtonDown = down;
}

void CWidgetBase::setModifierKey(TModifierType key)
{
	m_modifierKey = key;
}

void CWidgetBase::clearMouseStatus()
{
	m_leftButtonDown = false;
	m_rightButtonDown = false;
	m_middleButtonDown = false;
}

int CWidgetBase::getWidth()
{
	return m_size[0];
}

int CWidgetBase::getHeight()
{
	return m_size[1];
}

void CWidgetBase::fullscreen()
{
	doFullscreen();
}

//
// Implementation specific interface routines
//

void CWidgetBase::doFullscreen()
{

}

void CWidgetBase::doDraw()
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

void CWidgetBase::doResize(int width, int height)
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

void CWidgetBase::doMouseDown(int x, int y)
{
	IvfDispatchPositional(m_mouseDownEvents,onMouseDown,x,y);
}

void CWidgetBase::doMouseUp(int x, int y)
{
	IvfDispatchPositional(m_mouseUpEvents,onMouseUp,x,y);
}

void CWidgetBase::doMouseMove(int x, int y)
{
	IvfDispatchPositional(m_mouseMoveEvents,onMouseMove,x,y);
}

void CWidgetBase::doFocus()
{
	IvfDispatchNotify(m_focusEvents,onFocus);
}

void CWidgetBase::doUnFocus()
{
	IvfDispatchNotify(m_unFocusEvents,onUnFocus);
}

void CWidgetBase::doMouseEnter(int x, int y)
{
	IvfDispatchPositional(m_mouseEnterEvents,onMouseEnter,x,y);
}

void CWidgetBase::doMouseLeave(int x, int y)
{
	IvfDispatchPositional(m_mouseLeaveEvents,onMouseLeave,x,y);
}

void CWidgetBase::doKeyboard(int key, int x, int y)
{
	IvfDispatchKeyboard(m_keyboardEvents,onKeyboard,key,x,y);
}

void CWidgetBase::doModifierDown()
{
	IvfDispatchNotify(m_modifierDownEvents,onModifierDown);
}

void CWidgetBase::doModifierUp()
{
	IvfDispatchNotify(m_modifierUpEvents,onModifierUp);
}

void CWidgetBase::doFunctionKey(TFunctionKey key, int x, int y)
{
	IvfDispatchKeyboard(m_functionKeyEvents,onFunctionKey,key,x,y);
}

//
// Event methods
//

void CWidgetBase::onInit(int width, int height)
{

}

void CWidgetBase::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CWidgetBase::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWidgetBase::onResize(int width, int height)
{

}

void CWidgetBase::onRender()
{

}

void CWidgetBase::onInitOverlay(int width, int height)
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

void CWidgetBase::onDestroy()
{

}


void CWidgetBase::onOverlay()
{

}

void CWidgetBase::onMouse(int x, int y)
{

}

void CWidgetBase::onMouseDown(int x, int y)
{

}

void CWidgetBase::onMouseUp(int x, int y)
{

}

void CWidgetBase::onFocus()
{

}

void CWidgetBase::onUnFocus()
{

}

void CWidgetBase::onMouseEnter(int x, int y)
{

}

void CWidgetBase::onMouseLeave(int x, int y)
{

}

void CWidgetBase::onMouseMove(int x, int y)
{

}

void CWidgetBase::onKeyboard(int key, int x, int y)
{

}

void CWidgetBase::onMenuItem(int id)
{

}

void CWidgetBase::onModifierDown()
{

}

void CWidgetBase::onModifierUp()
{

}

void CWidgetBase::onFunctionKey(TFunctionKey key, int x, int y)
{

}


CWidgetBase::TModifierType CWidgetBase::getModifierKey()
{
	return m_modifierKey;
}

bool CWidgetBase::isAnyButtonDown()
{
	return (isLeftButtonDown())||(isRightButtonDown())||(isMiddleButtonDown());
}

void CWidgetBase::onIdle()
{

}

void CWidgetBase::doIdle()
{
	onIdle();
}

void CWidgetBase::enableIdleProcessing()
{
	doEnableIdleProcessing();
	m_idleProcessing = true;
}

void CWidgetBase::disableIdleProcessing()
{
	doDisableIdleProcessing();
	m_idleProcessing = false;
}

void CWidgetBase::doEnableIdleProcessing()
{

}

void CWidgetBase::doDisableIdleProcessing()
{

}

bool CWidgetBase::isIdleProcessing()
{
	return m_idleProcessing;
}

void CWidgetBase::enableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = time;
		doEnableTimeout(time, nbr);
	}
}

void CWidgetBase::disableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		m_timeouts[nbr] = -1.0f;
		doDisableTimeout(nbr);
	}
}

void CWidgetBase::doEnableTimeout(float time, int nbr)
{

}

bool CWidgetBase::onTimeout0()
{
	return false;
}

bool CWidgetBase::onTimeout1()
{
	return false;
}

bool CWidgetBase::onTimeout2()
{
	return false;
}

bool CWidgetBase::onTimeout3()
{
	return false;
}

bool CWidgetBase::onTimeout4()
{
	return false;
}

bool CWidgetBase::onTimeout5()
{
	return false;
}

bool CWidgetBase::onTimeout6()
{
	return false;
}

bool CWidgetBase::onTimeout7()
{
	return false;
}

bool CWidgetBase::onTimeout8()
{
	return false;
}

bool CWidgetBase::onTimeout9()
{
	return false;
}

bool CWidgetBase::doTimeout0()
{
	IvfDispatchTimeout(0);
}

bool CWidgetBase::doTimeout1()
{
	IvfDispatchTimeout(1);
}

bool CWidgetBase::doTimeout2()
{
	IvfDispatchTimeout(2);
}

bool CWidgetBase::doTimeout3()
{
	IvfDispatchTimeout(3);
}

bool CWidgetBase::doTimeout4()
{
	IvfDispatchTimeout(4);
}

bool CWidgetBase::doTimeout5()
{
	IvfDispatchTimeout(5);
}

bool CWidgetBase::doTimeout6()
{
	IvfDispatchTimeout(6);
}

bool CWidgetBase::doTimeout7()
{
	IvfDispatchTimeout(7);
}

bool CWidgetBase::doTimeout8()
{
	IvfDispatchTimeout(8);
}

bool CWidgetBase::doTimeout9()
{
	IvfDispatchTimeout(9);
}

float CWidgetBase::getTimeout(int number)
{
	if ((number>=0)&&(number<10))
		return m_timeouts[number];
	else
		return -1.0;
}

void CWidgetBase::doRedraw()
{

}

void CWidgetBase::doInit(int width, int height)
{
	IvfDispatchDimensional(m_initEvents,onInit,width,height);
}

void CWidgetBase::doInitContext(int width, int height)
{
	IvfDispatchDimensional(m_initContextEvents,onInitContext,width,height);
}

void CWidgetBase::doRender()
{
	IvfDispatchNotify(m_renderEvents,onRender);
}

void CWidgetBase::doClear()
{
	IvfDispatchNotify(m_clearEvents,onClear);
}

void CWidgetBase::doInitOverlay(int width, int height)
{
	IvfDispatchDimensional(m_initOverlayEvents,onInitOverlay,width,height);
}

void CWidgetBase::doOverlay()
{
	IvfDispatchNotify(m_overlayEvents,onOverlay);
}

void CWidgetBase::doDestroy()
{
	IvfDispatchNotify(m_destroyEvents,onDestroy);
}

void CWidgetBase::doMenuItem(int id)
{
	IvfDispatchMenuItem(m_menuItemEvents,onMenuItem,id)	
}

bool CWidgetBase::isInitDone()
{
	return m_initDone;
}

void CWidgetBase::doDisableTimeout(int nbr)
{

}


bool CWidgetBase::isTimeoutEnabled(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		return m_timeouts[nbr]>=0.0f;
	else
		return false;
}

void CWidgetBase::setUseOverlay(bool flag)
{
	m_doOverlay = flag;
}

bool CWidgetBase::getUseOverlay()
{
	return m_doOverlay;
}

void CWidgetBase::redraw()
{
	doRedraw();
}

double CWidgetBase::elapsedTime()
{
	return doElapsedTime();
}

double CWidgetBase::doElapsedTime()
{
	return 0.0;
}

void CWidgetBase::setSize(int w, int h)
{
	doSetSize(w, h);
	doResize(w, h);
}

void CWidgetBase::setPosition(int x, int y)
{
	doSetPosition(x, y);
}

void CWidgetBase::doSetSize(int w, int h)
{

}

void CWidgetBase::doSetPosition(int x, int y)
{

}

bool CWidgetBase::doAppLoop()
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

bool CWidgetBase::onAppLoop()
{
	return true;
}

void CWidgetBase::addRenderEvent(CRenderEvent *event)
{
	m_renderEvents.push_back(event);
}

void CWidgetBase::addClearEvent(CClearEvent* event)
{
	m_clearEvents.push_back(event);
}

void CWidgetBase::assignAppLoopEvent(CAppLoopEvent* event)
{
	m_appLoopEvent = event;
}

void CWidgetBase::addDestroyEvent(CDestroyEvent* event)
{
	m_destroyEvents.push_back(event);
}

void CWidgetBase::addFocusEvent(CFocusEvent* event)
{
	m_focusEvents.push_back(event);
}

void CWidgetBase::addFunctionKeyEvent(CFunctionKeyEvent* event)
{
	m_functionKeyEvents.push_back(event);
}

void CWidgetBase::addIdleEvent(CIdleEvent* event)
{
	m_idleEvents.push_back(event);
}

void CWidgetBase::addInitContextEvent(CInitContextEvent* event)
{
	m_initContextEvents.push_back(event);
}

void CWidgetBase::addInitEvent(CInitEvent* event)
{
	m_initEvents.push_back(event);
}

void CWidgetBase::addInitOverlayEvent(CInitOverlayEvent* event)
{
	m_initOverlayEvents.push_back(event);
}

void CWidgetBase::addKeyboardEvent(CKeyboardEvent* event)
{
	m_keyboardEvents.push_back(event);
}

void CWidgetBase::addMouseDownEvent(CMouseDownEvent* event)
{
	m_mouseDownEvents.push_back(event);
}

void CWidgetBase::addMouseEnterEvent(CMouseEnterEvent* event)
{
	m_mouseEnterEvents.push_back(event);
}

void CWidgetBase::addMouseEvent(CMouseEvent* event)
{
	m_mouseEvents.push_back(event);
}

void CWidgetBase::addMouseLeaveEvent(CMouseLeaveEvent* event)
{
	m_mouseLeaveEvents.push_back(event);
}

void CWidgetBase::addMouseMoveEvent(CMouseMoveEvent* event)
{
	m_mouseMoveEvents.push_back(event);
}

void CWidgetBase::addMouseUpEvent(CMouseUpEvent* event)
{
	m_mouseUpEvents.push_back(event);
}

void CWidgetBase::addOverlayEvent(COverlayEvent* event)
{
	m_overlayEvents.push_back(event);
}

void CWidgetBase::addResizeEvent(CResizeEvent* event)
{
	m_resizeEvents.push_back(event);
}

void CWidgetBase::addUnFocusEvent(CUnFocusEvent* event)
{
	m_unFocusEvents.push_back(event);
}

void CWidgetBase::removeRenderEvent(CRenderEvent* event)
{
	IvfRemoveEvent(CRenderEvent,m_renderEvents,event);
}

void CWidgetBase::removeClearEvent(CClearEvent* event)
{
	IvfRemoveEvent(CClearEvent,m_clearEvents,event);
}

void CWidgetBase::removeDestroyEvent(CDestroyEvent* event)
{
	IvfRemoveEvent(CDestroyEvent,m_destroyEvents,event);
}

void CWidgetBase::removeFocusEvent(CFocusEvent* event)
{
	IvfRemoveEvent(CFocusEvent,m_focusEvents,event);
}

void CWidgetBase::removeFunctionKeyEvent(CFunctionKeyEvent* event)
{
	IvfRemoveEvent(CFunctionKeyEvent,m_functionKeyEvents,event);
}

void CWidgetBase::removeIdleEvent(CIdleEvent* event)
{
	IvfRemoveEvent(CIdleEvent,m_idleEvents,event);
}

void CWidgetBase::removeInitContextEvent(CInitContextEvent* event)
{
	IvfRemoveEvent(CInitContextEvent,m_initContextEvents,event);
}

void CWidgetBase::removeInitEvent(CInitEvent* event)
{
	IvfRemoveEvent(CInitEvent,m_initEvents,event);
}

void CWidgetBase::removeInitOverlayEvent(CInitOverlayEvent* event)
{
	IvfRemoveEvent(CInitOverlayEvent,m_initOverlayEvents,event);
}

void CWidgetBase::removeKeyboardEvent(CKeyboardEvent* event)
{
	IvfRemoveEvent(CKeyboardEvent,m_keyboardEvents,event);
}

void CWidgetBase::removeMouseDownEvent(CMouseDownEvent* event)
{
	IvfRemoveEvent(CMouseDownEvent,m_mouseDownEvents,event);
}

void CWidgetBase::removeMouseEnterEvent(CMouseEnterEvent* event)
{
	IvfRemoveEvent(CMouseEnterEvent,m_mouseEnterEvents,event);
}

void CWidgetBase::removeMouseEvent(CMouseEvent* event)
{
	IvfRemoveEvent(CMouseEvent,m_mouseEvents,event);
}

void CWidgetBase::removeMouseLeaveEvent(CMouseLeaveEvent* event)
{
	IvfRemoveEvent(CMouseLeaveEvent,m_mouseLeaveEvents,event);
}

void CWidgetBase::removeMouseMoveEvent(CMouseMoveEvent* event)
{
	IvfRemoveEvent(CMouseMoveEvent,m_mouseMoveEvents,event);
}

void CWidgetBase::removeMouseUpEvent(CMouseUpEvent* event)
{
	IvfRemoveEvent(CMouseUpEvent,m_mouseUpEvents,event);
}

void CWidgetBase::removeOverlayEvent(COverlayEvent* event)
{
	IvfRemoveEvent(COverlayEvent,m_overlayEvents,event);
}

void CWidgetBase::removeResizeEvent(CResizeEvent* event)
{
	IvfRemoveEvent(CResizeEvent,m_resizeEvents,event);
}

void CWidgetBase::removeUnFocusEvent(CUnFocusEvent* event)
{
	IvfRemoveEvent(CUnFocusEvent,m_unFocusEvents,event);
}

void CWidgetBase::assignTimeoutEvent(int nbr, CTimeoutEvent* event)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = event;
}

void CWidgetBase::removeTimeoutEvent(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		m_timeoutEvents[nbr] = NULL;
}

void CWidgetBase::addModifierDownEvent(CModifierDownEvent *event)
{
	m_modifierDownEvents.push_back(event);
}

void CWidgetBase::removeModifierDownEvent(CModifierDownEvent *event)
{
	IvfRemoveEvent(CModifierDownEvent,m_modifierDownEvents,event);
}

void CWidgetBase::addModifierUpEvent(CModifierUpEvent *event)
{
	m_modifierUpEvents.push_back(event);
}

void CWidgetBase::removeModifierUpEvent(CModifierUpEvent *event)
{
	IvfRemoveEvent(CModifierUpEvent,m_modifierUpEvents,event);
}

void CWidgetBase::addMenuItemEvent(CMenuItemEvent *event)
{
	m_menuItemEvents.push_back(event);
}

void CWidgetBase::removeMenuItemEvent(CMenuItemEvent *event)
{
	IvfRemoveEvent(CMenuItemEvent,m_menuItemEvents,event);
}
