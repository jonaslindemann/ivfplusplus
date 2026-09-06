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
};

} // namespace ivf
