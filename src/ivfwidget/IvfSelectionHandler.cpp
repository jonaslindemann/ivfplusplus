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

#include <ivfwidget/IvfSelectionHandler.h>

CIvfSelectionHandler::CIvfSelectionHandler(CIvfWidgetBase* widget, CIvfScene* scene)
{
	m_widget = widget;
	m_scene = scene;

	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);

	m_singleSelectionEvent = NULL;
	m_multipleSelectionEvent = NULL;
	m_highlightEvent = NULL;
}

CIvfSelectionHandler::~CIvfSelectionHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
}

void CIvfSelectionHandler::doMouseDown(int x, int y)
{
	m_scene->pick(x, y);
	dispatchSingleSelectionEvent(m_scene->getSelectedShape());
	if (m_scene->getSelectionSize()>1)
		dispatchMultipleSelectionEvent(m_scene->getSelectedShapes());
}

void CIvfSelectionHandler::doMouseMove(int x, int y)
{
	m_scene->pick(x, y);

	if (m_scene->getSelectedShape()!=NULL)
	{
		if (m_highlightEvent==NULL)
		{
			if (m_oldShape!=NULL)
				if (m_oldShape!=m_scene->getSelectedShape())
					m_oldShape->setHighlight(CIvfShape::HS_OFF);
			m_scene->getSelectedShape()->setHighlight(CIvfShape::HS_ON);
			m_oldShape = m_scene->getSelectedShape();
		}
		else
		{
			dispatchHighlightEvent(m_scene->getSelectedShape());			
		}
	}
	else
	{
		if (m_highlightEvent==NULL)
		{
			if (m_oldShape!=NULL)
				m_oldShape->setHighlight(CIvfShape::HS_OFF);
		}
		else
		{
			dispatchHighlightEvent(NULL);
		}
	}
	m_widget->redraw();
}

void CIvfSelectionHandler::doMouseUp(int x, int y)
{

}

void CIvfSelectionHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void CIvfSelectionHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void CIvfSelectionHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void CIvfSelectionHandler::dispatchSingleSelectionEvent(CIvfShape *shape)
{
	if (m_singleSelectionEvent!=NULL)
		m_singleSelectionEvent->onSelect(shape);
}

void CIvfSelectionHandler::dispatchHighlightEvent(CIvfShape* shape)
{
	if (m_highlightEvent!=NULL)
		m_highlightEvent->onHighlight(shape);
}

void CIvfSelectionHandler::setSingleSelectionEvent(CIvfSingleSelectionEvent *event)
{
	m_singleSelectionEvent = event;
}

void CIvfSelectionHandler::setMultipleSelectionEvent(CIvfMultipleSelectionEvent *event)
{
	m_multipleSelectionEvent = event;
}

void CIvfSelectionHandler::setHighlightEvent(CIvfHighlightEvent* event)
{
	m_highlightEvent = event;
}

void CIvfSelectionHandler::dispatchMultipleSelectionEvent(CIvfSelectedShapesVector &selectedShapes)
{
	if (m_multipleSelectionEvent!=NULL)
		m_multipleSelectionEvent->onSelectMultiple(selectedShapes);
}

