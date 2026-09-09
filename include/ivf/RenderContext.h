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
#include <ivf/GL.h>

#include <glm/glm.hpp>

#include <array>
#include <stack>
#include <string>
#include <memory>

namespace ivf {

/** Maximum number of lights supported by the Blinn-Phong shader. */
static constexpr int IVF_MAX_LIGHTS = 8;

/**
 * Which rendering pipeline the library is expected to use.
 *
 * This is the explicit form of a decision that used to be a side effect of
 * whether a shader happened to be linked. Applications that depend on the
 * fixed-function pipeline -- ObjectiveFrame, principally -- can now ask for it
 * rather than getting it by accident.
 */
enum class RenderProfile {
    /**
     * Fixed function only. No shader is used even if one is set, and every
     * object draws exactly as it did before the modern path existed.
     * Requires a compatibility context.
     */
    Legacy,

    /**
     * Shader drawing on a compatibility context, which is the default.
     *
     * Objects that have a modern path draw through the shader. Objects that do
     * not have the shader unbound around their geometry, so their
     * fixed-function code runs against the fixed-function pipeline as it always
     * did. Without that unbinding their glBegin/glVertex calls would be fed
     * through a shader nobody had given matrices to -- which is what used to
     * happen, and why some classes drew nothing at all.
     */
    Mixed,

    /**
     * Shader only. Every legacy GL call is suppressed, so this is the profile a
     * core context can run. Classes that have not been converted yet cannot
     * draw here; profile_test reports which.
     */
    Core
};

/**
 * Per-light data passed to the shader.
 *
 * Mirrors the Light class properties. Populated by Light::render() during
 * scene traversal and uploaded to the active ShaderProgram each frame.
 */
struct LightData {
    glm::vec4 position  = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f); // w=0 directional, w=1 positional
    glm::vec4 ambient   = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 diffuse   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 specular  = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 spotDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    float     spotCutoff    = 180.0f; // 180 = point/directional (no cone)
    float     spotExponent  = 0.0f;
    float     constAtt      = 1.0f;
    float     linearAtt     = 0.0f;
    float     quadAtt       = 0.0f;
    int       type          = 0;      // 0=point, 1=directional, 2=spot
    bool      enabled       = false;
};

/**
 * Global rendering context for modern OpenGL scene traversal.
 *
 * RenderContext is a singleton that replaces the implicit OpenGL matrix
 * stack and fixed-function light state. It is accessed via instance() and
 * updated by Camera and Light objects during render().
 *
 * Typical frame flow:
 *   ctx.beginFrame();                       // clear lights, reset model stack
 *   ctx.setProjection(proj);                // from Camera
 *   ctx.setView(view);                      // from Camera
 *   // ... scene graph traversal ...
 *   ctx.pushMatrix(); ctx.multiply(T); ...  // from Shape::doBeginTransform
 *   ctx.uploadToShader(prog);               // before each draw call
 *   ctx.popMatrix();                        // from Shape::doEndTransform
 */
class IVF_API RenderContext {
public:
    /** Return the singleton instance. */
    static RenderContext& instance();

    // Prevent copying
    RenderContext(const RenderContext&) = delete;
    RenderContext& operator=(const RenderContext&) = delete;

    // ---- Render profile ----

    /**
     * Select the pipeline the library should use. Default is Mixed.
     *
     * Setting Legacy does not discard the shader; it only stops it being used,
     * so a caller can switch back and forth to compare the two.
     */
    void setProfile(RenderProfile profile);

    /** Returns the active profile. */
    RenderProfile profile() const;

    /**
     * True when the live OpenGL context is a core profile context.
     *
     * Determined once, at the first beginFrame(). Distinct from the render
     * profile: the profile says what the library intends to do, this says what
     * the driver will actually accept.
     */
    bool isCoreContext() const;

    /**
     * True when fixed-function calls are permitted, i.e. anywhere but Core.
     *
     * Call sites that must not emit deprecated GL in a core context test this.
     */
    bool legacyAllowed() const;

