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

#include <ivf/LineSet.h>

using namespace ivf;

LineSet::LineSet()
{
    m_useColor = false;
    m_useAlpha = false;
    m_lineWidth = 1.0;
}

LineSet::~LineSet()
{
}

void LineSet::doCreateGeometry()
{
    Index* coordIdx;
    Index* colorIdx;
    Index* textureIdx;

    long i, j;
    float oldWidth[1];

    glPushAttrib(GL_LIGHTING | GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);

    glGetFloatv(GL_LINE_WIDTH, oldWidth);

    glLineWidth(m_lineWidth);

    if (m_useColor)
        glEnable(GL_COLOR_MATERIAL);

    for (i = 0; i < (int)m_coordIndexSet.size(); i++)
    {
        if (m_idxLineWidth.size() > 0)
        {
            if (i < m_idxLineWidth.size())
                glLineWidth(m_idxLineWidth[i]);
        }
        glBegin(GL_LINES);

        coordIdx = m_coordIndexSet[i];
        if (m_useColor)
        {
            if (i < (int)m_colorIndexSet.size())
                colorIdx = m_colorIndexSet[i];
        }

        if (i < (int)m_textureIndexSet.size())
            textureIdx = m_textureIndexSet[i];
        else
            textureIdx = nullptr;

        for (j = 0; j < coordIdx->getSize(); j++)
        {
            if (m_useColor)
            {
                if (m_useAlpha)
                    glColor4fv(m_colorSet[colorIdx->getIndex(j)]->getColor());
                else
                    glColor3fv(m_colorSet[colorIdx->getIndex(j)]->getColor());
            }
            else if (Shape::getMaterial() != nullptr)
                Shape::getMaterial()->render();
            else if (Shape::getMaterial() != nullptr)
                Shape::getMaterial()->render();
            else
                glColor3f(1.0f, 1.0f, 1.0f);

            if (textureIdx != nullptr)
                glTexCoord2dv(m_textureCoordSet[textureIdx->getIndex(j)]->getComponents());

            glVertex3dv(m_coordSet[coordIdx->getIndex(j)]->getComponents());
        }
        glEnd();
    }

    glLineWidth(oldWidth[0]);

    glPopAttrib();
}

void LineSet::setUseColor(bool flag)
{
    m_useColor = flag;
}

bool LineSet::getUseColor()
{
    return m_useColor;
}

void LineSet::setUseAlpha(bool flag)
{
    m_useAlpha = flag;
}

bool LineSet::getUseAlpha()
{
    return m_useAlpha;
}

void LineSet::setLineWidth(float width)
{
    m_lineWidth = width;
}

float LineSet::getLineWidth()
{
    return m_lineWidth;
}

void ivf::LineSet::addIndexLineWidth(float w)
{
    m_idxLineWidth.push_back(w);
}

void ivf::LineSet::clearIndexWidths()
{
    m_idxLineWidth.clear();
}
