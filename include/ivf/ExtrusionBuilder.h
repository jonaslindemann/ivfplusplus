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

#include <ivf/ExtrusionProfile.h>
#include <ivf/MeshData.h>
#include <ivf/PathFrames.h>

#include <glm/glm.hpp>

#include <functional>
#include <vector>

namespace ivf {

/** Surface normal style for the swept walls. */
enum class NormalStyle {
    Smooth, // Per-vertex normals shared between adjacent faces (rounded look)
    Facet   // Per-face normals (flat look); profile points are duplicated per edge
};

/** How the V texture coordinate is generated along the path. */
enum class TexMode {
    Normalized, // V runs from 0 to 1 over the whole path
    PerLength   // V equals the cumulative arc length
};

/**
 * Per-station affine transform applied to the profile before placement.
 *
 * Drives tapering (scale), twisting (twist) and per-station coloring (color),
 * in the spirit of the gle library's gleSuperExtrusion and gleTwistExtrusion.
 */
struct SectionTransform {
    glm::vec2 scale{1.0f, 1.0f}; // Profile scale in (x, y)
    float twist{0.0f};           // Rotation of the profile about the tangent, in radians
    glm::vec4 color{1.0f};       // Vertex color for this station
};

/** Options controlling how a profile is swept along a set of frames. */
struct ExtrusionOptions {
    bool capStart{true};                          // Generate a cap at the first frame
    bool capEnd{true};                            // Generate a cap at the last frame
    bool closedPath{false};                       // Connect the last frame back to the first
    NormalStyle normalStyle{NormalStyle::Smooth}; // Wall normal style
    TexMode texMode{TexMode::Normalized};         // V texture coordinate mode

    /**
     * Optional per-station transform.
     *
     * Called once per station with the station index and the frame itself.
     * Callers holding per control point data (colors, twist, section scale)
     * should interpolate it using PathFrame::source, since stations and
     * control points are not one to one for every join style.
     */
    std::function<SectionTransform(int, const PathFrame &)> sectionFn;
};

/**
 * Sweeps a 2D profile along a sequence of oriented frames into an indexed
 * triangle mesh.
 *
 * The builder is a pure geometry generator: given a profile, a list of path
 * frames and a set of options it produces MeshData (walls plus optional caps).
 * It contains no OpenGL or scene graph dependencies.
 *
 * @author Jonas Lindemann
 */
class IVF_API ExtrusionBuilder {
public:
    static MeshData build(const ExtrusionProfile &profile, const std::vector<PathFrame> &frames,
                          const ExtrusionOptions &options = ExtrusionOptions());
};

} // namespace ivf
