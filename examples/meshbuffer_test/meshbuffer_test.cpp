// ------------------------------------------------------------
//
// meshbuffer_test -- verification and side-by-side display of MeshBuffer
//
// MeshBuffer holds an indexed mesh in a vertex buffer and can draw it two
// ways from the same buffers: through generic vertex attributes for the
// shader pipeline, and through the fixed-function client arrays for when no
// shader is active. Both have to work, and neither may leak state into the
// other, because an application running on a compatibility profile switches
// between them freely.
//
// The context created here matches what ObjectiveFrame asks for: OpenGL 3.2,
// compatibility profile.
//
// The program first runs a batch of automated checks -- buffer contents, both
// draw paths, state leakage, in-place updates -- and prints the number of
// checks performed. It then opens a viewer drawing the same MeshBuffer twice
// per frame: fixed-function on the left, shader on the right. The two halves
// should be indistinguishable apart from the shading model.
//
//   space  toggle per-vertex colours
//   r      toggle rotation
//   esc    quit
//
// Pass --headless to skip the viewer and exit as soon as the checks are done,
// which is how this is useful as a regression test. The exit code is non-zero
// if any check failed.
//
// ------------------------------------------------------------

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <ivf/ExtrusionBuilder.h>
#include <ivf/ExtrusionProfile.h>
#include <ivf/MeshBuffer.h>
#include <ivf/PathFrames.h>
#include <ivf/ShaderProgram.h>
#include <ivf/rc.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

using namespace ivf;

static int g_failures = 0;
static int g_checks = 0;

// Viewer state, driven from the key callback.

static bool g_useVertexColors = true;
static bool g_rotating = true;

static void check(bool cond, const std::string &what)
{
    g_checks++;

    if (!cond)
    {
        g_failures++;
        std::printf("  FAIL: %s\n", what.c_str());
    }
}

static void checkNoGLError(const std::string &what)
{
    GLenum err = glGetError();

    g_checks++;

    if (err != GL_NO_ERROR)
    {
        g_failures++;
        std::printf("  FAIL: %s (GL error 0x%04x)\n", what.c_str(), err);
    }

    while (glGetError() != GL_NO_ERROR)
    {
    }
}

// Count non-background pixels, so an actual draw can be told from a no-op.

static int countDrawnPixels(int w, int h)
{
    std::vector<unsigned char> pixels(std::size_t(w) * h * 4);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    int n = 0;

    for (std::size_t i = 0; i < pixels.size(); i += 4)
        if ((pixels[i] > 8) || (pixels[i + 1] > 8) || (pixels[i + 2] > 8))
            n++;

    return n;
}

// A straight tube along -z, used by the checks because its geometry is easy
// to assert against.

static MeshData makeTube(float radius, int segments, float length)
{
    ExtrusionProfile profile = ExtrusionProfile::circle(radius, segments);

    std::vector<glm::vec3> spine = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -length)};
    std::vector<PathFrame> frames = buildPathFrames(spine, SpineInterp::Polyline, JoinStyle::Angle, 64,
                                                    FrameMethod::RotationMinimizing, glm::vec3(0, 1, 0));

    return ExtrusionBuilder::build(profile, frames, ExtrusionOptions());
}

// A bent, tapering, colour graded tube for the viewer. Rounded joins and a
// per-station transform give the eye something to check the two draw paths
// against.

static MeshData makeShowpiece()
{
    ExtrusionProfile profile = ExtrusionProfile::circle(0.30f, 24);

    std::vector<glm::vec3> spine = {glm::vec3(-1.6f, -0.9f, 0.0f), glm::vec3(-0.6f, -0.9f, 0.0f),
                                    glm::vec3(0.2f, 0.9f, 0.4f),   glm::vec3(1.2f, 0.9f, -0.4f),
                                    glm::vec3(1.6f, -0.3f, 0.0f)};

    std::vector<PathFrame> frames = buildPathFrames(spine, SpineInterp::CatmullRom, JoinStyle::Round, 96,
                                                    FrameMethod::RotationMinimizing, glm::vec3(0, 1, 0), false, 0.25f,
                                                    8);

    ExtrusionOptions options;
    options.sectionFn = [](int s, const PathFrame &f) {
        SectionTransform st;

        // Taper towards both ends, and run a colour ramp along the spine.

        float t = f.source / 4.0f;
        st.scale = glm::vec2(0.45f + 0.55f * std::sin(t * glm::pi<float>()));
        st.twist = t * glm::pi<float>();
        st.color = glm::vec4(t, 0.35f + 0.5f * (1.0f - t), 1.0f - t, 1.0f);

        return st;
    };

    return ExtrusionBuilder::build(profile, frames, options);
}

// ------------------------------------------------------------
// Automated checks
// ------------------------------------------------------------

