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

// Implementation of: public class ExtrusionBuilder

#include <ivf/ExtrusionBuilder.h>

#include <algorithm>
#include <cmath>

using namespace ivf;

namespace {

constexpr float kEps = 1e-8f;

// Apply the per-station scale and twist to a profile point.

glm::vec2 transformPoint(const glm::vec2 &p, const SectionTransform &st)
{
    glm::vec2 q(p.x * st.scale.x, p.y * st.scale.y);
    float c = std::cos(st.twist);
    float s = std::sin(st.twist);
    return glm::vec2(q.x * c - q.y * s, q.x * s + q.y * c);
}

// Apply the inverse scale and the twist to a profile normal, which keeps it
// perpendicular to the surface under a non-uniform scale. Returns a unit 2D
// normal.

glm::vec2 transformNormal(const glm::vec2 &n, const SectionTransform &st)
{
    float sx = (std::abs(st.scale.x) > kEps) ? st.scale.x : 1.0f;
    float sy = (std::abs(st.scale.y) > kEps) ? st.scale.y : 1.0f;

    glm::vec2 q(n.x / sx, n.y / sy);

    float len = glm::length(q);
    if (len > kEps)
        q /= len;
    else
        q = glm::vec2(1.0f, 0.0f);

    float c = std::cos(st.twist);
    float s = std::sin(st.twist);

    return glm::vec2(q.x * c - q.y * s, q.x * s + q.y * c);
}

glm::vec3 placePoint(const PathFrame &f, const glm::vec2 &q)
{
    return f.position + q.x * f.normal + q.y * f.binormal;
}

glm::vec3 worldNormal(const PathFrame &f, const glm::vec2 &n2)
{
    glm::vec3 n = n2.x * f.normal + n2.y * f.binormal;
    float len = glm::length(n);
    return (len > kEps) ? n / len : f.normal;
}

} // namespace