    /**
     * True when drawing should go through the shader.
     *
     * False in Legacy, and false anywhere if no linked shader is set. This is
     * what rcIsShaderActive() reports, and what every geometry class branches on.
     */
    bool shaderPathActive() const;

    /**
     * True when this object's geometry must be drawn with no program bound.
     *
     * Answers "is this a Mixed-profile object that has no modern path", which is
     * the case where the shader has to be unbound so the object's
     * fixed-function code reaches the fixed-function pipeline.
     */
    bool needsLegacyDraw(bool objectHasModernPath) const;

    /**
     * Mark the start and end of a draw made with no program bound.
     *
     * Between these calls shaderPathActive() reports false, which is the truth
     * for the object being drawn: there is no program bound, so anything that
     * tried to talk to one -- Material uploading its uniforms, Shape mirroring
     * its transform -- would raise GL_INVALID_OPERATION for no purpose. Nesting
     * is counted, so a legacy object inside another one behaves.
     */
    void beginLegacyDraw();
    void endLegacyDraw();

    // ---- Frame lifecycle ----

    /**
     * Call at the start of each frame.
     * Resets the model matrix stack, clears accumulated lights, and
     * activates the current shader (if one is set).
     */
    void beginFrame();

    // ---- Projection and view (set by Camera) ----

    void setProjection(const glm::mat4& proj);
    void setView(const glm::mat4& view);

    const glm::mat4& projection() const;
    const glm::mat4& view() const;

    // ---- Model matrix stack (used by Shape::doBeginTransform/doEndTransform) ----

    /** Push a copy of the current model matrix onto the stack. */
    void pushMatrix();

    /** Pop the top of the model matrix stack. */
    void popMatrix();

    /** Replace current model matrix. */
    void loadMatrix(const glm::mat4& m);

    /** Post-multiply the current model matrix. */
    void multiply(const glm::mat4& m);

    /** Convenience: apply a translation to the current model matrix. */
    void translate(float x, float y, float z);

    /** Convenience: apply a rotation (degrees, axis) to the current model matrix. */
    void rotate(float angleDeg, float ax, float ay, float az);

    /** Convenience: apply a scale to the current model matrix. */
    void scale(float sx, float sy, float sz);

    /** Returns the current (top-of-stack) model matrix. */
    const glm::mat4& modelMatrix() const;

    // ---- Light accumulation (called by Light::doCreateGeometry or similar) ----

    /** Remove all accumulated lights. Called by beginFrame(). */
    void clearLights();

    /**
     * Add a light for this frame.
     * Returns false if IVF_MAX_LIGHTS is already reached.
     */
    bool addLight(const LightData& light);

    /** Return number of lights added this frame. */
    int lightCount() const;

    /** Read-only access to a specific light. */
    const LightData& light(int index) const;

    // ---- Shader upload ----

    /**
     * Upload the current MVP matrices, normal matrix, and light list to
     * the given shader program.
     *
     * Expected uniform names in the shader:
     *   mat4  uModel
     *   mat4  uView
     *   mat4  uProjection
     *   mat3  uNormalMatrix
     *   int   uLightCount
     *   struct Light { vec4 position; vec4 ambient; vec4 diffuse; vec4 specular;
     *                  vec3 spotDirection; float spotCutoff; float spotExponent;
     *                  float constAtt; float linearAtt; float quadAtt; int type; } uLights[8]
     */
    void updateShader(ShaderProgram* prog) const;

    /**
     * Draw simple unlit helper geometry with the active shader.
     *
     * Positions are tightly packed xyz floats. Colors are optional tightly
     * packed rgba floats; when omitted, the active material diffuse color is used.
     * Returns false when no linked shader is active, so callers can use their
     * legacy fixed-function fallback.
     */
    bool drawUnlit(GLenum primitive, const float* positions, const float* colors, int vertexCount) const;

    // ---- Active shader ----

