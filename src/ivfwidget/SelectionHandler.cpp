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

#include <ivfwidget/SelectionHandler.h>

using namespace ivf;

SelectionHandler::SelectionHandler(WidgetBase* widget, Scene* scene)
{
	m_widget = widget;
	m_scene = scene;

	m_widget->addMouseMoveEvent(this);
	m_widget->addMouseDownEvent(this);
	m_widget->addMouseUpEvent(this);

	m_singleSelectionEvent = nullptr;
	m_multipleSelectionEvent = nullptr;
	m_highlightEvent = nullptr;
}

SelectionHandler::~SelectionHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
}

void SelectionHandler::doMouseDown(int x, int y)
{
	m_scene->pick(x, y);
	dispatchSingleSelectionEvent(m_scene->getSelectedShape());
	if (m_scene->getSelectionSize()>1)
		dispatchMultipleSelectionEvent(m_scene->getSelectedShapes());
}

void SelectionHandler::doMouseMove(int x, int y)
{
	m_scene->pick(x, y);

	if (m_scene->getSelectedShape()!=nullptr)
	{
		if (m_highlightEvent==nullptr)
		{
			if (m_oldShape!=nullptr)
				if (m_oldShape!=m_scene->getSelectedShape())
					m_oldShape->setHighlight(Shape::HS_OFF);
			m_scene->getSelectedShape()->setHighlight(Shape::HS_ON);
			m_oldShape = m_scene->getSelectedShape();
		}
		else
		{
			dispatchHighlightEvent(m_scene->getSelectedShape());			
		}
	}
	else
	{
		if (m_highlightEvent==nullptr)
		{
			if (m_oldShape!=nullptr)
				m_oldShape->setHighlight(Shape::HS_OFF);
		}
		else
		{
			dispatchHighlightEvent(nullptr);
		}
	}
	m_widget->redraw();
}

void SelectionHandler::doMouseUp(int x, int y)
{

}

void SelectionHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void SelectionHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void SelectionHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void SelectionHandler::dispatchSingleSelectionEvent(Shape *shape)
{
	if (m_singleSelectionEvent!=nullptr)
		m_singleSelectionEvent->onSelect(shape);
}

void SelectionHandler::dispatchHighlightEvent(Shape* shape)
{
	if (m_highlightEvent!=nullptr)
		m_highlightEvent->onHighlight(shape);
}

void SelectionHandler::setSingleSelectionEvent(SingleSelectionEvent *event)
{
	m_singleSelectionEvent = event;
}

void SelectionHandler::setMultipleSelectionEvent(MultipleSelectionEvent *event)
{
	m_multipleSelectionEvent = event;
}

void SelectionHandler::setHighlightEvent(HighlightEvent* event)
{
	m_highlightEvent = event;
}

void SelectionHandler::dispatchMultipleSelectionEvent(SelectedShapesVector &selectedShapes)
{
	if (m_multipleSelectionEvent!=nullptr)
		m_multipleSelectionEvent->onSelectMultiple(selectedShapes);
}

