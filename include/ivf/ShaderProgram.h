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

#include <string>
#include <unordered_map>

namespace ivf {

IvfSmartPointer(ShaderProgram);

/**
 * GLSL shader program wrapper
 *
 * Compiles and links a vertex + fragment shader pair and provides
 * typed uniform setters. Intended as the foundation for modern
 * OpenGL rendering in ivf.
 *
 * Usage:
 *   auto prog = ShaderProgram::create();
 *   prog->loadFromFiles("blinn_phong.vert", "blinn_phong.frag");
 *   prog->use();
 *   prog->setUniformMat4("uMVP", mvp);
 *   // ... draw ...
 *   prog->unuse();
 */
class IVF_API ShaderProgram : public Base {
public:
    ShaderProgram();
    virtual ~ShaderProgram();

    IvfClassInfo("ShaderProgram", Base);
    IvfStdFactory(ShaderProgram);

    /**
     * Compile and link shaders loaded from files on disk.
     * @return true on success, false on compile/link error (error written to stderr).
     */
    bool loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    /**
     * Compile and link shaders provided as source strings.
     * @return true on success, false on compile/link error (error written to stderr).
     */
    bool loadFromStrings(const std::string& vertSrc, const std::string& fragSrc);

    /** Bind this program for subsequent draw calls. */
    void use();

    /** Unbind (bind program 0). */
    void unuse();

    /** Returns true if the program was compiled and linked successfully. */
    bool isLinked() const;

    /** Returns the OpenGL program object handle, or 0 if not linked. */
    GLuint id() const;

    // ---- Uniform setters ----

    void setUniformInt(const std::string& name, int value);
    void setUniformFloat(const std::string& name, float value);
    void setUniformVec3(const std::string& name, const glm::vec3& value);
    void setUniformVec4(const std::string& name, const glm::vec4& value);
    void setUniformMat3(const std::string& name, const glm::mat3& value);
    void setUniformMat4(const std::string& name, const glm::mat4& value);

private:
    GLuint m_programId;
    bool m_linked;
    std::unordered_map<std::string, GLint> m_uniformCache;

    GLint uniformLocation(const std::string& name);

    GLuint compileShader(GLenum type, const std::string& src);
    bool linkProgram(GLuint vert, GLuint frag);

    static std::string readFile(const std::string& path);
};

} // namespace ivf
