// ------------------------------------------------------------
//
// profile_test -- acceptance harness for the modern OpenGL migration
//
// Instantiates the public ivf shape classes, renders each one in isolation and
// reports which of them draw without upsetting the driver. It is the gate every
// phase of the migration has to pass: convert a class, add it here, and it has
// to stay clean in every profile it claims to support.
//
// Usage:
//   profile_test [legacy|legacy-shader|mixed|core] [options]
//
//     legacy         OpenGL 3.2 compatibility context, no shader ever built.
//                    What the library did before any of this.
//     legacy-shader  The same, except the Blinn-Phong shader IS built and linked
//                    and RenderProfile::Legacy is then selected anyway. Must come
//                    out pixel-identical to legacy: an application that has not
//                    been ported must not be affected by a shader merely existing
//                    in the process.
//     mixed          Compatibility context plus the shader, the default. Classes
//                    with a modern path draw through it; the rest have it unbound
//                    around them and draw fixed-function.
//     core           OpenGL 3.3 core profile, forward compatible, plus the
//                    shader. Expected to fail until the migration is finished --
//                    the point of this harness is to say precisely where.
//
//   --headless    Run the checks, print the table, exit. No window is shown.
//   --list        List the registered cases and exit without a GL context.
//   --only NAME   Run just the case whose name matches NAME.
//   --notify      Report driver messages of Notification severity as well.
//   --pick        Sweep a grid of pick samples over a small scene and print the
//                 shape picked at each one as a character map. Diff the maps for
//                 two profiles to check that colour-id picking agrees with
//                 GL_SELECT. Implies --headless.
//   --shot DIR    Render each case alone from a fixed camera into
//                 DIR/<profile>_<case>.bmp. Run it for two profiles and diff
//                 the images to see which classes changed appearance. Implies
//                 --headless; DIR must already exist.
//
// Exit code is 0 when every case ran clean, 1 otherwise, 2 on a setup failure.
// In windowed mode the cases are laid out on a grid; drag to orbit, Esc quits.
//
// ------------------------------------------------------------

#include <ivf/Arrow.h>
#include <ivf/Axis.h>
#include <ivf/Brick.h>
#include <ivf/BufferSelection.h>
#include <ivf/Camera.h>
#include <ivf/Composite.h>
#include <ivf/Cone.h>
#include <ivf/Cube.h>
#include <ivf/Cursor.h>
#include <ivf/Cylinder.h>
#include <ivf/ExtrArrow.h>
#include <ivf/FaceSet.h>
#include <ivf/Extrusion.h>
#include <ivf/GLDebug.h>
#include <ivf/Grid.h>
#include <ivf/Index.h>
#include <ivf/LineSet.h>
#include <ivf/Mesh.h>
#include <ivf/LineStripSet.h>
#include <ivf/Light.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Node.h>
#include <ivf/PointSet.h>
#include <ivf/PolySet.h>
#include <ivf/QuadPlane.h>
#include <ivf/QuadSet.h>
#include <ivf/QuadStripSet.h>
#include <ivf/Ruler.h>
#include <ivf/SimpleLineSet.h>
#include <ivf/SelectionBox.h>
#include <ivf/SolidLine.h>
#include <ivf/Sphere.h>
#include <ivf/SweptExtrusion.h>
#include <ivf/SweptSolidLine.h>
#include <ivf/Texture.h>
#include <ivf/Transform.h>
#include <ivf/TriSet.h>
#include <ivf/TriStripSet.h>
#include <ivf/TubeExtrusion.h>
#include <ivf/VertexElements.h>
#include <ivf/VertexIndex.h>
#include <ivf/VertexList.h>
#include <ivf/WireBrick.h>
#include <ivf/rc.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

using namespace ivf;

// ============================================================
// Profiles
//
// Phase 0 defines a profile as a context configuration plus a decision about
// whether to activate the shader. Phase 1 introduces RenderContext::setProfile()
// and this enum becomes a thin wrapper over it.

enum class TestProfile {
    Legacy,
    LegacyWithShader,
    Mixed,
    Core
};

const char *profileName(TestProfile p)
{
    switch (p)
    {
    case TestProfile::Legacy:
        return "legacy";
    case TestProfile::LegacyWithShader:
        return "legacy-shader";
    case TestProfile::Mixed:
        return "mixed";
    default:
        return "core";
    }
}

// ============================================================
// Case registry

struct TestCase {
    std::string name;
    std::function<ShapePtr()> make;

    // Filled in by runChecks()
    ShapePtr shape;
    int glErrors = 0;
    int driverErrors = 0;
    int driverWarnings = 0;
    bool built = false;
};

std::vector<TestCase> g_cases;

void addCase(const std::string &name, std::function<ShapePtr()> make)
{
    TestCase c;
    c.name = name;
    c.make = make;
    g_cases.push_back(c);
}

// ------------------------------------------------------------
// Shared materials. Created once the context exists, because Material has no
// GL state of its own but the shapes that reference it are built alongside.

MaterialPtr g_material;

MaterialPtr defaultMaterial()
{
    if (g_material == nullptr)
    {
        g_material = Material::create();
        g_material->setAmbientColor(0.15f, 0.10f, 0.10f, 1.0f);
        g_material->setDiffuseColor(0.75f, 0.35f, 0.25f, 1.0f);
        g_material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
        g_material->setEmissionColor(0.0f, 0.0f, 0.0f, 1.0f);
        g_material->setShininess(40.0f);
    }

    return g_material;
}

