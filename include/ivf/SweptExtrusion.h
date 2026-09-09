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
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivf/ExtrusionBuilder.h>
#include <ivf/ExtrusionProfile.h>
#include <ivf/MeshBuffer.h>
#include <ivf/PathFrames.h>
#include <ivf/Shape.h>

#include <glm/glm.hpp>

#include <vector>

namespace ivf {

IvfSmartPointer(SweptExtrusion);

// Join styles. The values match the gle TUBE_JN_* constants, so code written
// against the gle backed Extrusion class keeps working unchanged.

const int IVF_JN_RAW = 0x1;
const int IVF_JN_ANGLE = 0x2;
const int IVF_JN_CUT = 0x3;
const int IVF_JN_ROUND = 0x4;
const int IVF_JN_MASK = 0xf;
const int IVF_JN_CAP = 0x10;

// Normal styles, again matching the gle TUBE_NORM_* constants.

const int IVF_NORM_FACET = 0x100;
const int IVF_NORM_EDGE = 0x200;
const int IVF_NORM_MASK = 0xf00;

// Forces the section to be treated as a closed ring even when its first point
// is not repeated at the end. Matches gle's TUBE_CONTOUR_CLOSED.

const int IVF_CONTOUR_CLOSED = 0x1000;

// Texture coordinate generation. Only the along-the-spine (V) coordinate
// differs between the modes. See setTextureMode().

const int IVF_TEXTURE_NORMALIZED = 0;
const int IVF_TEXTURE_PER_LENGTH = 1;

/**
 * Extrusion shape without the gle library
 *
 * SweptExtrusion sweeps a 2D section along a 3D spine, exactly as the
 * gle backed Extrusion class does, but generates the mesh itself and renders
 * it from a vertex buffer. It works both with the modern shader pipeline and
 * with the fixed-function pipeline of an OpenGL compatibility profile, and
 * needs neither the gle library nor display lists.
 *
 * The public interface is that of Extrusion, so the two are interchangeable
 * at the call site:
 *
 * \code
 *   SweptExtrusion* extrusion = new SweptExtrusion();
 *
 *   // Section
 *
 *   r = 0.5;
 *   nSides = 12;
 *
 *   extrusion->setSectionSize(nSides + 1);
 *
 *   for (i = 0; i<=nSides; i++)
 *   {
 *       angle = 2.0*M_PI*( ((double)i) / ((double)nSides) );
 *       x = r * cos(angle);
 *       y = r * sin(angle);
 *       extrusion->setSectionCoord(i, x, y);
 *       extrusion->setSectionNormal(i, x/r, y/r);
 *   }
 *
 *   // Spine
 *
 *   extrusion->setSpineSize(6);
 *   extrusion->setSpineCoord(0,  0.5,  0.0,  1.5);
 *   ...
 *
 *   extrusion->setUpVector(0.0, 1.0, 0.0);
 *   extrusion->setJoinStyle(IVF_JN_ANGLE);
 * \endcode
 *
 * As in gle, the first and last spine vertices are not swept. They only
 * establish the orientation of the section at the two ends, so a spine of
 * n vertices produces a tube through the n-2 interior ones. This is why the
 * spine size is documented as "actual vertices + 2".
 *
 * Geometry is regenerated lazily: the setters only mark the shape dirty, and
 * the mesh is rebuilt on the next render. Setting the same spine repeatedly
 * between frames therefore costs one rebuild, not one per call.
 *
 * @author Jonas Lindemann
 */
class IVF_API SweptExtrusion : public Shape {
private:
    // Section, in the local (normal, binormal) plane of each spine station.

    std::vector<glm::dvec2> m_sectionCoords;
    std::vector<glm::dvec2> m_sectionNormals;
    std::vector<bool> m_sectionNormalSet;

    // Spine, in the shape's own coordinate system, with the per vertex
    // attributes that ride along with it.

    std::vector<glm::dvec3> m_spineCoords;
    std::vector<glm::vec4> m_spineColors;
    std::vector<double> m_spineTwist;
    std::vector<glm::dvec2> m_spineScale;

    glm::dvec3 m_upVector;

    bool m_useColors;
    bool m_useTwist;
    double m_selectScale;
    int m_spineStart;
    int m_spineEnd;
    int m_textureMode;
    int m_joinStyle;
    double m_cornerRadius;
    int m_cornerSegments;

    MeshBuffer m_buffer;
    MeshBuffer m_selectBuffer;

    bool m_geometryDirty;
    bool m_selectDirty;

