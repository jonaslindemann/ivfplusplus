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

#pragma once

#include <ivf/ShaderProgram.h>

namespace ivf {

/**
 * Position-only shader used to fill a ShadowMap.
 *
 * The fragment shader is empty: depth is written by the fixed hardware stage,
 * and a shader that writes no colour and contains no discard lets the driver
 * take its double-rate depth-only path. That is why this is a separate program
 * rather than a branch inside BlinnPhongShader -- that one contains a discard
 * for the alpha test, which costs the fast path for every fragment whether the
 * branch is taken or not.
 *
 * It also means the depth pass uploads no material, no lights and no texture
 * state, which is most of the per-shape cost of a scene traversal.
 *
 * The vertex attribute layout matches BlinnPhongShader so the same VAOs draw
 * through either program without rebuilding:
 *   location 0 - vec3 position
 *   location 1 - vec3 normal    (declared but unused)
 *   location 2 - vec2 texcoord  (declared but unused)
 *   location 3 - vec4 color     (declared but unused)
 *
 * Uniforms are uModel, uView and uProjection, uploaded by RenderContext exactly
 * as for the main pass. The depth pass installs the light's view and projection
 * for its duration, so nothing in the traversal has to know which pass it is
 * drawing into.
 */
class IVF_API DepthShader {
public:
    /** Compile and link the depth shader. Returns the program, linked or not. */
    static ShaderProgramPtr create();
};

} // namespace ivf