    /**
     * Activate the built-in Blinn-Phong shader.
     *
     * Creates the shader on first call (or if previously unlinked), sets it
     * as the active shader, and calls use() on it. Call once from onInit().
     */
    void useBlinnPhong();

    /** Set the shader program to use. Does not call use() on it. */
    void setShader(ShaderProgram* prog);

    // ---- Picking ----

    /**
     * Activate the built-in flat-colour pick shader and return the shader that
     * was active, so the caller can put it back.
     *
     * Creates the program on first use. Returns nullptr if it will not link, in
     * which case the caller should fall back to GL_SELECT.
     */
    ShaderProgram* usePickShader();

    /**
     * Turn colour-id picking on for the duration of a pick pass.
     *
     * While this is set, Shape::doBeginTransform() reports each shape's object
     * name through setPickName() instead of glLoadName(), which is the same
     * mechanism the GL_SELECT path used and so gives the same answer about which
     * shape a pixel belongs to.
     */
    void setPickMode(bool flag);
    bool pickMode() const;

    /**
     * Set the object name the next draws are painted with.
     *
     * The name is encoded into 24 bits of colour, with zero reserved for
     * background -- so the value written is name + 1. Alpha is forced to 1: a
     * zero alpha would let a blending stage that should not be running destroy
     * the id silently.
     */
    void setPickName(unsigned int name);

    /** Decode an object name from a pixel read back from the pick target. */
    static unsigned int decodePickName(unsigned char r, unsigned char g, unsigned char b, bool& valid);

    /** Returns the currently assigned shader, or nullptr. */
    ShaderProgram* shader() const;

    // ---- Global shader parameters ----

    /**
     * Set the global ambient light colour uploaded as uGlobalAmbient each frame.
     * Default is (0, 0, 0, 1) — no ambient fill.
     */
    void setGlobalAmbient(float r, float g, float b, float a = 1.0f);

    /**
     * Enable or disable texture modulation in the Blinn-Phong shader (uUseTexture).
     * Default is false.
     */
    void setUseTexture(bool flag);
    bool useTexture() const;

    // ---- Fixed-function state the shader has to reproduce ----
    //
    // Each of these mirrors a piece of GL state the core profile removed. The
    // class that owns the legacy call sets the matching value here, so the two
    // pipelines stay described by the same numbers.

    /**
     * Texture environment, mirroring glTexEnv.
     * mode: 0 modulate (GL's default), 1 decal, 2 replace, 3 blend.
     */
    void setTextureMode(int mode);
    void setTextureEnvColor(float r, float g, float b, float a);

    /** The GL_TEXTURE matrix, as the 2D affine transform it always was. */
    void setTextureMatrix(const glm::mat3& m);

    /**
     * Fog, mirroring glFog.
     * mode: 0 off, 1 GL_LINEAR, 2 GL_EXP, 3 GL_EXP2.
     */
    void setFogMode(int mode);
    void setFogColor(float r, float g, float b, float a);
    void setFogDensity(float density);
    void setFogRange(float start, float end);
    int  fogMode() const;

    /** GL_LIGHT_MODEL_TWO_SIDE. */
    void setTwoSided(bool flag);

    /**
     * Alpha test, mirroring glAlphaFunc plus GL_ALPHA_TEST.
     * func is the GL comparison enum, or 0 to turn the test off. The shader
     * discards fragments that fail, which is the core-profile equivalent.
     */
    void setAlphaTest(GLenum func, float ref);
    void disableAlphaTest();

    /**
     * Whether a line of this width has to be built from triangles.
     *
     * A forward-compatible core context accepts only glLineWidth(1.0) and
     * raises GL_INVALID_VALUE for anything wider, so wide lines there have to be
     * expanded into screen-facing quads. Outside Core, glLineWidth still works
     * and is both cheaper and better looking, so this returns false.
     */
    bool needsWideLineExpansion(float width) const;

