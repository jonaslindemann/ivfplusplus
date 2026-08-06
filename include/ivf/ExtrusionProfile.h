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

#pragma once

#include <ivfdef/Def.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>

namespace ivf {

/**
 * A 2D cross section (profile) to be swept along a 3D path.
 *
 * An ExtrusionProfile holds an ordered list of 2D points describing a cross
 * section in the local (normal, binormal) plane of a path frame. The profile
 * may be open or closed and can optionally carry explicit per-point 2D
 * normals. When no normals are supplied, smooth normals are computed from the
 * adjacent edges.
 *
 * A set of static factory methods construct common profiles (circle,
 * rectangle, rounded rectangle, regular polygon, arbitrary polyline).
 *
 * @author Jonas Lindemann
 */
class IVF_API ExtrusionProfile {
private:
    std::vector<glm::vec2> m_points;  // Cross section points in profile space
    std::vector<glm::vec2> m_normals; // Optional explicit per-point 2D normals
    bool m_closed;                    // Whether the profile forms a closed loop

public:
    /** ExtrusionProfile constructor. Creates an empty, closed profile. */
    ExtrusionProfile();

    /**
     * ExtrusionProfile constructor.
     *
     * @param points Cross section points.
     * @param closed Whether the profile is a closed loop.
     */
    explicit ExtrusionProfile(std::vector<glm::vec2> points, bool closed = true);

    /**
     * Creates a circular profile.
     *
     * @param radius Circle radius.
     * @param segments Number of points around the circle.
     * @param start Start angle in radians.
     * @param sweep Sweep angle in radians (2*pi for a full circle).
     */
    static ExtrusionProfile circle(float radius = 1.0f, int segments = 32, float start = 0.0f,
                                   float sweep = glm::two_pi<float>());

    /** Creates a rectangular profile centered at the origin. */
    static ExtrusionProfile rectangle(float width = 1.0f, float height = 1.0f);

    /**
     * Creates a rounded rectangle profile centered at the origin.
     *
     * @param width Total width.
     * @param height Total height.
     * @param radius Corner radius.
     * @param cornerSegments Number of segments per rounded corner.
     */
    static ExtrusionProfile roundedRect(float width, float height, float radius, int cornerSegments = 4);

    /**
     * Creates a regular polygon profile.
     *
     * @param sides Number of sides.
     * @param radius Circumscribed radius.
     */
    static ExtrusionProfile regularPolygon(int sides, float radius = 1.0f);

    /** Creates a profile from an arbitrary list of 2D points. */
    static ExtrusionProfile polyline(const std::vector<glm::vec2> &points, bool closed = true);

    /** Appends a point. Discards any explicit normals. */
    void addPoint(const glm::vec2 &p);

    /** Replaces all points. Discards any explicit normals. */
    void setPoints(const std::vector<glm::vec2> &points);

    /** Removes all points and normals. */
    void clear();

    /** Returns the profile points. */
    const std::vector<glm::vec2> &points() const;

    /** Returns the number of profile points. */
    std::size_t pointCount() const;

    /** Sets whether the profile forms a closed loop. */
    void setClosed(bool closed);

    /** Returns the closed flag. */
    bool closed() const;

    /**
     * Sets explicit per-point 2D normals.
     *
     * The number of normals must match the number of points, otherwise they
     * are ignored and smooth normals are computed instead. See hasNormals().
     */
    void setNormals(const std::vector<glm::vec2> &normals);

    /** Returns the explicit normals, which may be empty. */
    const std::vector<glm::vec2> &normals() const;

    /** Returns true when usable explicit normals have been supplied. */
    bool hasNormals() const;

    /** Returns the geometric centroid of the profile points. */
    glm::vec2 centroid() const;

    /**
     * Computes smooth per-point 2D normals.
     *
     * Adjacent edge normals are averaged and oriented outward from the
     * centroid. Returns one normal per point.
     */
    std::vector<glm::vec2> computeSmoothNormals() const;
};

} // namespace ivf
