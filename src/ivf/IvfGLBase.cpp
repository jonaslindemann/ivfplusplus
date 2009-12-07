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

// Implementation of: public class CIvfGLBase

#include <ivf/ivfconfig.h>
#include <ivf/IvfGLBase.h>

// ------------------------------------------------------------
CIvfGLBase::CIvfGLBase ()
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
CIvfGLBase::CIvfGLBase (const CIvfGLBase&)
{
}

// ------------------------------------------------------------
CIvfGLBase::~CIvfGLBase ()
{
	if (m_useList)
		if (glIsList(m_displayList))
			glDeleteLists(m_displayList,1);
}

// ------------------------------------------------------------
CIvfGLBase& CIvfGLBase::operator = (const CIvfGLBase &arg)
{
	return *this;
}

// ------------------------------------------------------------
void CIvfGLBase::render ()
{
	if ((m_state == OS_ON)&&(!m_culled))
	{
		if (!m_useList)
		{
			if (m_renderState!=NULL)
				m_renderState->apply();

			doBeginTransform();

			if ((m_selectState == SS_ON)&&(m_useSelectShape))
				doCreateSelect();

			if (m_renderMaterial)
				doCreateMaterial();

			doPreGeometry();
			doCreateGeometry();
			doPostGeometry();
			doEndTransform();

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
void CIvfGLBase::doCreateGeometry ()
{
}

// ------------------------------------------------------------
void CIvfGLBase::doCreateMaterial ()
{

}

// ------------------------------------------------------------
void CIvfGLBase::doBeginTransform ()
{
}

// ------------------------------------------------------------
void CIvfGLBase::doEndTransform ()
{
}

// ------------------------------------------------------------
void CIvfGLBase::setSelect(TSelectState selectState)
{
	m_selectState = selectState;
}

// ------------------------------------------------------------
CIvfGLBase::TSelectState CIvfGLBase::getSelect()
{
	return m_selectState;
}

// ------------------------------------------------------------
void CIvfGLBase::doCreateSelect()
{

}

// ------------------------------------------------------------
void CIvfGLBase::compileList()
{
	// Remove any existing display list

	if (glIsList(m_displayList))
		glDeleteLists(m_displayList,1);

	// Assign a new displaylist name

	m_displayList = glGenLists(1);

	// Render display list

	glNewList(m_displayList, GL_COMPILE);
		doBeginTransform();
			doCreateMaterial();
			doCreateGeometry();
		doEndTransform();
	glEndList();
}

// ------------------------------------------------------------
void CIvfGLBase::setUselist(bool flag)
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
bool CIvfGLBase::getUselist()
{
	return m_useList;
}

// ------------------------------------------------------------
void CIvfGLBase::setDisplayList(GLuint nList)
{
	m_displayList = nList;
}

// ------------------------------------------------------------
GLuint CIvfGLBase::getDisplayList()
{
	return m_displayList;
}

// ------------------------------------------------------------
CIvfBoundingSphere* CIvfGLBase::getBoundingSphere()
{
	return m_boundSphere;
}

// ------------------------------------------------------------
void CIvfGLBase::setState(TObjectState state)
{
	m_state = state;
}

// ------------------------------------------------------------
CIvfGLBase::TObjectState CIvfGLBase::getState()
{
	return m_state;
}

// ------------------------------------------------------------
void CIvfGLBase::setTag(long tag)
{
	m_tag = tag;
}

// ------------------------------------------------------------
long CIvfGLBase::getTag()
{
	return m_tag;
}

// ------------------------------------------------------------
void CIvfGLBase::setUseSelectShape(bool flag)
{
	m_useSelectShape = flag;
}

// ------------------------------------------------------------
bool CIvfGLBase::getUseSelectShape()
{
	return m_useSelectShape;
}

// ------------------------------------------------------------
void CIvfGLBase::initBoundingSphere()
{
	// Create a bounding sphere

	if (m_boundSphere==NULL)
		m_boundSphere = new CIvfBoundingSphere();

	// Update bounding sphere

	this->doUpdateBoundingSphere();
}

// ------------------------------------------------------------
void CIvfGLBase::doUpdateBoundingSphere()
{
	// Should be overridden
}

// ------------------------------------------------------------
void CIvfGLBase::setCulled(bool flag)
{
	m_culled = flag;
}

// ------------------------------------------------------------
bool CIvfGLBase::getCulled()
{
	return m_culled;
}

// ------------------------------------------------------------
void CIvfGLBase::setName(const std::string& name)
{
	m_objectName = name;
}

// ------------------------------------------------------------
const std::string CIvfGLBase::getName()
{
	return m_objectName;
}

// ------------------------------------------------------------
void CIvfGLBase::setRenderState(CIvfRenderState *state)
{
	m_renderState = state;
}

// ------------------------------------------------------------
CIvfRenderState* CIvfGLBase::getRenderState()
{
	return m_renderState;
}

void CIvfGLBase::doPostGeometry()
{

}

void CIvfGLBase::doPreGeometry()
{

}

void CIvfGLBase::enable()
{
	m_state = OS_ON;
}

void CIvfGLBase::disable()
{
	m_state = OS_OFF;
}

bool CIvfGLBase::isEnabled()
{
	return (m_state==OS_ON);
}

void CIvfGLBase::enableSelect()
{
	m_selectState = SS_ON;
}

void CIvfGLBase::disableSelect()
{
	m_selectState = SS_OFF;
}

bool CIvfGLBase::isSelectEnabled()
{
	return (m_selectState==SS_ON);
}

void CIvfGLBase::setRenderMaterial(bool flag)
{
	m_renderMaterial = flag;
}

bool CIvfGLBase::getRenderMaterial()
{
	return m_renderMaterial;
}
