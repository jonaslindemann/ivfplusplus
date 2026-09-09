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

#include <ivf/Base.h>
#include <ivf/GL.h>

#include <glm/glm.hpp>

namespace ivf {

IvfSmartPointer(ShadowMap);

/**
 * Depth-only render target for shadow mapping.
 *
 * A framebuffer with a depth texture and no colour attachment. The scene is
 * drawn into it from the light's point of view, and the result is sampled while
 * shading to decide whether a surface can see the light.
 *
 * The texture is created with depth comparison enabled and linear filtering, so
 * it is read in the shader as a sampler2DShadow. Each fetch then returns the
 * bilinearly filtered result of four depth comparisons rather than a single
 * depth value, which is both cheaper and smoother than doing the comparisons in
 * the shader: a 2x2 tap pattern gives the same softness as a 3x3 pattern of
 * plain samples, for less than half the fetches.
 */
class IVF_API ShadowMap : public Base {
public:
    ShadowMap();
    virtual ~ShadowMap();

    IvfClassInfo("ShadowMap", Base);
    IvfStdFactory(ShadowMap);

    /**
     * Create the framebuffer and depth texture at the given size.
     *
     * Requires a current GL context, so this cannot happen in the constructor
     * of an object an application creates up front. Calling it again with the
     * same size does nothing; with a different size it rebuilds. Returns false
     * if the framebuffer will not complete, in which case the caller should
     * leave shadows off rather than draw into an invalid target.
     */
    bool initialize(int size);

    /** Release the framebuffer and texture. Safe to call without a context. */
    void drop();

    /** True once initialize() has produced a complete framebuffer. */
    bool isValid() const;

    /**
     * Bind the framebuffer and set the viewport to the map size.
     *
     * The caller is responsible for restoring the previous framebuffer and
     * viewport; this class does not save them, because the depth pass has other
     * state to put back as well and one owner for all of it is clearer.
     */
    void bind();

    /** The depth texture, or 0 if not initialized. */
    GLuint depthTexture() const;

    /** Edge length of the (square) map in texels. */
    int size() const;

    /** The projection * view matrix the map was last rendered with. */
    const glm::mat4& lightSpaceMatrix() const;
    void setLightSpaceMatrix(const glm::mat4& matrix);

private:
    GLuint m_fbo;
    GLuint m_depthTexture;
    int m_size;
    bool m_valid;
    glm::mat4 m_lightSpaceMatrix;
};

} // namespace ivf
