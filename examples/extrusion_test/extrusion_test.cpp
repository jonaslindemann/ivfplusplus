// ------------------------------------------------------------
//
// extrusion_test -- verification of the extrusion geometry layer
//
// The extrusion geometry classes are pure geometry generators with no OpenGL
// dependency at all, so they can be exercised without a window or a context.
// This example doubles as a regression test for them and as a worked reference
// for how the three pieces fit together:
//
//   ExtrusionProfile  -- the 2D cross section to sweep
//   buildPathFrames() -- the spine, turned into oriented stations
//   ExtrusionBuilder  -- the two, swept into an indexed triangle mesh
//
// Run it as a console program. It prints the number of checks performed and
// returns a non-zero exit code if any of them failed.
//
// ------------------------------------------------------------

#include <ivf/ExtrusionBuilder.h>
#include <ivf/ExtrusionProfile.h>
#include <ivf/PathFrames.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
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

static void checkNear(double a, double b, double tol, const std::string &what)
{
    g_checks++;

    if (!(std::fabs(a - b) <= tol))
    {
        g_failures++;
        std::printf("  FAIL: %s (got %.9g, expected %.9g, tol %.3g)\n", what.c_str(), a, b, tol);
    }
}

static bool isFinite(const glm::vec3 &v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}

// ------------------------------------------------------------
// ExtrusionProfile
// ------------------------------------------------------------

static void testProfile()
{
    std::printf("ExtrusionProfile\n");

    ExtrusionProfile c = ExtrusionProfile::circle(2.0f, 16);
    check(c.pointCount() == 16, "full circle emits `segments` points (no duplicated seam)");
    check(c.closed(), "full circle is closed");
    check(c.hasNormals(), "circle carries explicit normals");

    for (std::size_t i = 0; i < c.pointCount(); i++)
    {
        checkNear(glm::length(c.points()[i]), 2.0, 1e-5, "circle point lies on the radius");
        checkNear(glm::length(c.normals()[i]), 1.0, 1e-5, "circle normal is unit length");
    }

    ExtrusionProfile arc = ExtrusionProfile::circle(1.0f, 8, 0.0f, glm::pi<float>());
    check(arc.pointCount() == 9, "half circle emits segments+1 points");
    check(!arc.closed(), "partial sweep is open");

    // A square, wound counter clockwise, with computed normals.

    ExtrusionProfile r = ExtrusionProfile::rectangle(2.0f, 2.0f);
    std::vector<glm::vec2> rn = r.computeSmoothNormals();

    check(rn.size() == 4, "rectangle yields one normal per point");

    for (std::size_t i = 0; i < rn.size(); i++)
    {
        checkNear(glm::length(rn[i]), 1.0, 1e-5, "computed normal is unit length");
        check(glm::dot(rn[i], r.points()[i]) > 0.0f, "computed normal points outward");
    }

    // The same square wound clockwise. Sections handed to an extrusion come
    // from all sorts of places, so the normals must face outward either way.

    std::vector<glm::vec2> cw = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
    ExtrusionProfile rcw = ExtrusionProfile::polyline(cw, true);
    std::vector<glm::vec2> cwn = rcw.computeSmoothNormals();

    for (std::size_t i = 0; i < cwn.size(); i++)
        check(glm::dot(cwn[i], cw[i]) > 0.0f, "normals face outward regardless of winding");

    // Degenerate: a profile whose points all coincide with the centroid has no
    // radial direction to fall back on.

    std::vector<glm::vec2> degen = {{0, 0}, {0, 0}, {0, 0}};
    ExtrusionProfile dp = ExtrusionProfile::polyline(degen, true);
    std::vector<glm::vec2> dn = dp.computeSmoothNormals();

    for (const auto &n : dn)
        check(std::isfinite(n.x) && std::isfinite(n.y), "degenerate profile yields finite normals");

    ExtrusionProfile p = ExtrusionProfile::circle(1.0f, 8);
    p.addPoint(glm::vec2(5.0f, 5.0f));
    check(!p.hasNormals(), "adding a point invalidates stale explicit normals");
}

