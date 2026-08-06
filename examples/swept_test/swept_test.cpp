// ------------------------------------------------------------
//
// swept_test -- SweptExtrusion / SweptSolidLine against the gle originals
//
// SweptExtrusion and SweptSolidLine reimplement Extrusion and SolidLine
// without the gle library, generating the swept mesh themselves and drawing
// it from a vertex buffer. They are meant to be drop-in replacements, so the
// test that matters is whether they put the same geometry on screen.
//
// This program builds matched pairs -- the same section, the same spine, the
// same options on the gle class and on the new one -- renders each into the
// same viewport with the same camera, and compares the resulting silhouettes.
// The configurations are the ones a structural analysis application actually
// uses: solid lines between nodes, conical end caps with offsets, a twisted
// section extrusion, and per-vertex result colours along a line.
//
// The comparison runs on the fixed-function pipeline, because that is the one
// gle can draw through at all.
//
//   space  cycle through the test cases
//   g      toggle between the gle class and the swept class in both halves
//   r      toggle rotation
//   esc    quit
//
// Pass --headless to skip the viewer. The exit code is non-zero if any check
// failed.
//
// ------------------------------------------------------------

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <ivf/Extrusion.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Node.h>
#include <ivf/SolidLine.h>
#include <ivf/SweptExtrusion.h>
#include <ivf/SweptSolidLine.h>

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
// Silhouette capture and comparison
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

    // Light from off to one side rather than straight down the view axis. A
    // head-on light is the worst case for spotting a normal that points the
    // wrong way, because the surface it wrongly lights is the one facing away
    // from the camera. An oblique light puts the error in plain sight.

    GLfloat lightPos[4] = {-0.6f, 0.7f, 0.4f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat lightAmbient[4] = {0.05f, 0.05f, 0.05f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);

    // Two sided lighting, which is what an application showing this kind of
    // model turns on. It makes the comparison sensitive to triangle winding:
    // GL lights a back facing polygon with the negated normal, so geometry
    // wound the wrong way comes out lit from the wrong side even though its
    // vertex normals are correct.

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glRotated(20.0, 1.0, 0.0, 0.0);
    glRotated(35.0, 0.0, 1.0, 0.0);
}

// Render one shape and return a per-pixel "covered" mask.

static Mask captureMask(Shape *shape, double distance)
{
    setupCamera(distance);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shape->render();

    std::vector<unsigned char> pixels(std::size_t(kSize) * kSize * 4);
    glReadPixels(0, 0, kSize, kSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    Mask mask(std::size_t(kSize) * kSize, 0);

    for (std::size_t i = 0; i < mask.size(); i++)
    {
        unsigned char r = pixels[i * 4];
        unsigned char g = pixels[i * 4 + 1];
        unsigned char b = pixels[i * 4 + 2];

        mask[i] = ((r > 6) || (g > 6) || (b > 6)) ? 1 : 0;
    }

    return mask;
}

static int maskArea(const Mask &m)
{
    int n = 0;

    for (std::size_t i = 0; i < m.size(); i++)
        n += m[i];

    return n;
}

// Intersection over union of two silhouettes. 1.0 means they cover exactly
// the same pixels.

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

    if (uni == 0)
        return 0.0;

    return double(inter) / double(uni);
}

// Mean colour over the covered pixels, for comparing the result colouring.

