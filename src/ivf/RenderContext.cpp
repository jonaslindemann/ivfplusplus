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
#include <cstddef>
#include <iostream>
#include <vector>

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
    , m_profile(RenderProfile::Mixed)
    , m_legacyDrawDepth(0)
{
    m_modelStack.push(glm::mat4(1.0f));
}

void RenderContext::setProfile(RenderProfile profile)
{
    m_profile = profile;
}

RenderProfile RenderContext::profile() const
{
    return m_profile;
}

bool RenderContext::legacyAllowed() const
{
    return m_profile != RenderProfile::Core;
}

bool RenderContext::shaderPathActive() const
{
    if (m_profile == RenderProfile::Legacy)
        return false;

    // While an object is drawing with the program unbound, there is no active
    // shader as far as anything it calls is concerned. Reporting otherwise sends
    // Material and Shape off to set uniforms on program 0.

    if (m_legacyDrawDepth > 0)
        return false;

    return (m_shader != nullptr) && m_shader->isLinked();
}

void RenderContext::beginLegacyDraw()
{
    m_legacyDrawDepth++;
}

void RenderContext::endLegacyDraw()
{
    if (m_legacyDrawDepth > 0)
        m_legacyDrawDepth--;
}

bool RenderContext::needsLegacyDraw(bool objectHasModernPath) const
{
    // Only Mixed has anything to fall back to. Legacy never binds a program in
    // the first place, and in Core there is no fixed-function pipeline waiting
    // behind the shader -- an unconverted object simply cannot draw there.

    if (m_profile != RenderProfile::Mixed)
        return false;

    return shaderPathActive() && !objectHasModernPath;
}

void RenderContext::beginFrame()
{
    // Reset model stack to identity
    while (!m_modelStack.empty())
        m_modelStack.pop();
    m_modelStack.push(glm::mat4(1.0f));

    clearLights();

    // Activate the shader so it is ready for per-shape uploads -- but only if the
    // profile actually wants it. Binding it unconditionally meant that selecting
    // RenderProfile::Legacy did nothing once a shader had been created: the
    // program was rebound at the top of every frame, and the fixed-function
    // drawing underneath then had a program bound over it. Unbinding in the
    // other direction is what makes the switch take effect on its own, without
    // the caller having to remember to unbind.

    if (shaderPathActive())
        m_shader->use();
    else
        glUseProgram(0);
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
		std::cerr << "\t light count: " << m_lightCount << "\n";
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

bool RenderContext::drawUnlit(GLenum primitive, const float* positions, const float* colors, int vertexCount) const
{
    // Respect the profile, not merely the presence of a linked shader. Returning
    // true here in Legacy would draw helper geometry through the shader while
    // everything around it drew fixed-function.

    if (!shaderPathActive() || positions == nullptr || vertexCount <= 0)
        return false;

    struct GpuVertex {
        float position[3];
        float normal[3];
        float texcoord[2];
        float color[4];
    };

    std::vector<GpuVertex> vertices;
    vertices.reserve(vertexCount);

    for (int i = 0; i < vertexCount; ++i) {
        GpuVertex v = {};
        v.position[0] = positions[i * 3 + 0];
        v.position[1] = positions[i * 3 + 1];
        v.position[2] = positions[i * 3 + 2];
        v.color[0] = colors ? colors[i * 4 + 0] : 1.0f;
        v.color[1] = colors ? colors[i * 4 + 1] : 1.0f;
        v.color[2] = colors ? colors[i * 4 + 2] : 1.0f;
        v.color[3] = colors ? colors[i * 4 + 3] : 1.0f;
        vertices.push_back(v);
    }

    GLuint vao = 0;
    GLuint vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(GpuVertex)), vertices.data(), GL_STREAM_DRAW);

    GLsizei stride = sizeof(GpuVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, texcoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, color));
    glEnableVertexAttribArray(3);

    m_shader->use();
    updateShader(m_shader);
    m_shader->setUniformInt("uUnlit", 1);
    m_shader->setUniformInt("uUseVertexColor", colors ? 1 : 0);

    glDrawArrays(primitive, 0, vertexCount);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return true;
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
    applyDefaultMaterial();
}

void RenderContext::setShader(ShaderProgram* prog)
{
    m_shader = prog;
    applyDefaultMaterial();
}

void RenderContext::applyDefaultMaterial()
{
    // Seed the material uniforms with OpenGL's own fixed-function defaults.
    //
    // Shape::doCreateMaterial() uploads nothing when a shape has no material,
    // which mirrors the legacy path: there, an unmaterialled shape simply draws
    // with whatever glMaterial state the previous one left behind. The shader
    // path inherits that same stickiness, but it starts from zero-initialised
    // uniforms rather than from GL's defaults -- so the first object drawn
    // without a material came out black, and every later one took the colour of
    // whichever object happened to precede it. Starting where GL starts makes
    // the two paths agree.

    if (!m_shader || !m_shader->isLinked())
        return;

    m_shader->use();
    m_shader->setUniformVec4("uMatAmbient",  glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    m_shader->setUniformVec4("uMatDiffuse",  glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    m_shader->setUniformVec4("uMatSpecular", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_shader->setUniformVec4("uMatEmission", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_shader->setUniformFloat("uMatShininess", 0.0f);
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

bool RenderContext::useTexture() const
{
    return m_useTexture;
}

} // namespace ivf
