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
