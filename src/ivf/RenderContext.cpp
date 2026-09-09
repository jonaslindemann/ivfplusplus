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
#include <ivf/Texture.h>
#include <ivf/PickShader.h>
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
    , m_coreContext(-1)
    , m_legacyDrawDepth(0)
    , m_unlitVao(0)
    , m_unlitVbo(0)
    , m_unlitCapacity(0)
    , m_pickMode(false)
    , m_pickColor(0.0f, 0.0f, 0.0f, 1.0f)
    , m_textureMode(0)
    , m_textureEnvColor(1.0f, 1.0f, 1.0f, 1.0f)
    , m_textureMatrix(1.0f)
    , m_fogMode(0)
    , m_fogColor(0.0f, 0.0f, 0.0f, 1.0f)
    , m_fogDensity(1.0f)
    , m_fogStart(0.0f)
    , m_fogEnd(1.0f)
    , m_twoSided(false)
    , m_alphaTestFunc(0)
    , m_alphaTestRef(0.0f)
    , m_wideLineDraw(false)
    , m_forceUnlit(false)
    , m_whiteTexture(0)
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

bool RenderContext::isCoreContext() const
{
    return m_coreContext == 1;
}

bool RenderContext::legacyAllowed() const
{
    // Two separate questions, and this used to ask only the first: does the
    // library intend to use fixed function, and will the context accept it.
    //
    // Mixed on a core context answered "yes" to the first and got a torrent of
    // GL_INVALID_OPERATION for the second -- and quietly broke lighting, because
    // Material asks glIsEnabled(GL_LIGHTING), which is itself illegal there and
    // returns false, so no material was ever uploaded and specular vanished.
    // That combination is exactly what an application hits when it flips its
    // context hints before its render profile.

    return (m_profile != RenderProfile::Core) && (m_coreContext != 1);
}

