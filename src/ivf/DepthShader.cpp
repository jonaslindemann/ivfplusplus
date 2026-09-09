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

#include <ivf/DepthShader.h>

namespace ivf {

static const char* s_depthVertSrc = R"GLSL(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
)GLSL";

static const char* s_depthFragSrc = R"GLSL(
#version 330 core

void main()
{
    // Nothing to write. Depth reaches the depth attachment on its own, and the
    // framebuffer has no colour attachment to write to in any case.
}
)GLSL";

ShaderProgramPtr DepthShader::create()
{
    auto prog = ShaderProgramPtr(new ShaderProgram());
    prog->loadFromStrings(s_depthVertSrc, s_depthFragSrc);
    return prog;
}

} // namespace ivf
