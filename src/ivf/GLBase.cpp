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

// Implementation of: public class CIvfGLBase

#include <ivf/config.h>
#include <ivf/GLBase.h>
#include <ivf/Material.h>
#include <ivf/rc.h>

using namespace ivf;

// ------------------------------------------------------------
GLBase::GLBase ()
{
	// Set default state on

	m_state = OS_ON;
	m_selectState = SS_OFF;

	// Default drawing behavior

	m_useList = false;
	m_listDirty = true;
	m_displayList = 0;

	m_tag = -1;

	// Dynamics, default is a static object

	m_dynamic = false;

	// Default a select shape is rendered

	m_useSelectShape = true;

	// Default all objects are not culled.

	m_culled = false;

	// Set objectname to nullptr

	m_renderMaterial = true;
}

// ------------------------------------------------------------
GLBase::GLBase (const GLBase&)
{
}

// ------------------------------------------------------------
GLBase::~GLBase ()
{
	// A list can outlive the flag that created it -- setUselist(false) is not the
	// only way m_useList becomes false -- so release on the list itself.

	if (m_displayList != 0)
		if (glIsList(m_displayList))
			glDeleteLists(m_displayList,1);
}

// ------------------------------------------------------------
GLBase& GLBase::operator = (const GLBase &arg)
{
	return *this;
}

// ------------------------------------------------------------
void GLBase::render ()
{
	if ((m_state == OS_ON)&&(!m_culled))
	{
		if (useDisplayList())
		{
			// Deliberately no glIsList() probe here. It is a server-side query
			// that can force a driver flush, and paying one per object per frame
			// on the hot path costs more than the list saves. m_listDirty and a
			// non-zero m_displayList are sufficient to track validity.

			if (m_listDirty)
				compileList();

			// compileList() clears m_useList if the driver refused to hand out a
			// list, so re-check rather than calling an invalid one.

			if (m_displayList != 0)
				glCallList(m_displayList);
			else
				renderImmediate();
		}
		else
		{
			renderImmediate();
		}
	}
	m_culled = false;
}

// ------------------------------------------------------------
bool GLBase::hasModernPath ()
{
	return false;
}
void GLBase::renderImmediate ()
{
	// An object with no modern path has to draw against the fixed-function
	// pipeline, which means no program may be bound while it runs. Binding one
	// globally and leaving it bound was the reason unported classes drew nothing:
	// their glVertex calls went through a shader whose matrices nobody had
	// uploaded, so every vertex collapsed to a point.
	//
	// The bracket covers the transform and material calls as well as the
	// geometry, because those are where the fixed-function state the legacy
	// drawing code reads gets established.

	const bool suppressShader = rcNeedsLegacyDraw(this->hasModernPath());

	if (suppressShader)
	{
		rcUnuseShader();
		rcBeginLegacyDraw();
	}

	if (m_renderState!=nullptr)
		m_renderState->apply();

	doBeginTransform();

	if ((m_selectState == SS_ON)&&(m_useSelectShape))
		doCreateSelect();

	if (m_renderMaterial)
		doCreateMaterial();

	doPreGeometry();

	// In Core there is no fixed-function pipeline behind the shader, so an object
	// that has not been ported simply cannot draw. Its geometry code is all lg*
	// no-ops by now, so calling it would achieve nothing; skipping it says so
	// plainly and keeps the profile_test report about coverage rather than about
	// a flood of errors from calls that were going to be ignored anyway.

	if (rcCanDrawGeometry(this->hasModernPath()))
		doCreateGeometry();

	doPostGeometry();
	doEndTransform();

	if (m_renderState!=nullptr)
		m_renderState->remove();

	if (suppressShader)
	{
		rcEndLegacyDraw();
		rcUseShader();
	}
}

// ------------------------------------------------------------
bool GLBase::useDisplayList ()
{
	// Display lists are a legacy-profile feature and cannot be combined with the
	// shader path. glNewList records drawing commands, but glUniform calls
	// execute immediately rather than being recorded -- so a replayed list draws
	// with whatever matrices were current when it was compiled. The result is
	// silently wrong rather than an error, which is the worst way to fail.

	if (rcIsShaderActive())
		return false;

	return m_useList && !m_dynamic;
}

// ------------------------------------------------------------
void GLBase::doCreateGeometry ()
{
}

// ------------------------------------------------------------
void GLBase::doCreateMaterial ()
{

}

// ------------------------------------------------------------
void GLBase::doBeginTransform ()
{
}

// ------------------------------------------------------------
void GLBase::doEndTransform ()
{
}

// ------------------------------------------------------------
void GLBase::setSelect(TSelectState selectState)
{
	m_selectState = selectState;
}

// ------------------------------------------------------------
GLBase::TSelectState GLBase::getSelect()
{
	return m_selectState;
}

// ------------------------------------------------------------
void GLBase::doCreateSelect()
{

}

