#version 330 core

// ---- Inputs from vertex shader ----
in vec3 vFragPos;   // view-space position
in vec3 vNormal;    // view-space normal (already normalized)
in vec2 vTexCoord;
in vec4 vColor;

// ---- Material ----
uniform vec4  uMatAmbient;
uniform vec4  uMatDiffuse;
uniform vec4  uMatSpecular;
uniform vec4  uMatEmission;
uniform float uMatShininess;

// ---- Texture ----
uniform bool      uUseTexture;
uniform sampler2D uTexture;

// ---- Vertex-color / unlit modes ----
uniform bool uUseVertexColor;  // replace uMatDiffuse with vColor
uniform bool uUnlit;           // skip lighting for points, lines, and helpers

// ---- Global scene ambient ----
uniform vec4 uGlobalAmbient;

// ---- Lights ----
struct Light {
    vec4  position;       // view-space: w=0 directional, w=1 positional
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec3  spotDirection;  // view-space
    float spotCutoff;     // degrees; 180 = not a spotlight
    float spotExponent;
    float constAtt;
    float linearAtt;
    float quadAtt;
    int   type;           // 0=point, 1=directional, 2=spot
};

uniform int   uLightCount;
uniform Light uLights[8];

// ---- Output ----
out vec4 fragColor;

// ---- Blinn-Phong for a single light ----
vec4 computeLight(Light light, vec3 N, vec3 V, vec4 ambientColor, vec4 diffuseColor)
{
    vec3 L;
    float attenuation = 1.0;

    if (light.type == 1 || light.position.w == 0.0) {
        // Directional light: position holds direction (toward light)
        L = normalize(vec3(light.position));
    } else {
        // Point or spot: compute direction and attenuation
        vec3 lightVec = vec3(light.position) - vFragPos;
        float dist    = length(lightVec);
        L             = lightVec / dist;

        attenuation = 1.0 / (light.constAtt
                            + light.linearAtt  * dist
                            + light.quadAtt    * dist * dist);

        // Spot cone
        if (light.type == 2 && light.spotCutoff < 180.0) {
            vec3  spotDir = normalize(vec3(light.spotDirection));
            float cosAngle = dot(-L, spotDir);
            float cosCutoff = cos(radians(light.spotCutoff));
            if (cosAngle < cosCutoff) {
                return vec4(0.0); // outside cone
            }
            attenuation *= pow(max(cosAngle, 0.0), light.spotExponent);
        }
    }

    // Ambient
    vec4 ambient = light.ambient * ambientColor;

    // Diffuse
    float NdotL  = max(dot(N, L), 0.0);
    vec4 diffuse = NdotL * light.diffuse * diffuseColor;

    // Specular (Blinn-Phong half-vector)
    vec4 specular = vec4(0.0);
    if (NdotL > 0.0 && uMatShininess > 0.0) {
        vec3  H      = normalize(L + V);
        float NdotH  = max(dot(N, H), 0.0);
        specular     = pow(NdotH, uMatShininess) * light.specular * uMatSpecular;
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
    vec3 V = normalize(-vFragPos); // view direction in view space

    // Per-vertex colour stands in for the material the same way legacy
    // glColorMaterial does, and its default mode is GL_AMBIENT_AND_DIFFUSE --
    // so the vertex colour has to replace ambient as well as diffuse. Replacing
    // diffuse alone left vertex-coloured geometry visibly duller than the
    // fixed-function path drew it.
    vec4 ambientColor = uUseVertexColor ? vColor : uMatAmbient;
    vec4 diffuseColor = uUseVertexColor ? vColor : uMatDiffuse;

    // Start with emission and global ambient
    vec4 color = uMatEmission + uGlobalAmbient * ambientColor;

    for (int i = 0; i < uLightCount; ++i) {
        color += computeLight(uLights[i], N, V, ambientColor, diffuseColor);
    }

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
