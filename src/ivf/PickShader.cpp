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

#include <ivf/PickShader.h>

namespace ivf {

// The attribute layout has to match BlinnPhongShader exactly, so a VAO built for
// normal drawing can be replayed through this program untouched. The unused
// attributes are declared for that reason alone.

static const char *s_pickVertSrc = R"GLSL(
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

// Nothing here may alter the value. It is an identifier that happens to travel
// through a colour channel, not a colour.

static const char *s_pickFragSrc = R"GLSL(
#version 330 core

uniform vec4 uPickColor;

out vec4 fragColor;

void main()
{
    fragColor = uPickColor;
}
)GLSL";

ShaderProgramPtr PickShader::create()
{
    ShaderProgramPtr prog = ShaderProgram::create();
    prog->loadFromStrings(s_pickVertSrc, s_pickFragSrc);
    return prog;
}

} // namespace ivf
