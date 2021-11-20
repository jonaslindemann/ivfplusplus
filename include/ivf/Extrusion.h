//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#pragma once

#include <GL/gle.h>

#include <ivf/Shape.h>

namespace ivf {

void ivfSetGLEJoinStyle(int style);
int ivfGetGLEJoinStyle();

IvfSmartPointer(CExtrusion);

/**
 * Extrusion shape class
 *
 * CIvfExtrusion implements an extrusion shape using the
 * gle library functions. A spine and a section are 
 * specified. Colors can also be set at each spine 
 * vertex. The following code shows how the extrusion class
 * is used:
 *
 * \code
 * 	// Create extrusion
 * 
 * 	CIvfExtrusion* extrusion = new CIvfExtrusion();
 * 	
 * 	// Create section
 * 	
 * 	r = 0.5;
 * 	nSides = 12;
 * 	
 * 	extrusion->setSectionSize(nSides + 1);
 * 	
 * 	for (i = 0; i<=nSides; i++)
 * 	{
 * 		angle = 2.0*M_PI*( ((double)i) / ((double)nSides) );
 * 		x = r * cos(angle);
 * 		y = r * sin(angle);
 * 		extrusion->setSectionCoord(i, x, y);
 * 		extrusion->setSectionNormal(i, x/r, y/r);
 * 	}
 * 	
 * 	// Set spine
 * 
 * 	extrusion->setSpineSize(6);
 * 	extrusion->setSpineCoord(0,  0.5,  0.0,  1.5);
 * 	extrusion->setSpineCoord(1,  1.0,  0.0,  1.0);
 * 	extrusion->setSpineCoord(2,  1.0,  0.0, -1.0);
 * 	extrusion->setSpineCoord(3, -1.0,  0.0, -1.0);
 * 	extrusion->setSpineCoord(4, -1.0,  0.0,  1.0);
 * 	extrusion->setSpineCoord(5, -0.5,  0.0,  1.5);
 * 
 * 	// Set up-vector
 * 	
 * 	extrusion->setUpVector(0.0, 1.0, 0.0);
 * 
 * 	// Set join style
 * 
 * 	extrusion->setJoinStyle(TUBE_NORM_EDGE|TUBE_JN_ANGLE|TUBE_JN_CAP);
 * 
 * \endcode
 * @author Jonas Lindemann
 * @version 1.0.0
 */
class IVF_API CExtrusion : public CShape {
public:
	/** CIvfExtrusion constructor. */
	CExtrusion ();

	/** CIvfExtrusion destructor. */
	virtual ~CExtrusion ();

	IvfClassInfo("CIvfExtrusion",CShape);

	/** 
	 * Sets section size
	 *
	 * The size specifies the number of points
	 * and normals that constructs the section.
	 * @param size Number of vertices in the section.
	 */
	void setSectionSize(int size);
	
	/** Returns the section size */
	int getSectionSize();

	/**
	 * Set section coordinate
	 *
	 * The section coordinates are specified in a 2D 
	 * coordinate space. The spine is located at (0,0)
	 * @param pos Section coordinate index. (0 to getSectionSize()-1)
	 * @param x Section x-coordinate.
	 * @param y Section y-coordinate.
	 */
	void setSectionCoord(int pos, double x, double y);

	/** Return section coordinate */
	void getSectionCoord(int pos, double &x, double &y);

	/**
	 * Set section normal
	 * 
	 * The section normals are specified in a 2D
	 * coordinate space. 
	 * @param pos Section normal index. (0 to getSectionSize()-1)
	 * @param x Section normal x component.
	 * @param y Section nromal y component.
	 */
	void setSectionNormal(int pos, double x, double y);

	/** Return section normal */
	void getSectionNormal(int pos, double &x, double &y);

	/** 
	 * Set spine size
	 *
	 * The spine size property specifies how many
	 * vertices the spine should consist of. The first
	 * and last spine vertices are used to determine
	 * the orientation of the section in the first
	 * and last sections.
	 * @param size Number of vertices in the spine (actual vertices + 2)
	 */
	void setSpineSize(int size);

	/** Returns spine size */
	int getSpineSize();

	/**
	 * Set spine coordinate 
	 *
	 * The spine coordinates are specified in the global coordinate
	 * system. 
	 * @param pos Spine coordinate index (0 to getSpineSize()-1)
	 * @param x Spine x coordinate
	 * @param y Spine y coordinate
	 * @param z Spine z coordinate
	 */
	void setSpineCoord(int pos, double x, double y, double z);

	/** Return spine coordinate */
	void getSpineCoord(int pos, double &x, double &y, double &z);