// ------------------------------------------------------------
// A circular section and a bent spine, shared by the extrusion cases so they
// are compared on the same input.

void applyRoundSection(SweptExtrusion *e, int sides, double r)
{
    e->setSectionSize(sides + 1);

    for (int i = 0; i <= sides; i++)
    {
        double a = 2.0 * 3.14159265358979 * (double)i / (double)sides;
        double x = r * cos(a);
        double y = r * sin(a);
        e->setSectionCoord(i, x, y);
        e->setSectionNormal(i, x / r, y / r);
    }
}

void applyRoundSection(Extrusion *e, int sides, double r)
{
    e->setSectionSize(sides + 1);

    for (int i = 0; i <= sides; i++)
    {
        double a = 2.0 * 3.14159265358979 * (double)i / (double)sides;
        double x = r * cos(a);
        double y = r * sin(a);
        e->setSectionCoord(i, x, y);
        e->setSectionNormal(i, x / r, y / r);
    }
}

// Takes the handle by reference rather than as T*, so it works with both a
// CPointer and a raw pointer -- template deduction will not apply CPointer's
// conversion operator on its own.

template <typename T> void applyBentSpine(T &e)
{
    e->setSpineSize(5);
    e->setSpineCoord(0, -0.9, -0.5, 0.0);
    e->setSpineCoord(1, -0.5, 0.0, 0.0);
    e->setSpineCoord(2, 0.0, 0.3, 0.0);
    e->setSpineCoord(3, 0.5, 0.0, 0.0);
    e->setSpineCoord(4, 0.9, -0.5, 0.0);
}

// ------------------------------------------------------------
// The eight corners and six faces of a unit cube, reused by the indexed set
// cases so a failure is about the class and not about the data.

void addCubeCoords(GLPrimitive *p, double s)
{
    p->addCoord(-s, -s, s);
    p->addCoord(s, -s, s);
    p->addCoord(s, -s, -s);
    p->addCoord(-s, -s, -s);
    p->addCoord(-s, s, s);
    p->addCoord(s, s, s);
    p->addCoord(s, s, -s);
    p->addCoord(-s, s, -s);
}

IndexPtr cubeQuadIndex()
{
    auto idx = Index::create();
    idx->add(0, 1, 5, 4);
    idx->add(1, 2, 6, 5);
    idx->add(2, 3, 7, 6);
    idx->add(3, 0, 4, 7);
    idx->add(4, 5, 6, 7);
    idx->add(0, 3, 2, 1);
    return idx;
}

void addRainbowColors(GLPrimitive *p)
{
    p->addColor(1.0f, 0.2f, 0.2f);
    p->addColor(0.2f, 1.0f, 0.2f);
    p->addColor(0.2f, 0.2f, 1.0f);
    p->addColor(1.0f, 1.0f, 0.2f);
    p->addColor(1.0f, 0.2f, 1.0f);
    p->addColor(0.2f, 1.0f, 1.0f);
    p->addColor(1.0f, 1.0f, 1.0f);
    p->addColor(0.5f, 0.5f, 0.5f);
}

