// ------------------------------------------------------------
//
// sphere_test -- ivf::Sphere against the gluSphere it replaced
//
// Sphere used to carry two tessellations: a hand written vertex buffer for
// the shader pipeline and a gluSphere for the fixed-function one. It now has
// one, drawn from a vertex buffer through whichever pipeline is active, and
// needs no display list to stay cheap.
//
// This program renders the class next to a gluSphere built the way the old
// fixed-function path built it, and compares the two. It renders under the
// conditions the application actually uses -- two sided lighting, a light off
// to one side, and shapes drawn in sequence so one can contaminate another --
// because those are the conditions that have historically hidden bugs here:
// a head-on light hides a normal pointing the wrong way, single sided
// lighting hides reversed winding, and shapes drawn in isolation hide
// material state leaking between them.
//
//   space  toggle between the class and the gluSphere reference on the right
//   r      toggle rotation
//   esc    quit
//
// Pass --headless to skip the viewer. The exit code is non-zero if any check
// failed.
//
// ------------------------------------------------------------

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <ivf/Material.h>
#include <ivf/Sphere.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace ivf;

static int g_failures = 0;
static int g_checks = 0;

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

// ------------------------------------------------------------
// Rendering and comparison
// ------------------------------------------------------------

const int kSize = 320;

typedef std::vector<unsigned char> Mask;

static void setupCamera(double distance)
{
    glViewport(0, 0, kSize, kSize);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -distance);

    // Oblique light: a head-on light is the worst case for spotting a normal
    // that points the wrong way, since the surface it wrongly lights is the
    // one facing away from the camera.

    GLfloat lightPos[4] = {-0.6f, 0.7f, 0.4f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat lightAmbient[4] = {0.05f, 0.05f, 0.05f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);

    // Two sided lighting, as the application sets it. This makes the
    // comparison sensitive to winding: a back facing polygon is lit with the
    // negated normal.

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glRotated(20.0, 1.0, 0.0, 0.0);
    glRotated(35.0, 0.0, 1.0, 0.0);
}

// The fixed-function path Sphere used to take, kept here as the reference to
// compare against rather than in the library.

static void drawReferenceSphere(double radius, int slices, int stacks)
{
    GLUquadricObj *qobj = gluNewQuadric();

    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricTexture(qobj, GL_TRUE);
    gluQuadricOrientation(qobj, GLU_OUTSIDE);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    glPushMatrix();
    glRotated(90, 1.0, 0.0, 0.0);
    gluSphere(qobj, radius, slices, stacks);
    glPopMatrix();

    gluDeleteQuadric(qobj);
}