static void meanColor(Shape *shape, double distance, double &r, double &g, double &b)
{
    setupCamera(distance);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shape->render();

    std::vector<unsigned char> pixels(std::size_t(kSize) * kSize * 4);
    glReadPixels(0, 0, kSize, kSize, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    double sr = 0.0;
    double sg = 0.0;
    double sb = 0.0;
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

// ------------------------------------------------------------
// Matched pair construction
// ------------------------------------------------------------

// A cross section shaped like an I-beam, given the way an application builds
// one from a beam profile: as a closed ring whose first point is repeated at
// the end, with normals set for every point but the last.

static void buildISection(std::vector<double> &xs, std::vector<double> &ys)
{
    const double w = 0.5;
    const double h = 0.8;
    const double tw = 0.12;
    const double tf = 0.14;

    double px[] = {w,  w,      tw / 2, tw / 2,  w,       w,       -w,      -w,
                   -tw / 2, -tw / 2, -w,     -w};
    double py[] = {-h, -h + tf, -h + tf, h - tf, h - tf, h,      h,       h - tf,
                   h - tf,  -h + tf, -h + tf, -h};

    xs.clear();
    ys.clear();

    for (int i = 0; i < 12; i++)
    {
        xs.push_back(px[i]);
        ys.push_back(py[i]);
    }

    // Repeat the first point to close the ring.

    xs.push_back(px[0]);
    ys.push_back(py[0]);
}

// Apply the same section and spine to whichever extrusion class is passed in.
// Templated so the gle class and the swept class go through identical code --
// if the two interfaces ever drift apart, this stops compiling. The parameter
// is the pointer type itself, so both a raw pointer and one of the library's
// smart pointers can be passed.

// A plain square section, also with its first point repeated at the end.

static void buildSquareSection(std::vector<double> &xs, std::vector<double> &ys)
{
    double px[] = {0.5, 0.5, -0.5, -0.5, 0.5};
    double py[] = {-0.5, 0.5, 0.5, -0.5, -0.5};

    xs.clear();
    ys.clear();

    for (int i = 0; i < 5; i++)
    {
        xs.push_back(px[i]);
        ys.push_back(py[i]);
    }
}

// An L section, which has no symmetry at all. A square cannot tell a mirrored
// section basis from a correct one, and an I section is symmetric about both
// axes so it cannot either. This one can.

static void buildLSection(std::vector<double> &xs, std::vector<double> &ys)
{
    double px[] = {-0.4, 0.6, 0.6, -0.1, -0.1, -0.4, -0.4};
    double py[] = {-0.5, -0.5, -0.2, -0.2, 0.7, 0.7, -0.5};

    xs.clear();
    ys.clear();

    for (int i = 0; i < 7; i++)
    {
        xs.push_back(px[i]);
        ys.push_back(py[i]);
    }
}

enum SectionKind {
    skSquare,
    skISection,
    skLSection
};

template <typename T> static void configureExtrusion(T extrusion, SectionKind kind, double twistDeg)
{
    std::vector<double> xs, ys;

    if (kind == skSquare)
        buildSquareSection(xs, ys);
    else if (kind == skLSection)
        buildLSection(xs, ys);
    else
        buildISection(xs, ys);

    extrusion->setUseTwist(twistDeg != 0.0);
    extrusion->setSectionSize(int(xs.size()));

    for (std::size_t i = 0; i < xs.size(); i++)
    {
        extrusion->setSectionCoord(int(i), xs[i], ys[i]);

        // Deliberately leaves the last normal unset, exactly as the callers
        // in the wild do.

        if (i < xs.size() - 1)
        {
            double nx = xs[i];
            double ny = ys[i];
            double len = std::sqrt(nx * nx + ny * ny);

            if (len > 0.0)
                extrusion->setSectionNormal(int(i), nx / len, ny / len);
        }
    }

    extrusion->setSpineSize(4);
    extrusion->setSpineCoord(0, -3.1, 0.0, 0.0);
    extrusion->setSpineCoord(1, -3.0, 0.0, 0.0);
    extrusion->setSpineCoord(2, 3.0, 0.0, 0.0);
    extrusion->setSpineCoord(3, 3.1, 0.0, 0.0);
    extrusion->setSpineTwist(1, twistDeg);
    extrusion->setSpineTwist(2, twistDeg);
    extrusion->setUpVector(0.0, 1.0, 0.0);
}

template <typename T> static void configureTwistedExtrusion(T extrusion)
{
    configureExtrusion(extrusion, skISection, 25.0);
}

template <typename T> static void configureSolidLine(T line, Node *n1, Node *n2, bool coneCap, double nodeSize)
{
    line->setRadius(0.35);
    line->setSides(12);
    line->setNodes(n1, n2);

    if (coneCap)
    {
        line->setUseOffsetConeCap(true);
        line->setOffsetConeCapRadius(0.0);
        line->setOffsets(-nodeSize * 2.0, -nodeSize * 2.0);
        line->setStartOffsets(-nodeSize, -nodeSize);
        line->refresh();
    }
    else
    {
        line->setUseOffsetConeCap(false);
        line->setOffsets(0.0, 0.0);
    }
}

// Replicates the IVF_BEAM_SOLID branch of ObjectiveFrame's Beam::refresh()
// call for call, including the order of the offset calls. That order matters:
// setStartOffsets() deliberately does not rebuild the spine, so the spine ends
// up built by whichever setOffsets() call came last, using whatever start
// offset was in force at that moment. Bars and beams take different branches
// and end up with different spines.

template <typename T>
static void configureBeamSolid(T line, Node *n1, Node *n2, bool isBar, double nodeSize, double lineRadius, long sides)
{
    line->setRadius(lineRadius);

    line->setUseColor(false);
    line->setNodes(n1, n2);

    line->setUseOffsetConeCap(true);
    line->setOffsetConeCapRadius(0.0);
    line->setOffsets(-nodeSize * 2.0, -nodeSize * 2.0);
    line->setStartOffsets(-nodeSize, -nodeSize);

    if (line->getSides() != sides)
        line->setSides(sides);

    if (isBar)
    {
        line->setOffsets(-nodeSize * 2.0, -nodeSize * 2.0);
        line->setStartOffsets(-nodeSize, -nodeSize);
    }
    else
        line->setOffsets(0.0, 0.0);

    line->setTextureMode(0);
}

// Colour the spine the way a result plot along a beam does: set the spine size
// directly, then a colour and a coordinate per station.

template <typename T> static void configureResultColors(T line, int n)
{
    const double x0 = -3.0;
    const double x1 = 3.0;
    const double cap = 0.1;

    line->setUseColor(true);
    line->setSpineSize(n + 2);

    for (int i = 0; i < n + 2; i++)
    {
        double t;

        if (i == 0)
            t = -cap / (x1 - x0);
        else if (i == n + 1)
            t = 1.0 + cap / (x1 - x0);
        else
            t = double(i - 1) / double(n - 1);

        double x = x0 + (x1 - x0) * t;

        line->setSpineCoord(i, x, 0.0, 0.0);
        line->setSpineColor(i, float(t), 0.0f, float(1.0 - t));
        line->setSpineScale(i, 1.0, 1.0);
    }
}

// ------------------------------------------------------------
// Checks
// ------------------------------------------------------------

static MaterialPtr makeMaterial()
{
    MaterialPtr material = Material::create();
    material->setDiffuseColor(0.3f, 0.6f, 0.9f, 1.0f);
    material->setAmbientColor(0.2f, 0.3f, 0.4f, 1.0f);
    material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

    return material;
}

static void comparePair(const std::string &label, Shape *legacy, Shape *swept, double distance, double minIoU)
{
    Mask a = captureMask(legacy, distance);
    Mask b = captureMask(swept, distance);

    int areaA = maskArea(a);
    int areaB = maskArea(b);

    check(areaA > 500, label + ": the gle shape drew something");
    check(areaB > 500, label + ": the swept shape drew something");

    double iou = maskIoU(a, b);
    double areaRatio = (areaA > 0) ? double(areaB) / double(areaA) : 0.0;

    std::printf("  %-34s IoU %.4f  area ratio %.4f\n", label.c_str(), iou, areaRatio);

    check(iou > minIoU, label + ": silhouettes agree with the gle original");
    check((areaRatio > 0.95) && (areaRatio < 1.05), label + ": covered area matches the gle original");
}

static void runChecks()
{
    MaterialPtr material = makeMaterial();

    // ------------------------------------------------------------
    // Solid line between two nodes
    // ------------------------------------------------------------

    std::printf("SolidLine vs SweptSolidLine\n");

    NodePtr n1 = Node::create();
    n1->setPosition(-3.0, 0.0, 0.0);

    NodePtr n2 = Node::create();
    n2->setPosition(3.0, 0.5, 1.0);

    SolidLinePtr legacyLine = SolidLine::create();
    legacyLine->setMaterial(material);
    legacyLine->setUseName(false);
    legacyLine->setUseSelectShape(false);
    configureSolidLine(legacyLine, n1, n2, false, 0.25);

    SweptSolidLinePtr sweptLine = SweptSolidLine::create();
    sweptLine->setMaterial(material);
    sweptLine->setUseName(false);
    sweptLine->setUseSelectShape(false);
    configureSolidLine(sweptLine, n1, n2, false, 0.25);

    comparePair("plain solid line", legacyLine, sweptLine, 12.0, 0.97);

    check(std::fabs(legacyLine->getLength() - sweptLine->getLength()) < 1e-9, "getLength() agrees");
    check(legacyLine->getSides() == sweptLine->getSides(), "getSides() agrees");
    check(std::fabs(legacyLine->getRadius() - sweptLine->getRadius()) < 1e-9, "getRadius() agrees");
    check(sweptLine->getNode(0) == n1 && sweptLine->getNode(1) == n2, "getNode() returns the assigned nodes");

    // ------------------------------------------------------------
    // Solid line with conical end caps and offsets
    // ------------------------------------------------------------

    SolidLinePtr legacyCap = SolidLine::create();
    legacyCap->setMaterial(material);
    legacyCap->setUseName(false);
    legacyCap->setUseSelectShape(false);
    configureSolidLine(legacyCap, n1, n2, true, 0.25);

    SweptSolidLinePtr sweptCap = SweptSolidLine::create();
    sweptCap->setMaterial(material);
    sweptCap->setUseName(false);
    sweptCap->setUseSelectShape(false);
    configureSolidLine(sweptCap, n1, n2, true, 0.25);

    comparePair("cone capped solid line", legacyCap, sweptCap, 12.0, 0.95);

    // ------------------------------------------------------------
    // A vertical line, where the default up vector is parallel to the spine
    // ------------------------------------------------------------

    NodePtr v1 = Node::create();
    v1->setPosition(0.0, -3.0, 0.0);

    NodePtr v2 = Node::create();
    v2->setPosition(0.0, 3.0, 0.0);

    SolidLinePtr legacyVert = SolidLine::create();
    legacyVert->setMaterial(material);
    legacyVert->setUseName(false);
    legacyVert->setUseSelectShape(false);
    configureSolidLine(legacyVert, v1, v2, false, 0.25);

    SweptSolidLinePtr sweptVert = SweptSolidLine::create();
    sweptVert->setMaterial(material);
    sweptVert->setUseName(false);
    sweptVert->setUseSelectShape(false);
    configureSolidLine(sweptVert, v1, v2, false, 0.25);

    comparePair("vertical solid line", legacyVert, sweptVert, 12.0, 0.97);

    // ------------------------------------------------------------
    // The exact sequences ObjectiveFrame issues for a bar and for a beam
    // ------------------------------------------------------------
    //
    // These differ from the cases above in that they do not end with a
    // refresh(), so the spine is whatever the last setOffsets() built. For a
    // beam that leaves the first station beyond the second -- a spine that
    // doubles back on itself -- which is a case a straight test spine never
    // reaches.

    struct SolidCase {
        const char *name;
        bool isBar;
        double length;
        double nodeSize;
    };

    SolidCase solidCases[] = {{"bar, ObjectiveFrame sequence", true, 6.0, 0.25},
                              {"beam, ObjectiveFrame sequence", false, 6.0, 0.25},
                              {"short bar, offsets overlap", true, 1.2, 0.25},
                              {"short beam, offsets overlap", false, 1.2, 0.25}};

    for (const auto &sc : solidCases)
    {
        NodePtr a = Node::create();
        a->setPosition(-sc.length / 2.0, 0.0, 0.0);

        NodePtr b = Node::create();
        b->setPosition(sc.length / 2.0, 0.0, 0.0);

        SolidLinePtr legacy = SolidLine::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureBeamSolid(legacy, a, b, sc.isBar, sc.nodeSize, 0.35, 12);

        SweptSolidLinePtr swept = SweptSolidLine::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureBeamSolid(swept, a, b, sc.isBar, sc.nodeSize, 0.35, 12);

        double dist = (sc.length > 3.0) ? 12.0 : 3.0;

        comparePair(sc.name, legacy, swept, dist, 0.97);

        // A tube whose section frame spins along the spine still covers the
        // same silhouette when the section is circular, so compare the shading
        // as well -- that is what shows a twist on a round tube.

        double lr, lg, lb, sr, sg, sb;
        meanColor(legacy, dist, lr, lg, lb);
        meanColor(swept, dist, sr, sg, sb);

        std::printf("  %-34s shading gle %.1f  swept %.1f\n", "", (lr + lg + lb) / 3.0, (sr + sg + sb) / 3.0);

        check(std::fabs((lr + lg + lb) - (sr + sg + sb)) / 3.0 < 6.0,
              std::string(sc.name) + ": shades like the gle original");
    }

    // ------------------------------------------------------------
    // Twisted section extrusion
    // ------------------------------------------------------------

    std::printf("Extrusion vs SweptExtrusion\n");

    // Work up from the simplest case, so a failure says which ingredient is
    // responsible rather than just "the extrusion is wrong".

    struct ExtrusionCase {
        const char *name;
        SectionKind kind;
        double twist;
    };

    // Twisting has to actually change what each class draws, or the
    // comparison below would pass for the wrong reason.

    int untwistedArea = 0;

    for (double twist : {0.0, 25.0, 45.0})
    {
        ExtrusionPtr legacy = Extrusion::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureExtrusion(legacy, skSquare, twist);

        SweptExtrusionPtr swept = SweptExtrusion::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureExtrusion(swept, skSquare, twist);

        int legacyArea = maskArea(captureMask(legacy, 12.0));
        int sweptArea = maskArea(captureMask(swept, 12.0));

        std::printf("  twist %5.1f deg   gle area %6d   swept area %6d\n", twist, legacyArea, sweptArea);

        check(legacyArea == sweptArea, "twisted square covers the same area as the gle original");

        if (twist == 0.0)
            untwistedArea = sweptArea;
        else
            check(sweptArea != untwistedArea, "twisting actually changes what is drawn");
    }

    ExtrusionCase cases[] = {{"square section, no twist", skSquare, 0.0},
                             {"square section, twisted", skSquare, 25.0},
                             {"I section, no twist", skISection, 0.0},
                             {"L section, no twist", skLSection, 0.0},
                             {"L section, twisted", skLSection, 25.0}};

    for (const auto &ec : cases)
    {
        ExtrusionPtr legacy = Extrusion::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureExtrusion(legacy, ec.kind, ec.twist);

        SweptExtrusionPtr swept = SweptExtrusion::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureExtrusion(swept, ec.kind, ec.twist);

        comparePair(ec.name, legacy, swept, 12.0, 0.95);
    }

    ExtrusionPtr legacyExtr = Extrusion::create();
    legacyExtr->setMaterial(material);
    legacyExtr->setUseName(false);
    legacyExtr->setUseSelectShape(false);
    configureTwistedExtrusion(legacyExtr);

    SweptExtrusionPtr sweptExtr = SweptExtrusion::create();
    sweptExtr->setMaterial(material);
    sweptExtr->setUseName(false);
    sweptExtr->setUseSelectShape(false);
    configureTwistedExtrusion(sweptExtr);

    comparePair("twisted I section", legacyExtr, sweptExtr, 12.0, 0.95);

    check(legacyExtr->getSpineSize() == sweptExtr->getSpineSize(), "getSpineSize() agrees");
    check(legacyExtr->getSectionSize() == sweptExtr->getSectionSize(), "getSectionSize() agrees");

    double lt = 0.0;
    double st = 0.0;
    legacyExtr->getSpineTwist(1, lt);
    sweptExtr->getSpineTwist(1, st);
    check(std::fabs(lt - st) < 1e-9, "getSpineTwist() agrees");

    // ------------------------------------------------------------
    // Per-vertex result colours along a line
    // ------------------------------------------------------------

    std::printf("Result colours\n");

    SolidLinePtr legacyResult = SolidLine::create();
    legacyResult->setMaterial(material);
    legacyResult->setUseName(false);
    legacyResult->setUseSelectShape(false);
    legacyResult->setRadius(0.35);
    legacyResult->setSides(12);
    configureResultColors(legacyResult, 12);

    SweptSolidLinePtr sweptResult = SweptSolidLine::create();
    sweptResult->setMaterial(material);
    sweptResult->setUseName(false);
    sweptResult->setUseSelectShape(false);
    sweptResult->setRadius(0.35);
    sweptResult->setSides(12);
    configureResultColors(sweptResult, 12);

    // The colour ramp only reaches the lit surface through GL_COLOR_MATERIAL,
    // which is how an application drives it.

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    comparePair("result coloured line", legacyResult, sweptResult, 12.0, 0.97);

    double lr, lg, lb;
    double sr, sg, sb;
    meanColor(legacyResult, 12.0, lr, lg, lb);
    meanColor(sweptResult, 12.0, sr, sg, sb);

    std::printf("  %-34s gle (%.1f %.1f %.1f)  swept (%.1f %.1f %.1f)\n", "mean colour", lr, lg, lb, sr, sg, sb);

    check(std::fabs(lr - sr) < 20.0, "mean red matches the gle original");
    check(std::fabs(lg - sg) < 20.0, "mean green matches the gle original");
    check(std::fabs(lb - sb) < 20.0, "mean blue matches the gle original");

    // Recolouring must actually change what is drawn.

    for (int i = 0; i < sweptResult->getSpineSize(); i++)
        sweptResult->setSpineColor(i, 0.0f, 1.0f, 0.0f);

    double gr, gg, gb;
    meanColor(sweptResult, 12.0, gr, gg, gb);
    check(gg > sg + 10.0, "changing the spine colours changes the rendered colour");

    glDisable(GL_COLOR_MATERIAL);

    // Restoring the ramp must restore the original appearance, which exercises
    // the in-place vertex buffer update path.

    configureResultColors(sweptResult, 12);

    glEnable(GL_COLOR_MATERIAL);
    double br, bg, bb;
    meanColor(sweptResult, 12.0, br, bg, bb);
    glDisable(GL_COLOR_MATERIAL);

    check(std::fabs(br - sr) < 2.0 && std::fabs(bg - sg) < 2.0 && std::fabs(bb - sb) < 2.0,
          "restoring the spine colours restores the rendering");

    // ------------------------------------------------------------
    // Behaviour that is not about pixels
    // ------------------------------------------------------------

    std::printf("Material state\n");

    // The gle version compiled its material into a display list, so the list
    // replayed glMaterialfv every frame. The swept version has no list and
    // applies the material live, which leans on Material's redundancy cache
    // being in step with actual GL state.
    //
    // GL_COLOR_MATERIAL takes that state out from under the cache: while it is
    // enabled glColor overwrites ambient and diffuse, and the overwritten
    // values persist after it is disabled. An application that renders
    // anything that way between two draws of the same material -- which is how
    // result colours are drawn -- leaves the cache believing a material is
    // applied that no longer is.

    Material::invalidateStateCache();

    SweptSolidLinePtr plainLine = SweptSolidLine::create();
    plainLine->setMaterial(material);
    plainLine->setUseName(false);
    plainLine->setUseSelectShape(false);
    configureBeamSolid(plainLine, n1, n2, true, 0.25, 0.35, 12);

    double cr0, cg0, cb0;
    meanColor(plainLine, 12.0, cr0, cg0, cb0);

    // Draw a result coloured line -- which engages GL_COLOR_MATERIAL and
    // therefore rewrites the ambient and diffuse material -- and then draw the
    // plain line again. It has to come back the colour of its own material,
    // not the colour the ramp left behind.

    SweptSolidLinePtr rampLine = SweptSolidLine::create();
    rampLine->setMaterial(material);
    rampLine->setUseName(false);
    rampLine->setUseSelectShape(false);
    rampLine->setRadius(0.35);
    rampLine->setSides(12);
    configureResultColors(rampLine, 12);

    meanColor(rampLine, 12.0, sr, sg, sb);

    double cr1, cg1, cb1;
    meanColor(plainLine, 12.0, cr1, cg1, cb1);

    std::printf("  %-34s before (%.1f %.1f %.1f)  after (%.1f %.1f %.1f)\n", "material after a colour ramp draw", cr0,
                cg0, cb0, cr1, cg1, cb1);

    check(std::fabs(cr0 - cr1) < 4.0 && std::fabs(cg0 - cg1) < 4.0 && std::fabs(cb0 - cb1) < 4.0,
          "a colour ramp draw does not leave its colours on the next object");

    std::printf("Behaviour\n");

    // Applications turn display lists on to make the gle version affordable.
    // The swept version ignores that, but it must keep tracking changes --
    // a stale display list here would freeze the geometry.

    sweptLine->setUselist(true);

    Mask beforeRadius = captureMask(sweptLine, 12.0);

    sweptLine->setRadius(0.9);

    Mask afterRadius = captureMask(sweptLine, 12.0);

    check(maskArea(afterRadius) > maskArea(beforeRadius) * 3 / 2,
          "geometry still updates with display lists enabled");

    sweptLine->setRadius(0.35);
    sweptLine->setUselist(false);

    // markListDirty() has to reach the generated mesh, since that is what
    // applications call after mutating something the setters cannot see.

    sweptLine->initBoundingSphere();
    double radiusBefore = sweptLine->getBoundingSphere()->getRadius();

    n2->setPosition(9.0, 0.5, 1.0);
    sweptLine->refresh();
    sweptLine->doUpdateBoundingSphere();

    double radiusAfter = sweptLine->getBoundingSphere()->getRadius();
    check(radiusAfter > radiusBefore + 1.0, "moving a node grows the bounding sphere");

    n2->setPosition(3.0, 0.5, 1.0);
    sweptLine->refresh();

    // A degenerate spine must draw nothing rather than crash.

    SweptExtrusionPtr degenerate = SweptExtrusion::create();
    degenerate->setUseName(false);
    degenerate->setUseSelectShape(false);
    degenerate->setSectionSize(4);
    degenerate->setSectionCoord(0, -1.0, -1.0);
    degenerate->setSectionCoord(1, 1.0, -1.0);
    degenerate->setSectionCoord(2, 1.0, 1.0);
    degenerate->setSectionCoord(3, -1.0, 1.0);
    degenerate->setSpineSize(4);

    for (int i = 0; i < 4; i++)
        degenerate->setSpineCoord(i, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    degenerate->render();
    checkNoGLError("a fully degenerate spine renders without error");

    SweptExtrusionPtr empty = SweptExtrusion::create();
    empty->setUseName(false);
    empty->setUseSelectShape(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    empty->render();
    checkNoGLError("an unconfigured extrusion renders without error");

    // The select shape is generated separately and must not disturb the
    // ordinary drawing.

    sweptLine->setUseSelectShape(true);
    sweptLine->setSelect(Shape::SS_ON);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sweptLine->render();
    checkNoGLError("rendering the select shape produced no GL error");

    Mask selected = captureMask(sweptLine, 12.0);
    check(maskArea(selected) > 500, "the selected line still draws");

    sweptLine->setSelect(Shape::SS_OFF);
    sweptLine->setUseSelectShape(false);
}

// ------------------------------------------------------------
// Viewer
// ------------------------------------------------------------

struct ViewerCase {
    std::string name;
    ShapePtr legacy;
    ShapePtr swept;
    bool colorMaterial;
};

static std::vector<ViewerCase> g_cases;
static int g_currentCase = 0;
static bool g_showLegacyBoth = false;
static bool g_rotating = true;

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
        if (!g_cases.empty())
        {
            g_currentCase = (g_currentCase + 1) % int(g_cases.size());
            std::printf("case: %s\n", g_cases[g_currentCase].name.c_str());
        }
        break;
    case GLFW_KEY_G:
        g_showLegacyBoth = !g_showLegacyBoth;
        std::printf("%s\n", g_showLegacyBoth ? "both halves: gle" : "left: gle, right: swept");
        break;
    case GLFW_KEY_R:
        g_rotating = !g_rotating;
        break;
    default:
        break;
    }
}

static void drawHalf(Shape *shape, int x, int y, int w, int h, float angle, bool colorMaterial)
{
    if (h <= 0 || w <= 0)
        return;

    glViewport(x, y, w, h);

    float aspect = float(w) / float(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5 * aspect, 0.5 * aspect, -0.5, 0.5, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -12.0);
    glRotated(20.0, 1.0, 0.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[4] = {0.4f, 0.8f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glRotated(angle, 0.0, 1.0, 0.0);

    if (colorMaterial)
    {
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
    }

    shape->render();

    glDisable(GL_COLOR_MATERIAL);
}

static void runViewer(GLFWwindow *window)
{
    std::printf("\nViewer: gle on the left, swept on the right.\n");
    std::printf("  space  cycle test case\n");
    std::printf("  g      draw the gle class in both halves\n");
    std::printf("  r      toggle rotation\n");
    std::printf("  esc    quit\n");
    std::printf("case: %s\n", g_cases.empty() ? "(none)" : g_cases[0].name.c_str());

    glfwSetKeyCallback(window, keyCallback);

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

        if ((fbWidth > 0) && (fbHeight > 0) && !g_cases.empty())
        {
            glViewport(0, 0, fbWidth, fbHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const ViewerCase &c = g_cases[g_currentCase];

            int half = fbWidth / 2;

            drawHalf(c.legacy, 0, 0, half - 1, fbHeight, angle, c.colorMaterial);
            drawHalf(g_showLegacyBoth ? c.legacy : c.swept, half + 1, 0, fbWidth - half - 1, fbHeight, angle,
                     c.colorMaterial);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

static void buildViewerCases()
{
    MaterialPtr material = makeMaterial();

    NodePtr n1 = Node::create();
    n1->setPosition(-3.0, 0.0, 0.0);

    NodePtr n2 = Node::create();
    n2->setPosition(3.0, 0.5, 1.0);

    {
        SolidLinePtr legacy = SolidLine::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureSolidLine(legacy, n1, n2, false, 0.25);

        SweptSolidLinePtr swept = SweptSolidLine::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureSolidLine(swept, n1, n2, false, 0.25);

        g_cases.push_back({"plain solid line", legacy, swept, false});
    }

    {
        SolidLinePtr legacy = SolidLine::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureSolidLine(legacy, n1, n2, true, 0.25);

        SweptSolidLinePtr swept = SweptSolidLine::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureSolidLine(swept, n1, n2, true, 0.25);

        g_cases.push_back({"cone capped solid line", legacy, swept, false});
    }

    {
        NodePtr a = Node::create();
        a->setPosition(-3.0, 0.0, 0.0);

        NodePtr b = Node::create();
        b->setPosition(3.0, 0.0, 0.0);

        for (bool isBar : {true, false})
        {
            SolidLinePtr legacy = SolidLine::create();
            legacy->setMaterial(material);
            legacy->setUseName(false);
            legacy->setUseSelectShape(false);
            configureBeamSolid(legacy, a, b, isBar, 0.25, 0.35, 12);

            SweptSolidLinePtr swept = SweptSolidLine::create();
            swept->setMaterial(material);
            swept->setUseName(false);
            swept->setUseSelectShape(false);
            configureBeamSolid(swept, a, b, isBar, 0.25, 0.35, 12);

            g_cases.push_back({isBar ? "bar, ObjectiveFrame sequence" : "beam, ObjectiveFrame sequence", legacy,
                               swept, false});
        }
    }

    struct ViewerExtrusion {
        const char *name;
        SectionKind kind;
        double twist;
    };

    ViewerExtrusion extrusions[] = {{"square section, no twist", skSquare, 0.0},
                                    {"square section, twisted", skSquare, 25.0},
                                    {"I section, no twist", skISection, 0.0},
                                    {"twisted I section", skISection, 25.0},
                                    {"L section, no twist", skLSection, 0.0},
                                    {"L section, twisted", skLSection, 25.0}};

    for (const auto &ve : extrusions)
    {
        ExtrusionPtr legacy = Extrusion::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        configureExtrusion(legacy, ve.kind, ve.twist);

        SweptExtrusionPtr swept = SweptExtrusion::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        configureExtrusion(swept, ve.kind, ve.twist);

        g_cases.push_back({ve.name, legacy, swept, false});
    }

    {
        SolidLinePtr legacy = SolidLine::create();
        legacy->setMaterial(material);
        legacy->setUseName(false);
        legacy->setUseSelectShape(false);
        legacy->setRadius(0.35);
        legacy->setSides(12);
        configureResultColors(legacy, 12);

        SweptSolidLinePtr swept = SweptSolidLine::create();
        swept->setMaterial(material);
        swept->setUseName(false);
        swept->setUseSelectShape(false);
        swept->setRadius(0.35);
        swept->setSides(12);
        configureResultColors(swept, 12);

        g_cases.push_back({"result coloured line", legacy, swept, true});
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
        else if ((arg == "--case") && (i + 1 < argc))
            g_currentCase = std::atoi(argv[++i]);
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

    GLFWwindow *window = glfwCreateWindow(1024, 512, "Swept extrusion -- gle | swept", nullptr, nullptr);

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
    {
        buildViewerCases();
        runViewer(window);
        g_cases.clear();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return (g_failures == 0) ? 0 : 1;
}