// ------------------------------------------------------------
void registerCases()
{
    // ---- Quadric-style solids ----

    addCase("Sphere", [] {
        auto s = Sphere::create();
        s->setRadius(0.7);
        s->setSlices(24);
        s->setStacks(16);
        s->setMaterial(defaultMaterial());
        return ShapePtr(s);
    });

    addCase("Cone", [] {
        auto c = Cone::create();
        c->setTopRadius(0.0);
        c->setBottomRadius(0.6);
        c->setHeight(1.2);
        c->setSlices(24);
        c->setStacks(8);
        c->setMaterial(defaultMaterial());
        return ShapePtr(c);
    });

    addCase("Cylinder", [] {
        auto c = Cylinder::create();
        c->setTopRadius(0.5);
        c->setBottomRadius(0.5);
        c->setHeight(1.2);
        c->setSlices(24);
        c->setMaterial(defaultMaterial());
        return ShapePtr(c);
    });

    // ---- GLPrimitive family ----

    addCase("TriSet", [] {
        auto t = TriSet::create();
        t->addCoord(-0.7, -0.5, 0.0);
        t->addCoord(0.7, -0.5, 0.0);
        t->addCoord(0.0, 0.7, 0.0);
        t->addCoord(0.0, -0.5, -0.7);

        auto idx = Index::create();
        idx->add(0, 1, 2);
        idx->add(1, 3, 2);
        idx->add(3, 0, 2);
        t->addCoordIndex(idx);

        t->setMaterial(defaultMaterial());
        return ShapePtr(t);
    });

    addCase("TriStripSet", [] {
        auto t = TriStripSet::create();
        for (int i = 0; i < 6; i++)
        {
            double x = -0.75 + 0.3 * i;
            t->addCoord(x, (i % 2 == 0) ? -0.4 : 0.4, 0.0);
        }

        auto idx = Index::create();
        idx->createLinear(6);
        t->addCoordIndex(idx);

        t->setMaterial(defaultMaterial());
        return ShapePtr(t);
    });

    addCase("QuadSet", [] {
        auto q = QuadSet::create();
        addCubeCoords(q, 0.5);
        q->addCoordIndex(cubeQuadIndex());
        q->setMaterial(defaultMaterial());
        return ShapePtr(q);
    });

    addCase("QuadSet+colors", [] {
        auto q = QuadSet::create();
        addCubeCoords(q, 0.5);

        auto idx = cubeQuadIndex();
        q->addCoordIndex(idx);

        addRainbowColors(q);
        auto colorIdx = Index::create();
        colorIdx->assignFrom(idx);
        q->addColorIndex(colorIdx);
        q->setUseColor(true);

        return ShapePtr(q);
    });

    addCase("QuadStripSet", [] {
        auto q = QuadStripSet::create();
        for (int i = 0; i < 6; i++)
        {
            double x = -0.75 + 0.3 * i;
            q->addCoord(x, -0.4, 0.0);
            q->addCoord(x, 0.4, 0.0);
        }

        auto idx = Index::create();
        idx->createLinear(12);
        q->addCoordIndex(idx);

        q->setMaterial(defaultMaterial());
        return ShapePtr(q);
    });

    addCase("PolySet", [] {
        auto p = PolySet::create();
        addCubeCoords(p, 0.5);
        p->addCoordIndex(cubeQuadIndex());
        p->setMaterial(defaultMaterial());
        return ShapePtr(p);
    });

    addCase("LineSet", [] {
        auto l = LineSet::create();
        addCubeCoords(l, 0.5);

        auto idx = Index::create();
        idx->createLinear(8);
        l->addCoordIndex(idx);

        addRainbowColors(l);
        auto colorIdx = Index::create();
        colorIdx->createLinear(8);
        l->addColorIndex(colorIdx);
        l->setUseColor(true);
        l->setLineWidth(2.0f);

        return ShapePtr(l);
    });

    addCase("LineStripSet", [] {
        auto l = LineStripSet::create();
        addCubeCoords(l, 0.5);

        auto idx = Index::create();
        idx->createLinear(8);
        l->addCoordIndex(idx);

        l->setLineWidth(2.0f);
        return ShapePtr(l);
    });

    addCase("PointSet", [] {
        auto p = PointSet::create();
        addCubeCoords(p, 0.5);

        auto idx = Index::create();
        idx->createLinear(8);
        p->addCoordIndex(idx);

        addRainbowColors(p);
        auto colorIdx = Index::create();
        colorIdx->createLinear(8);
        p->addColorIndex(colorIdx);
        p->setUseColor(true);
        p->setPointSize(6);

        return ShapePtr(p);
    });

    // ---- Boxes and planes ----

    addCase("Brick", [] {
        auto b = Brick::create();
        b->setSize(1.0, 0.8, 0.6);
        b->setMaterial(defaultMaterial());
        return ShapePtr(b);
    });

    addCase("Cube", [] {
        auto c = Cube::create();
        c->setSize(0.9);
        c->setMaterial(defaultMaterial());
        return ShapePtr(c);
    });

    addCase("WireBrick", [] {
        auto w = WireBrick::create();
        w->setSize(1.0, 0.8, 0.6);
        return ShapePtr(w);
    });

    addCase("SelectionBox", [] {
        auto s = SelectionBox::create();
        s->setSize(1.0, 0.8, 0.6);
        return ShapePtr(s);
    });

    addCase("QuadPlane", [] {
        auto q = QuadPlane::create();
        q->setSize(1.2, 1.2);
        q->setMaterial(defaultMaterial());
        return ShapePtr(q);
    });

    // ---- Extrusions ----

    addCase("Extrusion", [] {
        auto e = Extrusion::create();
        applyRoundSection(e, 12, 0.15);
        applyBentSpine(e);
        e->setMaterial(defaultMaterial());
        return ShapePtr(e);
    });

    addCase("TubeExtrusion", [] {
        auto t = TubeExtrusion::create();
        t->setSides(12);
        t->setRadius(0.15);
        applyBentSpine(t);
        t->setMaterial(defaultMaterial());
        return ShapePtr(t);
    });

    addCase("SolidLine", [] {
        auto n1 = Node::create();
        auto n2 = Node::create();
        n1->setPosition(-0.8, -0.4, 0.0);
        n2->setPosition(0.8, 0.4, 0.0);

        auto s = SolidLine::create();
        s->setSides(12);
        s->setRadius(0.12);
        s->setNodes(n1, n2);
        s->setMaterial(defaultMaterial());
        return ShapePtr(s);
    });

    addCase("SweptExtrusion", [] {
        auto e = SweptExtrusion::create();
        applyRoundSection(e, 12, 0.15);
        applyBentSpine(e);
        e->setMaterial(defaultMaterial());
        return ShapePtr(e);
    });

    addCase("SweptSolidLine", [] {
        auto n1 = Node::create();
        auto n2 = Node::create();
        n1->setPosition(-0.8, -0.4, 0.0);
        n2->setPosition(0.8, 0.4, 0.0);

        auto s = SweptSolidLine::create();
        s->setSides(12);
        s->setRadius(0.12);
        s->setNodes(n1, n2);
        s->setMaterial(defaultMaterial());
        return ShapePtr(s);
    });

    addCase("ExtrArrow", [] {
        auto a = ExtrArrow::create();
        a->setSize(1.2, 0.4);
        a->setRadius(0.16, 0.07);
        a->setDirection(0.0, 1.0, 0.0);
        a->setMaterial(defaultMaterial());
        return ShapePtr(a);
    });

    // ---- Composites and helpers ----

    addCase("Node", [] {
        auto n = Node::create();
        n->setSize(0.5);
        n->setMaterial(defaultMaterial());
        return ShapePtr(n);
    });

    addCase("Arrow", [] {
        auto a = Arrow::create();
        a->setSize(1.2, 0.4);
        a->setRadius(0.16, 0.07);
        return ShapePtr(a);
    });

    addCase("Axis", [] {
        auto a = Axis::create();
        a->setSize(0.9);
        return ShapePtr(a);
    });

    addCase("Cursor", [] {
        auto c = Cursor::create();
        c->setSize(1.4);
        return ShapePtr(c);
    });

    addCase("Grid", [] {
        auto g = Grid::create();
        g->setSize(1.6, 1.6);
        g->setGridSpacing(0.2);
        return ShapePtr(g);
    });

    addCase("Ruler", [] {
        RulerPtr r = new Ruler();
        r->setStartPoint(-0.9, 0.0, 0.0);
        r->setEndPoint(0.9, 0.0, 0.0);
        r->setTickStep(0.2);
        return ShapePtr(r);
    });

    // ---- Classes with no modern path yet (Phase 4 work in progress) ----

    addCase("FaceSet", [] {
        auto f = FaceSet::create();
        f->setCoordSize(8);

        const double c[8][3] = {{-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},  {0.5, -0.5, -0.5},
                                {-0.5, -0.5, -0.5}, {-0.5, 0.5, 0.5},  {0.5, 0.5, 0.5},
                                {0.5, 0.5, -0.5},   {-0.5, 0.5, -0.5}};

        for (long i = 0; i < 8; i++)
            f->setCoord(i, c[i][0], c[i][1], c[i][2]);

        f->setCoordIndexSize(6);
        f->setCoordIndex(0, 0, 1, 5, 4);
        f->setCoordIndex(1, 1, 2, 6, 5);
        f->setCoordIndex(2, 2, 3, 7, 6);
        f->setCoordIndex(3, 3, 0, 4, 7);
        f->setCoordIndex(4, 4, 5, 6, 7);
        f->setCoordIndex(5, 0, 3, 2, 1);

        f->setMaterial(defaultMaterial());
        return ShapePtr(f);
    });

    addCase("SimpleLineSet", [] {
        auto l = SimpleLineSet::create();
        l->setCoordSize(8);

        for (int i = 0; i < 8; i++)
        {
            double a = 2.0 * 3.14159265358979 * (double)i / 8.0;
            l->setCoord(i, 0.7 * cos(a), 0.7 * sin(a), 0.0);
        }

        l->setLineSize(16);

        for (int i = 0; i < 8; i++)
        {
            l->setLineIndex(i * 2, i);
            l->setLineIndex(i * 2 + 1, (i + 1) % 8);
        }

        return ShapePtr(l);
    });

    addCase("Mesh", [] {
        auto m = Mesh::create();
        m->setSize(4, 4);

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                double x = -0.75 + 0.5 * i;
                double z = -0.75 + 0.5 * j;
                m->setControlPoint(i, j, x, 0.3 * sin(3.0 * x) * cos(3.0 * z), z);
            }

        m->setMeshResolution(8, 8);
        m->setMaterial(defaultMaterial());
        return ShapePtr(m);
    });

    addCase("VertexElements", [] {
        auto v = VertexElements::create();

        auto verts = VertexList::create();
        verts->add(-0.7, -0.5, 0.0);
        verts->add(0.7, -0.5, 0.0);
        verts->add(0.0, 0.7, 0.0);
        verts->add(0.0, -0.5, -0.7);

        auto idx = VertexIndex::create();
        long tris[12] = {0, 1, 2, 1, 3, 2, 3, 0, 2, 0, 3, 1};
        idx->addArray(tris, 12);

        v->setVertices(verts);
        v->setIndices(idx);
        v->setPrimitive(VertexElements::PT_TRIANGLES);
        v->setMaterial(defaultMaterial());
        return ShapePtr(v);
    });

    // A checkerboard built in memory, so the case needs no image file. Exercises
    // uUseTexture, the texture environment mode and the texture matrix.

    addCase("TexturedPlane", [] {
        static unsigned char checker[16 * 16 * 4];

        for (int y = 0; y < 16; y++)
            for (int x = 0; x < 16; x++)
            {
                const bool on = (((x / 4) + (y / 4)) % 2) == 0;
                unsigned char *p = &checker[(y * 16 + x) * 4];
                p[0] = on ? 240 : 40;
                p[1] = on ? 200 : 40;
                p[2] = on ? 60 : 90;
                p[3] = 255;
            }

        auto tex = Texture::create();
        tex->setImageMap(checker);
        tex->setSize(16, 16);
        tex->setMode(GL_MODULATE);
        tex->setFilters(GL_NEAREST, GL_NEAREST);

        auto q = QuadPlane::create();
        q->setSize(1.4, 1.4);
        q->setMaterial(defaultMaterial());
        q->setTexture(tex);

        return ShapePtr(q);
    });

    addCase("Composite", [] {
        auto c = Composite::create();

        auto s = Sphere::create();
        s->setRadius(0.35);
        s->setPosition(-0.4, 0.0, 0.0);
        s->setMaterial(defaultMaterial());
        c->addChild(s);

        auto b = Cube::create();
        b->setSize(0.6);
        b->setPosition(0.4, 0.0, 0.0);
        b->setMaterial(defaultMaterial());
        c->addChild(b);

        return ShapePtr(c);
    });

    addCase("Transform", [] {
        auto t = Transform::create();
        t->setRotation(0.0, 30.0, 0.0);

        auto b = Cube::create();
        b->setSize(0.8);
        b->setMaterial(defaultMaterial());
        t->addChild(b);

        return ShapePtr(t);
    });
}