static void runChecks(int W, int H)
{
    GLint profileMask = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileMask);
    check((profileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) != 0, "running on a compatibility profile context");

    glViewport(0, 0, W, H);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    MeshData mesh = makeTube(0.5f, 16, 2.0f);
    check(mesh.isValid(), "test mesh is valid");

    // ------------------------------------------------------------
    // Buffer contents
    // ------------------------------------------------------------

    std::printf("MeshBuffer upload\n");

    MeshBuffer buffer;
    check(buffer.isEmpty(), "a fresh buffer is empty");

    buffer.setMesh(mesh);
    check(!buffer.isEmpty(), "buffer holds the mesh");
    check(buffer.vertexCount() == int(mesh.vertexCount()), "vertex count matches the mesh");
    check(buffer.indexCount() == int(mesh.triangleCount() * 3), "index count is three per triangle");

    // Bounds: the tube runs along -z with a radius of 0.5.

    check(std::fabs(buffer.boundsMin().z + 2.0f) < 1e-4f, "bounds reach the far end of the tube");
    check(std::fabs(buffer.boundsMax().x - 0.5f) < 1e-4f, "bounds reach the tube radius");
    check(std::fabs(buffer.boundingRadius() - std::sqrt(0.5 * 0.5 + 0.5 * 0.5 + 2.0 * 2.0)) < 1e-4,
          "bounding radius is measured from the local origin");

    checkNoGLError("no GL calls made before the first draw");

    // ------------------------------------------------------------
    // Fixed-function path
    // ------------------------------------------------------------

    std::printf("Fixed-function draw\n");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -4.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("fixed-function draw produced no GL error");

    int litPixels = countDrawnPixels(W, H);
    check(litPixels > 1000, "fixed-function draw put geometry on screen");

    // The buffers must be left unbound and the client state disabled, so the
    // draw cannot leak into whatever the application does next.

    GLint boundArray = -1;
    GLint boundElement = -1;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundArray);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundElement);

    check(boundArray == 0, "array buffer unbound after the draw");
    check(boundElement == 0, "element buffer unbound after the draw");
    check(glIsEnabled(GL_VERTEX_ARRAY) == GL_FALSE, "vertex array client state disabled after the draw");
    check(glIsEnabled(GL_NORMAL_ARRAY) == GL_FALSE, "normal array client state disabled after the draw");
    check(glIsEnabled(GL_COLOR_ARRAY) == GL_FALSE, "color array client state disabled after the draw");

    // With a colour array and GL_COLOR_MATERIAL, the per-vertex colours drive
    // the lit surface. This is how result colours along a beam are rendered.

    MeshData colored = mesh;

    for (auto &c : colored.colors)
        c = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    MeshBuffer colorBuffer;
    colorBuffer.setMesh(colored);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    colorBuffer.drawFixedFunction(GL_TRIANGLES, true);
    checkNoGLError("colored fixed-function draw produced no GL error");

    std::vector<unsigned char> px(std::size_t(W) * H * 4);
    glReadPixels(0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, px.data());

    long redSum = 0;
    long greenSum = 0;
    long blueSum = 0;

    for (std::size_t i = 0; i < px.size(); i += 4)
    {
        redSum += px[i];
        greenSum += px[i + 1];
        blueSum += px[i + 2];
    }

    check(redSum > 4 * (greenSum + blueSum + 1), "per-vertex color array reached the framebuffer");
    glDisable(GL_COLOR_MATERIAL);

    // ------------------------------------------------------------
    // Shader path
    // ------------------------------------------------------------

    std::printf("Shader draw\n");

    rcUseBlinnPhong();
    check(rcIsShaderActive(), "the built-in Blinn-Phong shader linked");

    LightData light;
    light.position = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    light.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    light.enabled = true;

    rcBeginFrame();
    rcAddLight(light);
    rcSetProjection(glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 20.0f));
    rcSetView(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)));

    rcUseShader();
    rcUpdateShader();
    rcShader()->setUniformVec4("uMatDiffuse", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    rcShader()->setUniformVec4("uMatAmbient", glm::vec4(0.0f, 0.3f, 0.0f, 1.0f));
    rcShader()->setUniformInt("uUseVertexColor", 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawShader(GL_TRIANGLES);
    checkNoGLError("shader draw produced no GL error");

    int shaderPixels = countDrawnPixels(W, H);
    check(shaderPixels > 1000, "shader draw put geometry on screen");

    // Both paths read the same buffer, so they must cover the same silhouette.

    double coverageRatio = double(shaderPixels) / double(litPixels);
    check((coverageRatio > 0.95) && (coverageRatio < 1.05), "both draw paths cover the same silhouette");

    // draw() dispatches on whether a shader is active.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.draw(GL_TRIANGLES, false);
    checkNoGLError("draw() dispatch produced no GL error");
    check(countDrawnPixels(W, H) > 1000, "draw() rendered through the shader path");

    GLint boundVao = -1;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVao);
    check(boundVao == 0, "vertex array object unbound after the shader draw");

    // A fixed-function draw issued while a program is still bound must not
    // quietly render through the shader. Most drivers alias glVertexPointer
    // onto generic attribute 0, so the geometry would appear in the right
    // place with no normals -- a lighting bug that is hard to trace back to
    // its cause. Check both that the program is dropped and that the surface
    // comes out lit.

    rcUseShader();

    GLint boundProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram);
    check(boundProgram != 0, "test precondition: a program is bound");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("fixed-function draw with a program bound produced no GL error");

    glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram);
    check(boundProgram == 0, "fixed-function draw unbinds the shader program");

    std::vector<unsigned char> mixed(std::size_t(W) * H * 4);
    glReadPixels(0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, mixed.data());

    int brightPixels = 0;

    for (std::size_t i = 0; i < mixed.size(); i += 4)
        if (mixed[i] > 64 || mixed[i + 1] > 64 || mixed[i + 2] > 64)
            brightPixels++;

    check(brightPixels > 500, "fixed-function draw is lit, not silently shader-drawn without normals");

    // ------------------------------------------------------------
    // Switching back and forth, and in-place updates
    // ------------------------------------------------------------

    std::printf("Path switching and updates\n");

    rcSetShader(nullptr);
    check(!rcIsShaderActive(), "shader deactivated");

    glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.draw(GL_TRIANGLES, false);
    checkNoGLError("draw() fell back to fixed function without error");
    check(countDrawnPixels(W, H) > 1000, "draw() rendered through the fixed-function path");

    // setVertexData updates in place without disturbing the topology.

    MeshData moved = mesh;

    for (auto &p : moved.positions)
        p.x += 0.25f;

    buffer.setVertexData(moved);
    check(buffer.vertexCount() == int(mesh.vertexCount()), "in-place update keeps the vertex count");
    check(std::fabs(buffer.boundsMax().x - 0.75f) < 1e-4f, "in-place update recomputed the bounds");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("draw after in-place update produced no GL error");
    check(countDrawnPixels(W, H) > 1000, "geometry still drawn after an in-place update");

    // A vertex count change must fall back to a full respecification, rather
    // than leaving a stale index list pointing past the end of the buffer.

    MeshData bigger = makeTube(0.5f, 32, 2.0f);
    check(bigger.vertexCount() != mesh.vertexCount(), "test precondition: the mesh size changed");

    buffer.setVertexData(bigger);
    check(buffer.vertexCount() == int(bigger.vertexCount()), "setVertexData fell back to a full replace");
    check(buffer.indexCount() == int(bigger.triangleCount() * 3), "index list replaced along with the vertices");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("draw after a size-changing update produced no GL error");
    check(countDrawnPixels(W, H) > 1000, "geometry drawn after a size-changing update");

    // releaseGL drops the OpenGL objects. The next draw must rebuild them.

    buffer.releaseGL();
    checkNoGLError("releaseGL produced no GL error");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    buffer.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("draw after releaseGL produced no GL error");
    check(countDrawnPixels(W, H) > 1000, "buffers were recreated after releaseGL");

    // An empty buffer draws nothing and raises nothing.

    MeshBuffer empty;
    empty.drawShader(GL_TRIANGLES);
    empty.drawFixedFunction(GL_TRIANGLES, true);
    empty.draw(GL_TRIANGLES, false);
    checkNoGLError("drawing an empty buffer is a no-op");

    MeshBuffer cleared;
    cleared.setMesh(mesh);
    cleared.drawFixedFunction(GL_TRIANGLES, false);
    cleared.clear();
    check(cleared.isEmpty(), "clear() empties the buffer");
    cleared.drawFixedFunction(GL_TRIANGLES, false);
    checkNoGLError("drawing a cleared buffer is a no-op");
}

