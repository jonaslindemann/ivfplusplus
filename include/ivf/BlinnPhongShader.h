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
 * Blinn-Phong shader — embedded GLSL source.
 *
 * Replicates the OpenGL fixed-function Blinn-Phong lighting model.
 * Supports up to 8 lights (point, directional, spot), per-fragment
 * shading, texturing, and all standard material properties.
 *
 * Vertex attribute layout:
 *   location 0 - vec3 position
 *   location 1 - vec3 normal
 *   location 2 - vec2 texcoord
 *   location 3 - vec4 color
 *
 * Uniforms uploaded by RenderContext::uploadToShader():
 *   mat4  uModel, uView, uProjection
 *   mat3  uNormalMatrix
 *   int   uLightCount
 *   Light uLights[8]  (see RenderContext.h for struct layout)
 *
 * Additional material/texture uniforms (set manually or by a future Material port):
 *   vec4  uMatAmbient, uMatDiffuse, uMatSpecular, uMatEmission
 *   float uMatShininess
 *   bool  uUseTexture
 *   sampler2D uTexture
 *   vec4  uGlobalAmbient
 *
 * Usage:
 *   auto prog = BlinnPhongShader::create();
 *   if (!prog->isLinked()) { ... handle error ... }
 */
class IVF_API BlinnPhongShader {
public:
    /**
     * Compile and return a linked BlinnPhong ShaderProgram from embedded source.
     * Returns a ShaderProgram (may be unlinked if compilation failed — check isLinked()).
     * Must be called after an OpenGL context is current.
     */
    static ShaderProgramPtr create();

    /** Raw embedded vertex shader source (GLSL 3.30 core). */
    static const char* vertexSource();

    /** Raw embedded fragment shader source (GLSL 3.30 core). */
    static const char* fragmentSource();

private:
    BlinnPhongShader() = delete;
};

} // namespace ivf
