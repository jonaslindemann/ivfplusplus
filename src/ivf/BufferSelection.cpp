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

#include <ivf/BufferSelection.h>

using namespace ivf;

CBufferSelection::CBufferSelection()
{
	m_composite = NULL;
	m_selectedShape = NULL;
	m_camera = NULL;
}

CBufferSelection::~CBufferSelection()
{

}

int CBufferSelection::pick(int x, int y)
{
	m_selectedShapes.clear();
	m_selectedShape = NULL;

	if ((m_composite!=NULL)&&(m_camera!=NULL))
	{
		
		GLint hits;
		
		glSelectBuffer(512, m_selectBuf);
		
		glRenderMode(GL_SELECT);
		
		glInitNames();
		glPushName(IVF_NONAME);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();
		
		m_camera->initializeSelect(x, y, 4, 4);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		m_camera->render();
		m_composite->render();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		
		hits = glRenderMode(GL_RENDER);
		
		if (hits!=0)
			processHits(hits, m_selectBuf);
		
		return hits;
	}
	return -1;
}

void CBufferSelection::setComposite(CComposite *composite)
{
	m_composite = composite;
	update();
}

CComposite* CBufferSelection::getComposite()
{
	return m_composite;
}

void CBufferSelection::update()
{
	m_allObjects.clear();
	nameChildren(m_composite);
}

void CBufferSelection::nameChildren(CShape *shape)
{
	// Recursively name all children

	int i;

	if (shape->isClass("CIvfComposite"))
	{
		// Name all children of composite

		CComposite* composite = (CComposite*) shape;
		for (i = 0; i<composite->getSize(); i++)
		{
			CShape* child = composite->getChild(i);
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

CShape* CBufferSelection::getSelectedShape()
{
	/*
	if (m_selectedShapes.size()>0)
		return m_selectedShapes[m_selectedShapes.size()-1];
	else
		return NULL;
	*/
	return m_selectedShape;
}

void CBufferSelection::setView(CView *view)
{
	m_camera = view;
}

CView* CBufferSelection::getView()
{
	return m_camera;
}

void CBufferSelection::processHits(GLint hits, GLuint buffer[])
{
	GLuint depth = ~0;
	unsigned int i, getThisName, j;
	GLuint nNames, *ptr;
	GLuint objectName;
	GLuint nearZ, farZ;

	double minZ = 1e300;
	
	ptr = (GLuint *) buffer;

	m_selectedShape = NULL;

	for (i = 0; i < (unsigned int)hits; i++) 
	{  
		getThisName = 0;
		nNames = *ptr;
		ptr++;

		for (j=0; j< nNames; j++)
		{			
			nearZ = *ptr;
			ptr++;
			farZ = *ptr;
			ptr++;
			objectName = *ptr;
			ptr++;

			if (objectName!=IVF_NONAME)
			{
				m_selectedShapes.push_back(m_allObjects[objectName]);

				if (nearZ<minZ)
				{
					minZ = nearZ;
					m_selectedShape = m_allObjects[objectName];
				}
			}
		}
	}
}

int CBufferSelection::getSize()
{
	return m_selectedShapes.size();
}

CShape* CBufferSelection::getSelectedShape(int idx)
{
	if ((idx>=0)&&(idx<(int)m_selectedShapes.size()))
		return m_selectedShapes[idx];
	else
		return 0;
}

CIvfSelectedShapesVector& CBufferSelection::getSelectedShapes()
{
	return m_selectedShapes;
}
