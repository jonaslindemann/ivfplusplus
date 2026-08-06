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

#include <ivf/Shape.h>
#include <ivf/SelectionBox.h>
#include <ivf/MeshBuffer.h>
#include <ivf/GL.h>

namespace ivf {

IvfSmartPointer(Sphere);

/**
 * Sphere primitive
 *
 * The sphere is tessellated once into a vertex buffer and drawn from there,
 * through the shader pipeline or the fixed-function one depending on what is
 * active. There is a single tessellation behind both, so the two pipelines
 * cannot disagree about what a sphere looks like, and no display list is
 * needed to keep it cheap -- see useDisplayList().
 *
 * @author Jonas Lindemann
 */
class IVF_API Sphere : public Shape {
public:
	/** Constructor */
	Sphere ();

	/** Destructor */
	virtual ~Sphere ();

	IvfClassInfo("Sphere",Shape);
	IvfStdFactory(Sphere);

	/** Set sphere radius */
	void setRadius (double radius);

	/** Return sphere radius */
	double getRadius ();

	/** Set number of stacks in sphere representation */
	void setStacks(int stacks);

	/** Return number of stacks */
	int getStacks();

	/** Set number of slices in sphere representation */
	void setSlices(int slices);

	/** Return number of slices */
	int getSlices();

	/** Update bounding sphere size */
	virtual void doUpdateBoundingSphere() override;

	/**
	 * Marks the tessellation as out of date.
	 *
	 * Rebuilt on the next render. The property setters do this themselves.
	 */
	virtual void markListDirty() override;

private:
	void updateSelectBox();

	/** Regenerates the tessellation into the vertex buffer. */
	void updateGeometry();

	SelectionBox*  m_selectionBox;
	int            m_stacks;
	int            m_slices;
	double         m_radius;

	MeshBuffer     m_buffer;
	bool           m_meshDirty;

protected:
    virtual void doCreateSelect() override;
    virtual void doCreateGeometry() override;

	/**
	 * Never replays a display list.
	 *
	 * The vertex buffer is already the cached representation. Wrapping a
	 * buffer draw in a display list caches nothing and adds a second thing to
	 * keep in step, so setUselist() has no effect on a sphere.
	 */
	virtual bool useDisplayList() override;
};

}