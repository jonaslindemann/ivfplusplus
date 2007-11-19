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

#ifndef _CIvfNode_h_
#define _CIvfNode_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCube.h>

IvfSmartPointer(CIvfNode);

/**
 * Node shape class
 *
 * CIvfNode implements a geometric node. The node shape
 * can represent a node as a sphere or a cube. The node class
 * is used by CIvfSolidLine.
 * @author Jonas Lindemann
 */
class IVF_API CIvfNode : public CIvfShape {
public:
	enum TNodeType {
		NT_SPHERE,
		NT_CUBE,
		NT_INVISIBLE
	};
public:
	/** CIvfNode constructor */
	CIvfNode ();

	/** CIvfNode destructor */
	virtual ~CIvfNode ();

	IvfClassInfo("CIvfNode",CIvfBase);
	
	/**
	 * Set node size.
	 *
	 * The node size is often controlled at a global
	 * level. When calling this method a reference to 
	 * a variable must be given.
	 */
	void setSize(double size);

	/** Return node size */
	double getSize();

	/**
	 * Set node type.
	 *
	 * @param nodeType IVF_NODE_SPHERE = spherical node shape. IVF_NODE_CUBE
	 * node is represented by a cube
	 */
	void setType(TNodeType nodeType);

	/** Return node type */
	TNodeType getNodeType();

	/** Updates bounding sphere for node */
	virtual void updateBoundingSphere();
private:
	double m_nodeSize;
	CIvfSelectionBoxPtr m_selectionBox;
	TNodeType m_nodeType;
	CIvfShapePtr m_nodeShape;
	CIvfCubePtr m_cubeShape;
	CIvfSpherePtr m_sphereShape;
protected:
	virtual void createSelect();
	virtual void createGeometry();
};
#endif
