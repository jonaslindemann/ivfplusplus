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

#ifndef _CIvfComposite_h_
#define _CIvfComposite_h_

#include <ivf/ivfconfig.h>
#include <ivf/IvfShape.h>

IvfSmartPointer(CIvfComposite);

/**
 * Composite shape class.
 *
 * CIvfComposite implements an shape consisting of multiple
 * child objects. Example:
 *
 *\code
 * void CExampleWindow::onInit(int width, int heigth)
 * {
 *     .
 *     .
 *
 *     m_scene = new CIvfComposite();
 *
 *     CIvfSphere* sphere = new CIvfSphere();
 *     sphere->setPosition(3.0, 0.0, 0.0);
 *     CIvfCube* cube = new CIvfCube();
 *     cube->setPosition(-3.0, 0.0, 0.0);
 *
 *     m_scene->addChild(sphere);
 *     m_scene->addChild(cube);
 *
 *     .
 *     .
 *     .
 * }
 *
 * .
 * .
 * .
 * void CExampleWindow::onRender()
 * {
 *     .
 *     .
 *
 *     m_scene->render();
 *
 *     .
 *     .
 * }
 *\endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfComposite : public CIvfShape {
public:
	/** CIvfComposite constructor. */
	CIvfComposite ();

	/** CIvfComposite destructor. */
	virtual ~CIvfComposite ();

	IvfClassInfo("CIvfComposite",CIvfShape);

	/** Force object refresh */
	virtual void refresh();

	/**
	 * Determines if objects should be referenced.
	 *
	 * If set to true child object reference counting is to be
	 * used. If a child object is not referenced when it has been
	 * dereferenced by CIvfComposite it is deleted. If this property
	 * is set to false CIvfComposite does not reference its child
	 * objects.
	 */
	void setUseReference(bool flag);

	/**
	 * Set rendering interval
	 *
	 * @param interval - Renders every interval object in the composite
	 */
	void setRenderInterval(int interval);

	/**
	 * Set the highlight state of the child objects.
	 *
	 * Loops through the child objects and calls their
	 * setHighlight() methods.
	 * @param state HS_ON highlights all child objects.
	 * HS_OFF removes highlight from child objects.
	 */
	void setHighlightChildren(THighlightState state);

	/**
	 * Set the highlight state of the child objects.
	 *
	 * Loops through the child objects and calls their
	 * setHighlight() methods.
	 * @param state HS_ON highlights all child objects.
	 * HS_OFF removes highlight from child objects.
	 */
	void setHighlight(THighlightState state);

	void setHighlightComposite(THighlightState state);
	/**
	 * Set the select state of the child objects.
	 *
	 * Loops through the child objects and class their
	 * setSelect() methods.
	 * @param state IVF_SELECT_ON selectes all child objects.
	 * IVF_SELECT_OFF deselects all child objects.
	 */
	void setSelectChildren(TSelectState state);

	/** Refreshes all children */
	void refreshChildren();

	/**
	 * Deletes all child objects
	 *
	 * Deletes all child objects if they are unreferenced.
	 */
	void deleteAll();

	/**
	 * Removes child
	 *
	 * Removes specified child at index dereferences it
	 * and returns a reference.
	 * @param index Index of child to be removed (zero based)
	 * @returns Reference to removed object.
	 */
	virtual CIvfShape* removeChild(int index);

	/**
	 * Removes child shape
	 *
	 * Remove specified CIvfShape instance, dereferences it
	 * and returns a reference.
	 * @param removeShape Reference of object to remove
	 * @returns Reference to removed object. Returns NULL if
	 * object was not found.
	 */
	virtual CIvfShape* removeShape(CIvfShape* removeShape);

	/**
	 * Add child
	 *
	 * Adds a CIvfShape object to the composite object.
	 * @param shape CIvfShape object to be added.
	 */
	virtual void addChild(CIvfShape* shape);

	/**
	 * Delete child
	 *
	 * Deletes child at index. To be deleted the child object
	 * must be unreferenced.
	 * @param index Index of child object to be deleted.
	 */
	virtual void deleteChild(int index);

	/**
	 * Return child
	 *
	 * Returns a reference to the child at a given position.
	 * @param index Index of child to be returned (zero based)
	 * @returns Reference to a CIvfShape object. If invalid index
	 * NULL is returned.
	 */
	CIvfShape* getChild(int index);

	/**
	 * Clear composite object
	 *
	 * Removes all object references without deleting objects.
	 * If object reference counting is used all objects are
	 * dereferenced before being removed.
	 */
	void clear();

	/** Returns the number of objects in the composite object. */
	int getSize();

	/**
	 * Updates bounding sphere
	 *
	 * Upates the radius of the assigned bounding sphere, if any.
	 */
	void doUpdateBoundingSphere();

	/**
	 * Finds a named shape
	 *
	 * This method recursively searches the composite
	 * for a shape with a given name. This name is assigned
	 * using the setName() method. This function is often used
	 * in conjunction with the AC3D file reader which assigns
	 * names to each shape.
	 */
	CIvfShape* findShape(const std::string& name);

private:
	CIvfShape* compositeFind(CIvfComposite* composite, const std::string& name);
	bool m_useReference;
	int m_renderInterval;
	std::vector<CIvfShape*> m_children;
protected:
	void compositeRemove(CIvfComposite* composite, CIvfShape* deleteChild);
	virtual void doCreateGeometry();
};
/** \example placement.cpp
 * This is an example of how to position objects in a scene.
 * Using both primitives and CIvfComposite classes.
 */

#endif