	/**
	 * Set spine color
	 *
	 * A color can be specified at each spine vertex. This 
	 * color used if the UseColor flag is set.
	 * @param pos Spine color index (0 to getSpineSize()-1)
	 * @param red Red color component.
	 * @param green Green color component.
	 * @param blue Blue color component.
	 */
	void setSpineColor(int pos, float red, float green, float blue);

	/** Return spine color */
	void getSpineColor(int pos, float &red, float &green, float &blue);

	/** 
	 * Set Use color flag 
	 * 
	 * If this flag is set to TRUE the spine is drawn using the colors
	 * specified by the setSpineColor() method.
	 * @param flag TRUE colors are used. FALSE standard material is used
	 * for the entire extrusion.
	 */
	void setUseColor(bool flag);

	/** Return UseColor flag */
	bool getUseColor();

	/** 
	 * Set selection scaling
	 *
	 * Sets the scalefactor on the select shape. (default 1.05)
	 * @param scale 1.0 = same size as extrusion.
	 */
	void setSelectScale(double scale);

	/** 
	 * Set the up-vector
	 *
	 * Determines the orientation of the section.
	 * @param x up vector x component
	 * @param y up vector y component
	 * @param z up vector z component
	 */
	void setUpVector(double x, double y, double z);

	/**
	 * Sets the texture mode used
	 *
	 * Texture modes corresponds to the defines in the GL/tube.h
	 * header file:
	 * /code
	 * GLE_TEXTURE_ENABLE
	 * GLE_TEXTURE_STYLE_MASK
	 * GLE_TEXTURE_VERTEX_FLAT
	 * GLE_TEXTURE_NORMAL_FLAT
	 * GLE_TEXTURE_VERTEX_CYL
	 * GLE_TEXTURE_NORMAL_CYL
	 * GLE_TEXTURE_VERTEX_SPH
	 * GLE_TEXTURE_NORMAL_SPH
	 * GLE_TEXTURE_VERTEX_MODEL_FLAT
	 * GLE_TEXTURE_NORMAL_MODEL_FLAT
	 * GLE_TEXTURE_VERTEX_MODEL_CYL
	 * GLE_TEXTURE_NORMAL_MODEL_CYL
	 * GLE_TEXTURE_VERTEX_MODEL_SPH
	 * GLE_TEXTURE_NORMAL_MODEL_SPH
	 * /endcode
	 */
	void setTextureMode(int mode);

	/**
	 * Sets the join style used
	 *
	 * Texture modes corresponds to the defines in the GL/tube.h
	 * header file:
	 * /code
	 * TUBE_JN_RAW
	 * TUBE_JN_ANGLE
	 * TUBE_JN_CUT
	 * TUBE_JN_ROUND
	 * TUBE_JN_MASK
	 * TUBE_JN_CAP
	 * /endcode
	 */
	void setJoinStyle(int style);

	/**
	 * Set start and end index for spine
	 *
	 * This method can be used when drawing only a part
	 * of the extrusion
	 */
	void setStartEnd(int startIdx, int endIdx);

	/** Enable or disable twisting */
	void setUseTwist(bool flag);

	/** Return twisting state */
	bool getUseTwist();

	/** 
	 * Sets the spine twist at a given spine position 
	 * 
	 * To use this method twisting should be enabled using
	 * the setUseTwist method.
	 */
	void setSpineTwist(int pos, double twist);

	/** 
	 * Returns the spine twist at a given spine position 
	 *
	 * To use this method twisting should be enabled using
	 * the setUseTwist method.
	 */
	void getSpineTwist(int pos, double &twist);
	
	/** 
	 * Updates bounding sphere
	 *
	 * Upates the radius of the assigned bounding sphere, if any.
	 */
	void doUpdateBoundingSphere();
private:
	double m_selectScale;
	bool m_useColors;
	bool m_useTwist;
	int m_sectionSize;
	int m_spineSize;
	int m_spineStart;
	int m_spineEnd;
	int m_spineDelta;
    int m_textureMode;
	gleDouble (*m_sectionCoords)[2];	
	gleDouble (*m_sectionNormals)[2];
	gleDouble (*m_selectSectionCoords)[2];	
	gleDouble (*m_selectSectionNormals)[2];
	gleDouble m_upVector[3];
	gleDouble (*m_spineCoords)[3];
	float (*m_spineColors)[3];
	gleDouble (*m_twist);
protected:
	virtual void doCreateSelect();
	virtual void doCreateGeometry();
};

/** \example extrusion.cpp
 * This is an example of how to use the 
 * CIvfExtrusion class.
 */

}