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

#include <ivffltk/IvfFltkWorkspace.h>

#include <FL/x.H>

static void cb_fltkWorkspaceIdle(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	fltkBase->doIdle();
}

static void cb_fltkWorkspace0(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout0())
		Fl::repeat_timeout(fltkBase->getTimeout(0), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace1(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout1())
		Fl::repeat_timeout(fltkBase->getTimeout(1), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace2(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout2())
		Fl::repeat_timeout(fltkBase->getTimeout(2), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace3(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout3())
		Fl::repeat_timeout(fltkBase->getTimeout(3), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace4(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout4())
		Fl::repeat_timeout(fltkBase->getTimeout(4), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace5(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout5())
		Fl::repeat_timeout(fltkBase->getTimeout(5), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace6(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout6())
		Fl::repeat_timeout(fltkBase->getTimeout(6), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace7(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout7())
		Fl::repeat_timeout(fltkBase->getTimeout(7), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace8(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout8())
		Fl::repeat_timeout(fltkBase->getTimeout(8), cb_fltkWorkspace0, fltkBase);
}

static void cb_fltkWorkspace9(void* p)
{
	CIvfFltkWorkspace* fltkBase = (CIvfFltkWorkspace*) p;
	if (fltkBase->doTimeout9())
		Fl::repeat_timeout(fltkBase->getTimeout(9), cb_fltkWorkspace0, fltkBase);
}

CIvfFltkWorkspace::CIvfFltkWorkspace(int X, int Y, int W, int H, const char *L) :
Fl_Gl_Window(X, Y, W, H, L), CIvfWorkspaceWidget()
{
}

void CIvfFltkWorkspace::draw()
{
	if (!valid())
		doResize(w(), h());
	
	doDraw();
}

int CIvfFltkWorkspace::handle(int event)
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
			
			setModifierKey(CIvfWidgetBase::MT_NONE);
			
			if (Fl::get_key(FL_Shift_L))
				setModifierKey(CIvfWidgetBase::MT_SHIFT);
			if (Fl::get_key(FL_Shift_R))
				setModifierKey(CIvfWidgetBase::MT_SHIFT);
			if (Fl::get_key(FL_Control_L))
				setModifierKey(CIvfWidgetBase::MT_CTRL);
			if (Fl::get_key(FL_Control_R))
				setModifierKey(CIvfWidgetBase::MT_CTRL);
			
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
			setModifierKey(CIvfWidgetBase::MT_NONE);
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
			switch (Fl::event_key()) {
			case FL_Button:
				doFunctionKey(CIvfWidgetBase::FK_BUTTON, x, y);
				return 1;
				break;
			case FL_BackSpace:
				doFunctionKey(CIvfWidgetBase::FK_BACKSPACE, x, y);
				return 1;
				break;
			case FL_Tab:
				doFunctionKey(CIvfWidgetBase::FK_TAB, x, y);
				return 1;
				break; 
			case FL_Enter:
				doFunctionKey(CIvfWidgetBase::FK_ENTER, x, y);
				return 1;
				break;
			case FL_Pause:
				doFunctionKey(CIvfWidgetBase::FK_PAUSE, x, y);
				return 1;
				break;
			case FL_Scroll_Lock:
				doFunctionKey(CIvfWidgetBase::FK_SCROLL_LOCK, x, y);
				return 1;
				break;
			case FL_Escape:
				doFunctionKey(CIvfWidgetBase::FK_ESCAPE, x, y);
				return 1;
				break;
			case FL_Home:
				doFunctionKey(CIvfWidgetBase::FK_HOME, x, y);
				return 1;
				break;
			case FL_Left:
				doFunctionKey(CIvfWidgetBase::FK_LEFT, x, y);
				return 1;
				break;
			case FL_Up:
				doFunctionKey(CIvfWidgetBase::FK_UP, x, y);
				return 1;
				break; 
			case FL_Right:
				doFunctionKey(CIvfWidgetBase::FK_RIGHT, x, y);
				return 1;
				break;
			case FL_Down:
				doFunctionKey(CIvfWidgetBase::FK_RIGHT, x, y);
				return 1;
				break; 
			case FL_Page_Up:
				doFunctionKey(CIvfWidgetBase::FK_PAGE_UP, x, y);
				return 1;
				break;
			case FL_Page_Down:
				doFunctionKey(CIvfWidgetBase::FK_PAGE_DOWN, x, y);
				return 1;
				break;
			case FL_End:
				doFunctionKey(CIvfWidgetBase::FK_END, x, y);
				return 1;
				break;
			case FL_Print:
				doFunctionKey(CIvfWidgetBase::FK_PRINT, x, y);
				return 1;
				break;
			case FL_Insert:
				doFunctionKey(CIvfWidgetBase::FK_INSERT, x, y);
				return 1;
				break;
			case FL_Menu:
				doFunctionKey(CIvfWidgetBase::FK_MENU, x, y);
				return 1;
				break;
			case FL_Num_Lock:
				doFunctionKey(CIvfWidgetBase::FK_END, x, y);
				return 1;
				break;
			case FL_F+1:
				doFunctionKey(CIvfWidgetBase::FK_F1, x, y);
				return 1;
				break;
			case FL_F+2:
				doFunctionKey(CIvfWidgetBase::FK_F2, x, y);
				return 1;
				break;
			case FL_F+3:
				doFunctionKey(CIvfWidgetBase::FK_F3, x, y);
				return 1;
				break;
			case FL_F+4:
				doFunctionKey(CIvfWidgetBase::FK_F4, x, y);
				return 1;
				break;
			case FL_F+5:
				doFunctionKey(CIvfWidgetBase::FK_F5, x, y);
				return 1;
				break;
			case FL_F+6:
				doFunctionKey(CIvfWidgetBase::FK_F6, x, y);
				return 1;
				break;
			case FL_F+7:
				doFunctionKey(CIvfWidgetBase::FK_F7, x, y);
				return 1;
				break;
			case FL_F+8:
				doFunctionKey(CIvfWidgetBase::FK_F8, x, y);
				return 1;
				break;
			case FL_F+9:
				doFunctionKey(CIvfWidgetBase::FK_F9, x, y);
				return 1;
				break;
			case FL_F+10:
				doFunctionKey(CIvfWidgetBase::FK_F10, x, y);
				return 1;
				break;
			case FL_F+11:
				doFunctionKey(CIvfWidgetBase::FK_F11, x, y);
				return 1;
				break;
			case FL_F+12:
				doFunctionKey(CIvfWidgetBase::FK_F12, x, y);
				return 1;
				break;
			case FL_KP+0:
				doFunctionKey(CIvfWidgetBase::FK_KP0, x, y);
				return 1;
				break;
			case FL_KP+1:
				doFunctionKey(CIvfWidgetBase::FK_KP1, x, y);
				return 1;
				break;
			case FL_KP+2:
				doFunctionKey(CIvfWidgetBase::FK_KP2, x, y);
				return 1;
				break;
			case FL_KP+3:
				doFunctionKey(CIvfWidgetBase::FK_KP3, x, y);
				return 1;
				break;
			case FL_KP+4:
				doFunctionKey(CIvfWidgetBase::FK_KP4, x, y);
				return 1;
				break;
			case FL_KP+5:
				doFunctionKey(CIvfWidgetBase::FK_KP5, x, y);
				return 1;
				break;
			case FL_KP+6:
				doFunctionKey(CIvfWidgetBase::FK_KP6, x, y);
				return 1;
				break;
			case FL_KP+7:
				doFunctionKey(CIvfWidgetBase::FK_KP7, x, y);
				return 1;
				break;
			case FL_KP+8:
				doFunctionKey(CIvfWidgetBase::FK_KP8, x, y);
				return 1;
				break;
			case FL_KP+9:
				doFunctionKey(CIvfWidgetBase::FK_KP9, x, y);
				return 1;
				break;
			case FL_KP_Enter:
				doFunctionKey(CIvfWidgetBase::FK_KP_ENTER, x, y);
				return 1;
				break;
			case FL_Shift_L:
				doFunctionKey(CIvfWidgetBase::FK_SHIFT_L, x, y);
				return 1;
				break;
			case FL_Shift_R:
				doFunctionKey(CIvfWidgetBase::FK_SHIFT_R, x, y);
				return 1;
				break;
			case FL_Control_L:
				doFunctionKey(CIvfWidgetBase::FK_CONTROL_L, x, y);
				return 1;
				break;
			case FL_Control_R:
				doFunctionKey(CIvfWidgetBase::FK_CONTROL_R, x, y);
				return 1;
				break;
			case FL_Caps_Lock:
				doFunctionKey(CIvfWidgetBase::FK_CAPS_LOCK, x, y);
				return 1;
				break;
			case FL_Meta_L:
				doFunctionKey(CIvfWidgetBase::FK_META_L, x, y);
				return 1;
				break;
			case FL_Meta_R:
				doFunctionKey(CIvfWidgetBase::FK_META_R, x, y);
				return 1;
				break;
			case FL_Alt_L:
				doFunctionKey(CIvfWidgetBase::FK_ALT_L, x, y);
				return 1;
				break;
			case FL_Alt_R:
				doFunctionKey(CIvfWidgetBase::FK_ALT_R, x, y);
				return 1;
				break;
			case FL_Delete:
				doFunctionKey(CIvfWidgetBase::FK_DELETE, x, y);
				return 1;
				break;
			default:
				break;
			}
			doKeyboard(Fl::event_key(), x, y);
			return 1;
		default:
			return 0;
		}
	}
	else
		return 0;
}


void CIvfFltkWorkspace::doEnableIdleProcessing()
{
	Fl::add_idle(cb_fltkWorkspaceIdle, (void*) this);
}

void CIvfFltkWorkspace::doDisableIdleProcessing()
{
	Fl::remove_idle(cb_fltkWorkspaceIdle, (void*) this);
}

void CIvfFltkWorkspace::doEnableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		switch (nbr) {
		case 0:
			Fl::add_timeout(time, cb_fltkWorkspace0, this);
			break;
		case 1:
			Fl::add_timeout(time, cb_fltkWorkspace1, this);
			break;
		case 2:
			Fl::add_timeout(time, cb_fltkWorkspace2, this);
			break;
		case 3:
			Fl::add_timeout(time, cb_fltkWorkspace3, this);
			break;
		case 4:
			Fl::add_timeout(time, cb_fltkWorkspace4, this);
			break;
		case 5:
			Fl::add_timeout(time, cb_fltkWorkspace5, this);
			break;
		case 6:
			Fl::add_timeout(time, cb_fltkWorkspace6, this);
			break;
		case 7:
			Fl::add_timeout(time, cb_fltkWorkspace7, this);
			break;
		case 8:
			Fl::add_timeout(time, cb_fltkWorkspace8, this);
			break;
		case 9:
			Fl::add_timeout(time, cb_fltkWorkspace9, this);
			break;
		default:
			break;
		}
	}
}

void CIvfFltkWorkspace::doRedraw()
{
	Fl_Gl_Window::redraw();
}
