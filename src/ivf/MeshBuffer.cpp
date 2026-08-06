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

// Implementation of: public class MeshBuffer

#include <ivf/MeshBuffer.h>

#include <ivf/rc.h>

#include <cmath>
#include <cstddef>

using namespace ivf;

namespace {

const GLsizei kStride = static_cast<GLsizei>(sizeof(MeshVertex));

// Byte offsets of the interleaved attributes, as buffer object offsets.

const void *kPositionOffset = reinterpret_cast<const void *>(offsetof(MeshVertex, position));
const void *kNormalOffset = reinterpret_cast<const void *>(offsetof(MeshVertex, normal));
const void *kTexCoordOffset = reinterpret_cast<const void *>(offsetof(MeshVertex, texCoord));
const void *kColorOffset = reinterpret_cast<const void *>(offsetof(MeshVertex, color));

} // namespace

// ------------------------------------------------------------
MeshBuffer::MeshBuffer()
    : m_vao(0), m_vbo(0), m_ebo(0), m_bufferDirty(false), m_verticesDirty(false), m_indexCount(0), m_vertexCount(0),
      m_boundsMin(0.0f), m_boundsMax(0.0f)
{
}

// ------------------------------------------------------------
MeshBuffer::~MeshBuffer()
{
    releaseGL();
}

// ------------------------------------------------------------
void MeshBuffer::pack(const MeshData &data)
{
    std::size_t n = data.positions.size();

    m_vertices.clear();
    m_vertices.resize(n);

    bool hasNormals = (data.normals.size() == n);
    bool hasTexCoords = (data.texCoords.size() == n);
    bool hasColors = (data.colors.size() == n);

    m_boundsMin = glm::vec3(0.0f);
    m_boundsMax = glm::vec3(0.0f);

    for (std::size_t i = 0; i < n; i++)
    {
        MeshVertex &v = m_vertices[i];

        const glm::vec3 &p = data.positions[i];
        v.position[0] = p.x;
        v.position[1] = p.y;
        v.position[2] = p.z;

        if (hasNormals)
        {
            const glm::vec3 &nrm = data.normals[i];
            v.normal[0] = nrm.x;
            v.normal[1] = nrm.y;
            v.normal[2] = nrm.z;
        }
        else
        {
            v.normal[0] = 0.0f;
            v.normal[1] = 0.0f;
            v.normal[2] = 1.0f;
        }

        if (hasTexCoords)
        {
            const glm::vec2 &t = data.texCoords[i];
            v.texCoord[0] = t.x;
            v.texCoord[1] = t.y;
        }
        else
        {
            v.texCoord[0] = 0.0f;
            v.texCoord[1] = 0.0f;
        }

        if (hasColors)
        {
            const glm::vec4 &c = data.colors[i];
            v.color[0] = c.r;
            v.color[1] = c.g;
            v.color[2] = c.b;
            v.color[3] = c.a;
        }
        else
        {
            v.color[0] = 1.0f;
            v.color[1] = 1.0f;
            v.color[2] = 1.0f;
            v.color[3] = 1.0f;
        }

        if (i == 0)
        {
            m_boundsMin = p;
            m_boundsMax = p;
        }
        else
        {
            m_boundsMin = glm::min(m_boundsMin, p);
            m_boundsMax = glm::max(m_boundsMax, p);
        }
    }
}

// ------------------------------------------------------------
void MeshBuffer::setMesh(const MeshData &data)
{
    pack(data);

    m_indices.clear();
    m_indices.reserve(data.indices.size() * 3);

    for (const auto &tri : data.indices)
    {
        m_indices.push_back(static_cast<GLuint>(tri.x));
        m_indices.push_back(static_cast<GLuint>(tri.y));
        m_indices.push_back(static_cast<GLuint>(tri.z));
    }

    m_bufferDirty = true;
    m_verticesDirty = false;
}

// ------------------------------------------------------------
void MeshBuffer::setVertexData(const MeshData &data)
{
    // The caller promises the topology is unchanged. Verify the one thing that
    // would make the existing index list invalid, and fall back to a full
    // respecification rather than drawing past the end of the buffer.

    if (data.positions.size() != m_vertices.size())
    {
        setMesh(data);
        return;
    }

    pack(data);

    // A pending full respecification already covers this update.

    if (!m_bufferDirty)
        m_verticesDirty = true;
}

// ------------------------------------------------------------
void MeshBuffer::clear()
{
    releaseGL();

    m_vertices.clear();
    m_indices.clear();

    m_bufferDirty = false;
    m_verticesDirty = false;
    m_boundsMin = glm::vec3(0.0f);
    m_boundsMax = glm::vec3(0.0f);
}

