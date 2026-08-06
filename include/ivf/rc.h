#pragma once

#include <ivf/RenderContext.h>

namespace ivf {

// ---- Frame lifecycle ----
inline void rcBeginFrame()                               { RenderContext::instance().beginFrame(); }

// ---- Shader activation ----
inline void rcUseBlinnPhong()                            { RenderContext::instance().useBlinnPhong(); }
inline void rcSetShader(ShaderProgram* prog)             { RenderContext::instance().setShader(prog); }
inline ShaderProgram* rcShader()                         { return RenderContext::instance().shader(); }
inline bool rcIsShaderActive() {
    auto* s = RenderContext::instance().shader();
    return s && s->isLinked();
}
inline void rcUseShader() {
    if (auto* s = RenderContext::instance().shader()) s->use();
}

// ---- Shader update ----
inline void rcUpdateShader() {
    RenderContext::instance().updateShader(RenderContext::instance().shader());
}
inline void rcUpdateShader(ShaderProgram* prog)          { RenderContext::instance().updateShader(prog); }
inline bool rcDrawUnlit(GLenum primitive, const float* positions, const float* colors, int vertexCount)
    { return RenderContext::instance().drawUnlit(primitive, positions, colors, vertexCount); }

// ---- Global shader params ----
inline void rcSetGlobalAmbient(float r, float g, float b, float a = 1.0f)
    { RenderContext::instance().setGlobalAmbient(r, g, b, a); }
inline void rcSetUseTexture(bool flag)                   { RenderContext::instance().setUseTexture(flag); }
inline bool rcUseTexture()                               { return RenderContext::instance().useTexture(); }

// ---- Matrices ----
inline void rcSetProjection(const glm::mat4& m)          { RenderContext::instance().setProjection(m); }
inline void rcSetView(const glm::mat4& m)                { RenderContext::instance().setView(m); }
inline const glm::mat4& rcProjection()                   { return RenderContext::instance().projection(); }
inline const glm::mat4& rcView()                         { return RenderContext::instance().view(); }
inline const glm::mat4& rcModelMatrix()                  { return RenderContext::instance().modelMatrix(); }
inline void rcPushMatrix()                               { RenderContext::instance().pushMatrix(); }
inline void rcPopMatrix()                                { RenderContext::instance().popMatrix(); }
inline void rcTranslate(float x, float y, float z)       { RenderContext::instance().translate(x, y, z); }
inline void rcRotate(float a, float x, float y, float z) { RenderContext::instance().rotate(a, x, y, z); }
inline void rcScale(float x, float y, float z)           { RenderContext::instance().scale(x, y, z); }
inline void rcMultiply(const glm::mat4& m)               { RenderContext::instance().multiply(m); }
inline void rcLoadMatrix(const glm::mat4& m)             { RenderContext::instance().loadMatrix(m); }

// ---- Lights ----
inline bool rcAddLight(const LightData& l)               { return RenderContext::instance().addLight(l); }
inline int  rcLightCount()                               { return RenderContext::instance().lightCount(); }

} // namespace ivf
