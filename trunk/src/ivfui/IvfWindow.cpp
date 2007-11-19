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

#include <ivfui/IvfWindow.h>

CIvfWindow::CIvfWindow(int X, int Y, int W, int H)
:CIvfFltkBase(X, Y, W, H, "Ivf++"), CIvfPopupMenu()
{
	setParent(this);
	this->resizable(this);

	// Workaround: On Windows the window is resizable. On Linux it is locked.
	// Settings this will enable resizing the window on Linux.

	this->size_range(0,0,20000,20000);

	m_popupButtonDown = false;
}

CIvfWindow::~CIvfWindow()
{

}

void CIvfWindow::setWindowTitle(const char *title)
{
	label(title);
}


void CIvfWindow::show()
{
	Fl_Gl_Window::show();
}

void CIvfWindow::doMouseDown(int x, int y)
{
	CIvfFltkBase::doMouseDown(x, y);

	switch (getActivationButton()) {
	case AB_LEFT:
		m_popupButtonDown = isLeftButtonDown();
		break;
	case AB_MIDDLE:
		m_popupButtonDown = isMiddleButtonDown();
		break;
	case AB_RIGHT:
		m_popupButtonDown = isRightButtonDown();
		break;
	default:
		break;
	}
}

void CIvfWindow::doMouseUp(int x, int y)
{
	CIvfFltkBase::doMouseUp(x, y);

	if (m_popupButtonDown)
		this->popup();

	m_popupButtonDown = false;
}