// ------------------------------------------------------------
void GLBase::compileList()
{
	// Remove any existing display list

	if ((m_displayList != 0) && glIsList(m_displayList))
		glDeleteLists(m_displayList,1);

	// Assign a new displaylist name

	m_displayList = glGenLists(1);

	if (m_displayList == 0)
	{
		// No list available -- most likely no current context. Stop asking for one
		// every frame and fall back to drawing directly.

		m_useList = false;
		m_listDirty = false;
		return;
	}

	// Inside glNewList() the material calls are recorded rather than executed, so
	// what the material cache believes is currently applied says nothing about
	// what this list will contain. Suppress it for the duration.

	const bool materialCacheWasEnabled = Material::setStateCacheEnabled(false);

	// Render display list

	glNewList(m_displayList, GL_COMPILE);
		if (m_renderState!=nullptr)
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

		if (m_renderState!=nullptr)
			m_renderState->remove();
	glEndList();

	Material::setStateCacheEnabled(materialCacheWasEnabled);

	m_listDirty = false;
}

// ------------------------------------------------------------
void GLBase::setUselist(bool flag)
{
	m_useList = flag;

	if (m_useList)
	{
		// Compile lazily, on the next render(). Deferring it keeps this callable
		// from a constructor, or at any other point where no context is current.

		m_listDirty = true;
	}
	else
	{
		if ((m_displayList != 0) && glIsList(m_displayList))
			glDeleteLists(m_displayList,1);

		m_displayList = 0;
		m_listDirty = true;
	}
}

// ------------------------------------------------------------
bool GLBase::getUselist()
{
	return m_useList;
}

// ------------------------------------------------------------
void GLBase::markListDirty()
{
	m_listDirty = true;
}

// ------------------------------------------------------------
bool GLBase::isListDirty()
{
	return m_listDirty;
}

// ------------------------------------------------------------
void GLBase::setDynamic(bool flag)
{
	m_dynamic = flag;

	// Whatever was compiled describes a stale frame, so make sure the list is
	// rebuilt if the object later goes static again.

	if (m_dynamic)
		m_listDirty = true;
}

// ------------------------------------------------------------
bool GLBase::getDynamic()
{
	return m_dynamic;
}

// ------------------------------------------------------------
void GLBase::setDisplayList(GLuint nList)
{
	m_displayList = nList;
}

// ------------------------------------------------------------
GLuint GLBase::getDisplayList()
{
	return m_displayList;
}

// ------------------------------------------------------------
BoundingSphere* GLBase::getBoundingSphere()
{
	return m_boundSphere;
}

// ------------------------------------------------------------
void GLBase::setState(TObjectState state)
{
	m_state = state;
}

// ------------------------------------------------------------
GLBase::TObjectState GLBase::getState()
{
	return m_state;
}

// ------------------------------------------------------------
void GLBase::setTag(long tag)
{
	m_tag = tag;
}

// ------------------------------------------------------------
long GLBase::getTag()
{
	return m_tag;
}

// ------------------------------------------------------------
void GLBase::setUseSelectShape(bool flag)
{
	m_useSelectShape = flag;
}

// ------------------------------------------------------------
bool GLBase::getUseSelectShape()
{
	return m_useSelectShape;
}

// ------------------------------------------------------------
void GLBase::initBoundingSphere()
{
	// Create a bounding sphere

	if (m_boundSphere==nullptr)
		m_boundSphere = new BoundingSphere();

	// Update bounding sphere

	this->doUpdateBoundingSphere();
}

// ------------------------------------------------------------
void GLBase::doUpdateBoundingSphere()
{
	// Should be overridden
}

// ------------------------------------------------------------
void GLBase::setCulled(bool flag)
{
	m_culled = flag;
}

// ------------------------------------------------------------
bool GLBase::getCulled()
{
	return m_culled;
}

// ------------------------------------------------------------
void GLBase::setName(const std::string& name)
{
	m_objectName = name;
}

// ------------------------------------------------------------
const std::string GLBase::getName()
{
	return m_objectName;
}

// ------------------------------------------------------------
void GLBase::setRenderState(RenderState *state)
{
	m_renderState = state;
}

// ------------------------------------------------------------
RenderState* GLBase::getRenderState()
{
	return m_renderState;
}

void GLBase::doPostGeometry()
{

}

void GLBase::doPreGeometry()
{

}

void GLBase::enable()
{
	m_state = OS_ON;
}

void GLBase::disable()
{
	m_state = OS_OFF;
}

bool GLBase::isEnabled()
{
	return (m_state==OS_ON);
}

void GLBase::enableSelect()
{
	m_selectState = SS_ON;
}

void GLBase::disableSelect()
{
	m_selectState = SS_OFF;
}

bool GLBase::isSelectEnabled()
{
	return (m_selectState==SS_ON);
}

void GLBase::setRenderMaterial(bool flag)
{
	m_renderMaterial = flag;
}

bool GLBase::getRenderMaterial()
{
	return m_renderMaterial;
}