bool RenderContext::shaderPathActive() const
{
    // Legacy asks for the fixed-function pipeline. On a core context there is
    // none, so honouring it literally would draw nothing at all and report an
    // error for every call that tried. The shader is the only pipeline present,
    // so use it: a mismatched profile degrades to something that works rather
    // than to a black window.

    if ((m_profile == RenderProfile::Legacy) && (m_coreContext != 1))
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

    // Ask the context what it is, once. A context is guaranteed to exist here,
    // which is not true everywhere legacyAllowed() is called from.

    if (m_coreContext < 0)
    {
        GLint mask = 0;
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);

        // A context too old to know the query will have raised an error for it;
        // clear that rather than leaving it for the next thing to trip over.

        while (glGetError() != GL_NO_ERROR)
        {
        }

        m_coreContext = ((mask & GL_CONTEXT_CORE_PROFILE_BIT) != 0) ? 1 : 0;
    }

    clearLights();

    // The view belongs to the frame, not to the context. Leaving the previous
    // frame's view in place meant a light rendered before the camera -- which is
    // exactly what LM_LOCAL does -- was captured against a stale view instead of
    // against the identity the fixed-function pipeline would have given it.

    m_view = glm::mat4(1.0f);

    // The shader's uTexture sampler always points at unit 0, so the driver
    // treats that unit as used on every draw -- even the ones where uUseTexture
    // is 0 and the sample never happens. If nothing is bound there it warns
    // about an incomplete texture, once per draw, which buries anything else
    // the debug output has to say.
    //
    // Binding the placeholder at the top of the frame keeps the unit complete.
    // Texture's own bind cache has to be told, or it will skip the next real
    // bind believing it is already current.

    if (shaderPathActive())
    {
        ensureWhiteTexture();

        if (m_whiteTexture != 0)
        {
            glBindTexture(GL_TEXTURE_2D, m_whiteTexture);
            Texture::invalidateBindCache();
        }
    }

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

    prog->setUniformInt("uTextureMode",       m_textureMode);
    prog->setUniformVec4("uTextureEnvColor",  m_textureEnvColor);
    prog->setUniformMat3("uTextureMatrix",    m_textureMatrix);

    prog->setUniformInt("uFogMode",     m_fogMode);
    prog->setUniformVec4("uFogColor",   m_fogColor);
    prog->setUniformFloat("uFogDensity", m_fogDensity);
    prog->setUniformFloat("uFogStart",   m_fogStart);
    prog->setUniformFloat("uFogEnd",     m_fogEnd);

    prog->setUniformInt("uWideLine", m_wideLineDraw ? 1 : 0);

    if (m_wideLineDraw)
    {
        GLint viewport[4] = { 0, 0, 1, 1 };
        glGetIntegerv(GL_VIEWPORT, viewport);
        prog->setUniformVec2("uViewportSize",
                             glm::vec2((float)viewport[2], (float)viewport[3]));
    }

    prog->setUniformInt("uTwoSided",       m_twoSided ? 1 : 0);
    prog->setUniformInt("uAlphaTestFunc",  m_alphaTestFunc);
    prog->setUniformFloat("uAlphaTestRef", m_alphaTestRef);
    prog->setUniformInt("uUnlit",          0); // reset; GLPrimitive sets 1 for points/lines
    prog->setUniformInt("uUseVertexColor", 0); // reset; GLPrimitive sets 1 for colored geometry

    // Re-assert the pick colour every draw. setPickName() uploads it when the
    // name changes, but a draw path that rebinds or a shader swap in between
    // would otherwise leave the previous shape's id in place.

    if (m_pickMode)
        prog->setUniformVec4("uPickColor", m_pickColor);

    prog->setUniformInt("uLightCount", m_lightCount);

    for (int i = 0; i < m_lightCount; ++i) {
        const LightData& l = m_lights[i];
        std::string base = "uLights[" + std::to_string(i) + "].";

        // Already in eye space: Light::doCreateGeometry() applies the modelview
        // in force when the light was rendered, which is what glLightfv did and
        // what makes SceneBase's LM_LOCAL and LM_WORLD differ.
        const glm::vec4& posView     = l.position;
        const glm::vec3& spotDirView = l.spotDirection;

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

    // Reuse one VAO and buffer across calls. Helper geometry changes every frame
    // so there is nothing worth caching in it, but generating and deleting a VAO
    // and a VBO on every call of every frame is overhead for nothing. The vertex
    // layout never varies, so the VAO only needs setting up once.

    const bool firstUse = (m_unlitVao == 0);

    if (firstUse)
    {
        glGenVertexArrays(1, &m_unlitVao);
        glGenBuffers(1, &m_unlitVbo);
    }

    glBindVertexArray(m_unlitVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_unlitVbo);

    const GLsizeiptr needed = (GLsizeiptr)(vertices.size() * sizeof(GpuVertex));

    if (needed > m_unlitCapacity)
    {
        // Grow, and only grow. Reallocating to the exact size each time would
        // reintroduce the churn this is meant to avoid.

        glBufferData(GL_ARRAY_BUFFER, needed, vertices.data(), GL_STREAM_DRAW);
        m_unlitCapacity = needed;
    }
    else
    {
        // Orphan the previous contents so the driver hands back fresh storage
        // rather than stalling until the last draw that read this buffer retires.

        glBufferData(GL_ARRAY_BUFFER, m_unlitCapacity, nullptr, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices.data());
    }

    if (firstUse)
    {
        GLsizei stride = sizeof(GpuVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, texcoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(GpuVertex, color));
        glEnableVertexAttribArray(3);
    }

    m_shader->use();
    updateShader(m_shader);
    m_shader->setUniformInt("uUnlit", 1);
    m_shader->setUniformInt("uUseVertexColor", colors ? 1 : 0);

    glDrawArrays(primitive, 0, vertexCount);

    glBindVertexArray(0);

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
    m_shader->setUniformInt("uTexture", 0);

    ensureWhiteTexture();
    applyDefaultMaterial();
}