// ============================================================
// Harness

TestProfile g_profile = TestProfile::Mixed;
bool g_headless = false;
bool g_reportNotifications = false;
bool g_pickTest = false;
std::string g_only;
std::string g_shotDir;

CameraPtr g_camera;
LightPtr g_light;

int g_setupGlErrors = 0;
int g_setupDriverErrors = 0;
int g_setupDriverWarnings = 0;

// ------------------------------------------------------------
// Whether this run should build a shader at all. legacy-shader deliberately
// builds one and then asks for RenderProfile::Legacy anyway, which is the case
// an application like ObjectiveFrame depends on: a linked shader present in the
// process must not change what gets drawn.

bool shaderRequested()
{
    return g_profile != TestProfile::Legacy;
}

// Whether drawing should actually go through that shader.

bool shaderUsed()
{
    return (g_profile == TestProfile::Mixed) || (g_profile == TestProfile::Core);
}

// ------------------------------------------------------------
// Establishes the per-frame state every case is rendered against.
//
// Runs the camera and the light, which under a core profile is itself a source
// of errors -- they are reported separately as a "frame setup" row so that a
// Camera problem is not blamed on whichever shape happened to be drawn first.

void beginTestFrame()
{
    rcBeginFrame();

    if (shaderUsed())
        rcSetGlobalAmbient(0.15f, 0.15f, 0.15f, 1.0f);

    g_camera->render();
    g_light->render();
}

