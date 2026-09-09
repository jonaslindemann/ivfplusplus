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
 * Flat-colour shader used for picking.
 *
 * Replaces the GL_SELECT render mode, which the core profile removed. Instead of
 * asking the driver which named primitives fell inside a small region, the scene
 * is drawn once into an offscreen buffer with every shape painted a colour that
 * encodes its object name, and the result is read back. Depth testing decides
 * what is in front, exactly as it does for the visible image -- which is more
 * faithful than GL_SELECT ever was, since that reported hits for geometry which
 * was drawn but hidden.
 *
 * The fragment shader writes uPickColor unchanged: no lighting, no texturing, no
 * vertex colour. That matters more than it looks. Anything that perturbs the
 * value -- blending, dithering, multisample resolve, sRGB conversion -- corrupts
 * the id, so the caller must also make sure those are off and the target is
 * single-sampled.
 *
 * The vertex attribute layout matches BlinnPhongShader so the same VAOs draw
 * through either program without rebuilding:
 *   location 0 - vec3 position
 *   location 1 - vec3 normal    (declared but unused)
 *   location 2 - vec2 texcoord  (declared but unused)
 *   location 3 - vec4 color     (declared but unused)
 *
 * Uniforms: mat4 uModel, uView, uProjection (uploaded by RenderContext), and
 * vec4 uPickColor (set per shape by Shape::doBeginTransform while picking).
 */
class IVF_API PickShader {
public:
    /** Compile and link the pick shader. Returns the program, linked or not. */
    static ShaderProgramPtr create();
};

} // namespace ivf
