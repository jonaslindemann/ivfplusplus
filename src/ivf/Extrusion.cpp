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

#include <ivf/Extrusion.h>

#include <ivf/config.h>

using namespace ivf;

// ------------------------------------------------------------
void ivf::ivfSetGLEJoinStyle(int style)
{
    gleSetJoinStyle(style);
}

// ------------------------------------------------------------
int ivf::ivfGetGLEJoinStyle()
{
    return gleGetJoinStyle();
}

// ------------------------------------------------------------
Extrusion::Extrusion()
    : Shape()
{
    m_sectionCoords = nullptr;
    m_sectionNormals = nullptr;
    m_selectSectionCoords = nullptr;
    m_selectSectionNormals = nullptr;
    m_spineColors = nullptr;
    m_spineCoords = nullptr;
    m_sectionTransform = nullptr;
    m_twist = nullptr;
    m_upVector[0] = 0.0;
    m_upVector[1] = 1.0;
    m_upVector[2] = 0.0;
    m_useColors = false;
    m_selectScale = 1.05;
    m_useTwist = false;
    m_spineStart = 0;
    m_spineEnd = 0;
    m_spineSize = 0;
    m_textureMode = 0;

    // m_coords = new GLdouble[m_nCoords][3];
}

// ------------------------------------------------------------
Extrusion::~Extrusion()
{

    // Clean up

    if (m_sectionCoords != nullptr)
        delete[] m_sectionCoords;
    if (m_sectionNormals != nullptr)
        delete[] m_sectionNormals;
    if (m_spineCoords != nullptr)
        delete[] m_spineCoords;
    if (m_spineColors != nullptr)
        delete[] m_spineColors;
    if (m_selectSectionCoords != nullptr)
        delete[] m_selectSectionCoords;
    if (m_selectSectionNormals != nullptr)
        delete[] m_selectSectionNormals;
    if (m_twist != nullptr)
        delete[] m_twist;
    if (m_sectionTransform != nullptr)
        delete[] m_sectionTransform;
}

// ------------------------------------------------------------
void Extrusion::doCreateGeometry()
{
    if ((m_spineStart >= 0) && (m_spineEnd > 0))
        m_spineDelta = m_spineEnd - m_spineStart + 1;
    else
        m_spineDelta = m_spineSize;

    if (m_sectionCoords == nullptr)
        return;

    if (m_sectionNormals == nullptr)
        return;

    if (m_spineCoords == nullptr)
        return;

    if (m_useColors)
        if (m_spineColors == nullptr)
            return;

    if (m_useColors)
    {
        if (!m_useTwist)
        {
            if (m_sectionTransform == nullptr)
                gleExtrusion(m_sectionSize,
                    m_sectionCoords,
                    m_sectionNormals,
                    m_upVector,
                    m_spineDelta,
                    &m_spineCoords[m_spineStart],
                    &m_spineColors[m_spineStart]);
            else
                gleSuperExtrusion(m_sectionSize,
                    m_sectionCoords,
                    m_sectionNormals,
                    m_upVector,
                    m_spineDelta,
                    &m_spineCoords[m_spineStart],
                    &m_spineColors[m_spineStart],
                    &m_sectionTransform[m_spineStart]);
        }
        else
            gleTwistExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineDelta,
                &m_spineCoords[m_spineStart],
                &m_spineColors[m_spineStart],
                m_twist);
    }
    else
    {
        if (!m_useTwist)
        {
            if (m_sectionTransform == nullptr) 
            {
                gleExtrusion(m_sectionSize,
                    m_sectionCoords,
                    m_sectionNormals,
                    m_upVector,
                    m_spineDelta,
                    &m_spineCoords[m_spineStart],
                    nullptr);
            }
            else
            {
                gleSuperExtrusion(m_sectionSize,
                    m_sectionCoords,
                    m_sectionNormals,
                    m_upVector,
                    m_spineDelta,
                    &m_spineCoords[m_spineStart],
                    nullptr,
                    &m_sectionTransform[m_spineStart]);
            }
        }
        else
            gleTwistExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineDelta,
                &m_spineCoords[m_spineStart],
                nullptr,
                m_twist);
    }
}