    /**
     * Whether anything other than the per vertex attributes changed.
     *
     * Recolouring a spine leaves the mesh topology alone, which lets the
     * vertex buffer be updated in place instead of respecified. Inferring
     * that from the vertex count is not safe -- a different spine and section
     * size can multiply out to the same count -- so track it directly.
     */
    bool m_topologyDirty;

    double m_boundingRadius;

    /** Builds the section as an ExtrusionProfile, filling in missing normals. */
    ExtrusionProfile buildProfile(double scale) const;

    /**
     * Builds the swept stations.
     *
     * Frames are generated across the whole active spine range and the two
     * end vertices are then dropped, which is how gle uses them: to orient
     * the ends without being swept themselves.
     */
    std::vector<PathFrame> buildFrames() const;

    /** Interpolates the per spine vertex attributes onto a station. */
    SectionTransform sectionAt(double spineIndex) const;

    /** Regenerates the mesh into the vertex buffer. */
    void updateGeometry();

    /** Regenerates the scaled mesh used to indicate selection. */
    void updateSelectGeometry();

    /** Marks the mesh, and the select mesh, as needing regeneration. */
    void markGeometryDirty();

public:
    /** SweptExtrusion constructor. */
    SweptExtrusion();

    /** SweptExtrusion destructor. */
    virtual ~SweptExtrusion();

    IvfClassInfo("SweptExtrusion", Shape);
    IvfStdFactory(SweptExtrusion);

	/** Draws through the shader when one is active. */
	virtual bool hasModernPath() override;


    /**
     * Sets section size
     *
     * The size specifies the number of points and normals that make up the
     * section. Existing coordinates are preserved where the size allows.
     *
     * @param size Number of vertices in the section.
     */
    void setSectionSize(int size);

    /** Returns the section size. */
    int getSectionSize();

    /**
     * Set section coordinate
     *
     * The section coordinates are specified in a 2D coordinate space. The
     * spine passes through (0,0).
     *
     * @param pos Section coordinate index (0 to getSectionSize()-1)
     * @param x Section x coordinate.
     * @param y Section y coordinate.
     */
    void setSectionCoord(int pos, double x, double y);

    /** Returns a section coordinate. */
    void getSectionCoord(int pos, double &x, double &y);

    /**
     * Set section normal
     *
     * Section normals are specified in the same 2D coordinate space as the
     * coordinates. Normals that are never set, or that are set to zero, are
     * computed from the adjacent section edges instead, so a section may be
     * given without normals at all.
     *
     * @param pos Section normal index (0 to getSectionSize()-1)
     * @param x Section normal x component.
     * @param y Section normal y component.
     */
    void setSectionNormal(int pos, double x, double y);

    /** Returns a section normal. */
    void getSectionNormal(int pos, double &x, double &y);

    /**
     * Set spine size
     *
     * The first and last spine vertices determine the orientation of the
     * section at the two ends and are not swept themselves, so the size is
     * the number of drawn vertices plus two.
     *
     * Resets the per vertex twist and section scale. Spine colors are reset
     * to the current material's diffuse color.
     *
     * @param size Number of vertices in the spine (actual vertices + 2)
     */
    void setSpineSize(int size);

    /** Returns the spine size. */
    int getSpineSize();

    /**
     * Set spine coordinate
     *
     * @param pos Spine coordinate index (0 to getSpineSize()-1)
     * @param x Spine x coordinate
     * @param y Spine y coordinate
     * @param z Spine z coordinate
     */
    void setSpineCoord(int pos, double x, double y, double z);

    /** Returns a spine coordinate. */
    void getSpineCoord(int pos, double &x, double &y, double &z);

    /**
     * Set spine color
     *
     * A color can be specified at each spine vertex, used when the UseColor
     * flag is set.
     *
     * @param pos Spine color index (0 to getSpineSize()-1)
     * @param red Red color component.
     * @param green Green color component.
     * @param blue Blue color component.
     */
    void setSpineColor(int pos, float red, float green, float blue);

    /** Returns a spine color. */
    void getSpineColor(int pos, float &red, float &green, float &blue);

    /**
     * Set use color flag
     *
     * When set, the extrusion is drawn using the colors given by
     * setSpineColor() instead of a single material color.
     *
     * @param flag true to use the spine colors, false to use the material.
     */
    void setUseColor(bool flag);

    /** Returns the UseColor flag. */
    bool getUseColor();

