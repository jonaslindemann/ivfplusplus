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

#include <ivf/GLBase.h>
#include <ivf/config.h>

using namespace ivf;

// ------------------------------------------------------------
GLBase::GLBase()
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

    // Set objectname to nullptr

    m_renderMaterial = true;
}

// ------------------------------------------------------------
GLBase::GLBase(const GLBase&)
{
}

// ------------------------------------------------------------
GLBase::~GLBase()
{
    if (m_useList)
        if (glIsList(m_displayList))
            glDeleteLists(m_displayList, 1);
}

// ------------------------------------------------------------
GLBase& GLBase::operator=(const GLBase& arg)
{
    return *this;
}

// ------------------------------------------------------------
void GLBase::render()
{
    if ((m_state == OS_ON) && (!m_culled))
    {
        if (!m_useList)
        {
            if (m_renderState != nullptr)
                m_renderState->apply();

            doBeginTransform();

            if ((m_selectState == SS_ON) && (m_useSelectShape))
                doCreateSelect();

            if (m_renderMaterial)
                doCreateMaterial();

            doPreGeometry();
            doCreateGeometry();
            doPostGeometry();
            doEndTransform();

            if (m_renderState != nullptr)
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
void GLBase::doCreateGeometry()
{
}

// ------------------------------------------------------------
void GLBase::doCreateMaterial()
{
}

// ------------------------------------------------------------
void GLBase::doBeginTransform()
{
}

// ------------------------------------------------------------
void GLBase::doEndTransform()
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

    if (glIsList(m_displayList))
        glDeleteLists(m_displayList, 1);

    // Assign a new displaylist name

    m_displayList = glGenLists(1);

    // Render display list

    glNewList(m_displayList, GL_COMPILE);
    if (m_renderState != nullptr)
        m_renderState->apply();

    doBeginTransform();

    if ((m_selectState == SS_ON) && (m_useSelectShape))
        doCreateSelect();

    if (m_renderMaterial)
        doCreateMaterial();

    doPreGeometry();
    doCreateGeometry();
    doPostGeometry();
    doEndTransform();

    if (m_renderState != nullptr)
        m_renderState->remove();
    glEndList();
}

// ------------------------------------------------------------
void GLBase::setUselist(bool flag)
{
    m_useList = flag;

    if (m_useList)
    {
        compileList();
    }
    else
    {
        if (glIsList(m_displayList))
            glDeleteLists(m_displayList, 1);
    }
}

// ------------------------------------------------------------
bool GLBase::getUselist()
{
    return m_useList;
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

    if (m_boundSphere == nullptr)
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
void GLBase::setRenderState(RenderState* state)
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
    return (m_state == OS_ON);
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
    return (m_selectState == SS_ON);
}

void GLBase::setRenderMaterial(bool flag)
{
    m_renderMaterial = flag;
}

bool GLBase::getRenderMaterial()
{
    return m_renderMaterial;
}
