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

// Implementation of: public class SweptExtrusion

#include <ivf/SweptExtrusion.h>

#include <ivf/config.h>
#include <ivf/rc.h>

#include <algorithm>
#include <cmath>

using namespace ivf;

namespace {

// The gle texture styles that are expressed in model space, and therefore
// correspond to a V coordinate running with the actual arc length rather than
// normalized over the spine. See GLE_TEXTURE_* in GL/gle.h.

bool isModelSpaceTextureStyle(int style)
{
    return (style >= 7) && (style <= 12);
}

// Reverse the winding of every triangle.
//
// buildFrames() mirrors the section frame to match the handedness gle sweeps
// in, and a mirror turns every triangle inside out: the vertex normals still
// point outward, but the winding now says they face the other way. That costs
// nothing while back faces are simply drawn, and everything as soon as they
// are treated differently -- under two sided lighting, which is the usual
// setting for this kind of model, GL lights a back facing polygon with the
// negated normal, so the surface comes out lit from the wrong side. Backface
// culling would drop it entirely.

void reverseWinding(MeshData &mesh)
{
    for (auto &tri : mesh.indices)
    {
        unsigned int t = tri.y;
        tri.y = tri.z;
        tri.z = t;
    }
}

} // namespace

// ------------------------------------------------------------
SweptExtrusion::SweptExtrusion()
    : Shape(), m_upVector(0.0, 1.0, 0.0), m_useColors(false), m_useTwist(false), m_selectScale(1.05), m_spineStart(0),
      m_spineEnd(0), m_textureMode(IVF_TEXTURE_NORMALIZED), m_joinStyle(IVF_JN_ANGLE), m_cornerRadius(0.0),
      m_cornerSegments(6), m_geometryDirty(true), m_selectDirty(true), m_topologyDirty(true), m_boundingRadius(0.0)
{
}

// ------------------------------------------------------------
SweptExtrusion::~SweptExtrusion()
{
}

// ------------------------------------------------------------
void SweptExtrusion::markGeometryDirty()
{
    m_geometryDirty = true;
    m_selectDirty = true;
    m_topologyDirty = true;
}

// ------------------------------------------------------------
void SweptExtrusion::markListDirty()
{
    markGeometryDirty();
    GLBase::markListDirty();
}

// ------------------------------------------------------------
void SweptExtrusion::refresh()
{
    markGeometryDirty();
}

// ------------------------------------------------------------
bool SweptExtrusion::useDisplayList()
{
    return false;
}

// ------------------------------------------------------------
void SweptExtrusion::setSectionSize(int size)
{
    if (size < 0)
        size = 0;

    // Callers commonly set the size and then fill the section in, but some
    // re-assert the same size every refresh. Resizing rather than clearing
    // keeps the latter from throwing away a section that has not changed.

    m_sectionCoords.resize(size, glm::dvec2(0.0));
    m_sectionNormals.resize(size, glm::dvec2(0.0));
    m_sectionNormalSet.resize(size, false);

    markGeometryDirty();
}

// ------------------------------------------------------------
int SweptExtrusion::getSectionSize()
{
    return static_cast<int>(m_sectionCoords.size());
}