    /**
     * Set selection scaling
     *
     * Sets the scale factor of the section used for the select shape.
     * (default 1.05)
     *
     * @param scale 1.0 = same size as the extrusion.
     */
    void setSelectScale(double scale);

    /**
     * Draw only the swept surface, with no transform or material of its own.
     *
     * Lets another shape borrow this class's geometry from inside its own
     * render pass -- Extrusion delegates its modern path here rather than
     * duplicating the sweep, so both classes produce the same surface from the
     * same code. Assumes the caller has already established the transform and
     * material, which is exactly what doCreateGeometry() can rely on.
     */
    void drawGeometry();

    /** As drawGeometry(), for the scaled select shape. */
    void drawSelectGeometry();

    /**
     * Set the up vector
     *
     * Determines the orientation of the section at the start of the spine.
     * The orientation is then carried along the spine without adding twist.
     */
    void setUpVector(double x, double y, double z);

    /**
     * Sets the texture mode used
     *
     * The section always produces a U coordinate running around its
     * perimeter. This selects how the V coordinate runs along the spine:
     *
     * \code
     * IVF_TEXTURE_NORMALIZED   V runs from 0 to 1 over the whole spine
     * IVF_TEXTURE_PER_LENGTH   V equals the distance along the spine
     * \endcode
     *
     * The gle GLE_TEXTURE_* values are accepted: the model space styles map
     * to IVF_TEXTURE_PER_LENGTH and everything else to the normalized mode.
     */
    void setTextureMode(int mode);

    /** Returns the texture mode. */
    int getTextureMode();

    /**
     * Sets the join style used
     *
     * Determines how the section is carried around a corner in the spine:
     *
     * \code
     * IVF_JN_RAW    Sections stay perpendicular to each segment
     * IVF_JN_ANGLE  Mitered, one section in the corner's bisecting plane
     * IVF_JN_CUT    Flat chamfer across the corner
     * IVF_JN_ROUND  Rounded corner formed by an arc of sections
     * IVF_JN_CAP    Or'ed in to close the two ends of the extrusion
     * \endcode
     *
     * The gle TUBE_JN_* values have the same numeric values and may be used
     * interchangeably. Normal style bits are honoured: IVF_NORM_FACET
     * produces flat shaded walls, anything else smooth ones.
     */
    void setJoinStyle(int style);

    /** Returns the join style. */
    int getJoinStyle();

    /** Sets the corner radius used by the Cut and Round join styles. */
    void setCornerRadius(double radius);

    /** Sets the number of arc segments used by the Round join style. */
    void setCornerSegments(int segments);

    /**
     * Set start and end index for the spine
     *
     * Restricts the extrusion to a part of the spine. As for the full spine,
     * the first and last vertices of the range orient the ends without being
     * swept. Passing an end index of 0 restores the full spine.
     */
    void setStartEnd(int startIdx, int endIdx);

    /** Enable or disable twisting. */
    void setUseTwist(bool flag);

    /** Returns the twisting state. */
    bool getUseTwist();

    /**
     * Sets the spine twist at a given spine position
     *
     * The twist is given in degrees and rotates the section about the spine.
     * Twisting must be enabled with setUseTwist() for it to take effect.
     */
    void setSpineTwist(int pos, double twist);

    /** Returns the spine twist at a given spine position. */
    void getSpineTwist(int pos, double &twist);

    /**
     * Sets the section scale at a given spine position
     *
     * Scales the section in its own x and y directions, which tapers the
     * extrusion. A scale of zero collapses the section to a point, which is
     * how conical end caps are made.
     */
    void setSpineScale(int pos, double sx, double sy);

    /** Returns the section scale at a given spine position. */
    void getSpineScale(int pos, double &sx, double &sy);

    /** Forces the geometry to be regenerated on the next render. */
    virtual void refresh() override;

    /**
     * Marks the generated mesh as out of date.
     *
     * The mesh is regenerated on the next render. The property setters do
     * this on their own; call it directly after changing something they
     * cannot see.
     */
    virtual void markListDirty() override;

    /**
     * Updates the bounding sphere
     *
     * Uses the extent of the generated mesh, so it accounts for the section
     * as well as the spine.
     */
    virtual void doUpdateBoundingSphere() override;

protected:
    virtual void doCreateSelect() override;
    virtual void doCreateGeometry() override;

    /**
     * Never replays a display list.
     *
     * The vertex buffer already is the cached representation, and compiling
     * a display list around a buffer draw only adds a second thing to keep
     * up to date.
     */
    virtual bool useDisplayList() override;
};

} // namespace ivf
