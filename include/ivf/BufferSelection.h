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

#pragma once

#include <ivf/Base.h>
#include <ivf/Shape.h>
#include <ivf/Composite.h>
#include <ivf/View.h>

namespace ivf {

IvfSmartPointer(BufferSelection);

typedef std::vector<Shape*> SelectedShapesVector;
typedef std::vector<Shape*>::iterator SelectedShapesIterator;

/**
 * Buffer selection class
 *
 * This class implements object selection using the OpenGL
 * selection buffer. The following code shows an example of
 * using the CIvfBufferSelection class:
 * 
 * \code
 * void CExampleWindow::onInit(int width, int heigth)
 * {
 *     m_camera = new CIvfCamera();
 *     m_camera->setPosition(...);
 *     
 *     m_scene = new CIvfComposite();
 *     
 *     ... Add some children to scene ...
 *
 *     m_selection = new CIvfBufferSelection();
 *     m_selection->setView(m_camera);
 *     
 *     .
 *     .
 *     .
 * }
 * 
 * .
 * .
 * .
 *
 *	if (isLeftButtonDown())
 *	{
 *		m_selection->pick(x, y);
 *		if (m_selection->getSelectedShape()!=nullptr)
 *		{
 *			m_selection->getSelectedShape()->setHighlight(Shape::HS_ON);
 *			redraw();
 *		}
 *		else
 *		{
 *			m_scene->setHighlightChildren(Shape::HS_OFF);
 *			redraw();
 *		}
 *	}
 *	
 *	if (isRightButtonDown())
 *	{
 *		m_selection->pick(x, y);
 *		if (m_selection->getSize()!=0)
 *		{
 *			for (i=0; i<m_selection->getSize(); i++)
 *				m_selection->getSelectedShape(i)->setHighlight(Shape::HS_ON);
 *
 *			redraw();
 *		}
 *		else
 *		{
 *			m_scene->setHighlightChildren(Shape::HS_OFF);
 *			redraw();
 *		}
 *	}
 * \endcode 
 * @author Jonas Lindemann
 */
class IVF_API BufferSelection : public Base {
private:
	Shape* m_selectedShape;
	Composite* m_composite;
	View* m_camera;
	std::vector<Shape*> m_allObjects;

	std::vector<Shape*> m_selectedShapes;

	int m_childCount;
	GLuint m_selectBuf[512];

	void nameChildren(Shape* shape);
	void processHits(GLint hits, GLuint buffer []);
public:
	/** CIvfBufferSelection constructor */
	BufferSelection();

	/** CIvfBufferSelection destructor */
	virtual ~BufferSelection();
	
	IvfClassInfo("CIvfBufferSelection",Base);
	IvfStdFactory(BufferSelection);

	/** 
	 * Initiates a object selection
	 * 
	 * This method can only be used when a valid OpenGL context exists.
	 * 
	 * @param x - x position of mouse cursor in current viewport
	 * @param y - y position of mouse cursor in current viewport
	 * @return number of object "hits"
	 */
	int pick(int x, int y);

	/**
	 * Updates object selection list
	 *
	 * Selection is implemented by giving all objects in the 
	 * composite a number and storing them in the object selection
	 * list, which is used for quick object lookup. When objects
	 * are added to the composite this list must be updated with the
	 * update() method.
	 */
	void update();

	/** 
	 * Sets the objects used in the selection operation (required)
	 *
	 * This method also calls the update() method for updating the
	 * object selection list.
	 * @see update
	 */
	void setComposite(Composite* composite);

	/** Returns the composite with objects used in the selection */
	Composite* getComposite();

	/** Sets the view used for selection (required)*/
	void setView(View* view);

	/** Returns view used for selection (required) */
	View* getView();
	
	/** 
	 * Returns the topmost selected shape.
	 *
	 * Returns nullptr if no object is selected or the selection
	 * operation failed.
	 */
	Shape* getSelectedShape();

	/** Return the total number of selected shapes */
	int getSize();

	/** 
	 * Returns the specified selected shape.
	 *
	 * Returns nullptr if no object is selected or the selection
	 * operation failed.
	 */
	Shape* getSelectedShape(int idx);

	/**
	 * Returns selected shapes
	 *
	 * Returns a STL vector of the selected shapes.
	 */
	SelectedShapesVector& getSelectedShapes();
};
/** \example selection.cpp
 * This is an example of how to use the CIvfBufferSelection
 * class to select objects in a scene.
 */
}