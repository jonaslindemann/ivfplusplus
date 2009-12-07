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

#include <ivf/IvfSelectComposite.h>

// ------------------------------------------------------------
CIvfSelectComposite::CIvfSelectComposite ()
		:CIvfComposite()
{
	m_camera = NULL;
	m_childCount = 0;
	m_selectedShape = NULL;
	m_renderCamera = true;
	m_useCustomTransform = false;
}

// ------------------------------------------------------------
CIvfSelectComposite::~CIvfSelectComposite ()
{
	if (m_camera!=NULL)
		m_camera->deleteReference();

	if (!m_camera->referenced())
		delete m_camera;
}

// ------------------------------------------------------------
void CIvfSelectComposite::setCamera(CIvfView * camera)
{
	if (m_camera!=NULL)
	{
		m_camera->deleteReference();
		if (!m_camera->referenced())
			delete m_camera;
	}
	m_camera = camera;
	m_camera->addReference();
}

// ------------------------------------------------------------
CIvfView* CIvfSelectComposite::getCamera()
{
	return m_camera;
}

// ------------------------------------------------------------
void CIvfSelectComposite::doCreateGeometry()
{
	CIvfComposite::doCreateGeometry();
}

// ------------------------------------------------------------
void CIvfSelectComposite::doBeginTransform()
{
	glPushMatrix();
	if (m_renderCamera)
	{
		if (m_camera!=NULL)
			m_camera->render();
		else
			if (m_useCustomTransform)
				this->customView();
	}
	CIvfShape::doBeginTransform();
}

// ------------------------------------------------------------
void CIvfSelectComposite::doEndTransform()
{
		CIvfShape::doEndTransform();
	glPopMatrix();
}

// ------------------------------------------------------------
GLint CIvfSelectComposite::pick(int x, int y)
{
	m_selectedShape = NULL;

	GLint hits;

	glSelectBuffer(512, m_selectBuf);

	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(IVF_NONAME);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		if (m_renderCamera)
			glLoadIdentity();

		if (m_camera!=NULL)
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
	else m_selectedShape = NULL;

	return hits;
}

// ------------------------------------------------------------
void CIvfSelectComposite::processHits(GLint hits, GLuint buffer [ ])
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
			m_selectedShape = NULL;
	}
	else
		m_selectedShape = NULL;
}

// ------------------------------------------------------------
void CIvfSelectComposite::nameChildren(CIvfShape* shape)
{
	// Recursively name all children

	int i;

	if (shape->isClass("CIvfComposite"))
	{
		// Name all children of composite

		CIvfComposite* composite = (CIvfComposite*) shape;
		for (i = 0; i<composite->getSize(); i++)
		{
			CIvfShape* child = composite->getChild(i);
			nameChildren(child);
		}

		// If the useName flag is set name the composite

		if (composite->getUseName()==TRUE)
		{
			m_allObjects.push_back(composite);
			composite->setObjectName(m_allObjects.size()-1);
		}
	}
	else
	{
		// Name shape

		m_allObjects.push_back(shape);
		shape->setObjectName(m_allObjects.size()-1);
	}
}

// ------------------------------------------------------------
void CIvfSelectComposite::addChild(CIvfShape * shape)
{
	CIvfComposite::addChild(shape);
	nameChildren(shape);
}

// ------------------------------------------------------------
void CIvfSelectComposite::deleteChild(int index)
{
	//int i, count;
	CIvfComposite::deleteChild(index);

	// Renumber shapes

	renameChildren();
}

// ------------------------------------------------------------
CIvfShape* CIvfSelectComposite::removeChild(int index)
{
	//int i, count;
	CIvfShape* removedChild;
	removedChild = CIvfComposite::removeChild(index);

	// Renumber shapes

	renameChildren();

	return removedChild;
}

// ------------------------------------------------------------
CIvfShape* CIvfSelectComposite::getSelectedShape()
{
	return m_selectedShape;
}

// ------------------------------------------------------------
void CIvfSelectComposite::renameChildren()
{
	m_allObjects.clear();
	nameChildren(this);
}

// ------------------------------------------------------------
CIvfShape* CIvfSelectComposite::removeShape(CIvfShape *removeShape)
{
	CIvfComposite::compositeRemove(this, removeShape);
	renameChildren();
	return removeShape;
}

// ------------------------------------------------------------
void CIvfSelectComposite::initializeSelect(int x, int y, int w, int h)
{

}

// ------------------------------------------------------------
void CIvfSelectComposite::customView()
{

}

// ------------------------------------------------------------
void CIvfSelectComposite::setUseCustomTransform(bool flag)
{
	m_useCustomTransform = flag;
}

// ------------------------------------------------------------
void CIvfSelectComposite::setRenderCamera(bool flag)
{
	m_renderCamera = flag;
}
