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
uniform bool uUseVertexColor;  // replace uMatDiffuse with vColor in lighting
uniform bool uUnlit;           // skip lighting, output vColor directly

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
vec4 computeLight(Light light, vec3 N, vec3 V, vec4 diffuseColor)
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
    vec4 ambient = light.ambient * uMatAmbient;

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

void main()
{
    // Unlit path: output vertex color directly (used for points, lines)
    if (uUnlit) {
        fragColor = vColor;
        return;
    }

    vec3 N = normalize(vNormal);
    vec3 V = normalize(-vFragPos); // view direction in view space

    // Choose diffuse source: per-vertex color or material uniform
    vec4 diffuseColor = uUseVertexColor ? vColor : uMatDiffuse;

    // Start with emission and global ambient
    vec4 color = uMatEmission + uGlobalAmbient * uMatAmbient;

    for (int i = 0; i < uLightCount; ++i) {
        color += computeLight(uLights[i], N, V, diffuseColor);
    }

    // Texture modulation
    if (uUseTexture) {
        color *= texture(uTexture, vTexCoord);
    }

    // Clamp final alpha to diffuse alpha
    color.a = diffuseColor.a;

    fragColor = clamp(color, 0.0, 1.0);
}
