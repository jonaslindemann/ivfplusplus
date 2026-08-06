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

#include <vector>

namespace ivf {

/**
 * An oriented coordinate frame at a station (sample point) along a path.
 *
 * Each frame carries a position and an orthonormal basis (tangent, normal,
 * binormal). The cross section of an extrusion is placed in the
 * (normal, binormal) plane at each frame. The v member holds the cumulative
 * arc length along the path, used to generate the V texture coordinate.
 */
struct PathFrame {
    glm::vec3 position{0.0f};             // Position of the frame
    glm::vec3 tangent{0.0f, 0.0f, 1.0f};  // Unit tangent (direction of travel)
    glm::vec3 normal{1.0f, 0.0f, 0.0f};   // Unit normal (profile local x axis)
    glm::vec3 binormal{0.0f, 1.0f, 0.0f}; // Unit binormal (profile local y axis)
    float v{0.0f};                        // Cumulative arc length along the path

    /**
     * Position of this station in control point space.
     *
     * Stations are not one to one with the control points that produced them:
     * the Raw, Cut and Round joins insert extra stations around a corner, and
     * CatmullRom resamples the spine entirely. This value is the (fractional)
     * control point index the station sits at, so per control point data --
     * colors, twist, section scale -- can be interpolated onto the stations.
     */
    float source{0.0f};
};

/** How control points are turned into a continuous spine. */
enum class SpineInterp {
    Polyline,  // Use the control points directly (piecewise linear)
    CatmullRom // Smoothly interpolate the control points with a cubic spline
};

/** Method used to assign the rotational orientation of frames along the path. */
enum class FrameMethod {
    RotationMinimizing, // Twist free frames (double reflection method). Recommended.
    Frenet,             // Curvature based frames (may flip at inflection points)
    FixedUp             // Keep the normal aligned to a fixed up vector where possible
};

/**
 * Corner treatment for piecewise linear (Polyline) spines.
 *
 * These correspond to the TUBE_JN_* join styles of the gle library. Only
 * meaningful for SpineInterp::Polyline. For closed polylines anything other
 * than Angle currently falls back to Angle.
 */
enum class JoinStyle {
    Raw,   // Sections kept perpendicular to each segment (sharp, may pinch)
    Angle, // Mitered: a single section in the corner bisecting plane
    Cut,   // Flat chamfer across the corner
    Round  // Rounded corner formed by an arc of sections
};

/**
 * Samples a set of control points into a list of positions.
 *
 * @param controlPoints Control points.
 * @param interp Polyline returns the control points unchanged, CatmullRom
 * samples a spline through them.
 * @param samples Number of samples used for CatmullRom interpolation.
 */
IVF_API std::vector<glm::vec3> sampleSpine(const std::vector<glm::vec3> &controlPoints, SpineInterp interp,
                                           int samples);

/**
 * Builds a complete list of oriented frames along a path defined by control
 * points.
 *
 * This is the main entry point used by the extrusion shapes. It samples and
 * joins the control points into stations (positions and tangents) and then
 * assigns twist free orientations.
 *
 * @param controlPoints Control points of the path.
 * @param interp Polyline or CatmullRom.
 * @param join Corner treatment (Polyline only).
 * @param samples Number of samples for CatmullRom.
 * @param frameMethod How to orient the frames.
 * @param upHint Reference up vector: the direction the profile's local +y
 * axis (the binormal) points at the start of the path. Auto chosen when near
 * zero.
 * @param closed Treat the path as a closed loop.
 * @param cornerRadius Corner radius used by the Cut and Round joins.
 * @param cornerSegments Number of arc segments used by the Round join.
 */
IVF_API std::vector<PathFrame> buildPathFrames(const std::vector<glm::vec3> &controlPoints,
                                               SpineInterp interp = SpineInterp::Polyline,
                                               JoinStyle join = JoinStyle::Angle, int samples = 64,
                                               FrameMethod frameMethod = FrameMethod::RotationMinimizing,
                                               const glm::vec3 &upHint = glm::vec3(0.0f), bool closed = false,
                                               float cornerRadius = 0.0f, int cornerSegments = 6);

/**
 * Assigns normals and binormals to frames that already have positions and
 * tangents set.
 *
 * Implements the double reflection rotation minimizing frame propagation
 * (Wang et al. 2008), with optional closed loop twist correction.
 */
IVF_API void assignFrameOrientations(std::vector<PathFrame> &frames, FrameMethod frameMethod,
                                     const glm::vec3 &upHint, bool closed);

} // namespace ivf
