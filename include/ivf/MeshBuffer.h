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

#pragma once

#include <ivfdef/Def.h>

#include <ivf/GL.h>
#include <ivf/MeshData.h>

#include <glm/glm.hpp>

#include <vector>

namespace ivf {

/**
 * Interleaved vertex layout used by MeshBuffer.
 *
 * The layout deliberately matches the attribute locations of the built-in
 * Blinn-Phong shader (position 0, normal 1, texture coordinate 2, color 3).
 */
struct MeshVertex {
    float position[3];
    float normal[3];
    float texCoord[2];
    float color[4];
};

/**
 * Indexed triangle mesh held in an OpenGL vertex buffer.
 *
 * MeshBuffer owns one interleaved vertex buffer and one element buffer, and
 * can draw them through either of the two paths an OpenGL compatibility
 * profile offers:
 *
 * - drawShader() binds a vertex array object wired to generic vertex
 *   attributes, for use with the modern shader pipeline.
 * - drawFixedFunction() binds the same buffers through glVertexPointer and
 *   friends, for use when no shader is active. This is still a buffer object
 *   draw -- no vertex data crosses the bus per frame -- it simply reaches the
 *   vertex data through the fixed-function attribute names.
 *
 * The two paths share the vertex and element buffers, so switching between
 * them costs nothing beyond the state changes of the draw itself.
 *
 * Mesh data is packed on the CPU as soon as it is handed over, but the OpenGL
 * objects are created lazily on the first draw. A MeshBuffer can therefore be
 * filled from a constructor, or at any other point before a context exists.
 *
 * @author Jonas Lindemann
 */
class IVF_API MeshBuffer {
private:
    std::vector<MeshVertex> m_vertices;
    std::vector<GLuint> m_indices;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    bool m_bufferDirty;    // Vertex/element data must be respecified
    bool m_verticesDirty;  // Vertex data changed in place, sizes unchanged
    GLsizei m_indexCount;  // Indices currently resident on the GPU
    GLsizei m_vertexCount; // Vertices currently resident on the GPU

    glm::vec3 m_boundsMin;
    glm::vec3 m_boundsMax;

    /** Uploads pending data, creating the OpenGL objects if needed. */
    void upload();

    /** Packs a MeshData into the interleaved CPU-side vertex array. */
    void pack(const MeshData &data);

public:
    /** MeshBuffer constructor. */
    MeshBuffer();

    /** MeshBuffer destructor. Releases the OpenGL objects. */
    ~MeshBuffer();

    // A MeshBuffer owns OpenGL names, which cannot be meaningfully copied.

    MeshBuffer(const MeshBuffer &) = delete;
    MeshBuffer &operator=(const MeshBuffer &) = delete;

    /**
     * Replaces the mesh.
     *
     * The data is packed immediately and uploaded on the next draw. Both the
     * vertex and the element buffer are respecified, so this is the method to
     * use whenever the topology may have changed.
     */
    void setMesh(const MeshData &data);

    /**
     * Replaces the vertex attributes, leaving the topology alone.
     *
     * Intended for the case where only positions, normals or colors changed
     * and the index list is known to be identical -- animating per vertex
     * colors, for instance. The element buffer is left untouched and the
     * vertex buffer is updated in place rather than reallocated.
     *
     * Falls back to setMesh() when the vertex count differs, so a caller that
     * gets this wrong produces a slower draw rather than a corrupt one.
     */
    void setVertexData(const MeshData &data);

    /** Removes all data and releases the OpenGL objects. */
    void clear();

    /** Returns true when there is nothing to draw. */
    bool isEmpty() const;

    /** Returns the number of vertices held. */
    int vertexCount() const;

    /** Returns the number of indices held. */
    int indexCount() const;

    /** Returns the lower corner of the axis aligned bounding box. */
    const glm::vec3 &boundsMin() const;

    /** Returns the upper corner of the axis aligned bounding box. */
    const glm::vec3 &boundsMax() const;

    /**
     * Returns the distance from the local origin to the furthest bounding box
     * corner, which is a valid bounding sphere radius for a shape whose
     * geometry is expressed in its own coordinate system.
     */
    double boundingRadius() const;

    /**
     * Draws through generic vertex attributes.
     *
     * The caller is responsible for having activated and updated a shader
     * program first -- MeshBuffer touches no uniforms and no material state.
     */
    void drawShader(GLenum mode = GL_TRIANGLES);

    /**
     * Draws through the fixed-function client arrays.
     *
     * Unbinds any shader program first, since a program in use would make the
     * fixed-function arrays inert. The program is left unbound afterwards.
     *
     * @param mode Primitive mode.
     * @param useColorArray When true the per vertex colors are submitted as a
     * color array. Lighting only reacts to that if the caller has enabled
     * GL_COLOR_MATERIAL.
     */
    void drawFixedFunction(GLenum mode = GL_TRIANGLES, bool useColorArray = false);

    /**
     * Draws through whichever path suits the current render context.
     *
     * Dispatches to drawShader() when a linked shader is active and to
     * drawFixedFunction() otherwise.
     */
    void draw(GLenum mode = GL_TRIANGLES, bool useColorArray = false);

    /**
     * Releases the OpenGL objects, keeping the CPU-side data.
     *
     * They are recreated on the next draw. Call this when the context the
     * buffers belong to is going away.
     */
    void releaseGL();
};

} // namespace ivf
