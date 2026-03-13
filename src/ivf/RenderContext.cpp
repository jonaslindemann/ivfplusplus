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

#include <ivf/RenderContext.h>
#include <ivf/BlinnPhongShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <cassert>
#include <iostream>

namespace ivf {

RenderContext& RenderContext::instance()
{
    static RenderContext s_instance;
    return s_instance;
}

RenderContext::RenderContext()
    : m_projection(1.0f)
    , m_view(1.0f)
    , m_lightCount(0)
    , m_shader(nullptr)
    , m_globalAmbient(0.0f, 0.0f, 0.0f, 1.0f)
    , m_useTexture(false)
{
    m_modelStack.push(glm::mat4(1.0f));
}

void RenderContext::beginFrame()
{
    // Reset model stack to identity
    while (!m_modelStack.empty())
        m_modelStack.pop();
    m_modelStack.push(glm::mat4(1.0f));

    clearLights();

    // Activate the shader so it is ready for per-shape uploads.
    if (m_shader && m_shader->isLinked())
        m_shader->use();
}

// ---- Projection / view ----

void RenderContext::setProjection(const glm::mat4& proj)
{
    m_projection = proj;
}

void RenderContext::setView(const glm::mat4& view)
{
    m_view = view;
}

const glm::mat4& RenderContext::projection() const
{
    return m_projection;
}

const glm::mat4& RenderContext::view() const
{
    return m_view;
}

// ---- Model matrix stack ----

void RenderContext::pushMatrix()
{
    m_modelStack.push(m_modelStack.top());
}

void RenderContext::popMatrix()
{
    if (m_modelStack.size() > 1)
        m_modelStack.pop();
    else
        std::cerr << "RenderContext: popMatrix() called on empty stack\n";
}

void RenderContext::loadMatrix(const glm::mat4& m)
{
    m_modelStack.top() = m;
}

void RenderContext::multiply(const glm::mat4& m)
{
    m_modelStack.top() = m_modelStack.top() * m;
}

void RenderContext::translate(float x, float y, float z)
{
    m_modelStack.top() = glm::translate(m_modelStack.top(), glm::vec3(x, y, z));
}

void RenderContext::rotate(float angleDeg, float ax, float ay, float az)
{
    m_modelStack.top() = glm::rotate(m_modelStack.top(),
                                     glm::radians(angleDeg),
                                     glm::vec3(ax, ay, az));
}

void RenderContext::scale(float sx, float sy, float sz)
{
    m_modelStack.top() = glm::scale(m_modelStack.top(), glm::vec3(sx, sy, sz));
}

const glm::mat4& RenderContext::modelMatrix() const
{
    return m_modelStack.top();
}

// ---- Lights ----

void RenderContext::clearLights()
{
    m_lightCount = 0;
    for (auto& l : m_lights)
        l = LightData{};
}

bool RenderContext::addLight(const LightData& light)
{
    if (m_lightCount >= IVF_MAX_LIGHTS) {
        std::cerr << "RenderContext: maximum light count (" << IVF_MAX_LIGHTS << ") exceeded\n";
        return false;
    }
    m_lights[m_lightCount++] = light;
    return true;
}

int RenderContext::lightCount() const
{
    return m_lightCount;
}

const LightData& RenderContext::light(int index) const
{
    assert(index >= 0 && index < m_lightCount);
    return m_lights[index];
}

// ---- Shader upload ----

void RenderContext::updateShader(ShaderProgram* prog) const
{
    if (!prog || !prog->isLinked())
        return;

    const glm::mat4& model = m_modelStack.top();

    prog->setUniformMat4("uModel",      model);
    prog->setUniformMat4("uView",       m_view);
    prog->setUniformMat4("uProjection", m_projection);

    // Normal matrix: transpose of inverse of upper-left 3x3 of (view * model)
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(m_view * model));
    prog->setUniformMat3("uNormalMatrix", normalMatrix);

    prog->setUniformVec4("uGlobalAmbient", m_globalAmbient);
    prog->setUniformInt("uUseTexture",     m_useTexture ? 1 : 0);
    prog->setUniformInt("uUnlit",          0); // reset; GLPrimitive sets 1 for points/lines
    prog->setUniformInt("uUseVertexColor", 0); // reset; GLPrimitive sets 1 for colored geometry

    prog->setUniformInt("uLightCount", m_lightCount);

    for (int i = 0; i < m_lightCount; ++i) {
        const LightData& l = m_lights[i];
        std::string base = "uLights[" + std::to_string(i) + "].";

        // Transform light position and spot direction from world space to view space.
        glm::vec4 posView      = m_view * l.position;
        glm::vec3 spotDirView  = glm::mat3(m_view) * l.spotDirection;

        prog->setUniformVec4(base + "position",      posView);
        prog->setUniformVec4(base + "ambient",       l.ambient);
        prog->setUniformVec4(base + "diffuse",       l.diffuse);
        prog->setUniformVec4(base + "specular",      l.specular);
        prog->setUniformVec3(base + "spotDirection", spotDirView);
        prog->setUniformFloat(base + "spotCutoff",   l.spotCutoff);
        prog->setUniformFloat(base + "spotExponent", l.spotExponent);
        prog->setUniformFloat(base + "constAtt",     l.constAtt);
        prog->setUniformFloat(base + "linearAtt",    l.linearAtt);
        prog->setUniformFloat(base + "quadAtt",      l.quadAtt);
        prog->setUniformInt(base + "type",           l.type);
    }
}

// ---- Active shader ----

void RenderContext::useBlinnPhong()
{
    if (!m_ownedShader || !m_ownedShader->isLinked()) {
        m_ownedShader = BlinnPhongShader::create();
        if (!m_ownedShader->isLinked()) {
            std::cerr << "RenderContext: BlinnPhongShader failed to compile/link\n";
            return;
        }
    }
    m_shader = (ShaderProgram*)m_ownedShader;
    m_shader->use();
}

void RenderContext::setShader(ShaderProgram* prog)
{
    m_shader = prog;
}

ShaderProgram* RenderContext::shader() const
{
    return m_shader;
}

// ---- Global shader parameters ----

void RenderContext::setGlobalAmbient(float r, float g, float b, float a)
{
    m_globalAmbient = glm::vec4(r, g, b, a);
}

void RenderContext::setUseTexture(bool flag)
{
    m_useTexture = flag;
}

} // namespace ivf
