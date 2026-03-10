#version 330 core

// ---- Vertex attributes ----
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

// ---- Matrices ----
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;   // transpose(inverse(mat3(uView * uModel)))

// ---- Outputs to fragment shader ----
out vec3 vFragPos;    // position in view space
out vec3 vNormal;     // normal  in view space
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