static void beginFrame()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static Mask readMask()
{
    std::vector<unsigned char> pixels(std::size_t(kSize) * kSize * 4);
    glReadPixels(0, 0, kSize, kSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    Mask mask(std::size_t(kSize) * kSize, 0);

    for (std::size_t i = 0; i < mask.size(); i++)
        mask[i] = ((pixels[i * 4] > 6) || (pixels[i * 4 + 1] > 6) || (pixels[i * 4 + 2] > 6)) ? 1 : 0;

    return mask;
}

static void readMeanColor(double &r, double &g, double &b)
{
    std::vector<unsigned char> pixels(std::size_t(kSize) * kSize * 4);
    glReadPixels(0, 0, kSize, kSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    double sr = 0.0, sg = 0.0, sb = 0.0;
    int n = 0;

    for (std::size_t i = 0; i < std::size_t(kSize) * kSize; i++)
    {
        unsigned char pr = pixels[i * 4];
        unsigned char pg = pixels[i * 4 + 1];
        unsigned char pb = pixels[i * 4 + 2];

        if ((pr > 6) || (pg > 6) || (pb > 6))
        {
            sr += pr;
            sg += pg;
            sb += pb;
            n++;
        }
    }

    r = (n > 0) ? sr / n : 0.0;
    g = (n > 0) ? sg / n : 0.0;
    b = (n > 0) ? sb / n : 0.0;
}

static int maskArea(const Mask &m)
{
    int n = 0;

    for (std::size_t i = 0; i < m.size(); i++)
        n += m[i];

    return n;
}

static double maskIoU(const Mask &a, const Mask &b)
{
    int inter = 0;
    int uni = 0;

    for (std::size_t i = 0; i < a.size(); i++)
    {
        if (a[i] && b[i])
            inter++;
        if (a[i] || b[i])
            uni++;
    }

    return (uni == 0) ? 0.0 : double(inter) / double(uni);
}

static void applyReferenceMaterial()
{
    GLfloat ambient[4] = {0.2f, 0.3f, 0.4f, 1.0f};
    GLfloat diffuse[4] = {0.3f, 0.6f, 0.9f, 1.0f};
    GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

static MaterialPtr makeMaterial()
{
    MaterialPtr material = Material::create();
    material->setAmbientColor(0.2f, 0.3f, 0.4f, 1.0f);
    material->setDiffuseColor(0.3f, 0.6f, 0.9f, 1.0f);
    material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
    material->setShininess(0.0f);

    return material;
}

// ------------------------------------------------------------
static void runChecks()
{
    MaterialPtr material = makeMaterial();

    std::printf("Sphere vs gluSphere\n");

    struct Case {
        const char *name;
        double radius;
        int slices;
        int stacks;
    };

    Case cases[] = {{"default tessellation", 1.0, 16, 16},
                    {"coarse tessellation", 1.0, 6, 5},
                    {"fine tessellation", 1.0, 48, 40},
                    {"small radius", 0.15, 16, 16}};

    for (const auto &c : cases)
    {
        SpherePtr sphere = Sphere::create();
        sphere->setMaterial(material);
        sphere->setUseName(false);
        sphere->setUseSelectShape(false);
        sphere->setRadius(c.radius);
        sphere->setSlices(c.slices);
        sphere->setStacks(c.stacks);

        double distance = 4.0 * ((c.radius < 0.5) ? 0.5 : 1.0);

        setupCamera(distance);
        beginFrame();
        sphere->render();
        Mask a = readMask();
        double sr, sg, sb;
        readMeanColor(sr, sg, sb);

        setupCamera(distance);
        beginFrame();
        applyReferenceMaterial();
        drawReferenceSphere(c.radius, c.slices, c.stacks);
        Mask b = readMask();
        double rr, rg, rb;
        readMeanColor(rr, rg, rb);

        double iou = maskIoU(a, b);

        std::printf("  %-24s IoU %.4f   shading glu %.1f  ivf %.1f\n", c.name, iou, (rr + rg + rb) / 3.0,
                    (sr + sg + sb) / 3.0);

        check(maskArea(a) > 400, std::string(c.name) + ": the sphere drew something");
        check(iou > 0.98, std::string(c.name) + ": silhouette matches gluSphere");

        // A reversed winding lights the surface with the negated normal under
        // two sided lighting, which shows up here and nowhere else.

        check(std::fabs((rr + rg + rb) - (sr + sg + sb)) / 3.0 < 6.0,
              std::string(c.name) + ": shades like gluSphere");
    }

    // ------------------------------------------------------------
    // No display list needed, and none used
    // ------------------------------------------------------------

    std::printf("Behaviour\n");

    SpherePtr sphere = Sphere::create();
    sphere->setMaterial(material);
    sphere->setUseName(false);
    sphere->setUseSelectShape(false);
    sphere->setRadius(1.0);

    // Node turns display lists on for its sphere. That has to remain harmless
    // rather than freezing the geometry, since the buffer is now the cache.

    sphere->setUselist(true);

    setupCamera(4.0);
    beginFrame();
    sphere->render();
    int areaBefore = maskArea(readMask());

    sphere->setRadius(2.0);

    setupCamera(4.0);
    beginFrame();
    sphere->render();
    int areaAfter = maskArea(readMask());

    check(areaAfter > areaBefore * 2, "geometry still updates with display lists enabled");

    sphere->setRadius(1.0);

    // Drawing repeatedly must not accumulate state or leak GL errors.

    for (int i = 0; i < 8; i++)
    {
        beginFrame();
        sphere->render();
    }

    checkNoGLError("repeated rendering produced no GL error");

    // A sphere drawn after something that engaged GL_COLOR_MATERIAL must still
    // pick up its own material. Material caches what it last uploaded, and a
    // colour pass rewrites the material behind that cache.

    setupCamera(4.0);
    beginFrame();
    sphere->render();
    double br, bg, bb;
    readMeanColor(br, bg, bb);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.1f, 0.0f);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    Material::invalidateStateCache();

    setupCamera(4.0);
    beginFrame();
    sphere->render();
    double ar, ag, ab;
    readMeanColor(ar, ag, ab);

    std::printf("  %-24s before (%.1f %.1f %.1f)  after (%.1f %.1f %.1f)\n", "material after colour pass", br, bg, bb,
                ar, ag, ab);

    check(std::fabs(br - ar) < 4.0 && std::fabs(bg - ag) < 4.0 && std::fabs(bb - ab) < 4.0,
          "material is reapplied after an outside colour pass");

    // Degenerate tessellations must not crash or emit errors.

    SpherePtr degenerate = Sphere::create();
    degenerate->setUseName(false);
    degenerate->setUseSelectShape(false);
    degenerate->setSlices(0);
    degenerate->setStacks(0);

    beginFrame();
    degenerate->render();
    checkNoGLError("a degenerate tessellation renders without error");

    SpherePtr zero = Sphere::create();
    zero->setUseName(false);
    zero->setUseSelectShape(false);
    zero->setRadius(0.0);

    beginFrame();
    zero->render();
    checkNoGLError("a zero radius sphere renders without error");
}

// ------------------------------------------------------------
// Viewer
// ------------------------------------------------------------

static bool g_showReference = false;
static bool g_rotating = true;
static int g_viewSlices = 24;
static int g_viewStacks = 20;

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_SPACE)
        g_showReference = !g_showReference;
    else if (key == GLFW_KEY_R)
        g_rotating = !g_rotating;
}

