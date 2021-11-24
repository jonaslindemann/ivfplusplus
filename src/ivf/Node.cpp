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

// Implementation of: public class Node

#include <ivf/config.h>
#include <ivf/Node.h>

using namespace ivf;

// ------------------------------------------------------------
Node::Node ()
		:Shape()
		//TODO: check and complete member initialisation list!
{
	m_nodeSize = 0.2;

	m_sphereShape = new Sphere();
	m_cubeShape = new Cube();

	m_sphereShape->setRadius(0.2);
	m_cubeShape->setSize(0.2);
	m_cubeShape->setUseName(false);
	m_sphereShape->setUseName(false);

	m_nodeShape = m_cubeShape;
	m_nodeType = NT_CUBE;

	m_selectionBox = new SelectionBox();
	m_selectionBox->setUseName(false);
	/*
	m_pBC = nullptr;
	m_pNodeForce = nullptr;
	m_pNodeMoment = nullptr;
	*/
}

// ------------------------------------------------------------
Node::~Node ()
{

}

// ------------------------------------------------------------
void Node::doCreateGeometry()
{
	m_sphereShape->setRadius(m_nodeSize);
	m_cubeShape->setSize(m_nodeSize);

	if (m_nodeShape)
		m_nodeShape->render();
}

// ------------------------------------------------------------
void Node::doCreateSelect()
{
	m_selectionBox->setSize(m_nodeSize, m_nodeSize, m_nodeSize);
	m_selectionBox->render();
}

// ------------------------------------------------------------
void Node::setSize(double size)
{
	m_nodeSize = size;
	doUpdateBoundingSphere();
}

// ------------------------------------------------------------
double Node::getSize()
{
	return m_nodeSize;
}

// ------------------------------------------------------------
void Node::setType(TNodeType nodeType)
{
	m_nodeType = nodeType;
	switch (m_nodeType) {
	case NT_SPHERE:
		m_nodeShape = m_sphereShape;
		break;
	case NT_CUBE:
		m_nodeShape = m_cubeShape;
		break;
	case NT_INVISIBLE:
		m_nodeShape = nullptr;
		break;
	default:
		m_nodeShape = m_cubeShape;
		break;
	}
}

// ------------------------------------------------------------
Node::TNodeType Node::getNodeType()
{
	return m_nodeType;
}

void Node::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		double radius;
		radius = sqrt(2.0*pow(m_nodeSize,2));
	}
}