// ------------------------------------------------------------
void MeshBuffer::releaseGL()
{
    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    m_indexCount = 0;
    m_vertexCount = 0;

    // The data still lives on the CPU, so a later draw can recreate the
    // objects. Anything already packed has to be sent again.

    if (!m_vertices.empty())
        m_bufferDirty = true;
}

// ------------------------------------------------------------
void MeshBuffer::upload()
{
    if (m_vertices.empty() || m_indices.empty())
    {
        m_indexCount = 0;
        m_vertexCount = 0;
        m_bufferDirty = false;
        m_verticesDirty = false;
        return;
    }

    if (m_vbo == 0)
    {
        glGenBuffers(1, &m_vbo);
        m_bufferDirty = true;
    }

    if (m_ebo == 0)
    {
        glGenBuffers(1, &m_ebo);
        m_bufferDirty = true;
    }

    if (m_bufferDirty)
    {
        // Respecify both buffers. Binding the element buffer would be captured
        // by whatever vertex array object happens to be bound, so make sure
        // that is the default one.

        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertices.size() * sizeof(MeshVertex)),
                     m_vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_indices.size() * sizeof(GLuint)),
                     m_indices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_vertexCount = static_cast<GLsizei>(m_vertices.size());
        m_indexCount = static_cast<GLsizei>(m_indices.size());

        m_bufferDirty = false;
        m_verticesDirty = false;

        // The vertex array object caches attribute pointers into m_vbo. The
        // name is unchanged, so the existing configuration stays valid.
    }
    else if (m_verticesDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_vertices.size() * sizeof(MeshVertex)),
                        m_vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_verticesDirty = false;
    }

    if (m_vao == 0)
    {
        glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kStride, kPositionOffset);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kStride, kNormalOffset);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kStride, kTexCoordOffset);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, kStride, kColorOffset);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

// ------------------------------------------------------------
bool MeshBuffer::isEmpty() const
{
    return m_vertices.empty() || m_indices.empty();
}

// ------------------------------------------------------------
int MeshBuffer::vertexCount() const
{
    return static_cast<int>(m_vertices.size());
}

// ------------------------------------------------------------
int MeshBuffer::indexCount() const
{
    return static_cast<int>(m_indices.size());
}

// ------------------------------------------------------------
const glm::vec3 &MeshBuffer::boundsMin() const
{
    return m_boundsMin;
}

// ------------------------------------------------------------
const glm::vec3 &MeshBuffer::boundsMax() const
{
    return m_boundsMax;
}

// ------------------------------------------------------------
double MeshBuffer::boundingRadius() const
{
    if (m_vertices.empty())
        return 0.0;

    // The geometry is expressed in the shape's own coordinate system, so the
    // sphere has to be centered on that origin rather than on the box center.

    glm::vec3 extreme = glm::max(glm::abs(m_boundsMin), glm::abs(m_boundsMax));

    return static_cast<double>(glm::length(extreme));
}

// ------------------------------------------------------------
void MeshBuffer::drawShader(GLenum mode)
{
    upload();

    if (m_indexCount == 0)
        return;

    glBindVertexArray(m_vao);
    glDrawElements(mode, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// ------------------------------------------------------------
void MeshBuffer::drawFixedFunction(GLenum mode, bool useColorArray)
{
    upload();

    if (m_indexCount == 0)
        return;

    // A bound program silently defeats this entire path. The fixed-function
    // arrays would be ignored, except that most drivers alias glVertexPointer
    // onto generic attribute 0 -- so the geometry still appears, in the right
    // place, with no normals or colors reaching the shader. That reads as a
    // lighting bug rather than a pipeline mix-up, so rule it out here: a
    // fixed-function draw is by definition one with no program in use.

    glUseProgram(0);

    // Client array state belongs to the bound vertex array object. Bind the
    // default one so this cannot leak into the shader path's configuration.

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, kStride, kPositionOffset);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, kStride, kNormalOffset);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, kStride, kTexCoordOffset);

    if (useColorArray)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, kStride, kColorOffset);
    }

    glDrawElements(mode, m_indexCount, GL_UNSIGNED_INT, nullptr);

    if (useColorArray)
        glDisableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ------------------------------------------------------------
void MeshBuffer::draw(GLenum mode, bool useColorArray)
{
    if (rcIsShaderActive())
        drawShader(mode);
    else
        drawFixedFunction(mode, useColorArray);
}