// ------------------------------------------------------------
MeshData ExtrusionBuilder::build(const ExtrusionProfile &profile, const std::vector<PathFrame> &frames,
                                 const ExtrusionOptions &options)
{
    MeshData data;

    int S = static_cast<int>(frames.size());
    int M = static_cast<int>(profile.pointCount());

    if ((S < 2) || (M < 2))
        return data;

    const std::vector<glm::vec2> &P = profile.points();
    std::vector<glm::vec2> pn = profile.hasNormals() ? profile.normals() : profile.computeSmoothNormals();

    bool closedProfile = profile.closed();
    bool closedPath = options.closedPath;

    float totalV = frames.back().v;
    if (totalV < kEps)
        totalV = 1.0f;

    auto sectionAt = [&](int s) -> SectionTransform {
        if (!options.sectionFn)
            return SectionTransform();
        return options.sectionFn(s, frames[s]);
    };

    auto vCoord = [&](const PathFrame &f) -> float {
        return (options.texMode == TexMode::Normalized) ? f.v / totalV : f.v;
    };

    int sEnd = closedPath ? S : S - 1;

    // Reserve up front. The wall vertex count is known exactly, and the caps
    // add at most M+1 vertices each.

    if (options.normalStyle == NormalStyle::Smooth)
    {
        int ringVerts = closedProfile ? M + 1 : M;

        // Cumulative profile arc length, used for the U coordinate.

        std::vector<float> ucum(ringVerts, 0.0f);

        for (int j = 1; j < ringVerts; j++)
        {
            int a = (j - 1) % M;
            int b = j % M;
            ucum[j] = ucum[j - 1] + glm::length(P[b] - P[a]);
        }

        float perim = ucum[ringVerts - 1];
        if (perim < kEps)
            perim = 1.0f;

        data.positions.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.normals.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.texCoords.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.colors.reserve(static_cast<std::size_t>(S) * ringVerts);

        for (int s = 0; s < S; s++)
        {
            SectionTransform st = sectionAt(s);
            float v = vCoord(frames[s]);

            for (int j = 0; j < ringVerts; j++)
            {
                int src = j % M;

                glm::vec2 q = transformPoint(P[src], st);
                glm::vec2 n2 = transformNormal(pn[src], st);

                data.positions.push_back(placePoint(frames[s], q));
                data.normals.push_back(worldNormal(frames[s], n2));
                data.texCoords.push_back(glm::vec2(ucum[j] / perim, v));
                data.colors.push_back(st.color);
            }
        }

        auto idx = [&](int s, int j) { return static_cast<unsigned int>(s * ringVerts + j); };

        for (int s = 0; s < sEnd; s++)
        {
            int s1 = (s + 1) % S;

            for (int j = 0; j < ringVerts - 1; j++)
            {
                unsigned int a = idx(s, j);
                unsigned int b = idx(s, j + 1);
                unsigned int c = idx(s1, j + 1);
                unsigned int d = idx(s1, j);

                data.indices.push_back(glm::uvec3(a, b, c));
                data.indices.push_back(glm::uvec3(a, c, d));
            }
        }
    }
    else // Facet
    {
        int edgeCount = closedProfile ? M : M - 1;
        int ringVerts = edgeCount * 2;

        std::vector<float> edgeUStart(edgeCount, 0.0f);
        float acc = 0.0f;

        for (int e = 0; e < edgeCount; e++)
        {
            edgeUStart[e] = acc;
            int a = e;
            int b = (e + 1) % M;
            acc += glm::length(P[b] - P[a]);
        }

        float perim = acc;
        if (perim < kEps)
            perim = 1.0f;

        data.positions.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.normals.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.texCoords.reserve(static_cast<std::size_t>(S) * ringVerts);
        data.colors.reserve(static_cast<std::size_t>(S) * ringVerts);

        for (int s = 0; s < S; s++)
        {
            SectionTransform st = sectionAt(s);
            float v = vCoord(frames[s]);

            for (int e = 0; e < edgeCount; e++)
            {
                int a = e;
                int b = (e + 1) % M;

                glm::vec2 edRaw = P[b] - P[a];
                glm::vec2 enRaw(edRaw.y, -edRaw.x);

                // Orient the face normal to agree with the point normals, which
                // are already outward facing. This makes the result independent
                // of the winding order the caller supplied the profile in.

                if (glm::dot(enRaw, pn[a] + pn[b]) < 0.0f)
                    enRaw = -enRaw;

                glm::vec2 en2 = transformNormal(enRaw, st);
                glm::vec3 enW = worldNormal(frames[s], en2);

                glm::vec2 qa = transformPoint(P[a], st);
                glm::vec2 qb = transformPoint(P[b], st);

                data.positions.push_back(placePoint(frames[s], qa));
                data.normals.push_back(enW);
                data.texCoords.push_back(glm::vec2(edgeUStart[e] / perim, v));
                data.colors.push_back(st.color);

                data.positions.push_back(placePoint(frames[s], qb));
                data.normals.push_back(enW);
                data.texCoords.push_back(glm::vec2((edgeUStart[e] + glm::length(edRaw)) / perim, v));
                data.colors.push_back(st.color);
            }
        }

        auto idx = [&](int s, int k) { return static_cast<unsigned int>(s * ringVerts + k); };

        for (int s = 0; s < sEnd; s++)
        {
            int s1 = (s + 1) % S;

            for (int e = 0; e < edgeCount; e++)
            {
                unsigned int a = idx(s, 2 * e);
                unsigned int b = idx(s, 2 * e + 1);
                unsigned int c = idx(s1, 2 * e + 1);
                unsigned int d = idx(s1, 2 * e);

                data.indices.push_back(glm::uvec3(a, b, c));
                data.indices.push_back(glm::uvec3(a, c, d));
            }
        }
    }

    // Caps. Only meaningful for a closed profile that is not swept around a
    // closed loop.

    if (closedProfile && (M >= 3) && !closedPath)
    {
        auto addCap = [&](int stationIdx, bool start) {
            const PathFrame &f = frames[stationIdx];

            SectionTransform st = sectionAt(stationIdx);
            glm::vec3 nrm = start ? -f.tangent : f.tangent;

            std::vector<glm::vec2> q(M);
            glm::vec2 c2(0.0f);

            for (int i = 0; i < M; i++)
            {
                q[i] = transformPoint(P[i], st);
                c2 += q[i];
            }

            c2 /= static_cast<float>(M);

            float r = 0.0f;
            for (int i = 0; i < M; i++)
                r = std::max(r, glm::length(q[i] - c2));

            if (r < kEps)
                r = 1.0f;

            unsigned int base = static_cast<unsigned int>(data.positions.size());

            data.positions.push_back(placePoint(f, c2));
            data.normals.push_back(nrm);
            data.texCoords.push_back(glm::vec2(0.5f, 0.5f));
            data.colors.push_back(st.color);

            for (int i = 0; i < M; i++)
            {
                data.positions.push_back(placePoint(f, q[i]));
                data.normals.push_back(nrm);
                data.texCoords.push_back(0.5f + 0.5f * (q[i] - c2) / r);
                data.colors.push_back(st.color);
            }

            for (int i = 0; i < M; i++)
            {
                unsigned int center = base;
                unsigned int a = base + 1 + i;
                unsigned int b = base + 1 + ((i + 1) % M);

                if (start)
                    data.indices.push_back(glm::uvec3(center, b, a));
                else
                    data.indices.push_back(glm::uvec3(center, a, b));
            }
        };

        if (options.capStart)
            addCap(0, true);
        if (options.capEnd)
            addCap(S - 1, false);
    }

    return data;
}
