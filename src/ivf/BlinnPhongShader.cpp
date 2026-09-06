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

#include <ivf/BlinnPhongShader.h>

namespace ivf {

// ---------------------------------------------------------------------------
// Embedded vertex shader — must stay in sync with data/shaders/blinn_phong.vert
// ---------------------------------------------------------------------------
static const char* s_vertSrc = R"GLSL(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vColor;

void main()
{
    vec4 viewPos  = uView * uModel * vec4(aPosition, 1.0);
    vFragPos      = vec3(viewPos);
    vNormal       = normalize(uNormalMatrix * aNormal);
    vTexCoord     = aTexCoord;
    vColor        = aColor;
    gl_Position   = uProjection * viewPos;
}
)GLSL";

// ---------------------------------------------------------------------------
// Embedded fragment shader — must stay in sync with data/shaders/blinn_phong.frag
// ---------------------------------------------------------------------------
static const char* s_fragSrc = R"GLSL(
#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

uniform vec4  uMatAmbient;
uniform vec4  uMatDiffuse;
uniform vec4  uMatSpecular;
uniform vec4  uMatEmission;
uniform float uMatShininess;

uniform bool      uUseTexture;
uniform sampler2D uTexture;

uniform bool uUseVertexColor;
uniform bool uUnlit;

uniform vec4 uGlobalAmbient;

struct Light {
    vec4  position;
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec3  spotDirection;
    float spotCutoff;
    float spotExponent;
    float constAtt;
    float linearAtt;
    float quadAtt;
    int   type;
};

uniform int   uLightCount;
uniform Light uLights[8];

out vec4 fragColor;

vec4 computeLight(Light light, vec3 N, vec3 V, vec4 ambientColor, vec4 diffuseColor)
{
    vec3 L;
    float attenuation = 1.0;

    if (light.type == 1 || light.position.w == 0.0) {
        L = normalize(vec3(light.position));
    } else {
        vec3  lightVec = vec3(light.position) - vFragPos;
        float dist     = length(lightVec);
        L              = lightVec / dist;
        attenuation    = 1.0 / (light.constAtt
                               + light.linearAtt * dist
                               + light.quadAtt   * dist * dist);

        if (light.type == 2 && light.spotCutoff < 180.0) {
            vec3  spotDir  = normalize(vec3(light.spotDirection));
            float cosAngle = dot(-L, spotDir);
            float cosCutoff = cos(radians(light.spotCutoff));
            if (cosAngle < cosCutoff) return vec4(0.0);
            attenuation *= pow(max(cosAngle, 0.0), light.spotExponent);
        }
    }

    vec4  ambient  = light.ambient * ambientColor;
    float NdotL    = max(dot(N, L), 0.0);
    vec4  diffuse  = NdotL * light.diffuse * diffuseColor;

    vec4 specular = vec4(0.0);
    if (NdotL > 0.0 && uMatShininess > 0.0) {
        vec3  H     = normalize(L + V);
        float NdotH = max(dot(N, H), 0.0);
        specular    = pow(NdotH, uMatShininess) * light.specular * uMatSpecular;
    }

    return ambient + attenuation * (diffuse + specular);
}

void main()
{
    if (uUnlit) {
        fragColor = uUseVertexColor ? vColor : uMatDiffuse;
        return;
    }

    vec3 N = normalize(vNormal);
    vec3 V = normalize(-vFragPos);

    // Vertex colour replaces ambient as well as diffuse, matching the
    // GL_AMBIENT_AND_DIFFUSE default of legacy glColorMaterial.
    vec4 ambientColor = uUseVertexColor ? vColor : uMatAmbient;
    vec4 diffuseColor = uUseVertexColor ? vColor : uMatDiffuse;

    vec4 color = uMatEmission + uGlobalAmbient * ambientColor;

    for (int i = 0; i < uLightCount; ++i)
        color += computeLight(uLights[i], N, V, ambientColor, diffuseColor);

    if (uUseTexture)
        color *= texture(uTexture, vTexCoord);

    color.a = diffuseColor.a;
    fragColor = clamp(color, 0.0, 1.0);
}
)GLSL";

// ---------------------------------------------------------------------------

ShaderProgramPtr BlinnPhongShader::create()
{
    auto prog = ShaderProgramPtr(new ShaderProgram());
    prog->loadFromStrings(s_vertSrc, s_fragSrc);
    return prog;
}

const char* BlinnPhongShader::vertexSource()
{
    return s_vertSrc;
}

const char* BlinnPhongShader::fragmentSource()
{
    return s_fragSrc;
}

} // namespace ivf
