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

// Implementation of: public class CIvfObject

#include <ivf/ivfconfig.h>
#include <ivf/IvfObject.h>

// ------------------------------------------------------------
CIvfObject::CIvfObject ()
{
	// Set default state on

	m_state = OS_ON;
	m_selectState = SS_OFF;

	// Default drawing behavior

	m_useList = false;

	m_tag = -1;

	// Dynamics, default is a static object

	m_dynamic = false;

	// Default a select shape is rendered

	m_useSelectShape = true;

	// Default all objects are not culled.

	m_culled = false;

	// Set objectname to NULL

	m_renderMaterial = true;
}

// ------------------------------------------------------------
CIvfObject::CIvfObject (const CIvfObject&)
{
}

// ------------------------------------------------------------
CIvfObject::~CIvfObject ()
{
	if (m_useList)
		if (glIsList(m_displayList))
			glDeleteLists(m_displayList,1);
}

// ------------------------------------------------------------
CIvfObject& CIvfObject::operator = (const CIvfObject &arg)
{
	return *this;
}

// ------------------------------------------------------------
void CIvfObject::render ()
{
	if ((m_state == OS_ON)&&(!m_culled))
	{
		if (!m_useList)
		{
			if (m_renderState!=NULL)
				m_renderState->apply();

			beginTransform();

			if ((m_selectState == SS_ON)&&(m_useSelectShape))
				createSelect();

			if (m_renderMaterial)
				createMaterial();

			preGeometry();
			createGeometry();
			postGeometry();
			endTransform();

			if (m_renderState!=NULL)
				m_renderState->remove();
		}
		else
		{
			glCallList(getDisplayList());
		}
	}
	m_culled = false;
}

// ------------------------------------------------------------
void CIvfObject::createGeometry ()
{
}

// ------------------------------------------------------------
void CIvfObject::createMaterial ()
{

}

// ------------------------------------------------------------
void CIvfObject::beginTransform ()
{
}

// ------------------------------------------------------------
void CIvfObject::endTransform ()
{
}

// ------------------------------------------------------------
void CIvfObject::setSelect(TSelectState selectState)
{
	m_selectState = selectState;
}

// ------------------------------------------------------------
CIvfObject::TSelectState CIvfObject::getSelect()
{
	return m_selectState;
}

// ------------------------------------------------------------
void CIvfObject::createSelect()
{

}

// ------------------------------------------------------------
void CIvfObject::compileList()
{
	// Remove any existing display list

	if (glIsList(m_displayList))
		glDeleteLists(m_displayList,1);

	// Assign a new displaylist name

	m_displayList = glGenLists(1);

	// Render display list

	glNewList(m_displayList, GL_COMPILE);
		beginTransform();
			createMaterial();
			createGeometry();
		endTransform();
	glEndList();
}

// ------------------------------------------------------------
void CIvfObject::setUselist(bool flag)
{
	m_useList = flag;

	if (m_useList) 
	{
		compileList();
	}
	else
	{
		if (glIsList(m_displayList))
			glDeleteLists(m_displayList,1);
	}
}

// ------------------------------------------------------------
bool CIvfObject::getUselist()
{
	return m_useList;
}

// ------------------------------------------------------------
void CIvfObject::setDisplayList(GLuint nList)
{
	m_displayList = nList;
}

// ------------------------------------------------------------
GLuint CIvfObject::getDisplayList()
{
	return m_displayList;	
}

// ------------------------------------------------------------
CIvfBoundingSphere* CIvfObject::getBoundingSphere()
{
	return m_boundSphere;
}

// ------------------------------------------------------------
void CIvfObject::setState(TObjectState state)
{
	m_state = state;
}

// ------------------------------------------------------------
CIvfObject::TObjectState CIvfObject::getState()
{
	return m_state;
}

// ------------------------------------------------------------
void CIvfObject::setTag(long tag)
{
	m_tag = tag;
}

// ------------------------------------------------------------
long CIvfObject::getTag()
{
	return m_tag;
}

// ------------------------------------------------------------
void CIvfObject::setUseSelectShape(bool flag)
{
	m_useSelectShape = flag;
}

// ------------------------------------------------------------
bool CIvfObject::getUseSelectShape()
{
	return m_useSelectShape;
}

// ------------------------------------------------------------
void CIvfObject::initBoundingSphere()
{
	// Create a bounding sphere

	if (m_boundSphere==NULL)
		m_boundSphere = new CIvfBoundingSphere();

	// Update bounding sphere

	this->updateBoundingSphere();
}

// ------------------------------------------------------------
void CIvfObject::updateBoundingSphere()
{
	// Should be overridden
}

// ------------------------------------------------------------
void CIvfObject::setCulled(bool flag)
{
	m_culled = flag;
}

// ------------------------------------------------------------
bool CIvfObject::getCulled()
{
	return m_culled;
}

// ------------------------------------------------------------
void CIvfObject::setName(const char *name)
{
	m_objectName = name;
}

// ------------------------------------------------------------
const char* CIvfObject::getName()
{
	return m_objectName.c_str();
}

// ------------------------------------------------------------
void CIvfObject::setRenderState(CIvfRenderState *state)
{
	m_renderState = state;
}

// ------------------------------------------------------------
CIvfRenderState* CIvfObject::getRenderState()
{
	return m_renderState;
}

void CIvfObject::postGeometry()
{

}

void CIvfObject::preGeometry()
{

}

void CIvfObject::enable()
{
	m_state = OS_ON;
}

void CIvfObject::disable()
{
	m_state = OS_OFF;
}

bool CIvfObject::isEnabled()
{
	return (m_state==OS_ON);
}

void CIvfObject::enableSelect()
{
	m_selectState = SS_ON;
}

void CIvfObject::disableSelect()
{
	m_selectState = SS_OFF;
}

bool CIvfObject::isSelectEnabled()
{
	return (m_selectState==SS_ON);
}

void CIvfObject::setRenderMaterial(bool flag)
{
	m_renderMaterial = flag;
}

bool CIvfObject::getRenderMaterial()
{
	return m_renderMaterial;
}