// ------------------------------------------------------------
// Renders one case with the driver watching, and records what it said.

void runCase(TestCase &c)
{
    if (!c.built)
    {
        c.shape = c.make();
        c.built = true;
    }

    // Frame setup first, then drain, so only what the shape does is counted.

    beginTestFrame();
    clearGLErrors();
    resetDebugMessageCount();

    c.shape->render();

    // The driver reports asynchronously unless debug output is synchronous.
    // glFinish() makes the attribution reliable either way.

    glFinish();

    c.glErrors = checkGLError(c.name.c_str());
    c.driverErrors = debugMessageCount(DebugSeverity::High);
    c.driverWarnings = debugMessageCount(DebugSeverity::Low) - c.driverErrors;
}

// ------------------------------------------------------------
void measureFrameSetup()
{
    clearGLErrors();
    resetDebugMessageCount();

    beginTestFrame();
    glFinish();

    g_setupGlErrors = checkGLError("frame setup (Camera, Light)");
    g_setupDriverErrors = debugMessageCount(DebugSeverity::High);
    g_setupDriverWarnings = debugMessageCount(DebugSeverity::Low) - g_setupDriverErrors;
}

// ------------------------------------------------------------
int runChecks()
{
    std::printf("profile: %s\n", profileName(g_profile));
    std::printf("GL %s\n", (const char *)glGetString(GL_VERSION));
    std::printf("GLSL %s\n", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

    bool debugAvailable = isDebugOutputEnabled();
    std::printf("driver debug output: %s\n\n", debugAvailable ? "on" : "unavailable");

    measureFrameSetup();

    int failures = 0;
    int warned = 0;
    int ran = 0;

    // Only errors fail a case. Drivers say a great deal at Low and Medium that
    // is advisory rather than wrong -- an unbound sampler the shader never reads
    // from, a program recompiled because some state changed -- and gating on
    // that would make the harness useless on the drivers that report the most.

    std::printf("%-18s %6s %6s %6s   %s\n", "case", "gl", "errors", "warns", "result");
    std::printf("---------------------------------------------------------\n");

    std::printf("%-18s %6d %6d %6d   %s\n", "(frame setup)", g_setupGlErrors, g_setupDriverErrors,
                g_setupDriverWarnings, ((g_setupGlErrors + g_setupDriverErrors) == 0) ? "ok" : "FAIL");

    if ((g_setupGlErrors + g_setupDriverErrors) != 0)
        failures++;

    for (auto &c : g_cases)
    {
        if (!g_only.empty() && (c.name != g_only))
            continue;

        runCase(c);
        ran++;

        int errors = c.glErrors + c.driverErrors;

        if (errors != 0)
            failures++;
        else if (c.driverWarnings != 0)
            warned++;

        std::printf("%-18s %6d %6d %6d   %s\n", c.name.c_str(), c.glErrors, c.driverErrors,
                    c.driverWarnings, (errors == 0) ? "ok" : "FAIL");
    }

    std::printf("---------------------------------------------------------\n");
    std::printf("%d cases, %d failing, %d clean but warned\n", ran, failures, warned);

    if (!debugAvailable)
        std::printf("\nnote: the driver offers no debug output, so only glGetError()\n"
                    "      results are reported. Coverage is weaker than it looks.\n");

    return failures;
}

// ============================================================
// Screenshots
//
// "No GL errors" is not the same as "draws the same picture". The legacy and
// shader paths compute lighting differently by construction, so the only way to
// find out which classes actually changed appearance -- and by how much -- is to
// render each one from a fixed camera in both profiles and compare pixels.
//
// BMP rather than PNG keeps this free of an image-library dependency, and
// glReadPixels hands back rows bottom-up, which is the order BMP stores them.

const int SHOT_SIZE = 256;

void writeLE32(unsigned char *p, int value)
{
    p[0] = (unsigned char)(value & 0xff);
    p[1] = (unsigned char)((value >> 8) & 0xff);
    p[2] = (unsigned char)((value >> 16) & 0xff);
    p[3] = (unsigned char)((value >> 24) & 0xff);
}

bool writeBMP(const std::string &path, const unsigned char *rgb, int width, int height)
{
    int rowBytes = width * 3;
    int padding = (4 - (rowBytes % 4)) % 4;
    int imageBytes = (rowBytes + padding) * height;

    FILE *f = std::fopen(path.c_str(), "wb");

    if (f == nullptr)
        return false;

    unsigned char header[54] = {};
    header[0] = 'B';
    header[1] = 'M';
    writeLE32(header + 2, 54 + imageBytes);
    writeLE32(header + 10, 54); // pixel data offset
    writeLE32(header + 14, 40); // DIB header size
    writeLE32(header + 18, width);
    writeLE32(header + 22, height);
    header[26] = 1;  // planes
    header[28] = 24; // bits per pixel
    writeLE32(header + 34, imageBytes);

    std::fwrite(header, 1, 54, f);

    std::vector<unsigned char> row((size_t)(rowBytes + padding), 0);

    for (int y = 0; y < height; y++)
    {
        const unsigned char *src = rgb + (size_t)y * rowBytes;

        for (int x = 0; x < width; x++)
        {
            row[x * 3 + 0] = src[x * 3 + 2]; // BMP stores BGR
            row[x * 3 + 1] = src[x * 3 + 1];
            row[x * 3 + 2] = src[x * 3 + 0];
        }

        std::fwrite(row.data(), 1, row.size(), f);
    }

    std::fclose(f);
    return true;
}

// ------------------------------------------------------------
// Renders every case alone from a fixed camera and writes one image each.
// The directory must already exist.

int runShots(const std::string &dir)
{
    std::vector<unsigned char> pixels((size_t)SHOT_SIZE * SHOT_SIZE * 3);
    int written = 0;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);

    for (auto &c : g_cases)
    {
        if (!g_only.empty() && (c.name != g_only))
            continue;

        if (!c.built)
        {
            c.shape = c.make();
            c.built = true;
        }

        g_camera->setPerspective(45.0, 0.1, 100.0);
        g_camera->setViewPort(SHOT_SIZE, SHOT_SIZE);
        g_camera->initialize();
        g_camera->setPosition(2.2, 1.8, 3.0);

        glViewport(0, 0, SHOT_SIZE, SHOT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        beginTestFrame();

        c.shape->setPosition(0.0, 0.0, 0.0);
        c.shape->render();

        glFinish();
        glReadPixels(0, 0, SHOT_SIZE, SHOT_SIZE, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        std::string path = dir + "/" + profileName(g_profile) + "_" + c.name + ".bmp";

        if (writeBMP(path, pixels.data(), SHOT_SIZE, SHOT_SIZE))
            written++;
        else
            std::printf("could not write %s\n", path.c_str());
    }

    std::printf("wrote %d images to %s\n", written, dir.c_str());
    return written;
}

// ============================================================
// Picking
//
// GL_SELECT is gone in core, so picking is done by drawing object ids into an
// offscreen buffer. The two implementations have to agree about which shape is
// under a given pixel, and the only convincing way to show that is to ask them
// both the same questions.
//
// This sweeps a grid of sample points over the viewport and prints the index of
// the shape picked at each one, as a small character map. Run it for two
// profiles and diff the maps: identical output means the replacement picks the
// same shapes as GL_SELECT did, pixel region for pixel region.

int runPickTest()
{
    auto scene = Composite::create();
    std::vector<Shape *> pickable;

    // Three spheres and a cube, spread across the view and overlapping in depth
    // so the "nearest wins" rule is actually exercised.

    struct Placement {
        double x, y, z, r;
    };

    const Placement places[] = {{-1.3, 0.6, 0.0, 0.55},
                                {1.3, 0.6, 0.0, 0.55},
                                {-1.3, -0.8, 0.8, 0.55},
                                {1.2, -0.8, -0.6, 0.55}};

    for (int i = 0; i < 4; i++)
    {
        auto s = Sphere::create();
        s->setRadius(places[i].r);
        s->setSlices(24);
        s->setStacks(16);
        s->setPosition(places[i].x, places[i].y, places[i].z);
        s->setMaterial(defaultMaterial());
        s->setUseName(true);

        scene->addChild(s);
        pickable.push_back((Shape *)s);
    }

    // A composite in the middle, named as a whole: picking any of its children
    // must report the composite, which is the inherited-name case.

    auto group = Composite::create();
    group->setUseName(true);
    group->setPosition(0.0, -0.1, 0.0);

    for (int i = 0; i < 2; i++)
    {
        auto c = Cube::create();
        c->setSize(0.5);
        c->setPosition(-0.25 + 0.5 * i, 0.0, 0.0);
        c->setMaterial(defaultMaterial());
        c->setUseName(false);
        group->addChild(c);
    }

    scene->addChild(group);
    pickable.push_back((Shape *)group);

    BufferSelectionPtr selection = BufferSelection::create();
    selection->setView((View *)g_camera);
    selection->setComposite(group ? (Composite *)scene : nullptr);
    selection->update();

    const int width = 640;
    const int height = 480;

    glViewport(0, 0, width, height);
    g_camera->setPerspective(45.0, 0.1, 100.0);
    g_camera->setViewPort(width, height);
    g_camera->initialize();
    g_camera->setPosition(0.0, 0.0, 6.0);
    g_camera->setTarget(0.0, 0.0, 0.0);

    // Draw the scene once normally first. Picking is supposed to work in the
    // middle of ordinary rendering, and doing it this way would catch a pick
    // pass that failed to put the framebuffer or shader back.

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    beginTestFrame();
    scene->render();
    glFinish();

    // Read back what was actually drawn. That, not the other implementation, is
    // the ground truth a pick has to agree with: a sample where something is
    // visible must pick something, and a sample showing background must not.

    std::vector<unsigned char> rendered((size_t)width * height * 3, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rendered.data());

    auto litAt = [&](int px, int py) {
        const int ry = height - 1 - py;

        if ((px < 0) || (px >= width) || (ry < 0) || (ry >= height))
            return false;

        const size_t o = ((size_t)ry * width + px) * 3;
        return (rendered[o] != 0) || (rendered[o + 1] != 0) || (rendered[o + 2] != 0);
    };

    std::printf("pick map (%s), . = nothing, digit = shape index\n\n", profileName(g_profile));

    const int cols = 40;
    const int rows = 20;
    int hitSamples = 0;
    int litButNotPicked = 0;
    int pickedButNotLit = 0;

    for (int r = 0; r < rows; r++)
    {
        std::string line;

        for (int c = 0; c < cols; c++)
        {
            const int px = (int)((c + 0.5) * width / cols);
            const int py = (int)((r + 0.5) * height / rows);

            selection->pick(px, py);
            Shape *picked = selection->getSelectedShape();

            char ch = '.';

            for (size_t i = 0; i < pickable.size(); i++)
                if (pickable[i] == picked)
                {
                    ch = (char)('0' + (int)i);
                    hitSamples++;
                    break;
                }

            const bool lit = litAt(px, py);

            if (lit && (ch == '.'))
                litButNotPicked++;
            if (!lit && (ch != '.'))
                pickedButNotLit++;

            line += ch;
        }

        std::printf("  %s\n", line.c_str());
    }

    std::printf("\n%d of %d samples hit a shape\n", hitSamples, cols * rows);
    std::printf("visible but not picked: %d    picked but not visible: %d\n",
                litButNotPicked, pickedButNotLit);

    // A map that is entirely empty means the pick never worked, which would
    // otherwise read as "both profiles agree".

    if (hitSamples == 0)
    {
        std::printf("FAIL: nothing was picked anywhere\n");
        return 1;
    }

    const int errors = checkGLError("pick test");

    if (errors != 0)
        std::printf("FAIL: %d GL errors during picking\n", errors);

    return errors;
}

// ============================================================
// Viewer

double g_angleH = 30.0;
double g_angleV = 20.0;
bool g_dragging = false;
double g_lastX = 0.0;
double g_lastY = 0.0;

void placeCamera(double distance)
{
    double radH = glm::radians(g_angleH);
    double radV = glm::radians(g_angleV);

    g_camera->setPosition(distance * cos(radV) * sin(radH), distance * sin(radV),
                          distance * cos(radV) * cos(radH));
}

// ------------------------------------------------------------
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    (void)mods;

    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    g_dragging = (action == GLFW_PRESS);
    glfwGetCursorPos(window, &g_lastX, &g_lastY);
}

// ------------------------------------------------------------
void cursorPosCallback(GLFWwindow *window, double x, double y)
{
    (void)window;

    if (!g_dragging)
        return;

    g_angleH += (x - g_lastX) * 0.4;
    g_angleV -= (y - g_lastY) * 0.4;

    if (g_angleV > 89.0)
        g_angleV = 89.0;
    if (g_angleV < -89.0)
        g_angleV = -89.0;

    g_lastX = x;
    g_lastY = y;
}

// ------------------------------------------------------------
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// ------------------------------------------------------------
// Lays the cases out on a grid so a visual regression is as obvious as an
// error-count one. A class that reports clean but draws nothing is still broken.

void runViewer(GLFWwindow *window)
{
    int count = 0;
    for (auto &c : g_cases)
        if (g_only.empty() || (c.name == g_only))
            count++;

    if (count == 0)
        return;

    int columns = (int)ceil(sqrt((double)count));
    int rows = (count + columns - 1) / columns;

    const double spacing = 2.2;
    double extent = spacing * (double)((columns > rows) ? columns : rows);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.10f, 0.11f, 0.14f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);

        if (height < 1)
            height = 1;

        glViewport(0, 0, width, height);

        g_camera->setPerspective(45.0, 0.1, 500.0);
        g_camera->setViewPort(width, height);
        g_camera->initialize();

        placeCamera(extent * 1.1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        beginTestFrame();

        int i = 0;

        for (auto &c : g_cases)
        {
            if (!g_only.empty() && (c.name != g_only))
                continue;

            if (!c.built)
            {
                c.shape = c.make();
                c.built = true;
            }

            int col = i % columns;
            int row = i / columns;

            c.shape->setPosition(((double)col - (columns - 1) / 2.0) * spacing, 0.0,
                                 ((double)row - (rows - 1) / 2.0) * spacing);
            c.shape->render();

            i++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ============================================================
int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "legacy")
            g_profile = TestProfile::Legacy;
        else if (arg == "legacy-shader")
            g_profile = TestProfile::LegacyWithShader;
        else if (arg == "mixed")
            g_profile = TestProfile::Mixed;
        else if (arg == "core")
            g_profile = TestProfile::Core;
        else if (arg == "--headless")
            g_headless = true;
        else if (arg == "--notify")
            g_reportNotifications = true;
        else if (arg == "--pick")
        {
            g_pickTest = true;
            g_headless = true;
        }
        else if ((arg == "--only") && (i + 1 < argc))
            g_only = argv[++i];
        else if ((arg == "--shot") && (i + 1 < argc))
        {
            g_shotDir = argv[++i];
            g_headless = true;
        }
        else if (arg == "--list")
        {
            registerCases();
            for (auto &c : g_cases)
                std::printf("%s\n", c.name.c_str());
            return 0;
        }
        else
        {
            std::printf("unrecognised argument: %s\n", arg.c_str());
            std::printf("usage: profile_test [legacy|legacy-shader|mixed|core] "
                        "[--headless] [--list] [--only NAME] [--notify]\n");
            return 2;
        }
    }

    registerCases();

    if (!g_only.empty())
    {
        bool found = false;
        for (auto &c : g_cases)
            if (c.name == g_only)
                found = true;

        if (!found)
        {
            std::printf("no such case: %s (use --list)\n", g_only.c_str());
            return 2;
        }
    }

    if (!glfwInit())
    {
        std::printf("glfwInit failed\n");
        return 2;
    }

    if (g_profile == TestProfile::Core)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    }
    else
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }

    // Ask for a debug context. Drivers are entitled to ignore the hint, which is
    // why enableDebugOutput() reports whether it actually got anything.

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, g_headless ? GLFW_FALSE : GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(1000, 750, "ivf profile_test", nullptr, nullptr);

    if (window == nullptr)
    {
        std::printf("glfwCreateWindow failed for the %s profile\n", profileName(g_profile));
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::printf("gladLoadGLLoader failed\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 2;
    }

    setDebugReportNotifications(g_reportNotifications);
    enableDebugOutput(true);

    // Select the profile before touching anything else. Camera::initialize() and
    // Lighting::enable() emit fixed-function calls on the spot, so setting the
    // profile after them would let a core run make exactly the calls the profile
    // exists to suppress.

    switch (g_profile)
    {
    case TestProfile::Legacy:
    case TestProfile::LegacyWithShader:
        rcSetProfile(RenderProfile::Legacy);
        break;
    case TestProfile::Mixed:
        rcSetProfile(RenderProfile::Mixed);
        break;
    default:
        rcSetProfile(RenderProfile::Core);
        break;
    }

    if (shaderRequested())
    {
        rcUseBlinnPhong();

        if (rcShader() == nullptr || !rcShader()->isLinked())
        {
            std::printf("the Blinn-Phong shader failed to link -- see the messages above\n");
            glfwDestroyWindow(window);
            glfwTerminate();
            return 2;
        }
    }

    // ---- Camera and light, shared by checks and viewer ----

    g_camera = Camera::create();
    g_camera->setTarget(0.0, 0.0, 0.0);
    g_camera->setPerspective(45.0, 0.1, 500.0);
    g_camera->setViewPort(1000, 750);
    g_camera->initialize();
    placeCamera(6.0);

    auto lighting = Lighting::getInstance();
    lighting->enable();

    g_light = lighting->getLight(0);
    g_light->setLightPosition(4.0f, 6.0f, 4.0f, 1.0f);
    g_light->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
    g_light->setDiffuseColor(1.0f, 1.0f, 0.95f, 1.0f);
    g_light->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
    g_light->enable();

    // Tell the library which pipeline this run is exercising, then give it a
    // shader if the profile wants one. Legacy deliberately gets no shader at
    // all, so this run reproduces what the library did before the modern path.

    // Nothing further for legacy-shader: the shader above was built and linked
    // while RenderProfile::Legacy was already selected, and selecting it has to
    // be enough on its own. If this had to unbind anything by hand, the switch
    // would not be doing its job.

    glEnable(GL_DEPTH_TEST);

    if (g_pickTest)
    {
        const int pickFailures = runPickTest();

        g_cases.clear();
        g_material = nullptr;
        g_camera = nullptr;
        g_light = nullptr;

        disableDebugOutput();
        glfwDestroyWindow(window);
        glfwTerminate();
        return (pickFailures == 0) ? 0 : 1;
    }

    int failures = runChecks();

    if (!g_shotDir.empty())
        runShots(g_shotDir);

    if (!g_headless)
        runViewer(window);

    disableDebugOutput();

    // Drop everything holding GL objects before the context goes away. Leaving
    // it to static destruction would delete buffers against no current context,
    // and would tear the scene down in whatever order the linker happens to pick
    // relative to the library's own singletons.

    g_cases.clear();
    g_material = nullptr;
    g_camera = nullptr;
    g_light = nullptr;

    glfwDestroyWindow(window);
    glfwTerminate();

    return (failures == 0) ? 0 : 1;
}
