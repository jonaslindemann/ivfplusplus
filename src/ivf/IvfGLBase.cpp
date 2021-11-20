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
CGLBase::CGLBase ()
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
CGLBase::CGLBase (const CGLBase&)
{
}

// ------------------------------------------------------------
CGLBase::~CGLBase ()
{
	if (m_useList)
		if (glIsList(m_displayList))
			glDeleteLists(m_displayList,1);
}

// ------------------------------------------------------------
CGLBase& CGLBase::operator = (const CGLBase &arg)
{
	return *this;
}

// ------------------------------------------------------------
void CGLBase::render ()
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
void CGLBase::doCreateGeometry ()
{
}

// ------------------------------------------------------------
void CGLBase::doCreateMaterial ()
{

}

// ------------------------------------------------------------
void CGLBase::doBeginTransform ()
{
}

// ------------------------------------------------------------
void CGLBase::doEndTransform ()
{
}

// ------------------------------------------------------------
void CGLBase::setSelect(TSelectState selectState)
{
	m_selectState = selectState;
}

// ------------------------------------------------------------
CGLBase::TSelectState CGLBase::getSelect()
{
	return m_selectState;
}

// ------------------------------------------------------------
void CGLBase::doCreateSelect()
{

}

// ------------------------------------------------------------
void CGLBase::compileList()
{
	// Remove any existing display list

	if (glIsList(m_displayList))
		glDeleteLists(m_displayList,1);

	// Assign a new displaylist name

	m_displayList = glGenLists(1);

	// Render display list

	glNewList(m_displayList, GL_COMPILE);
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
	glEndList();
}

// ------------------------------------------------------------
void CGLBase::setUselist(bool flag)
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
bool CGLBase::getUselist()
{
	return m_useList;
}

// ------------------------------------------------------------
void CGLBase::setDisplayList(GLuint nList)
{
	m_displayList = nList;
}

// ------------------------------------------------------------
GLuint CGLBase::getDisplayList()
{
	return m_displayList;
}

// ------------------------------------------------------------
CBoundingSphere* CGLBase::getBoundingSphere()
{
	return m_boundSphere;
}

// ------------------------------------------------------------
void CGLBase::setState(TObjectState state)
{
	m_state = state;
}

// ------------------------------------------------------------
CGLBase::TObjectState CGLBase::getState()
{
	return m_state;
}

// ------------------------------------------------------------
void CGLBase::setTag(long tag)
{
	m_tag = tag;
}

// ------------------------------------------------------------
long CGLBase::getTag()
{
	return m_tag;
}

// ------------------------------------------------------------
void CGLBase::setUseSelectShape(bool flag)
{
	m_useSelectShape = flag;
}

// ------------------------------------------------------------
bool CGLBase::getUseSelectShape()
{
	return m_useSelectShape;
}

// ------------------------------------------------------------
void CGLBase::initBoundingSphere()
{
	// Create a bounding sphere

	if (m_boundSphere==NULL)
		m_boundSphere = new CBoundingSphere();

	// Update bounding sphere

	this->doUpdateBoundingSphere();
}

// ------------------------------------------------------------
void CGLBase::doUpdateBoundingSphere()
{
	// Should be overridden
}

// ------------------------------------------------------------
void CGLBase::setCulled(bool flag)
{
	m_culled = flag;
}

// ------------------------------------------------------------
bool CGLBase::getCulled()
{
	return m_culled;
}

// ------------------------------------------------------------
void CGLBase::setName(const std::string& name)
{
	m_objectName = name;
}

// ------------------------------------------------------------
const std::string CGLBase::getName()
{
	return m_objectName;
}

// ------------------------------------------------------------
void CGLBase::setRenderState(CRenderState *state)
{
	m_renderState = state;
}

// ------------------------------------------------------------
CRenderState* CGLBase::getRenderState()
{
	return m_renderState;
}

void CGLBase::doPostGeometry()
{

}

void CGLBase::doPreGeometry()
{

}

void CGLBase::enable()
{
	m_state = OS_ON;
}

void CGLBase::disable()
{
	m_state = OS_OFF;
}

bool CGLBase::isEnabled()
{
	return (m_state==OS_ON);
}

void CGLBase::enableSelect()
{
	m_selectState = SS_ON;
}

void CGLBase::disableSelect()
{
	m_selectState = SS_OFF;
}

bool CGLBase::isSelectEnabled()
{
	return (m_selectState==SS_ON);
}

void CGLBase::setRenderMaterial(bool flag)
{
	m_renderMaterial = flag;
}

bool CGLBase::getRenderMaterial()
{
	return m_renderMaterial;
}
