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

// Implementation of: public class Sphere

#include <ivf/config.h>
#include <ivf/Sphere.h>
#include <ivf/rc.h>

#include <cmath>
#include <vector>

using namespace ivf;

// ------------------------------------------------------------
Sphere::Sphere()
    : Shape(), m_stacks(16), m_slices(16), m_radius(1.0), m_meshDirty(true)
{
    m_selectionBox = new SelectionBox();
    m_selectionBox->setUseName(false);

    updateSelectBox();
}

// ------------------------------------------------------------
Sphere::~Sphere()
{
    delete m_selectionBox;
}

// ------------------------------------------------------------
void Sphere::markListDirty()
{
    m_meshDirty = true;
    GLBase::markListDirty();
}

// ------------------------------------------------------------
bool Sphere::useDisplayList()
{
    return false;
}

// ------------------------------------------------------------
void Sphere::updateGeometry()
{
    m_meshDirty = false;

    MeshData mesh;

    int stacks = (m_stacks < 2) ? 2 : m_stacks;
    int slices = (m_slices < 3) ? 3 : m_slices;

    float r = static_cast<float>(m_radius);

    // Poles on the y axis. The previous implementation reached the same
    // orientation by drawing a gluSphere, whose poles are on z, and rotating
    // it a quarter turn about x.

    std::size_t ringVerts = static_cast<std::size_t>(slices) + 1;

    mesh.positions.reserve(ringVerts * (stacks + 1));
    mesh.normals.reserve(ringVerts * (stacks + 1));
    mesh.texCoords.reserve(ringVerts * (stacks + 1));
    mesh.colors.reserve(ringVerts * (stacks + 1));

    for (int i = 0; i <= stacks; i++)
    {
        float phi = static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(stacks);
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        for (int j = 0; j <= slices; j++)
        {
            // The quarter turn puts the first slice where gluSphere put it.
            // GLU starts its azimuth on +y (x = -sin(theta) sin(rho),
            // y = cos(theta) sin(rho)) rather than on +x, so without this the
            // facets and the texture seam sit 90 degrees off where they used
            // to. It goes unnoticed whenever the slice count divides 90
            // degrees evenly -- 16 and 48 both do -- and is obvious at 6.

            float theta = 0.5f * static_cast<float>(M_PI) +
                          2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / static_cast<float>(slices);

            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            glm::vec3 n(sinPhi * cosTheta, cosPhi, sinPhi * sinTheta);

            mesh.positions.push_back(r * n);
            mesh.normals.push_back(n);
            mesh.texCoords.push_back(glm::vec2(static_cast<float>(j) / static_cast<float>(slices),
                                               static_cast<float>(i) / static_cast<float>(stacks)));
            mesh.colors.push_back(glm::vec4(1.0f));
        }
    }

    // Wound counter clockwise as seen from outside, so the front face is the
    // one the vertex normals point out of. Under two sided lighting a surface
    // wound the other way is lit with the negated normal, and backface culling
    // would remove it entirely.

    mesh.indices.reserve(static_cast<std::size_t>(stacks) * slices * 2);

    for (int i = 0; i < stacks; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            unsigned int a = static_cast<unsigned int>(i * ringVerts + j);
            unsigned int b = static_cast<unsigned int>(a + ringVerts);

            mesh.indices.push_back(glm::uvec3(a, a + 1, b));
            mesh.indices.push_back(glm::uvec3(b, a + 1, b + 1));
        }
    }

    m_buffer.setMesh(mesh);
}

// ------------------------------------------------------------
void Sphere::setRadius(const double radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;

        updateSelectBox();
        markListDirty();
    }
}

// ------------------------------------------------------------
double Sphere::getRadius()
{
    return m_radius;
}

// ------------------------------------------------------------
void Sphere::setSlices(const int slices)
{
    if (m_slices != slices)
    {
        m_slices = slices;
        markListDirty();
    }
}

// ------------------------------------------------------------
int Sphere::getSlices()
{
    return m_slices;
}

// ------------------------------------------------------------
void Sphere::setStacks(const int stacks)
{
    if (m_stacks != stacks)
    {
        m_stacks = stacks;
        markListDirty();
    }
}

// ------------------------------------------------------------
int Sphere::getStacks()
{
    return m_stacks;
}

// ------------------------------------------------------------
void Sphere::doCreateGeometry()
{
    if (m_meshDirty)
        updateGeometry();

    if (m_buffer.isEmpty())
        return;

    if (rcIsShaderActive())
    {
        rcUseShader();
        rcUpdateShader();
        m_buffer.drawShader(GL_TRIANGLES);
        return;
    }

    m_buffer.drawFixedFunction(GL_TRIANGLES, false);
}

// ------------------------------------------------------------
void Sphere::updateSelectBox()
{
    m_selectionBox->setSize(m_radius * 2.0, m_radius * 2.0, m_radius * 2.0);
}

// ------------------------------------------------------------
void Sphere::doCreateSelect()
{
    m_selectionBox->render();
}

// ------------------------------------------------------------
void Sphere::doUpdateBoundingSphere()
{
    // This is very simple

    if (getBoundingSphere() != nullptr)
        getBoundingSphere()->setRadius(m_radius);
}
