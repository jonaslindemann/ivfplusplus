//
// Copyright 1999-2010 by Structural Mechanics, Lund University.
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

#include <ivfwidget/IvfImUiHandler.h>

CImUiHandler::CImUiHandler(CWidgetBase* widget)
{
	m_widget = widget;
	m_guiEvent = 0;
	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);
	m_widget->addResizeEvent(this);
	m_widget->addOverlayEvent(this);
	m_widget->setUseOverlay(true);
}

CImUiHandler::~CImUiHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
	m_widget->removeResizeEvent(this);
	m_widget->removeOverlayEvent(this);
}

void CImUiHandler::setGuiEvent(CGuiEvent* event)
{
	m_guiEvent = event;
}

void CImUiHandler::dispatchGuiEvent()
{
	if (m_guiEvent!=NULL)
		m_guiEvent->onGui();
}

void CImUiHandler::doMouseDown(int x, int y)
{

}

void CImUiHandler::doMouseMove(int x, int y)
{

}

void CImUiHandler::doMouseUp(int x, int y)
{

}

void CImUiHandler::doResize(int width, int height)
{

}

void CImUiHandler::doOverlay()
{
	dispatchGuiEvent();
}

void CImUiHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void CImUiHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void CImUiHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void CImUiHandler::onResize(int width, int height)
{
	if (isActive())
		doResize(width, height);
}

void CImUiHandler::onOverlay()
{
	if (isActive())
		doOverlay();
}