// ------------------------------------------------------------
// PathFrames
// ------------------------------------------------------------

static void checkFramesOrthonormal(const std::vector<PathFrame> &frames, const std::string &label)
{
    for (std::size_t i = 0; i < frames.size(); i++)
    {
        const PathFrame &f = frames[i];

        check(isFinite(f.position) && isFinite(f.tangent) && isFinite(f.normal) && isFinite(f.binormal),
              label + ": frame is finite");
        checkNear(glm::length(f.tangent), 1.0, 1e-4, label + ": tangent is unit length");
        checkNear(glm::length(f.normal), 1.0, 1e-4, label + ": normal is unit length");
        checkNear(glm::length(f.binormal), 1.0, 1e-4, label + ": binormal is unit length");
        checkNear(glm::dot(f.tangent, f.normal), 0.0, 1e-4, label + ": tangent perpendicular to normal");
        checkNear(glm::dot(f.tangent, f.binormal), 0.0, 1e-4, label + ": tangent perpendicular to binormal");
        checkNear(glm::dot(f.normal, f.binormal), 0.0, 1e-4, label + ": normal perpendicular to binormal");
    }
}

static void testFrames()
{
    std::printf("PathFrames\n");

    // Straight spine along +x.

    std::vector<glm::vec3> straight = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}};
    std::vector<PathFrame> f = buildPathFrames(straight, SpineInterp::Polyline, JoinStyle::Angle, 64,
                                               FrameMethod::RotationMinimizing, glm::vec3(0, 1, 0));

    check(f.size() == 4, "Angle join yields one station per control point");
    checkFramesOrthonormal(f, "straight");
    checkNear(f.back().v, 3.0, 1e-5, "arc length accumulates along the spine");

    for (std::size_t i = 0; i < f.size(); i++)
    {
        checkNear(f[i].source, double(i), 1e-5, "Angle join maps station to control point 1:1");
        checkNear(glm::dot(f[i].tangent, glm::vec3(1, 0, 0)), 1.0, 1e-5, "straight spine tangent follows +x");
    }

    // An up vector parallel to the tangent carries no orientation information
    // and must not produce a degenerate basis.

    std::vector<PathFrame> fp = buildPathFrames(straight, SpineInterp::Polyline, JoinStyle::Angle, 64,
                                                FrameMethod::RotationMinimizing, glm::vec3(1, 0, 0));
    checkFramesOrthonormal(fp, "up parallel to tangent");

    // L-shaped spine: the mitered corner tangent must bisect the two segments.

    std::vector<glm::vec3> corner = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
    std::vector<PathFrame> fc = buildPathFrames(corner, SpineInterp::Polyline, JoinStyle::Angle);

    checkFramesOrthonormal(fc, "L corner");

    glm::vec3 bisector = glm::normalize(glm::vec3(1, 0, 0) + glm::vec3(0, 1, 0));
    checkNear(glm::dot(fc[1].tangent, bisector), 1.0, 1e-4, "mitered corner tangent bisects the segments");

    // Coincident spine points, which is exactly what a solid line's zero length
    // cap segments produce.

    std::vector<glm::vec3> dup = {{0, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 0, 0}};
    std::vector<PathFrame> fd = buildPathFrames(dup, SpineInterp::Polyline, JoinStyle::Angle);

    check(fd.size() == 4, "coincident stations are preserved");
    checkFramesOrthonormal(fd, "coincident stations");

    // Every station coincident: no direction exists anywhere on the spine.

    std::vector<glm::vec3> allsame = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
    std::vector<PathFrame> fa = buildPathFrames(allsame, SpineInterp::Polyline, JoinStyle::Angle);
    checkFramesOrthonormal(fa, "all stations coincident");

    // Join styles.

    std::vector<PathFrame> fraw = buildPathFrames(corner, SpineInterp::Polyline, JoinStyle::Raw);
    check(fraw.size() == 4, "Raw join doubles the corner station");
    checkFramesOrthonormal(fraw, "Raw join");

    std::vector<PathFrame> fcut = buildPathFrames(corner, SpineInterp::Polyline, JoinStyle::Cut, 64,
                                                  FrameMethod::RotationMinimizing, glm::vec3(0.0f), false, 0.2f);
    check(fcut.size() == 4, "Cut join replaces the corner with two offset stations");
    checkFramesOrthonormal(fcut, "Cut join");
    check(fcut[1].source < 1.0f && fcut[2].source > 1.0f, "Cut stations straddle the corner in control point space");

    std::vector<PathFrame> fround = buildPathFrames(corner, SpineInterp::Polyline, JoinStyle::Round, 64,
                                                    FrameMethod::RotationMinimizing, glm::vec3(0.0f), false, 0.2f, 6);
    check(fround.size() == 2 + 2 + 5, "Round join inserts cornerSegments-1 arc stations");
    checkFramesOrthonormal(fround, "Round join");

    // Smooth spines, interpolated with the ivfmath spline.

    std::vector<glm::vec3> wave = {{0, 0, 0}, {1, 1, 0}, {2, 0, 0}, {3, 1, 0}};
    std::vector<PathFrame> fs = buildPathFrames(wave, SpineInterp::CatmullRom, JoinStyle::Angle, 32);

    check(fs.size() == 32, "CatmullRom honours the sample count");
    checkFramesOrthonormal(fs, "CatmullRom");
    checkNear(fs.front().source, 0.0, 1e-5, "first sample sits at control point 0");
    checkNear(fs.back().source, 3.0, 1e-5, "last sample sits at the final control point");
    checkNear(glm::length(fs.front().position - wave.front()), 0.0, 1e-4, "spline starts at the first control point");
    checkNear(glm::length(fs.back().position - wave.back()), 0.0, 1e-4, "spline ends at the last control point");

    // Rotation minimizing frames should not accumulate twist along a helix,
    // which is where a Frenet frame would visibly spin.

    std::vector<glm::vec3> helix;

    for (int i = 0; i <= 64; i++)
    {
        float t = float(i) / 64.0f * 4.0f * glm::pi<float>();
        helix.push_back(glm::vec3(std::cos(t), std::sin(t), t * 0.2f));
    }

    std::vector<PathFrame> fh = buildPathFrames(helix, SpineInterp::Polyline, JoinStyle::Angle);
    checkFramesOrthonormal(fh, "helix");

    double maxStep = 0.0;

    for (std::size_t i = 1; i < fh.size(); i++)
        maxStep = std::max(maxStep, double(glm::length(fh[i].normal - fh[i - 1].normal)));

    check(maxStep < 0.35, "rotation minimizing normals vary smoothly along a helix");

    // Under two control points there is nothing to sweep.

    check(buildPathFrames({{0, 0, 0}}).empty(), "a single control point yields no frames");
    check(buildPathFrames({}).empty(), "an empty spine yields no frames");
}

