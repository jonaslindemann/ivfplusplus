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

CSelectionHandler::CSelectionHandler(CWidgetBase* widget, CScene* scene)
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

CSelectionHandler::~CSelectionHandler()
{
	m_widget->removeMouseMoveEvent(this);
	m_widget->removeMouseDownEvent(this);
	m_widget->removeMouseUpEvent(this);
}

void CSelectionHandler::doMouseDown(int x, int y)
{
	m_scene->pick(x, y);
	dispatchSingleSelectionEvent(m_scene->getSelectedShape());
	if (m_scene->getSelectionSize()>1)
		dispatchMultipleSelectionEvent(m_scene->getSelectedShapes());
}

void CSelectionHandler::doMouseMove(int x, int y)
{
	m_scene->pick(x, y);

	if (m_scene->getSelectedShape()!=nullptr)
	{
		if (m_highlightEvent==nullptr)
		{
			if (m_oldShape!=nullptr)
				if (m_oldShape!=m_scene->getSelectedShape())
					m_oldShape->setHighlight(CShape::HS_OFF);
			m_scene->getSelectedShape()->setHighlight(CShape::HS_ON);
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
				m_oldShape->setHighlight(CShape::HS_OFF);
		}
		else
		{
			dispatchHighlightEvent(nullptr);
		}
	}
	m_widget->redraw();
}

void CSelectionHandler::doMouseUp(int x, int y)
{

}

void CSelectionHandler::onMouseDown(int x, int y)
{
	if (isActive())
		doMouseDown(x, y);
}

void CSelectionHandler::onMouseUp(int x, int y)
{
	if (isActive())
		doMouseUp(x, y);
}

void CSelectionHandler::onMouseMove(int x, int y)
{
	if (isActive())
		doMouseMove(x, y);
}

void CSelectionHandler::dispatchSingleSelectionEvent(CShape *shape)
{
	if (m_singleSelectionEvent!=nullptr)
		m_singleSelectionEvent->onSelect(shape);
}

void CSelectionHandler::dispatchHighlightEvent(CShape* shape)
{
	if (m_highlightEvent!=nullptr)
		m_highlightEvent->onHighlight(shape);
}

void CSelectionHandler::setSingleSelectionEvent(CSingleSelectionEvent *event)
{
	m_singleSelectionEvent = event;
}

void CSelectionHandler::setMultipleSelectionEvent(CMultipleSelectionEvent *event)
{
	m_multipleSelectionEvent = event;
}

void CSelectionHandler::setHighlightEvent(CHighlightEvent* event)
{
	m_highlightEvent = event;
}

void CSelectionHandler::dispatchMultipleSelectionEvent(CIvfSelectedShapesVector &selectedShapes)
{
	if (m_multipleSelectionEvent!=nullptr)
		m_multipleSelectionEvent->onSelectMultiple(selectedShapes);
}

