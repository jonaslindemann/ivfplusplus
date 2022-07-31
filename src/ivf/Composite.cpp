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

// Implementation of: public class CIvfComposite

#include <ivf/config.h>
#include <ivf/Composite.h>

#include <ivfmath/BoundingSphere.h>

using namespace ivf;

// ------------------------------------------------------------
Composite::Composite ()
		:Shape()
{
	setUseName(false);
	setUseReference(true);
	setRenderInterval(1);
}

// ------------------------------------------------------------
Composite::~Composite ()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		if (m_useReference)
		{
			shape->deleteReference();
			if (shape->referenced()!=TRUE)
				delete shape;
		}
	}
}

// ------------------------------------------------------------
void Composite::doCreateGeometry()
{
	unsigned int i;

	i=0;

	while (i<m_children.size())
	{
		if ((m_children[i]->getState()==GLBase::OS_ON)&&(!getCulled()))
			m_children[i]->render();

		i += m_renderInterval;
	}
}

// ------------------------------------------------------------
void Composite::addChild(Shape* shape)
{
	if (m_useReference)
		shape->addReference();
	m_children.push_back(shape);
}

// ------------------------------------------------------------
Shape* Composite::getChild(int index)
{
	if ((index>=0)&&(index<(int)m_children.size()))
		return m_children[index];
	else
		return nullptr;
}

// ------------------------------------------------------------
Shape* Composite::removeChild(int index)
{
	std::vector<Shape*>::iterator pos;

	if ((index>=0)&&(index<(int)m_children.size()))
	{
		Shape* shape = m_children[index];
		if (m_useReference)
			shape->deleteReference();
		pos = m_children.begin();
		pos += index;
		m_children.erase(pos);
		return shape;
	}
	else
		return nullptr;
}

// ------------------------------------------------------------
void Composite::deleteChild(int index)
{
	std::vector<Shape*>::iterator pos;

	if ((index>=0)&&(index<(int)m_children.size()))
	{
		Shape* shape = m_children[index];
		if (m_useReference)
			shape->deleteReference();
		pos = m_children.begin();
		pos += index;
		m_children.erase(pos);
		delete shape;
	}
}

// ------------------------------------------------------------
int Composite::getSize()
{
	return static_cast<int>(m_children.size());
}

// ------------------------------------------------------------
void Composite::clear()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		if (m_useReference)
			shape->deleteReference();
	}
	m_children.clear();
}

// ------------------------------------------------------------
void Composite::setSelectChildren(TSelectState state)
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		shape->setSelect(state);
	}
}

// ------------------------------------------------------------
void Composite::setHighlightChildren(THighlightState state)
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		shape->setHighlight(state);
	}
}

// ------------------------------------------------------------
void Composite::refresh()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		shape->refresh();
	}
}

// ------------------------------------------------------------
void Composite::setUseReference(bool flag)
{
	m_useReference = flag;
}

// ------------------------------------------------------------
void Composite::deleteAll()
{
	unsigned int i;

	for (i=0;i<m_children.size();i++)
	{
		Shape* shape = m_children[i];
		if (m_useReference)
		{
			shape->deleteReference();
			if (shape->referenced()!=TRUE)
				delete shape;
		}
	}
	m_children.clear();
}

// ------------------------------------------------------------
void Composite::setHighlight(THighlightState state)
{
	this->setHighlightChildren(state);
}

// ------------------------------------------------------------
void Composite::compositeRemove(Composite* composite, Shape* deleteChild)
{
	int i;

	for (i = 0; i<composite->getSize(); i++)
	{
		Shape* child = composite->getChild(i);

		if (child == deleteChild)
		{
			composite->removeChild(i);
		}
		else
			if (child->isClass("Composite"))
			{
				Composite* tree = (Composite*)child;
				compositeRemove(tree, deleteChild);
			}
	}
}

// ------------------------------------------------------------
Shape* Composite::removeShape(Shape *removeShape)
{
	compositeRemove(this, removeShape);
	return removeShape;
}

// ------------------------------------------------------------
void Composite::setRenderInterval(int interval)
{
	m_renderInterval = interval;
}

// ------------------------------------------------------------
void Composite::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		unsigned int i;

		double maxSize[3];
		double minSize[3];
		double midPoint[3];
		double x, y, z;
		double radius;

		maxSize[0] = maxSize[1] = maxSize[2] = -1e300;
		minSize[0] = minSize[1] = minSize[2] = 1e300;

		// Determine bounding box

		for (i=0;i<m_children.size();i++)
		{
			Shape* shape = m_children[i];
			shape->initBoundingSphere();
			shape->getPosition(x, y, z);
			BoundingSphere* bSphere = shape->getBoundingSphere();
			radius = bSphere->getRadius();

			if ( (x+radius)>maxSize[0] )
				maxSize[0] = x+radius;
			if ( (y+radius)>maxSize[1] )
				maxSize[1] = y+radius;
			if ( (z+radius)>maxSize[2] )
				maxSize[2] = x+radius;
			if ( (x-radius)<minSize[0] )
				minSize[0] = x-radius;
			if ( (y-radius)<minSize[1] )
				minSize[1] = y-radius;
			if ( (z-radius)<minSize[2] )
				minSize[2] = z-radius;
		}

		// Determine mid point of box

		midPoint[0] = (maxSize[0] + minSize[0])/2.0;
		midPoint[1] = (maxSize[1] + minSize[1])/2.0;
		midPoint[2] = (maxSize[2] + minSize[2])/2.0;

		// Determine sphere radius

		radius = sqrt( pow(maxSize[0]-midPoint[0],2) +
			pow(maxSize[1]-midPoint[1],2) +
			pow(maxSize[2]-midPoint[2],2));

		this->getBoundingSphere()->setRadius(radius);
	}
}

Shape* Composite::findShape(const std::string& name)
{
	return compositeFind(this, name);
}

Shape* Composite::compositeFind(Composite *composite, const std::string& name)
{
	int i;

	for (i = 0; i<composite->getSize(); i++)
	{
		Shape* child = composite->getChild(i);

		if (child->getName()!="")
		{
			if (child->getName() == name)
				return child;
		}

		if (child->isClass("Composite"))
		{
			Shape* shape = compositeFind((Composite*)child, name);
			if (shape!=nullptr)
				return shape;
		}
	}
	return nullptr;
}


void Composite::setHighlightComposite(THighlightState state)
{
	Shape::setHighlight(state);
}