// ------------------------------------------------------------
// ExtrusionBuilder
// ------------------------------------------------------------

static void testBuilder()
{
    std::printf("ExtrusionBuilder\n");

    const int segments = 12;
    const float radius = 0.5f;

    ExtrusionProfile profile = ExtrusionProfile::circle(radius, segments);

    std::vector<glm::vec3> spine = {{0, 0, 0}, {4, 0, 0}};
    std::vector<PathFrame> frames = buildPathFrames(spine, SpineInterp::Polyline, JoinStyle::Angle, 64,
                                                    FrameMethod::RotationMinimizing, glm::vec3(0, 1, 0));

    ExtrusionOptions opt;
    MeshData mesh = ExtrusionBuilder::build(profile, frames, opt);

    check(mesh.isValid(), "mesh arrays are consistent and indices are in range");
    check(!mesh.empty(), "mesh has geometry");

    // Smooth walls: 2 stations of (segments+1) ring vertices, plus two caps of
    // (segments+1) vertices each.

    int ringVerts = segments + 1;
    check(mesh.vertexCount() == std::size_t(2 * ringVerts + 2 * (segments + 1)), "smooth wall + cap vertex count");
    check(mesh.triangleCount() == std::size_t(2 * segments + 2 * segments), "wall + cap triangle count");

    for (std::size_t i = 0; i < mesh.normals.size(); i++)
        checkNear(glm::length(mesh.normals[i]), 1.0, 1e-4, "mesh normal is unit length");

    // Every wall vertex must sit exactly `radius` from the spine axis.

    for (int s = 0; s < 2; s++)
        for (int j = 0; j < ringVerts; j++)
        {
            const glm::vec3 &p = mesh.positions[s * ringVerts + j];
            checkNear(std::sqrt(p.y * p.y + p.z * p.z), radius, 1e-4, "wall vertex lies on the tube radius");
        }

    // ... and the wall normals must be purely radial, pointing away from it.

    for (int s = 0; s < 2; s++)
        for (int j = 0; j < ringVerts; j++)
        {
            std::size_t k = s * ringVerts + j;
            glm::vec3 radial = glm::normalize(glm::vec3(0.0f, mesh.positions[k].y, mesh.positions[k].z));
            checkNear(glm::dot(mesh.normals[k], radial), 1.0, 1e-3, "wall normal points radially outward");
        }

    // Cap normals oppose each other and align with the sweep direction.

    const glm::vec3 &startCapNormal = mesh.normals[2 * ringVerts];
    const glm::vec3 &endCapNormal = mesh.normals[2 * ringVerts + (segments + 1)];

    checkNear(glm::dot(startCapNormal, glm::vec3(-1, 0, 0)), 1.0, 1e-4, "start cap faces backwards");
    checkNear(glm::dot(endCapNormal, glm::vec3(1, 0, 0)), 1.0, 1e-4, "end cap faces forwards");

    // Texture coordinates: V spans 0..1 normalized, arc length otherwise.

    checkNear(mesh.texCoords[0].y, 0.0, 1e-5, "normalized V starts at 0");
    checkNear(mesh.texCoords[ringVerts].y, 1.0, 1e-5, "normalized V ends at 1");

    ExtrusionOptions perLength;
    perLength.texMode = TexMode::PerLength;
    MeshData ml = ExtrusionBuilder::build(profile, frames, perLength);
    checkNear(ml.texCoords[ringVerts].y, 4.0, 1e-4, "per-length V equals the arc length");

    // Caps can be suppressed independently.

    ExtrusionOptions noCaps;
    noCaps.capStart = false;
    noCaps.capEnd = false;
    MeshData mnc = ExtrusionBuilder::build(profile, frames, noCaps);

    check(mnc.vertexCount() == std::size_t(2 * ringVerts), "capless mesh has walls only");
    check(mnc.isValid(), "capless mesh is valid");

    ExtrusionOptions startOnly;
    startOnly.capEnd = false;
    MeshData mso = ExtrusionBuilder::build(profile, frames, startOnly);
    check(mso.vertexCount() == std::size_t(2 * ringVerts + (segments + 1)), "one cap adds one fan");

    // Facet normals duplicate the profile points per edge.

    ExtrusionOptions facet;
    facet.normalStyle = NormalStyle::Facet;
    MeshData mf = ExtrusionBuilder::build(profile, frames, facet);

    check(mf.isValid(), "facet mesh is valid");
    check(mf.vertexCount() == std::size_t(2 * segments * 2 + 2 * (segments + 1)), "facet wall vertex count");

    // Per-station transforms drive taper, twist and per-vertex colour.

    ExtrusionOptions taper;
    taper.sectionFn = [](int s, const PathFrame &) {
        SectionTransform st;
        st.scale = glm::vec2(s == 0 ? 1.0f : 0.5f);
        st.color = glm::vec4(float(s), 0.0f, 0.0f, 1.0f);
        return st;
    };

    MeshData mt = ExtrusionBuilder::build(profile, frames, taper);

    check(mt.isValid(), "tapered mesh is valid");
    checkNear(std::sqrt(mt.positions[0].y * mt.positions[0].y + mt.positions[0].z * mt.positions[0].z), radius, 1e-4,
              "station 0 keeps the full radius");
    checkNear(std::sqrt(mt.positions[ringVerts].y * mt.positions[ringVerts].y +
                        mt.positions[ringVerts].z * mt.positions[ringVerts].z),
              radius * 0.5, 1e-4, "station 1 is tapered to half the radius");
    checkNear(mt.colors[0].r, 0.0, 1e-6, "station 0 colour applied");
    checkNear(mt.colors[ringVerts].r, 1.0, 1e-6, "station 1 colour applied");

    // A zero scale collapses a ring to a point, which is how a solid line
    // builds its offset cone caps. It must not produce NaN geometry.

    ExtrusionOptions collapse;
    collapse.sectionFn = [](int s, const PathFrame &) {
        SectionTransform st;
        st.scale = glm::vec2(s == 0 ? 0.0f : 1.0f);
        return st;
    };

    MeshData mc = ExtrusionBuilder::build(profile, frames, collapse);
    check(mc.isValid(), "collapsed mesh is valid");

    for (std::size_t i = 0; i < mc.positions.size(); i++)
        check(isFinite(mc.positions[i]) && isFinite(mc.normals[i]), "collapsed ring stays finite");

    // Twist rotates the section about the tangent.

    ExtrusionOptions twist;
    twist.sectionFn = [](int, const PathFrame &) {
        SectionTransform st;
        st.twist = glm::half_pi<float>();
        return st;
    };

    MeshData mtw = ExtrusionBuilder::build(profile, frames, twist);
    check(mtw.isValid(), "twisted mesh is valid");

    for (int j = 0; j < ringVerts; j++)
        checkNear(std::sqrt(mtw.positions[j].y * mtw.positions[j].y + mtw.positions[j].z * mtw.positions[j].z), radius,
                  1e-4, "twist preserves the section radius");

    // An open profile has no interior to cap.

    ExtrusionProfile open = ExtrusionProfile::circle(radius, segments, 0.0f, glm::pi<float>());
    MeshData mo = ExtrusionBuilder::build(open, frames, opt);

    check(mo.isValid(), "open profile mesh is valid");
    check(mo.vertexCount() == std::size_t(2 * (segments + 1)), "open profile emits no cap fans");

    // A closed path has no ends to cap, and wraps back to the first station.

    std::vector<glm::vec3> square = {{0, 0, 0}, {2, 0, 0}, {2, 2, 0}, {0, 2, 0}};
    std::vector<PathFrame> loop = buildPathFrames(square, SpineInterp::Polyline, JoinStyle::Angle, 64,
                                                  FrameMethod::RotationMinimizing, glm::vec3(0, 0, 1), true);

    ExtrusionOptions loopOpt;
    loopOpt.closedPath = true;
    MeshData mloop = ExtrusionBuilder::build(profile, loop, loopOpt);

    check(mloop.isValid(), "closed path mesh is valid");
    check(mloop.vertexCount() == std::size_t(4 * ringVerts), "closed path emits no caps");
    check(mloop.triangleCount() == std::size_t(4 * 2 * segments), "closed path wraps the last station to the first");

    // Degenerate inputs return an empty mesh rather than crashing.

    check(ExtrusionBuilder::build(profile, {}, opt).empty(), "no frames yields an empty mesh");
    check(ExtrusionBuilder::build(ExtrusionProfile(), frames, opt).empty(), "no profile yields an empty mesh");
}

// ------------------------------------------------------------
int main()
{
    testProfile();
    testFrames();
    testBuilder();

    std::printf("\n%d checks, %d failures\n", g_checks, g_failures);

    return (g_failures == 0) ? 0 : 1;
}