// ------------------------------------------------------------
void SweptExtrusion::setSectionCoord(int pos, double x, double y)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_sectionCoords.size())))
    {
        m_sectionCoords[pos] = glm::dvec2(x, y);
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSectionCoord(int pos, double &x, double &y)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_sectionCoords.size())))
    {
        x = m_sectionCoords[pos].x;
        y = m_sectionCoords[pos].y;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setSectionNormal(int pos, double x, double y)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_sectionNormals.size())))
    {
        m_sectionNormals[pos] = glm::dvec2(x, y);

        // A zero normal carries no direction. Treat it as unset so it gets
        // computed from the section edges instead of blackening a facet.

        m_sectionNormalSet[pos] = ((x * x + y * y) > 1e-12);

        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSectionNormal(int pos, double &x, double &y)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_sectionNormals.size())))
    {
        x = m_sectionNormals[pos].x;
        y = m_sectionNormals[pos].y;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setSpineSize(int size)
{
    if (size < 0)
        size = 0;

    m_spineCoords.resize(size, glm::dvec3(0.0));

    // Twist and section scale go back to their neutral values, matching the
    // behaviour of the gle backed class.

    m_spineTwist.assign(size, 0.0);
    m_spineScale.assign(size, glm::dvec2(1.0, 1.0));

    // Spine colors start out at the material's diffuse color, so switching
    // the color flag on without setting any colors does not turn the shape
    // black.

    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

    Material *material = this->getMaterial();

    if (material != nullptr)
        material->getDiffuseColor(color[0], color[1], color[2], color[3]);

    m_spineColors.assign(size, color);

    markGeometryDirty();
}

// ------------------------------------------------------------
int SweptExtrusion::getSpineSize()
{
    return static_cast<int>(m_spineCoords.size());
}

// ------------------------------------------------------------
void SweptExtrusion::setSpineCoord(int pos, double x, double y, double z)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineCoords.size())))
    {
        m_spineCoords[pos] = glm::dvec3(x, y, z);
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSpineCoord(int pos, double &x, double &y, double &z)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineCoords.size())))
    {
        x = m_spineCoords[pos].x;
        y = m_spineCoords[pos].y;
        z = m_spineCoords[pos].z;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setSpineColor(int pos, float red, float green, float blue)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineColors.size())))
    {
        m_spineColors[pos] = glm::vec4(red, green, blue, m_spineColors[pos].a);

        // Only the colors changed, so the mesh keeps its topology. The
        // regeneration path notices this and updates the buffer in place.

        m_geometryDirty = true;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSpineColor(int pos, float &red, float &green, float &blue)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineColors.size())))
    {
        red = m_spineColors[pos].r;
        green = m_spineColors[pos].g;
        blue = m_spineColors[pos].b;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setUseColor(bool flag)
{
    m_useColors = flag;
}

// ------------------------------------------------------------
bool SweptExtrusion::getUseColor()
{
    return m_useColors;
}

// ------------------------------------------------------------
void SweptExtrusion::setSelectScale(double scale)
{
    if (m_selectScale != scale)
    {
        m_selectScale = scale;
        m_selectDirty = true;
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setUpVector(double x, double y, double z)
{
    glm::dvec3 up(x, y, z);

    if (m_upVector != up)
    {
        m_upVector = up;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setTextureMode(int mode)
{
    int resolved;

    if ((mode == IVF_TEXTURE_NORMALIZED) || (mode == IVF_TEXTURE_PER_LENGTH))
        resolved = mode;
    else
        resolved = isModelSpaceTextureStyle(mode & 0xff) ? IVF_TEXTURE_PER_LENGTH : IVF_TEXTURE_NORMALIZED;

    if (m_textureMode != resolved)
    {
        m_textureMode = resolved;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
int SweptExtrusion::getTextureMode()
{
    return m_textureMode;
}

// ------------------------------------------------------------
void SweptExtrusion::setJoinStyle(int style)
{
    if (m_joinStyle != style)
    {
        m_joinStyle = style;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
int SweptExtrusion::getJoinStyle()
{
    return m_joinStyle;
}

// ------------------------------------------------------------
void SweptExtrusion::setCornerRadius(double radius)
{
    if (m_cornerRadius != radius)
    {
        m_cornerRadius = radius;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setCornerSegments(int segments)
{
    if (m_cornerSegments != segments)
    {
        m_cornerSegments = segments;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::setStartEnd(int startIdx, int endIdx)
{
    m_spineStart = startIdx;
    m_spineEnd = endIdx;
    markGeometryDirty();
}

// ------------------------------------------------------------
void SweptExtrusion::setUseTwist(bool flag)
{
    if (m_useTwist != flag)
    {
        m_useTwist = flag;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
bool SweptExtrusion::getUseTwist()
{
    return m_useTwist;
}

// ------------------------------------------------------------
void SweptExtrusion::setSpineTwist(int pos, double twist)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineTwist.size())))
    {
        m_spineTwist[pos] = twist;
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSpineTwist(int pos, double &twist)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineTwist.size())))
        twist = m_spineTwist[pos];
}

// ------------------------------------------------------------
void SweptExtrusion::setSpineScale(int pos, double sx, double sy)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineScale.size())))
    {
        m_spineScale[pos] = glm::dvec2(sx, sy);
        markGeometryDirty();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::getSpineScale(int pos, double &sx, double &sy)
{
    if ((pos >= 0) && (pos < static_cast<int>(m_spineScale.size())))
    {
        sx = m_spineScale[pos].x;
        sy = m_spineScale[pos].y;
    }
}

// ------------------------------------------------------------
ExtrusionProfile SweptExtrusion::buildProfile(double scale) const
{
    std::vector<glm::vec2> points;
    points.reserve(m_sectionCoords.size());

    for (const auto &p : m_sectionCoords)
        points.push_back(glm::vec2(static_cast<float>(p.x * scale), static_cast<float>(p.y * scale)));

    // The convention this API grew up with is that a closed section repeats
    // its first point at the end -- both SolidLine's circle and the sections
    // an application builds from a beam profile do. Drop the duplicate and
    // close the ring, rather than sweeping a zero length edge whose normal is
    // undefined. A section that does not repeat its endpoint is left open,
    // which is what gle does with it, unless the contour is explicitly
    // declared closed.

    bool closed = (m_joinStyle & IVF_CONTOUR_CLOSED) != 0;

    if (points.size() >= 2)
    {
        glm::vec2 d = points.back() - points.front();

        if (glm::dot(d, d) < 1e-12f)
        {
            points.pop_back();
            closed = true;
        }
    }

    ExtrusionProfile profile(points, closed);

    if (points.empty())
        return profile;

    // Explicit normals are used where they were given. Anything left unset
    // falls back to one computed from the neighbouring section edges. This
    // matters in practice: callers routinely fill in normals for every point
    // but the last, which under gle left that facet lit by whatever happened
    // to be in the array.

    bool anySet = false;

    for (std::size_t i = 0; i < points.size(); i++)
        if (m_sectionNormalSet[i])
        {
            anySet = true;
            break;
        }

    if (!anySet)
        return profile;

    std::vector<glm::vec2> normals = profile.computeSmoothNormals();

    for (std::size_t i = 0; i < points.size(); i++)
        if (m_sectionNormalSet[i])
            normals[i] = glm::normalize(
                glm::vec2(static_cast<float>(m_sectionNormals[i].x), static_cast<float>(m_sectionNormals[i].y)));

    profile.setNormals(normals);

    return profile;
}

// ------------------------------------------------------------
std::vector<PathFrame> SweptExtrusion::buildFrames() const
{
    std::vector<PathFrame> frames;

    int spineSize = static_cast<int>(m_spineCoords.size());

    // Resolve the active spine range. An end index of zero means "all of it",
    // which is how the gle backed class read these too.

    int start = 0;
    int count = spineSize;

    if ((m_spineStart >= 0) && (m_spineEnd > 0))
    {
        start = m_spineStart;
        count = m_spineEnd - m_spineStart + 1;
    }

    if (start < 0)
        start = 0;
    if (start >= spineSize)
        return frames;
    if (count > spineSize - start)
        count = spineSize - start;

    // Three vertices leave a single interior one, which is a section rather
    // than a tube. Four is the smallest spine that sweeps anything.

    if (count < 4)
        return frames;

    std::vector<glm::vec3> points;
    points.reserve(count);

    for (int i = 0; i < count; i++)
        points.push_back(glm::vec3(static_cast<float>(m_spineCoords[start + i].x),
                                   static_cast<float>(m_spineCoords[start + i].y),
                                   static_cast<float>(m_spineCoords[start + i].z)));

    JoinStyle join = JoinStyle::Angle;

    switch (m_joinStyle & IVF_JN_MASK)
    {
    case IVF_JN_RAW:
        join = JoinStyle::Raw;
        break;
    case IVF_JN_CUT:
        join = JoinStyle::Cut;
        break;
    case IVF_JN_ROUND:
        join = JoinStyle::Round;
        break;
    default:
        join = JoinStyle::Angle;
        break;
    }

    glm::vec3 up(static_cast<float>(m_upVector.x), static_cast<float>(m_upVector.y),
                 static_cast<float>(m_upVector.z));

    std::vector<PathFrame> all =
        buildPathFrames(points, SpineInterp::Polyline, join, 64, FrameMethod::RotationMinimizing, up, false,
                        static_cast<float>(m_cornerRadius), m_cornerSegments);

    // Drop the stations belonging to the two end vertices. They exist to give
    // the ends a direction, and gle does not sweep them either. Everything
    // between the first and last control point survives, including any extra
    // stations a corner join inserted.

    float firstSource = 1.0f;
    float lastSource = static_cast<float>(count - 2);

    frames.reserve(all.size());

    for (const auto &f : all)
        if ((f.source >= firstSource - 1e-4f) && (f.source <= lastSource + 1e-4f))
            frames.push_back(f);

    if (frames.size() < 2)
    {
        frames.clear();
        return frames;
    }

    // The arc length now starts partway along the spine. Re-base it so the V
    // texture coordinate still runs from zero.

    float base = frames.front().v;

    for (auto &f : frames)
    {
        f.v -= base;
        f.source += static_cast<float>(start);

        // Match the handedness of the section frame gle uses.
        //
        // buildPathFrames() returns a right-handed frame, where the section's
        // local axes satisfy normal x binormal = +tangent. gle orients each
        // segment so that it runs along the section's local -z, which makes
        // its section frame left-handed. The two therefore differ by a mirror
        // about the up vector.
        //
        // The mirror is invisible for a section that is symmetric about that
        // axis -- circles, rectangles, I profiles -- which is most of them,
        // but it flips any section that is not, and it reverses the direction
        // a twist appears to turn. Flipping the normal here puts the section
        // in gle's frame while leaving the shared path code right-handed.

        f.normal = -f.normal;
    }

    return frames;
}

// ------------------------------------------------------------
SectionTransform SweptExtrusion::sectionAt(double spineIndex) const
{
    SectionTransform st;

    int n = static_cast<int>(m_spineCoords.size());

    if (n == 0)
        return st;

    // Stations and spine vertices are not one to one -- a corner join inserts
    // extra ones -- so blend between the two neighbouring spine vertices.

    double clamped = spineIndex;

    if (clamped < 0.0)
        clamped = 0.0;
    if (clamped > static_cast<double>(n - 1))
        clamped = static_cast<double>(n - 1);

    int i0 = static_cast<int>(std::floor(clamped));
    int i1 = (i0 + 1 < n) ? (i0 + 1) : (n - 1);
    double t = clamped - static_cast<double>(i0);

    if (i0 < static_cast<int>(m_spineScale.size()) && i1 < static_cast<int>(m_spineScale.size()))
    {
        glm::dvec2 s = m_spineScale[i0] * (1.0 - t) + m_spineScale[i1] * t;
        st.scale = glm::vec2(static_cast<float>(s.x), static_cast<float>(s.y));
    }

    if (m_useTwist && (i0 < static_cast<int>(m_spineTwist.size())) && (i1 < static_cast<int>(m_spineTwist.size())))
    {
        // The twist is given in degrees, as it is in gle.

        double twist = m_spineTwist[i0] * (1.0 - t) + m_spineTwist[i1] * t;
        st.twist = static_cast<float>(twist * M_PI / 180.0);
    }

    if (i0 < static_cast<int>(m_spineColors.size()) && i1 < static_cast<int>(m_spineColors.size()))
        st.color = m_spineColors[i0] * static_cast<float>(1.0 - t) + m_spineColors[i1] * static_cast<float>(t);

    return st;
}

// ------------------------------------------------------------
void SweptExtrusion::updateGeometry()
{
    m_geometryDirty = false;

    MeshData empty;

    if (m_sectionCoords.size() < 2)
    {
        m_buffer.setMesh(empty);
        m_boundingRadius = 0.0;
        return;
    }

    std::vector<PathFrame> frames = buildFrames();

    if (frames.size() < 2)
    {
        m_buffer.setMesh(empty);
        m_boundingRadius = 0.0;
        return;
    }

    ExtrusionProfile profile = buildProfile(1.0);

    ExtrusionOptions options;

    // gle only closes the ends when the cap bit is set, and the default join
    // style does not set it.

    options.capStart = (m_joinStyle & IVF_JN_CAP) != 0;
    options.capEnd = options.capStart;
    options.closedPath = false;
    options.normalStyle = ((m_joinStyle & IVF_NORM_MASK) == IVF_NORM_FACET) ? NormalStyle::Facet : NormalStyle::Smooth;
    options.texMode = (m_textureMode == IVF_TEXTURE_PER_LENGTH) ? TexMode::PerLength : TexMode::Normalized;
    options.sectionFn = [this](int, const PathFrame &f) { return sectionAt(static_cast<double>(f.source)); };

    MeshData mesh = ExtrusionBuilder::build(profile, frames, options);

    reverseWinding(mesh);

    // Animating the spine colors is a per frame operation in an application
    // showing results along a beam, and it does not move a single vertex. Keep
    // the element buffer when only the per vertex attributes changed.

    if (!m_topologyDirty)
        m_buffer.setVertexData(mesh);
    else
        m_buffer.setMesh(mesh);

    m_topologyDirty = false;

    m_boundingRadius = m_buffer.boundingRadius();
}

// ------------------------------------------------------------
void SweptExtrusion::updateSelectGeometry()
{
    m_selectDirty = false;

    MeshData empty;

    if (m_sectionCoords.size() < 2)
    {
        m_selectBuffer.setMesh(empty);
        return;
    }

    std::vector<PathFrame> frames = buildFrames();

    if (frames.size() < 2)
    {
        m_selectBuffer.setMesh(empty);
        return;
    }

    // The select shape is the same sweep with a slightly larger section, so it
    // encloses the shape rather than fighting it in the depth buffer.

    ExtrusionProfile profile = buildProfile(m_selectScale);

    ExtrusionOptions options;
    options.capStart = (m_joinStyle & IVF_JN_CAP) != 0;
    options.capEnd = options.capStart;
    options.texMode = TexMode::Normalized;
    options.sectionFn = [this](int, const PathFrame &f) { return sectionAt(static_cast<double>(f.source)); };

    MeshData selectMesh = ExtrusionBuilder::build(profile, frames, options);

    reverseWinding(selectMesh);

    m_selectBuffer.setMesh(selectMesh);
}

// ------------------------------------------------------------
void SweptExtrusion::doCreateGeometry()
{
    if (m_geometryDirty)
        updateGeometry();

    if (m_buffer.isEmpty())
        return;

    if (rcIsShaderActive())
    {
        rcUseShader();
        rcUpdateShader();

        ShaderProgram *prog = rcShader();
        prog->setUniformInt("uUseVertexColor", m_useColors ? 1 : 0);

        m_buffer.drawShader(GL_TRIANGLES);

        // Leave the flag off. The next shape to draw sets what it needs, and
        // a stale flag would colour it from whatever is in its buffer.

        prog->setUniformInt("uUseVertexColor", 0);
        return;
    }

    // Fixed-function path. Per vertex colours only reach the lighting
    // equation through GL_COLOR_MATERIAL, which the application may already
    // have set up for its own reasons -- so leave it alone if it is on.

    GLboolean hadColorMaterial = glIsEnabled(GL_COLOR_MATERIAL);

    if (m_useColors && !hadColorMaterial)
    {
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
    }

    m_buffer.drawFixedFunction(GL_TRIANGLES, m_useColors);

    if (m_useColors && !hadColorMaterial)
        glDisable(GL_COLOR_MATERIAL);

    if (m_useColors)
    {
        // Drawing through a colour array with GL_COLOR_MATERIAL engaged
        // rewrites the ambient and diffuse material behind Material's back,
        // and the last colour drawn stays there after it is disabled again.
        // Material caches what it last uploaded and skips redundant uploads,
        // so without this the next object sharing that material draws in
        // whatever colour this one left behind -- typically the dark end of a
        // result ramp.
        //
        // The gle backed class never had to do this: it cached its geometry
        // and its material together in a display list, and replaying the list
        // re-issued the material calls every frame.

        Material::invalidateStateCache();
    }
}

// ------------------------------------------------------------
void SweptExtrusion::doCreateSelect()
{
    if (m_selectDirty)
        updateSelectGeometry();

    if (m_selectBuffer.isEmpty())
        return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (rcIsShaderActive())
    {
        rcUseShader();
        rcUpdateShader();

        ShaderProgram *prog = rcShader();

        prog->setUniformInt("uUnlit", 1);
        prog->setUniformInt("uUseVertexColor", 0);
        prog->setUniformVec4("uMatDiffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        m_selectBuffer.drawShader(GL_TRIANGLES);

        prog->setUniformInt("uUnlit", 0);

        // doCreateMaterial() runs after this and restores the real material
        // uniforms, so uMatDiffuse does not need putting back here.
    }
    else
    {
        GLboolean hadLighting = glIsEnabled(GL_LIGHTING);

        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);

        m_selectBuffer.drawFixedFunction(GL_TRIANGLES, false);

        if (hadLighting)
            glEnable(GL_LIGHTING);

        // Setting the current colour tracks into the material whenever
        // GL_COLOR_MATERIAL happens to be enabled, so the cache can no longer
        // be trusted here either.

        if (glIsEnabled(GL_COLOR_MATERIAL))
            Material::invalidateStateCache();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ------------------------------------------------------------
void SweptExtrusion::doUpdateBoundingSphere()
{
    if (getBoundingSphere() == nullptr)
        return;

    if (m_geometryDirty)
        updateGeometry();

    getBoundingSphere()->setRadius(m_boundingRadius);
}