void RenderContext::setShader(ShaderProgram* prog)
{
    m_shader = prog;
    applyDefaultMaterial();
}

ShaderProgram* RenderContext::usePickShader()
{
    if (!m_pickShader || !m_pickShader->isLinked())
    {
        m_pickShader = PickShader::create();

        if (!m_pickShader->isLinked())
        {
            std::cerr << "RenderContext: PickShader failed to compile/link\n";
            return nullptr;
        }
    }

    ShaderProgram* previous = m_shader;

    m_shader = (ShaderProgram*)m_pickShader;
    m_shader->use();

    return previous;
}

void RenderContext::setPickMode(bool flag)
{
    m_pickMode = flag;
}

bool RenderContext::pickMode() const
{
    return m_pickMode;
}

void RenderContext::setPickName(unsigned int name)
{
    // Zero is background, so shift by one. 24 bits is 16.7 million shapes, well
    // past anything the scene graph will hold.

    const unsigned int encoded = name + 1;

    m_pickColor = glm::vec4(static_cast<float>((encoded)       & 0xffu) / 255.0f,
                            static_cast<float>((encoded >>  8) & 0xffu) / 255.0f,
                            static_cast<float>((encoded >> 16) & 0xffu) / 255.0f,
                            1.0f);

    if (m_shader && m_shader->isLinked())
        m_shader->setUniformVec4("uPickColor", m_pickColor);
}

unsigned int RenderContext::decodePickName(unsigned char r, unsigned char g, unsigned char b, bool& valid)
{
    const unsigned int encoded = (unsigned int)r | ((unsigned int)g << 8) | ((unsigned int)b << 16);

    valid = (encoded != 0);
    return valid ? (encoded - 1) : 0;
}

// ---- Fixed-function state mirrored for the shader ----

void RenderContext::setTextureMode(int mode)
{
    m_textureMode = mode;
}

void RenderContext::setTextureEnvColor(float r, float g, float b, float a)
{
    m_textureEnvColor = glm::vec4(r, g, b, a);
}

void RenderContext::setTextureMatrix(const glm::mat3& m)
{
    m_textureMatrix = m;
}

void RenderContext::setFogMode(int mode)
{
    m_fogMode = mode;
}

int RenderContext::fogMode() const
{
    return m_fogMode;
}

void RenderContext::setFogColor(float r, float g, float b, float a)
{
    m_fogColor = glm::vec4(r, g, b, a);
}

void RenderContext::setFogDensity(float density)
{
    m_fogDensity = density;
}

void RenderContext::setFogRange(float start, float end)
{
    m_fogStart = start;
    m_fogEnd = end;
}

void RenderContext::setTwoSided(bool flag)
{
    m_twoSided = flag;
}

void RenderContext::setAlphaTest(GLenum func, float ref)
{
    m_alphaTestFunc = (int)func;
    m_alphaTestRef = ref;
}

void RenderContext::disableAlphaTest()
{
    m_alphaTestFunc = 0;
}

bool RenderContext::needsWideLineExpansion(float width) const
{
    return (m_profile == RenderProfile::Core) && (width > 1.0f);
}

void RenderContext::endFrame()
{
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderContext::setForceUnlit(bool flag)
{
    m_forceUnlit = flag;
}

bool RenderContext::forceUnlit() const
{
    return m_forceUnlit;
}

void RenderContext::setWideLineDraw(bool flag)
{
    m_wideLineDraw = flag;
}

void RenderContext::ensureWhiteTexture()
{
    if (m_whiteTexture != 0)
        return;

    const unsigned char white[4] = { 255, 255, 255, 255 };

    GLint previous = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous);

    glGenTextures(1, &m_whiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_whiteTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Leave whatever the caller had bound in place, but if that was nothing,
    // leave the white one -- an incomplete unit 0 is what this exists to avoid.

    glBindTexture(GL_TEXTURE_2D, (previous != 0) ? (GLuint)previous : m_whiteTexture);
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
