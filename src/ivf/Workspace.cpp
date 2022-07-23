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
//
// Written by Jonas Lindemann
//

#include <ivf/Workspace.h>

using namespace ivf;

Workspace::Workspace()
{
    m_rootPlane = new ConstructionPlane();
    m_rootPlane->setSize(10.0, 10.0);
    m_rootPlane->disableCursor();

    if (this->getView()->isClass("Camera"))
        m_rootPlane->setCamera((Camera*)this->getView());

    m_workplanes.push_back(m_rootPlane);

    m_currentPlane = m_rootPlane;
    m_currentPlaneIdx = 0;
    m_usePlanes = true;
    m_useCursorShape = false;
    m_useCursor = true;
    m_hideCursor = false;

    //this->addChild(m_currentPlane->getCursor());
}

Workspace::~Workspace()
{
    clearPlanes();
    delete m_rootPlane;
}

void Workspace::addPlane(ConstructionPlane *plane)
{
    m_workplanes.push_back(plane);
    if (this->getView()->isClass("Camera"))
        plane->setCamera((Camera*)this->getView());
    plane->addReference();
}

void Workspace::clearPlanes()
{
    int i;

    for (i = 0; i < (int)m_workplanes.size(); i++)
    {
        if (i != 0)
        {
            ConstructionPlane* plane = m_workplanes[i];
            plane->deleteReference();

            if (!plane->referenced())
                delete plane;
        }
    }
    m_workplanes.clear();
    m_workplanes.push_back(m_rootPlane);
}

void Workspace::setCurrentPlane(int idx)
{
    if ((idx >= 0) && (idx < (int)m_workplanes.size()))
    {
        m_currentPlane->deactivate();
        m_currentPlane->unlockCursor();
        m_currentPlane = m_workplanes[idx];
        m_currentPlane->activate();
        m_currentPlaneIdx = 0;
    }
}

void Workspace::nextPlane()
{
    m_currentPlane->deactivate();
    m_currentPlane->unlockCursor();

    if (m_currentPlaneIdx < (int)m_workplanes.size() - 1)
        m_currentPlaneIdx++;
    else
        m_currentPlaneIdx = 0;

    m_currentPlane = m_workplanes[m_currentPlaneIdx];
    m_currentPlane->activate();
}

ConstructionPlane* Workspace::getCurrentPlane()
{
    return m_currentPlane;
}

void Workspace::prevPlane()
{
    m_currentPlane->deactivate();
    m_currentPlane->unlockCursor();

    if (m_currentPlaneIdx > 0)
        m_currentPlaneIdx--;
    else
        m_currentPlaneIdx = static_cast<int>(m_workplanes.size()) - 1;

    m_currentPlane = m_workplanes[m_currentPlaneIdx];
    m_currentPlane->activate();
}

void Workspace::updateCursor(int x, int y)
{
    m_currentPlane->updateCursor(x, y);
}

void Workspace::doCreateGeometry()
{
    SceneBase::doCreateGeometry();

    if (m_usePlanes)
    {
        m_rootPlane->render();

        int i;

        for (i = 0; i < (int)m_workplanes.size(); i++)
            m_workplanes[i]->render();
    }

    if ((m_useCursor)&&(!m_hideCursor))
    {
        double x, y, z, w, h;
        Vec3d pos = m_currentPlane->getCursorPosition();
        pos.getComponents(x, y, z);

        w = m_currentPlane->getWidth();
        h = m_currentPlane->getWidth();

        if ((x > -w / 2.0) && (x < w / 2.0) && (z > -w / 2.0) && (z < w / 2.0))
        {
            if (m_cursorShape != nullptr)
            {
                m_cursorShape->setPosition(x, y, z);
                if (m_useCursorShape)
                    m_cursorShape->enable();
                else
                    m_cursorShape->disable();

                m_cursorShape->render();
            }

            glPushAttrib(GL_ENABLE_BIT);

            glDisable(GL_LIGHTING);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            if (m_currentPlane->isCursorLocked())
            {
                glBegin(GL_LINES);
                glColor3f(0.3, 0.3, 0.3);
                glVertex3d(x, 0.0, z);
                glVertex3d(x, y, z);
                glEnd();
            }
            else
            {
                glBegin(GL_LINES);
                glColor3f(0.3, 0.3, 0.3);
                glVertex3d(-w / 2.0, y + 0.002, z);
                glVertex3d(+w / 2.0, y + 0.002, z);
                glVertex3d(x, y + 0.002, -w / 2.0);
                glVertex3d(x, y + 0.002, w / 2.0);
                glEnd();
            }

            glPopAttrib();
        }
    }
}

bool Workspace::isCursorLocked()
{
    return m_currentPlane->isCursorLocked();
}

void Workspace::lockCursor()
{
    m_currentPlane->lockCursor();
}

void Workspace::unlockCursor()
{
    m_currentPlane->unlockCursor();
}

Vec3d& Workspace::getCursorPosition()
{
    Vec3d& r = ivfGetTempVec3d();
    r = m_currentPlane->getCursorPosition();
    return r;
}

void Workspace::setView(View *view)
{
    SceneBase::setView(view);
    if (view->isClass("Camera"))
        m_currentPlane->setCamera((Camera*)view);
}

void Workspace::setRelativeCursorSize(double size)
{
    int i;

    for (i = 0; i < (int)m_workplanes.size(); i++)
        m_workplanes[i]->setRelativeCursorSize(size);
}

void Workspace::setWorkspaceSize(double size)
{
    m_rootPlane->setSize(size, size);
}

void Workspace::updateSizes()
{
    m_currentPlane->updateSizes();
}

void Workspace::setRelativeAxisSize(double size)
{
    int i;

    for (i = 0; i < (int)m_workplanes.size(); i++)
        m_workplanes[i]->setRelativeAxisSize(size);
}

void Workspace::setUsePlanes(bool flag)
{
    m_usePlanes = flag;
}

bool Workspace::getUsePlanes()
{
    return m_usePlanes;
}

void Workspace::setUseCursor(bool flag)
{
    m_useCursor = flag;
}

void Workspace::setCursorShape(Shape * shape)
{
    m_cursorShape = shape;
}

Shape * Workspace::cursorShape()
{
    return m_cursorShape;
}

void Workspace::setUseCursorShape(bool flag)
{
    m_useCursorShape = flag;
}

bool Workspace::useCursorShape()
{
    return m_useCursorShape;
}

bool Workspace::getUseCursor()
{
    return m_useCursor;
}

double Workspace::getWorkspaceSize()
{
    return m_rootPlane->getWidth();
}

void Workspace::setSize(double size)
{
    m_rootPlane->setSize(size, size);
}

double Workspace::getSize()
{
    return m_rootPlane->getWidth();
}

void Workspace::enableCursor()
{
    m_useCursor = true;
}

void Workspace::disableCursor()
{
    m_useCursor = false;
}

void Workspace::enableCursorShape()
{
    m_useCursorShape = true;
}

void Workspace::disableCursorShape()
{
    m_useCursorShape = false;
}

void Workspace::hideCursor()
{
    m_hideCursor = true;
}

void Workspace::showCursor()
{
    m_hideCursor = false;
}


void Workspace::doPostClear()
{
    this->addChild(m_currentPlane->getCursor());
}

