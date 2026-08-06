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

#include <glm/glm.hpp>

#include <vector>

namespace ivf {

/**
 * Plain CPU-side indexed triangle mesh.
 *
 * Positions, normals, texture coordinates and colors are parallel arrays with
 * one entry per vertex. Indices are triangle index triples referencing those
 * arrays.
 *
 * MeshData is deliberately free of any OpenGL or scene graph dependency: it is
 * what a geometry generator (see ExtrusionBuilder) produces and what a
 * MeshBuffer consumes.
 */
struct MeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec4> colors;
    std::vector<glm::uvec3> indices;

    /** Returns the number of vertices. */
    std::size_t vertexCount() const
    {
        return positions.size();
    }

    /** Returns the number of triangles. */
    std::size_t triangleCount() const
    {
        return indices.size();
    }

    /** Returns true when there is nothing to draw. */
    bool empty() const
    {
        return positions.empty() || indices.empty();
    }

    /** Removes all vertices and indices. */
    void clear()
    {
        positions.clear();
        normals.clear();
        texCoords.clear();
        colors.clear();
        indices.clear();
    }

    /**
     * Returns true when every populated attribute array has one entry per
     * vertex and every index is in range.
     *
     * Normals, texture coordinates and colors may legitimately be empty --
     * MeshBuffer substitutes defaults for those -- but a partially filled
     * array indicates a generator bug.
     */
    bool isValid() const
    {
        std::size_t n = positions.size();

        if (!normals.empty() && normals.size() != n)
            return false;
        if (!texCoords.empty() && texCoords.size() != n)
            return false;
        if (!colors.empty() && colors.size() != n)
            return false;

        for (const auto &tri : indices)
            if (tri.x >= n || tri.y >= n || tri.z >= n)
                return false;

        return true;
    }
};

} // namespace ivf
