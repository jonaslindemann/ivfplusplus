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

// ---- Wide lines ----
//
// Nonzero when this draw is a line expanded into triangles. See
// GLPrimitive::buildAndDrawVAO(): aNormal then holds the segment's other
// endpoint and aTexCoord holds (side, half width in pixels).
uniform int  uWideLine;
uniform vec2 uViewportSize;


void main()
{
    vec4 viewPos  = uView * uModel * vec4(aPosition, 1.0);
    vFragPos      = vec3(viewPos);
    vNormal       = normalize(uNormalMatrix * aNormal);
    vTexCoord     = aTexCoord;
    vColor        = aColor;

    gl_Position   = uProjection * viewPos;

    if (uWideLine != 0)
    {
        // Project both ends, measure the segment direction in pixels, and step
        // sideways from it. Doing this before projection would give a width that
        // shrank with distance; a line width is a screen measurement.

        vec4 other = uProjection * uView * uModel * vec4(aNormal, 1.0);

        // A vertex behind the eye has no meaningful screen position, so leave
        // the segment unexpanded rather than reflecting it through the origin.

        if ((gl_Position.w > 0.0) && (other.w > 0.0))
        {
            vec2 half_ = uViewportSize * 0.5;
            vec2 s0 = gl_Position.xy / gl_Position.w * half_;
            vec2 s1 = other.xy / other.w * half_;
            vec2 d  = s1 - s0;

            if (dot(d, d) > 1e-12)
            {
                vec2 n = normalize(vec2(-d.y, d.x));

                // aTexCoord.x is which side, aTexCoord.y the half width.
                vec2 offset = n * aTexCoord.x * aTexCoord.y;

                gl_Position.xy += offset / half_ * gl_Position.w;
            }
        }

        // The texture coordinate was carrying geometry, not a texture lookup.
        vTexCoord = vec2(0.0);
    }
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

// ---- Texture environment ----
//
// Mirrors glTexEnv. GL applies the texture to the lit colour, so this happens
// after the lighting loop, not to the material going into it.
//   0 modulate (the GL default), 1 decal, 2 replace, 3 blend
uniform int  uTextureMode;
uniform vec4 uTextureEnvColor;

// The GL_TEXTURE matrix stack, as the 2D affine transform it always really was.
uniform mat3 uTextureMatrix;

// ---- Fog ----
// 0 none, 1 linear, 2 exp, 3 exp2 -- matching GL_FOG_MODE.
uniform int   uFogMode;
uniform vec4  uFogColor;
uniform float uFogDensity;
uniform float uFogStart;
uniform float uFogEnd;

// ---- Two-sided lighting ----
// GL_LIGHT_MODEL_TWO_SIDE: flip the normal on back faces so the far side of an
// open surface is lit rather than black.
uniform bool uTwoSided;

// ---- Alpha test ----
// The GL comparison enum, or 0 for off. Removed from core, where the equivalent
// is to discard the fragment -- which is what this does.
uniform int   uAlphaTestFunc;
uniform float uAlphaTestRef;

// ---- Helpers ----

vec4 applyTexture(vec4 color)
{
    vec2 uv = (uTextureMatrix * vec3(vTexCoord, 1.0)).xy;
    vec4 texel = texture(uTexture, uv);

    if (uTextureMode == 1)
    {
        // Decal: the texture replaces colour in proportion to its own alpha and
        // leaves the fragment alpha alone.
        return vec4(mix(color.rgb, texel.rgb, texel.a), color.a);
    }
    else if (uTextureMode == 2)
    {
        return texel;
    }
    else if (uTextureMode == 3)
    {
        // Blend: the texture selects between the fragment colour and the
        // constant environment colour, per channel.
        return vec4(mix(color.rgb, uTextureEnvColor.rgb, texel.rgb), color.a * texel.a);
    }

    return color * texel;
}

float fogFactor()
{
    // Distance from the eye. Everything reaching here is already in view space,
    // where the eye sits at the origin.
    float d = length(vFragPos);

    if (uFogMode == 1)
    {
        float span = uFogEnd - uFogStart;
        return clamp((uFogEnd - d) / max(span, 1e-6), 0.0, 1.0);
    }
    else if (uFogMode == 2)
    {
        return clamp(exp(-uFogDensity * d), 0.0, 1.0);
    }

    float e = uFogDensity * d;
    return clamp(exp(-e * e), 0.0, 1.0);
}

bool alphaTestPasses(float a)
{
    if (uAlphaTestFunc == 0x0200) return false;                 // NEVER
    if (uAlphaTestFunc == 0x0201) return a <  uAlphaTestRef;    // LESS
    if (uAlphaTestFunc == 0x0202) return a == uAlphaTestRef;    // EQUAL
    if (uAlphaTestFunc == 0x0203) return a <= uAlphaTestRef;    // LEQUAL
    if (uAlphaTestFunc == 0x0204) return a >  uAlphaTestRef;    // GREATER
    if (uAlphaTestFunc == 0x0205) return a != uAlphaTestRef;    // NOTEQUAL
    if (uAlphaTestFunc == 0x0206) return a >= uAlphaTestRef;    // GEQUAL
    return true;                                                // ALWAYS
}

void main()
{
    // Unlit path: use vertex color when present, otherwise material diffuse.
    if (uUnlit) {
        vec4 flat_ = uUseVertexColor ? vColor : uMatDiffuse;

        if (uUseTexture)
            flat_ = applyTexture(flat_);

        if (!alphaTestPasses(flat_.a))
            discard;

        if (uFogMode != 0)
            flat_.rgb = mix(uFogColor.rgb, flat_.rgb, fogFactor());

        fragColor = clamp(flat_, 0.0, 1.0);
        return;
    }

    vec3 N = normalize(vNormal);

    // Back faces of an open surface get the flipped normal, otherwise they
    // face away from every light and render black.
    if (uTwoSided && !gl_FrontFacing)
        N = -N;
    vec3 V = normalize(-vFragPos);

    // Vertex colour replaces ambient as well as diffuse, matching the
    // GL_AMBIENT_AND_DIFFUSE default of legacy glColorMaterial.
    vec4 ambientColor = uUseVertexColor ? vColor : uMatAmbient;
    vec4 diffuseColor = uUseVertexColor ? vColor : uMatDiffuse;

    vec4 color = uMatEmission + uGlobalAmbient * ambientColor;

    for (int i = 0; i < uLightCount; ++i)
        color += computeLight(uLights[i], N, V, ambientColor, diffuseColor);

    // Alpha comes from the material before texturing, so that a texture with an
    // alpha channel can then modify it. Assigning it afterwards -- as this used
    // to -- threw the texture's alpha away, which is exactly the channel a glyph
    // atlas carries its shape in.
    color.a = diffuseColor.a;

    if (uUseTexture)
        color = applyTexture(color);

    if (!alphaTestPasses(color.a))
        discard;

    // Fog is applied last and to colour only: it is how far away the surface is,
    // not how transparent it is.
    if (uFogMode != 0)
        color.rgb = mix(uFogColor.rgb, color.rgb, fogFactor());

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
