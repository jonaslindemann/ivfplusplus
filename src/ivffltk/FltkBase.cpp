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

#include <ivffltk/FltkBase.h>

using namespace ivf;

#include <FL/x.H>

#ifndef WIN32
#include <sys/time.h>
#endif

static void cb_fltkBaseIdle(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	fltkBase->doIdle();
}

static void cb_fltkBase0(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout0())
		Fl::repeat_timeout(fltkBase->getTimeout(0), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase1(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout1())
		Fl::repeat_timeout(fltkBase->getTimeout(1), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase2(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout2())
		Fl::repeat_timeout(fltkBase->getTimeout(2), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase3(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout3())
		Fl::repeat_timeout(fltkBase->getTimeout(3), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase4(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout4())
		Fl::repeat_timeout(fltkBase->getTimeout(4), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase5(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout5())
		Fl::repeat_timeout(fltkBase->getTimeout(5), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase6(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout6())
		Fl::repeat_timeout(fltkBase->getTimeout(6), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase7(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout7())
		Fl::repeat_timeout(fltkBase->getTimeout(7), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase8(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout8())
		Fl::repeat_timeout(fltkBase->getTimeout(8), cb_fltkBase0, fltkBase);
}

static void cb_fltkBase9(void* p)
{
	FltkBase* fltkBase = (FltkBase*) p;
	if (fltkBase->doTimeout9())
		Fl::repeat_timeout(fltkBase->getTimeout(9), cb_fltkBase0, fltkBase);
}

FltkBase::FltkBase(int X, int Y, int W, int H, const char *L) :
Fl_Gl_Window(X, Y, W, H, L), WidgetBase()
{
#ifdef WIN32
	LARGE_INTEGER count;
	QueryPerformanceFrequency(&m_countsPerSec);
	QueryPerformanceCounter(&count);
	m_zeroTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	m_zeroTime = (double)tv.tv_sec + 1e-6*(double)tv.tv_usec;
#endif
	m_elapsedTime = 0.0;
}

void FltkBase::draw()
{
	if (!valid())
		doResize(w(), h());
	
	doDraw();
}

int FltkBase::handle(int event)
{
	int x = Fl::event_x();
	int y = Fl::event_y();

	if (isInitDone())
	{
		switch(event)
		{
		case FL_ENTER:
			Fl::belowmouse(this);
			doMouseEnter(x, y);
			return 1;
		case FL_LEAVE:
			doMouseLeave(x, y);
			return 1;
		case FL_MOVE:
			doMouseMove(x, y);
			return 1;
		case FL_PUSH:
			
			setModifierKey(WidgetBase::MT_NONE);
			
			if (Fl::get_key(FL_Shift_L))
				setModifierKey(WidgetBase::MT_SHIFT);
			if (Fl::get_key(FL_Shift_R))
				setModifierKey(WidgetBase::MT_SHIFT);
			if (Fl::get_key(FL_Control_L))
				setModifierKey(WidgetBase::MT_CTRL);
			if (Fl::get_key(FL_Control_R))
				setModifierKey(WidgetBase::MT_CTRL);
			
			if (Fl::event_button()==FL_LEFT_MOUSE)
				setLeftButtonStatus(true);
			if (Fl::event_button()==FL_MIDDLE_MOUSE)
				setMiddleButtonStatus(true);
			if (Fl::event_button()==FL_RIGHT_MOUSE)
				setRightButtonStatus(true);
			
			doMouseDown(x, y);
			return 1;
		case FL_DRAG:
			if (Fl::event_button()==FL_LEFT_MOUSE)
				setLeftButtonStatus(true);
			if (Fl::event_button()==FL_MIDDLE_MOUSE)
				setMiddleButtonStatus(true);
			if (Fl::event_button()==FL_RIGHT_MOUSE)
				setRightButtonStatus(true);
			doMouseMove(x, y);
			return 1;
		case FL_RELEASE:
			setModifierKey(WidgetBase::MT_NONE);
			clearMouseStatus();
			doMouseUp(x, y);
			return 1;
		case FL_FOCUS:
			doFocus();		
			return 1;
		case FL_UNFOCUS:
			doUnFocus();		
			return 1;
		case FL_KEYBOARD:
		//case FL_SHORTCUT:
			switch (Fl::event_key()) {
			case FL_Button:
				doFunctionKey(WidgetBase::FK_BUTTON, x, y);
				return 1;
				break;
			case FL_BackSpace:
				doFunctionKey(WidgetBase::FK_BACKSPACE, x, y);
				return 1;
				break;
			case FL_Tab:
				doFunctionKey(WidgetBase::FK_TAB, x, y);
				return 1;
				break; 
			case FL_Enter:
				doFunctionKey(WidgetBase::FK_ENTER, x, y);
				return 1;
				break;
			case FL_Pause:
				doFunctionKey(WidgetBase::FK_PAUSE, x, y);
				return 1;
				break;
			case FL_Scroll_Lock:
				doFunctionKey(WidgetBase::FK_SCROLL_LOCK, x, y);
				return 1;
				break;
			case FL_Escape:
				doFunctionKey(WidgetBase::FK_ESCAPE, x, y);
				return 1;
				break;
			case FL_Home:
				doFunctionKey(WidgetBase::FK_HOME, x, y);
				return 1;
				break;
			case FL_Left:
				doFunctionKey(WidgetBase::FK_LEFT, x, y);
				return 1;
				break;
			case FL_Up:
				doFunctionKey(WidgetBase::FK_UP, x, y);
				return 1;
				break; 
			case FL_Right:
				doFunctionKey(WidgetBase::FK_RIGHT, x, y);
				return 1;
				break;
			case FL_Down:
				doFunctionKey(WidgetBase::FK_RIGHT, x, y);
				return 1;
				break; 
			case FL_Page_Up:
				doFunctionKey(WidgetBase::FK_PAGE_UP, x, y);
				return 1;
				break;
			case FL_Page_Down:
				doFunctionKey(WidgetBase::FK_PAGE_DOWN, x, y);
				return 1;
				break;
			case FL_End:
				doFunctionKey(WidgetBase::FK_END, x, y);
				return 1;
				break;
			case FL_Print:
				doFunctionKey(WidgetBase::FK_PRINT, x, y);
				return 1;
				break;
			case FL_Insert:
				doFunctionKey(WidgetBase::FK_INSERT, x, y);
				return 1;
				break;
			case FL_Menu:
				doFunctionKey(WidgetBase::FK_MENU, x, y);
				return 1;
				break;
			case FL_Num_Lock:
				doFunctionKey(WidgetBase::FK_END, x, y);
				return 1;
				break;
			case FL_F+1:
				doFunctionKey(WidgetBase::FK_F1, x, y);
				return 1;
				break;
			case FL_F+2:
				doFunctionKey(WidgetBase::FK_F2, x, y);
				return 1;
				break;
			case FL_F+3:
				doFunctionKey(WidgetBase::FK_F3, x, y);
				return 1;
				break;
			case FL_F+4:
				doFunctionKey(WidgetBase::FK_F4, x, y);
				return 1;
				break;
			case FL_F+5:
				doFunctionKey(WidgetBase::FK_F5, x, y);
				return 1;
				break;
			case FL_F+6:
				doFunctionKey(WidgetBase::FK_F6, x, y);
				return 1;
				break;
			case FL_F+7:
				doFunctionKey(WidgetBase::FK_F7, x, y);
				return 1;
				break;
			case FL_F+8:
				doFunctionKey(WidgetBase::FK_F8, x, y);
				return 1;
				break;
			case FL_F+9:
				doFunctionKey(WidgetBase::FK_F9, x, y);
				return 1;
				break;
			case FL_F+10:
				doFunctionKey(WidgetBase::FK_F10, x, y);
				return 1;
				break;
			case FL_F+11:
				doFunctionKey(WidgetBase::FK_F11, x, y);
				return 1;
				break;
			case FL_F+12:
				doFunctionKey(WidgetBase::FK_F12, x, y);
				return 1;
				break;
			case FL_KP+0:
				doFunctionKey(WidgetBase::FK_KP0, x, y);
				return 1;
				break;
			case FL_KP+1:
				doFunctionKey(WidgetBase::FK_KP1, x, y);
				return 1;
				break;
			case FL_KP+2:
				doFunctionKey(WidgetBase::FK_KP2, x, y);
				return 1;
				break;
			case FL_KP+3:
				doFunctionKey(WidgetBase::FK_KP3, x, y);
				return 1;
				break;
			case FL_KP+4:
				doFunctionKey(WidgetBase::FK_KP4, x, y);
				return 1;
				break;
			case FL_KP+5:
				doFunctionKey(WidgetBase::FK_KP5, x, y);
				return 1;
				break;
			case FL_KP+6:
				doFunctionKey(WidgetBase::FK_KP6, x, y);
				return 1;
				break;
			case FL_KP+7:
				doFunctionKey(WidgetBase::FK_KP7, x, y);
				return 1;
				break;
			case FL_KP+8:
				doFunctionKey(WidgetBase::FK_KP8, x, y);
				return 1;
				break;
			case FL_KP+9:
				doFunctionKey(WidgetBase::FK_KP9, x, y);
				return 1;
				break;
			case FL_KP_Enter:
				doFunctionKey(WidgetBase::FK_KP_ENTER, x, y);
				return 1;
				break;
			case FL_Shift_L:
				doFunctionKey(WidgetBase::FK_SHIFT_L, x, y);
				return 1;
				break;
			case FL_Shift_R:
				doFunctionKey(WidgetBase::FK_SHIFT_R, x, y);
				return 1;
				break;
			case FL_Control_L:
				doFunctionKey(WidgetBase::FK_CONTROL_L, x, y);
				return 1;
				break;
			case FL_Control_R:
				doFunctionKey(WidgetBase::FK_CONTROL_R, x, y);
				return 1;
				break;
			case FL_Caps_Lock:
				doFunctionKey(WidgetBase::FK_CAPS_LOCK, x, y);
				return 1;
				break;
			case FL_Meta_L:
				doFunctionKey(WidgetBase::FK_META_L, x, y);
				return 1;
				break;
			case FL_Meta_R:
				doFunctionKey(WidgetBase::FK_META_R, x, y);
				return 1;
				break;
			case FL_Alt_L:
				doFunctionKey(WidgetBase::FK_ALT_L, x, y);
				return 1;
				break;
			case FL_Alt_R:
				doFunctionKey(WidgetBase::FK_ALT_R, x, y);
				return 1;
				break;
			case FL_Delete:
				doFunctionKey(WidgetBase::FK_DELETE, x, y);
				return 1;
				break;
			default:
				doKeyboard(Fl::event_key(), x, y);
				return Fl_Gl_Window::handle(event);
			}
			doKeyboard(Fl::event_key(), x, y);
			return Fl_Gl_Window::handle(event);
		default:
			return Fl_Gl_Window::handle(event);
		}
	}
	else
		return Fl_Gl_Window::handle(event);
}


void FltkBase::doEnableIdleProcessing()
{
	Fl::add_idle(cb_fltkBaseIdle, (void*) this);
}

void FltkBase::doDisableIdleProcessing()
{
	Fl::remove_idle(cb_fltkBaseIdle, (void*) this);
}

void FltkBase::doEnableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		switch (nbr) {
		case 0:
			Fl::add_timeout(time, cb_fltkBase0, this);
			break;
		case 1:
			Fl::add_timeout(time, cb_fltkBase1, this);
			break;
		case 2:
			Fl::add_timeout(time, cb_fltkBase2, this);
			break;
		case 3:
			Fl::add_timeout(time, cb_fltkBase3, this);
			break;
		case 4:
			Fl::add_timeout(time, cb_fltkBase4, this);
			break;
		case 5:
			Fl::add_timeout(time, cb_fltkBase5, this);
			break;
		case 6:
			Fl::add_timeout(time, cb_fltkBase6, this);
			break;
		case 7:
			Fl::add_timeout(time, cb_fltkBase7, this);
			break;
		case 8:
			Fl::add_timeout(time, cb_fltkBase8, this);
			break;
		case 9:
			Fl::add_timeout(time, cb_fltkBase9, this);
			break;
		default:
			break;
		}
	}
}

void FltkBase::doDisableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		switch (nbr) {
		case 0:
			Fl::remove_timeout(cb_fltkBase0, this);
			break;
		case 1:
			Fl::remove_timeout(cb_fltkBase1, this);
			break;
		case 2:
			Fl::remove_timeout(cb_fltkBase2, this);
			break;
		case 3:
			Fl::remove_timeout(cb_fltkBase3, this);
			break;
		case 4:
			Fl::remove_timeout(cb_fltkBase4, this);
			break;
		case 5:
			Fl::remove_timeout(cb_fltkBase5, this);
			break;
		case 6:
			Fl::remove_timeout(cb_fltkBase6, this);
			break;
		case 7:
			Fl::remove_timeout(cb_fltkBase7, this);
			break;
		case 8:
			Fl::remove_timeout(cb_fltkBase8, this);
			break;
		case 9:
			Fl::remove_timeout(cb_fltkBase9, this);
			break;
		default:
			break;
		}
	}
}

void FltkBase::doRedraw()
{
	Fl_Gl_Window::redraw();
}


void FltkBase::redraw()
{
	WidgetBase::redraw();
}

double FltkBase::doElapsedTime()
{
#ifdef WIN32
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	double currentTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	return currentTime - m_zeroTime;
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (double)tv.tv_sec + 1e-6*(double)tv.tv_usec - m_zeroTime;
#endif
}