// ------------------------------------------------------------
// Viewer
// ------------------------------------------------------------

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_SPACE:
        g_useVertexColors = !g_useVertexColors;
        break;
    case GLFW_KEY_R:
        g_rotating = !g_rotating;
        break;
    default:
        break;
    }
}

// Left half: the fixed-function pipeline reaching into the vertex buffer
// through the client arrays.

static void drawFixedFunctionHalf(MeshBuffer &buffer, int x, int y, int w, int h, float angle)
{
    glViewport(x, y, w, h);

    float aspect = float(w) / float(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5 * aspect, 0.5 * aspect, -0.5, 0.5, 1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -6.0);
    glRotated(20.0, 1.0, 0.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // GL_POSITION is transformed by the modelview matrix in force when it is
    // set, so it goes in here -- with the view loaded but before the model
    // rotation below. Setting it any later would leave the light spinning
    // along with the object, which the shader half does not do.

    GLfloat lightPos[4] = {0.4f, 0.8f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Match the light and the lighting model to what the shader half feeds
    // RenderContext, so the two halves are directly comparable. The default
    // light model ambient would otherwise add a term the shader has no
    // counterpart for.

    GLfloat lightAmbient[4] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightWhite[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat noAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightWhite);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightWhite);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noAmbient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glRotated(angle, 0.0, 1.0, 0.0);

    GLfloat diffuse[4] = {0.2f, 0.6f, 0.9f, 1.0f};
    GLfloat ambient[4] = {0.1f, 0.2f, 0.3f, 1.0f};
    GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40.0f);

    if (g_useVertexColors)
    {
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }

    buffer.drawFixedFunction(GL_TRIANGLES, g_useVertexColors);

    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

// Right half: the same buffer, same frame, through the shader pipeline.

static void drawShaderHalf(MeshBuffer &buffer, int x, int y, int w, int h, float angle)
{
    glViewport(x, y, w, h);

    float aspect = float(w) / float(h);

    LightData light;
    light.position = glm::vec4(0.4f, 0.8f, 1.0f, 0.0f);
    light.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.type = 1; // directional
    light.enabled = true;

    rcBeginFrame();
    rcAddLight(light);
    rcSetProjection(glm::frustum(-0.5f * aspect, 0.5f * aspect, -0.5f, 0.5f, 1.0f, 40.0f));

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
    view = glm::rotate(view, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rcSetView(view);

    rcPushMatrix();
    rcRotate(angle, 0.0f, 1.0f, 0.0f);

    rcUseShader();
    rcUpdateShader();

    ShaderProgram *prog = rcShader();
    prog->setUniformVec4("uMatDiffuse", glm::vec4(0.2f, 0.6f, 0.9f, 1.0f));
    prog->setUniformVec4("uMatAmbient", glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
    prog->setUniformVec4("uMatSpecular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    prog->setUniformFloat("uMatShininess", 40.0f);
    prog->setUniformInt("uUseVertexColor", g_useVertexColors ? 1 : 0);

    buffer.drawShader(GL_TRIANGLES);

    rcPopMatrix();
}

static void runViewer(GLFWwindow *window)
{
    std::printf("\nViewer: fixed-function on the left, shader on the right.\n");
    std::printf("  space  toggle per-vertex colours\n");
    std::printf("  r      toggle rotation\n");
    std::printf("  esc    quit\n");

    glfwSetKeyCallback(window, keyCallback);

    MeshBuffer buffer;
    buffer.setMesh(makeShowpiece());

    // The checks leave the shader deactivated. Bring it back for the right half.

    rcUseBlinnPhong();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.10f, 0.11f, 0.13f, 1.0f);

    float angle = 0.0f;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float dt = float(now - lastTime);
        lastTime = now;

        if (g_rotating)
            angle += 30.0f * dt;

        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        if (fbWidth > 0 && fbHeight > 0)
        {
            glViewport(0, 0, fbWidth, fbHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int half = fbWidth / 2;

            // A thin gap between the halves, so the split is unambiguous.

            drawFixedFunctionHalf(buffer, 0, 0, half - 1, fbHeight, angle);

            // The fixed-function half must not have disturbed the shader half,
            // and vice versa -- that is the point of drawing both per frame.

            drawShaderHalf(buffer, half + 1, 0, fbWidth - half - 1, fbHeight, angle);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ------------------------------------------------------------
int main(int argc, char **argv)
{
    bool headless = false;

    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "--headless")
            headless = true;

    if (!glfwInit())
    {
        std::printf("glfwInit failed\n");
        return 2;
    }

    // Exactly the context ObjectiveFrame asks for.

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, headless ? GLFW_FALSE : GLFW_TRUE);

    // Wide enough for two square halves side by side.

    const int windowWidth = 1024;
    const int windowHeight = 512;

    GLFWwindow *window =
        glfwCreateWindow(windowWidth, windowHeight, "MeshBuffer -- fixed function | shader", nullptr, nullptr);

    if (window == nullptr)
    {
        std::printf("glfwCreateWindow failed\n");
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::printf("gladLoadGLLoader failed\n");
        glfwTerminate();
        return 2;
    }

    std::printf("GL %s\n", glGetString(GL_VERSION));

    // The checks read the framebuffer back, so they run in a fixed square
    // viewport regardless of how large the window is.

    runChecks(256, 256);

    std::printf("\n%d checks, %d failures\n", g_checks, g_failures);

    if (!headless)
        runViewer(window);

    glfwDestroyWindow(window);
    glfwTerminate();

    return (g_failures == 0) ? 0 : 1;
}
