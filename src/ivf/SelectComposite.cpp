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

#include <ivf/SelectComposite.h>

using namespace ivf;

// ------------------------------------------------------------
SelectComposite::SelectComposite ()
		:Composite()
{
	m_camera = nullptr;
	m_childCount = 0;
	m_selectedShape = nullptr;
	m_renderCamera = true;
	m_useCustomTransform = false;
}

// ------------------------------------------------------------
SelectComposite::~SelectComposite ()
{
}

// ------------------------------------------------------------
void SelectComposite::setCamera(View * camera)
{
	m_camera = camera;
}

// ------------------------------------------------------------
View* SelectComposite::getCamera()
{
	return m_camera;
}

// ------------------------------------------------------------
void SelectComposite::doCreateGeometry()
{
	Composite::doCreateGeometry();
}

// ------------------------------------------------------------
void SelectComposite::doBeginTransform()
{
	glPushMatrix();
	if (m_renderCamera)
	{
		if (m_camera!=nullptr)
			m_camera->render();
		else
			if (m_useCustomTransform)
				this->customView();
	}
	Shape::doBeginTransform();
}

// ------------------------------------------------------------
void SelectComposite::doEndTransform()
{
		Shape::doEndTransform();
	glPopMatrix();
}

// ------------------------------------------------------------
GLint SelectComposite::pick(int x, int y)
{
	m_selectedShape = nullptr;

	GLint hits;

	glSelectBuffer(512, m_selectBuf);

	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(IVF_NONAME);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		if (m_renderCamera)
			glLoadIdentity();

		if (m_camera!=nullptr)
			m_camera->initializeSelect(x, y, 4, 4);
		else
			this->initializeSelect(x, y, 4, 4);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			doBeginTransform();
			doCreateGeometry();
			doEndTransform();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

	hits = glRenderMode(GL_RENDER);

	if (hits!=0)
		processHits(hits, m_selectBuf);
	else m_selectedShape = nullptr;

	return hits;
}

// ------------------------------------------------------------
void SelectComposite::processHits(GLint hits, GLuint buffer [ ])
{
	GLuint depth = ~0;
	unsigned int i, getThisName;
	GLuint names, *ptr;
	GLuint objectName;

	ptr = (GLuint *) buffer;
	objectName = 9999;
	for (i = 0; i < (unsigned int)hits; i++) {
		getThisName = 0;
		names = *ptr;
		ptr++;
		if (*ptr <= depth) {
			depth = *ptr;
			getThisName = 1;
		}
		ptr++;
		if (*ptr <= depth) {
			depth = *ptr;
			getThisName = 1;
		}
		ptr++;

		if (getThisName)
			objectName = *ptr;

		ptr += names;
	}

	// We have an object

	if (objectName!=9999)
	{
		if (objectName<m_allObjects.size())
			m_selectedShape = m_allObjects[objectName];
		else
			m_selectedShape = nullptr;
	}
	else
		m_selectedShape = nullptr;
}

// ------------------------------------------------------------
void SelectComposite::nameChildren(Shape* shape)
{
	// Recursively name all children

	int i;

	if (shape->isClass("Composite"))
	{
		// Name all children of composite

		Composite* composite = (Composite*) shape;
		for (i = 0; i<composite->getSize(); i++)
		{
			Shape* child = composite->getChild(i);
			nameChildren(child);
		}

		// If the useName flag is set name the composite

		if (composite->getUseName()==TRUE)
		{
			m_allObjects.push_back(composite);
			composite->setObjectName(static_cast<int>(m_allObjects.size())-1);
		}
	}
	else
	{
		// Name shape

		m_allObjects.push_back(shape);
		shape->setObjectName(static_cast<int>(m_allObjects.size())-1);
	}
}

// ------------------------------------------------------------
void SelectComposite::addChild(Shape * shape)
{
	Composite::addChild(shape);
	nameChildren(shape);
}

// ------------------------------------------------------------
void SelectComposite::deleteChild(int index)
{
	//int i, count;
	Composite::deleteChild(index);

	// Renumber shapes

	renameChildren();
}

// ------------------------------------------------------------
Shape* SelectComposite::removeChild(int index)
{
	//int i, count;
	Shape* removedChild;
	removedChild = Composite::removeChild(index);

	// Renumber shapes

	renameChildren();

	return removedChild;
}

// ------------------------------------------------------------
Shape* SelectComposite::getSelectedShape()
{
	return m_selectedShape;
}

// ------------------------------------------------------------
void SelectComposite::renameChildren()
{
	m_allObjects.clear();
	nameChildren(this);
}

// ------------------------------------------------------------
Shape* SelectComposite::removeShape(Shape *removeShape)
{
	Composite::compositeRemove(this, removeShape);
	renameChildren();
	return removeShape;
}

// ------------------------------------------------------------
void SelectComposite::initializeSelect(int x, int y, int w, int h)
{

}

// ------------------------------------------------------------
void SelectComposite::customView()
{

}

// ------------------------------------------------------------
void SelectComposite::setUseCustomTransform(bool flag)
{
	m_useCustomTransform = flag;
}

// ------------------------------------------------------------
void SelectComposite::setRenderCamera(bool flag)
{
	m_renderCamera = flag;
}