static void runViewer(GLFWwindow *window)
{
    std::printf("\nViewer: gluSphere on the left, ivf::Sphere on the right.\n");
    std::printf("  space  draw the gluSphere reference in both halves\n");
    std::printf("  r      toggle rotation\n");
    std::printf("  esc    quit\n");

    glfwSetKeyCallback(window, keyCallback);

    MaterialPtr material = makeMaterial();

    SpherePtr sphere = Sphere::create();
    sphere->setMaterial(material);
    sphere->setUseName(false);
    sphere->setUseSelectShape(false);
    sphere->setSlices(g_viewSlices);
    sphere->setStacks(g_viewStacks);

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
            angle += 25.0f * dt;

        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        if ((fbWidth > 0) && (fbHeight > 0))
        {
            glViewport(0, 0, fbWidth, fbHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int half = fbWidth / 2;

            for (int side = 0; side < 2; side++)
            {
                int x = (side == 0) ? 0 : half + 1;
                int w = (side == 0) ? (half - 1) : (fbWidth - half - 1);

                if (w <= 0)
                    continue;

                glViewport(x, 0, w, fbHeight);

                float aspect = float(w) / float(fbHeight);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glFrustum(-0.5 * aspect, 0.5 * aspect, -0.5, 0.5, 1.0, 200.0);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslated(0.0, 0.0, -4.0);

                GLfloat lightPos[4] = {-0.6f, 0.7f, 0.4f, 0.0f};
                glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
                glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);

                glRotated(20.0, 1.0, 0.0, 0.0);
                glRotated(angle, 0.0, 1.0, 0.0);

                if ((side == 0) || g_showReference)
                {
                    applyReferenceMaterial();
                    drawReferenceSphere(1.0, g_viewSlices, g_viewStacks);
                }
                else
                    sphere->render();
            }
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
    {
        std::string arg = argv[i];

        if (arg == "--headless")
            headless = true;
        else if ((arg == "--slices") && (i + 1 < argc))
            g_viewSlices = std::atoi(argv[++i]);
        else if ((arg == "--stacks") && (i + 1 < argc))
            g_viewStacks = std::atoi(argv[++i]);
    }

    if (!glfwInit())
    {
        std::printf("glfwInit failed\n");
        return 2;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, headless ? GLFW_FALSE : GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(1024, 512, "Sphere -- gluSphere | ivf::Sphere", nullptr, nullptr);

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

    std::printf("GL %s\n\n", glGetString(GL_VERSION));

    runChecks();

    std::printf("\n%d checks, %d failures\n", g_checks, g_failures);

    if (!headless)
        runViewer(window);

    glfwDestroyWindow(window);
    glfwTerminate();

    return (g_failures == 0) ? 0 : 1;
}
