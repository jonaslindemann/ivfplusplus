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

#include <ivf/ShadowMap.h>

#include <iostream>

using namespace ivf;

ShadowMap::ShadowMap()
    : m_fbo(0), m_depthTexture(0), m_size(0), m_valid(false), m_lightSpaceMatrix(1.0f)
{
}

ShadowMap::~ShadowMap()
{
    drop();
}

bool ShadowMap::initialize(int size)
{
    if (size <= 0)
        return false;

    if (m_valid && (m_size == size))
        return true;

    drop();

    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size, size, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Depth comparison happens in the sampler rather than in the shader. A fetch
    // then returns a filtered visibility fraction instead of a depth value, which
    // is what the GL_LINEAR filter below is for -- filtering raw depths would
    // average distances and describe a surface that exists nowhere.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anything outside the light's frustum has to read as lit. Clamping to an
    // edge texel would smear whatever sat at the border across the rest of the
    // world; a border depth of 1.0 is farther than any geometry, so the
    // comparison always passes there.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    const GLfloat border[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLint previousFbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFbo);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    // A framebuffer with no colour attachment is incomplete unless it is told
    // that this is deliberate.

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)previousFbo);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ShadowMap: framebuffer incomplete (0x" << std::hex << status << std::dec << ")\n";
        drop();
        return false;
    }

    m_size = size;
    m_valid = true;

    return true;
}

void ShadowMap::drop()
{
    if (m_fbo != 0)
    {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }

    if (m_depthTexture != 0)
    {
        glDeleteTextures(1, &m_depthTexture);
        m_depthTexture = 0;
    }

    m_size = 0;
    m_valid = false;
}

bool ShadowMap::isValid() const
{
    return m_valid;
}

void ShadowMap::bind()
{
    if (!m_valid)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_size, m_size);
}

GLuint ShadowMap::depthTexture() const
{
    return m_depthTexture;
}

int ShadowMap::size() const
{
    return m_size;
}

const glm::mat4& ShadowMap::lightSpaceMatrix() const
{
    return m_lightSpaceMatrix;
}

void ShadowMap::setLightSpaceMatrix(const glm::mat4& matrix)
{
    m_lightSpaceMatrix = matrix;
}