    /**
     * Tell the shader a draw carries expanded line vertices, and how big the
     * viewport is -- the expansion is measured in pixels, so it needs to know.
     */
    void setWideLineDraw(bool flag);

    /**
     * Draw following geometry unlit, whatever primitive it is.
     *
     * The shader picks unlit rendering automatically for points and lines,
     * which covers the classes that disabled GL_LIGHTING around a legacy
     * glBegin. It cannot see a class that disables lighting around geometry of
     * some other primitive -- SelectionBox draws a wireframe box that way --
     * because by then the disable has gone through the lg* shim and left no
     * trace the shader can read. Such a class brackets its draw with this
     * instead, which is the shader-path equivalent of that glDisable.
     */
    /**
     * Restores the state the library's own drawing changed.
     *
     * The counterpart to beginFrame(). Scene traversal leaves a program bound,
     * and may leave a vertex array object or a buffer bound with it. Anything
     * composited afterwards through the fixed-function pipeline -- an ImGui GL2
     * backend, an FLTK widget, an application's overlay -- silently draws
     * nothing under that state, which is a hard failure to diagnose from the
     * outside. Ending the frame here means an application does not have to know
     * any of it.
     *
     * Only state this library itself sets is restored. The active texture unit
     * and the scissor box are deliberately not touched: nothing here changes
     * them, so resetting them would be overreach into the application's own
     * state.
     */
    void endFrame();

    void setForceUnlit(bool flag);

    /** Returns true while geometry is being forced unlit. */
    bool forceUnlit() const;

private:
    RenderContext();

    /**
     * Seed the uMat* uniforms with OpenGL's fixed-function default material.
     *
     * Called whenever the active shader changes. Without it, a shape drawn
     * before any material has been applied renders black instead of picking up
     * GL's defaults the way the legacy path does.
     */
    void applyDefaultMaterial();

    glm::mat4 m_projection;
    glm::mat4 m_view;
    std::stack<glm::mat4> m_modelStack;

    std::array<LightData, IVF_MAX_LIGHTS> m_lights;
    int m_lightCount;

    ShaderProgram*    m_shader;
    ShaderProgramPtr  m_ownedShader;   // keeps BlinnPhong shader alive when created via useBlinnPhong()
    glm::vec4         m_globalAmbient;
    bool              m_useTexture;
    RenderProfile     m_profile;

    // Whether the live context is a core profile one. -1 until asked, which
    // beginFrame() does once, where a context is guaranteed to exist.

    int               m_coreContext;
    int               m_legacyDrawDepth;

    ShaderProgramPtr  m_pickShader;
    bool              m_pickMode;
    glm::vec4         m_pickColor;

    bool              m_forceUnlit;

    int               m_textureMode;
    glm::vec4         m_textureEnvColor;
    glm::mat3         m_textureMatrix;

    int               m_fogMode;
    glm::vec4         m_fogColor;
    float             m_fogDensity;
    float             m_fogStart;
    float             m_fogEnd;

    bool              m_twoSided;
    int               m_alphaTestFunc;
    float             m_alphaTestRef;
    bool              m_wideLineDraw;

    // A 1x1 opaque white texture kept bound to unit 0.
    //
    // The fragment shader declares a sampler, and drivers validate it whether or
    // not the branch that reads it is taken -- NVIDIA warns about an incomplete
    // texture on every single draw otherwise. Binding something valid costs one
    // texture object and silences a warning that would bury real ones.
    GLuint            m_whiteTexture;

    void ensureWhiteTexture();

    // Scratch buffers for drawUnlit(). Helper geometry -- cursors, grids, rulers
    // -- changes every frame, so there is nothing to cache, but generating and
    // deleting a VAO and VBO per call per frame is pure overhead. These are kept
    // and refilled instead. Mutable because drawUnlit() is const from the
    // caller's point of view: it draws, it does not change the context.
    mutable GLuint    m_unlitVao;
    mutable GLuint    m_unlitVbo;
    mutable GLsizeiptr m_unlitCapacity;
};

} // namespace ivf
