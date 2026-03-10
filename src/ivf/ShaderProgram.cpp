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

#include <ivf/ShaderProgram.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace ivf {

ShaderProgram::ShaderProgram()
    : m_programId(0)
    , m_linked(false)
{
}

ShaderProgram::~ShaderProgram()
{
    if (m_programId != 0)
        glDeleteProgram(m_programId);
}

bool ShaderProgram::loadFromFiles(const std::string& vertPath, const std::string& fragPath)
{
    std::string vertSrc = readFile(vertPath);
    std::string fragSrc = readFile(fragPath);

    if (vertSrc.empty()) {
        std::cerr << "ShaderProgram: could not read vertex shader: " << vertPath << "\n";
        return false;
    }
    if (fragSrc.empty()) {
        std::cerr << "ShaderProgram: could not read fragment shader: " << fragPath << "\n";
        return false;
    }

    return loadFromStrings(vertSrc, fragSrc);
}

bool ShaderProgram::loadFromStrings(const std::string& vertSrc, const std::string& fragSrc)
{
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    if (vert == 0)
        return false;

    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    if (frag == 0) {
        glDeleteShader(vert);
        return false;
    }

    bool ok = linkProgram(vert, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return ok;
}

void ShaderProgram::use()
{
    if (m_linked)
        glUseProgram(m_programId);
}

void ShaderProgram::unuse()
{
    glUseProgram(0);
}

bool ShaderProgram::isLinked() const
{
    return m_linked;
}

GLuint ShaderProgram::id() const
{
    return m_programId;
}

void ShaderProgram::setUniformInt(const std::string& name, int value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniform1i(loc, value);
}

void ShaderProgram::setUniformFloat(const std::string& name, float value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniform1f(loc, value);
}

void ShaderProgram::setUniformVec3(const std::string& name, const glm::vec3& value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniform3fv(loc, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformVec4(const std::string& name, const glm::vec4& value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniform4fv(loc, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformMat3(const std::string& name, const glm::mat3& value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniformMat4(const std::string& name, const glm::mat4& value)
{
    GLint loc = uniformLocation(name);
    if (loc >= 0)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

// ---- Private helpers ----

GLint ShaderProgram::uniformLocation(const std::string& name)
{
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end())
        return it->second;

    GLint loc = glGetUniformLocation(m_programId, name.c_str());
    m_uniformCache[name] = loc;

    if (loc < 0)
        std::cerr << "ShaderProgram: uniform '" << name << "' not found in program " << m_programId << "\n";

    return loc;
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string& src)
{
    GLuint shader = glCreateShader(type);
    const char* srcPtr = src.c_str();
    glShaderSource(shader, 1, &srcPtr, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        const char* typeName = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        std::cerr << "ShaderProgram: " << typeName << " shader compile error:\n" << log.data() << "\n";
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ShaderProgram::linkProgram(GLuint vert, GLuint frag)
{
    if (m_programId != 0)
        glDeleteProgram(m_programId);

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vert);
    glAttachShader(m_programId, frag);
    glLinkProgram(m_programId);

    GLint ok = GL_FALSE;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint logLen = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetProgramInfoLog(m_programId, logLen, nullptr, log.data());
        std::cerr << "ShaderProgram: link error:\n" << log.data() << "\n";
        glDeleteProgram(m_programId);
        m_programId = 0;
        m_linked = false;
        return false;
    }

    m_uniformCache.clear();
    m_linked = true;
    return true;
}

std::string ShaderProgram::readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return {};
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

} // namespace ivf