// ------------------------------------------------------------
void Extrusion::setSpineSize(int size)
{
    // Delete previous spine coords/colors if any

    if (m_spineCoords != nullptr)
        delete[] m_spineCoords;
    if (m_spineColors != nullptr)
        delete[] m_spineColors;
    if (m_useTwist)
        if (m_twist != nullptr)
            delete[] m_twist;

    if (m_sectionTransform != nullptr)
        delete[] m_sectionTransform;

    // Set new size

    m_spineSize = size;

    // Allocate memory for new spine

    m_spineCoords = new gleDouble[m_spineSize][3];
    m_spineColors = new float[m_spineSize][3];
    if (m_useTwist)
        m_twist = new gleDouble[m_sectionSize];

    m_sectionTransform = new gleDouble[m_spineSize][2][3];

    // initialize spine color array

    Material* material;
    float color[3], alfa;

    material = this->getMaterial();
    if (material != nullptr)
        material->getDiffuseColor(color[0], color[1], color[2], alfa);
    else
    {
        color[0] = 1.0;
        color[1] = 1.0;
        color[2] = 1.0;
    }

    int i;
    for (i = 0; i < m_spineSize; i++)
    {
        m_spineColors[i][0] = color[0];
        m_spineColors[i][1] = color[1];
        m_spineColors[i][2] = color[2];
        if (m_useTwist)
            m_twist[i] = 0.0;

        m_sectionTransform[i][0][0] = 1.0;
        m_sectionTransform[i][0][1] = 0.0;
        m_sectionTransform[i][0][2] = 0.0;

        m_sectionTransform[i][1][0] = 0.0;
        m_sectionTransform[i][1][1] = 1.0;
        m_sectionTransform[i][1][2] = 0.0;

    }
}

// ------------------------------------------------------------
void Extrusion::setSectionSize(int size)
{
    // Delete previous section/normal arrays

    if (m_sectionCoords != nullptr)
        delete[] m_sectionCoords;
    if (m_sectionNormals != nullptr)
        delete[] m_sectionNormals;
    if (m_selectSectionCoords != nullptr)
        delete[] m_selectSectionCoords;
    if (m_selectSectionNormals != nullptr)
        delete[] m_selectSectionNormals;

    // Set section size

    m_sectionSize = size;

    // Allocate memory for new section

    m_sectionCoords = new gleDouble[m_sectionSize][2];
    m_sectionNormals = new gleDouble[m_sectionSize][2];
    m_selectSectionCoords = new gleDouble[m_sectionSize][2];
    m_selectSectionNormals = new gleDouble[m_sectionSize][2];
}

int Extrusion::getSpineSize()
{
    return m_spineSize;
}

int Extrusion::getSectionSize()
{
    return m_sectionSize;
}

// ------------------------------------------------------------
void Extrusion::setSectionCoord(int pos, double x, double y)
{
    if (m_sectionCoords != nullptr)
    {
        if ((pos >= 0) && (pos < m_sectionSize))
        {
            m_sectionCoords[pos][0] = x;
            m_sectionCoords[pos][1] = y;
            m_selectSectionCoords[pos][0] = x * m_selectScale;
            m_selectSectionCoords[pos][1] = y * m_selectScale;
        }
    }
}

// ------------------------------------------------------------
void Extrusion::setSectionNormal(int pos, double x, double y)
{
    if (m_sectionNormals != nullptr)
    {
        if ((pos >= 0) && (pos < m_sectionSize))
        {
            m_sectionNormals[pos][0] = x;
            m_sectionNormals[pos][1] = y;
            m_selectSectionNormals[pos][0] = x;
            m_selectSectionNormals[pos][1] = y;
        }
    }
}

// ------------------------------------------------------------
void Extrusion::setSpineCoord(int pos, double x, double y, double z)
{
    if (m_spineCoords != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            m_spineCoords[pos][0] = x;
            m_spineCoords[pos][1] = y;
            m_spineCoords[pos][2] = z;
        }
    }
}

// ------------------------------------------------------------
void Extrusion::getSectionCoord(int pos, double& x, double& y)
{
    if (m_sectionCoords != nullptr)
    {
        if ((pos >= 0) && (pos < m_sectionSize))
        {
            x = m_sectionCoords[pos][0];
            y = m_sectionCoords[pos][1];
        }
    }
}

// ------------------------------------------------------------
void Extrusion::getSectionNormal(int pos, double& x, double& y)
{
    if (m_sectionNormals != nullptr)
    {
        if ((pos >= 0) && (pos < m_sectionSize))
        {
            x = m_sectionNormals[pos][0];
            y = m_sectionNormals[pos][1];
        }
    }
}

// ------------------------------------------------------------
void Extrusion::getSpineCoord(int pos, double& x, double& y, double& z)
{
    if (m_spineCoords != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            x = m_spineCoords[pos][0];
            y = m_spineCoords[pos][1];
            z = m_spineCoords[pos][2];
        }
    }
}

// ------------------------------------------------------------
void Extrusion::setSpineColor(int pos, float red, float green, float blue)
{
    if (m_spineColors != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            m_spineColors[pos][0] = red;
            m_spineColors[pos][1] = green;
            m_spineColors[pos][2] = blue;
        }
    }
}

