#pragma once

#include <ivf/RenderContext.h>

namespace ivf {

// ---- Frame lifecycle ----
inline void rcBeginFrame()                               { RenderContext::instance().beginFrame(); }

/**
 * Hands the pipeline back in the state it was found in.
 *
 * Call it once the scene has been drawn, before anything else draws into the
 * same context. WidgetBase does this for the widget classes; an application
 * driving its own render loop calls it itself.
 */
inline void rcEndFrame()                                 { RenderContext::instance().endFrame(); }

// ---- Render profile ----
inline void rcSetProfile(RenderProfile p)                { RenderContext::instance().setProfile(p); }
inline RenderProfile rcProfile()                         { return RenderContext::instance().profile(); }
inline bool rcLegacyAllowed()                            { return RenderContext::instance().legacyAllowed(); }
inline bool rcNeedsLegacyDraw(bool objectHasModernPath)
    { return RenderContext::instance().needsLegacyDraw(objectHasModernPath); }
inline void rcBeginLegacyDraw()                          { RenderContext::instance().beginLegacyDraw(); }
inline void rcEndLegacyDraw()                            { RenderContext::instance().endLegacyDraw(); }

/**
 * Whether an object with this modern-path answer can draw at all.
 *
 * False only in Core for an object that has no modern path: there is nothing
 * behind the shader for its fixed-function code to reach.
 */
inline bool rcCanDrawGeometry(bool objectHasModernPath)
{
    return objectHasModernPath || (RenderContext::instance().profile() != RenderProfile::Core);
}

// ---- Shader activation ----
inline void rcUseBlinnPhong()                            { RenderContext::instance().useBlinnPhong(); }
inline void rcSetShader(ShaderProgram* prog)             { RenderContext::instance().setShader(prog); }
inline ShaderProgram* rcShader()                         { return RenderContext::instance().shader(); }

/**
 * True when geometry should be drawn through the shader.
 *
 * Every geometry class branches on this to choose its modern path. It now also
 * respects the render profile, so RenderProfile::Legacy turns the whole library
 * back into its fixed-function self without unlinking anything.
 */
inline bool rcIsShaderActive()                           { return RenderContext::instance().shaderPathActive(); }

inline void rcUseShader() {
    if (RenderContext::instance().shaderPathActive())
        RenderContext::instance().shader()->use();
}

/** Unbind any program, so following calls reach the fixed-function pipeline. */
inline void rcUnuseShader()                              { glUseProgram(0); }

/**
 * Draw following geometry unlit, the shader-path equivalent of the
 * glDisable(GL_LIGHTING) a class used to wrap around a legacy draw.
 */
inline void rcSetForceUnlit(bool flag)                   { RenderContext::instance().setForceUnlit(flag); }
inline bool rcForceUnlit()                               { return RenderContext::instance().forceUnlit(); }

// ---- Picking ----
inline ShaderProgram* rcUsePickShader()                  { return RenderContext::instance().usePickShader(); }
inline void rcSetPickMode(bool flag)                     { RenderContext::instance().setPickMode(flag); }
inline bool rcPickMode()                                 { return RenderContext::instance().pickMode(); }
inline void rcSetPickName(unsigned int name)             { RenderContext::instance().setPickName(name); }

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

// ---- Fixed-function state mirrored for the shader ----
inline void rcSetTextureMode(int mode)                   { RenderContext::instance().setTextureMode(mode); }
inline void rcSetTextureEnvColor(float r, float g, float b, float a)
    { RenderContext::instance().setTextureEnvColor(r, g, b, a); }
inline void rcSetTextureMatrix(const glm::mat3& m)       { RenderContext::instance().setTextureMatrix(m); }
inline void rcSetFogMode(int mode)                       { RenderContext::instance().setFogMode(mode); }
inline void rcSetFogColor(float r, float g, float b, float a)
    { RenderContext::instance().setFogColor(r, g, b, a); }
inline void rcSetFogDensity(float d)                     { RenderContext::instance().setFogDensity(d); }
inline void rcSetFogRange(float s, float e)              { RenderContext::instance().setFogRange(s, e); }
inline int  rcFogMode()                                  { return RenderContext::instance().fogMode(); }
inline void rcSetTwoSided(bool flag)                     { RenderContext::instance().setTwoSided(flag); }
inline void rcSetAlphaTest(GLenum func, float ref)       { RenderContext::instance().setAlphaTest(func, ref); }
inline void rcDisableAlphaTest()                         { RenderContext::instance().disableAlphaTest(); }
inline bool rcNeedsWideLineExpansion(float w)            { return RenderContext::instance().needsWideLineExpansion(w); }
inline void rcSetWideLineDraw(bool flag)                 { RenderContext::instance().setWideLineDraw(flag); }

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