// ------------------------------------------------------------
void Extrusion::getSpineColor(int pos, float& red, float& green, float& blue)
{
    if (m_spineColors != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            red = m_spineColors[pos][0];
            green = m_spineColors[pos][1];
            blue = m_spineColors[pos][2];
        }
    }
}

// ------------------------------------------------------------
void Extrusion::setUseColor(bool flag)
{
    m_useColors = flag;
}

// ------------------------------------------------------------
bool Extrusion::getUseColor()
{
    return m_useColors;
}

// ------------------------------------------------------------
void Extrusion::setUpVector(double x, double y, double z)
{
    m_upVector[0] = x;
    m_upVector[1] = y;
    m_upVector[2] = z;
}

// ------------------------------------------------------------
void Extrusion::doCreateSelect()
{
    gleTextureMode(m_textureMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    if (m_useColors)
    {
        if (!m_useTwist)
            gleExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineSize,
                m_spineCoords,
                m_spineColors);
        else
            gleTwistExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineSize,
                m_spineCoords,
                m_spineColors,
                m_twist);
    }
    else
    {
        if (!m_useTwist)
            gleExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineSize,
                m_spineCoords,
                nullptr);
        else
            gleTwistExtrusion(m_sectionSize,
                m_sectionCoords,
                m_sectionNormals,
                m_upVector,
                m_spineSize,
                m_spineCoords,
                nullptr,
                m_twist);
    }
    glPopAttrib();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gleTextureMode(0);
}

// ------------------------------------------------------------
void Extrusion::setSelectScale(double scale)
{
    m_selectScale = scale;
}

// ------------------------------------------------------------
void Extrusion::setTextureMode(int mode)
{
    m_textureMode = mode;
    gleTextureMode(mode);
}

// ------------------------------------------------------------
void Extrusion::setUseTwist(bool flag)
{
    m_useTwist = flag;
    if (m_spineCoords != nullptr)
    {
        if (m_useTwist)
        {
            if (m_twist != nullptr)
                delete[] m_twist;

            m_twist = new gleDouble[m_spineSize];
            for (int i = 0; i < m_spineSize; i++)
                m_twist[i] = 0.0;
        }
        else
        {
            if (m_twist != nullptr)
                delete[] m_twist;

            m_twist = nullptr;
        }
    }
}

// ------------------------------------------------------------
bool Extrusion::getUseTwist()
{
    return m_useTwist;
}

// ------------------------------------------------------------
void Extrusion::setSpineTwist(int pos, double twist)
{
    if (m_twist != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            m_twist[pos] = twist;
        }
    }
}

// ------------------------------------------------------------
void Extrusion::getSpineTwist(int pos, double& twist)
{
    if (m_twist != nullptr)
    {
        if ((pos >= 0) && (pos < m_sectionSize))
        {
            twist = m_twist[pos];
        }
    }
}

void ivf::Extrusion::setSpineScale(int pos, double sx, double sy)
{
    if (m_sectionTransform != nullptr)
    {
        if ((pos >= 0) && (pos < m_spineSize))
        {
            m_sectionTransform[pos][0][0] = sx;
            m_sectionTransform[pos][0][1] = 0.0;
            m_sectionTransform[pos][0][2] = 0.0;

            m_sectionTransform[pos][1][0] = 0.0;
            m_sectionTransform[pos][1][1] = sy;
            m_sectionTransform[pos][1][2] = 0.0;
        }
    }
}

void Extrusion::setStartEnd(int startIdx, int endIdx)
{
    m_spineStart = startIdx;
    m_spineEnd = endIdx;
}

void Extrusion::setJoinStyle(int style)
{
    ivfSetGLEJoinStyle(style);
}

void Extrusion::doUpdateBoundingSphere()
{
    if (getBoundingSphere() != nullptr)
    {
        int i;
        double maxSize[3];
        double x, y, z;
        double radius;

        maxSize[0] = maxSize[1] = maxSize[2] = -1e300;

        for (i = 1; i < (m_spineSize - 1); i++)
        {
            x = m_spineCoords[i][0];
            y = m_spineCoords[i][1];
            z = m_spineCoords[i][2];

            if (fabs(x) > maxSize[0])
                maxSize[0] = x;
            if (fabs(y) > maxSize[1])
                maxSize[1] = y;
            if (fabs(z) > maxSize[2])
                maxSize[2] = z;
        }

        radius = sqrt(
            pow(maxSize[0], 2) + pow(maxSize[1], 2) + pow(maxSize[2], 2));

        getBoundingSphere()->setRadius(radius);
    }
}
