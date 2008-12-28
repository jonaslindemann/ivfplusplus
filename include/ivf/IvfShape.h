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

#ifndef _CIvfShape_h_
#define _CIvfShape_h_

#include <ivf/IvfObject.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfTexture.h>

#include <ivfmath/IvfPoint3d.h>
#include <ivfmath/IvfVec3d.h>
#include <ivfmath/IvfQuat.h>

IvfSmartPointer(CIvfShape);

#define IVF_NONAME			999999
#define IVF_NO_ENUM         999998

// Modified JL

/**
 * Abstract shape class with material and position.
 *
 * CIvfShape implements a CIvfObject with a material and a position.
 * Methods for moving, and rotation are implemented. The CIvfShape
 * also has a special material and state for a highlight effect.
 * CIvfShape should be considered an abstract class. Classes derived 
 * from CIvfShape should override the createGeometry, 
 * createSelect methods.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfShape : public CIvfObject {
public:
	enum THighlightState {
		HS_ON,
		HS_OFF
	};
	enum THighlightType {
		HT_MATERIAL,
		HT_LIGHTEN
	};
public:
	bool isEnabledHighlight();
	void disableHighlight();
	void enableHighlight();
	void setHighlightType(THighlightType type);
	/** CIvfShape constructor */
	CIvfShape ();

	/** CIvfShape destructor */
	virtual ~CIvfShape ();

	IvfClassInfo("CIvfShape",CIvfObject);

	/** Force object refresh */
	virtual void refresh();

	/**
	 * Sets the highlight state of the object.
	 *
	 * If the highlight state is IVF_HIGHLIGHT_ON the object
	 * is rendered using the highlight material instead of the
	 * normal material. This can be used to change object 
	 * appearance when the mouse moves over it.
	 *
	 * @param state HS_ON object is rendered using the
	 * highlightmaterial. HS_OFF the object is rendered 
	 * using the normal material.
	 */
	virtual void setHighlight(THighlightState state);

	/** Returns the highlight state */
	THighlightState getHighlight();

	/** 
	 * Sets name rendering flag.
	 *
	 * Names are used when using selection in OpenGL.
	 *
	 * @param flag If true the shape is rendered using a name. (default).
	 * if false no name is used when rendering.
	 */
	void setUseName(bool flag);

	/** Returns name rendering flag. */
	bool getUseName();

	/**
	 * Object name
	 *
	 * Sets the name (number) of the object to be rendered if the 
	 * name rendering flag is set to true.
	 */
	void setObjectName(GLuint name);

	/** Returns object name (number). */
	GLuint getObjectName();

	/**
	 * Assign a CIvfPoint the position of the object.
	 * @param point Reference to a CIvfPoint instance.
	 */
	void assignPointTo(CIvfPoint3d* point);

	/**
	 * Assigns the position of the object to a 
	 * CIvfShape instance.
	 * @param shape Reference to a CIvfShape instance.
	 */
	void assignPositionTo(CIvfShape* shape);

	/**
	 * Assign highlight material
	 *
	 * Assigns the material used to highlight the object. 
	 * The CIvfShape class increases the reference count of the
	 * material when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the material is not referenced it is deleted.
	 * @param material Reference of a CIvfMaterial instance.
	 */
	void setHighlightMaterial(CIvfMaterial* material);
	
	/** Returns the used highlight material */
	CIvfMaterial* getHightlightMaterial();

	/**
	 * Assign material
	 *
	 * Assigns the default material to the object. 
	 * The CIvfShape class increases the reference count of the
	 * material when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the material is not referenced it is deleted.
	 * @param material Reference of a CIvfMaterial instance.
	 */
	void setMaterial (CIvfMaterial* material);

	/** Returns the used material */
	virtual CIvfMaterial* getMaterial ();
	
	/**
	 * Assign texture
	 *
	 * Assigns the default texture to the object. 
	 * The CIvfShape class increases the reference count of the
	 * texture when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the texture is not referenced it is deleted.
	 * @param texture Reference of a CIvfTexture instance.
	 */
	void setTexture(CIvfTexture* texture);

	/** Returns used texture */
	CIvfTexture* getTexture();

	/** 
	 * Set the quaternion rotation of the object.
	 *
	 * Sets the rotation of the object specifying a rotation axis
	 * and a rotation angle.
	 * @param vx X component of the rotation axis
	 * @param vy Y component of the rotation axis
	 * @param vz Z component of the rotation axis
	 * @param theta Angle specifying rotation in degrees.
	 */
	void setRotationQuat(double vx, double vy, double vz, double theta);

	/** Sets the rotation of the object specified by an CIvfQuat. */
	void setRotation(CIvfQuat& q);

	/** Return quaternion rotation of the object */
	void getRotationQuat(double &vx, double &vy, double &vz, double &theta);

	/** Set rotation of object (obsolete) */
	void setRotation (const double xRot, const double yRot, const double zRot);
	
	/** Get rotation of object (obsolete) */
	void getRotation (double &xRot, double &yRot, double &zRot);
	
	/** 
	 * Position of object
	 *
	 * Sets the position of the object in the global coordinate system.
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	virtual void setPosition (const double x, const double y, const double z);

	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param shape Shape used to retrieve position from.
	 */
	virtual void setPosition (CIvfShape* shape);
	
	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param point Point used to retrieve position from.
	 */
	virtual void setPosition (CIvfPoint3d* point);

	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param pos Point used to retrieve position from.
	 */
	virtual void setPosition(CIvfPoint3d& pos);

	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param pos Point used to retrieve position from.
	 */
	virtual void setPosition(CIvfVec3d& pos);

	/** Return position of object */
	virtual void getPosition (double &x, double &y, double &z);

	/** Return position of object */
	virtual void getPosition (CIvfShape* shape);

	/** Return position of object */
	virtual void getPosition (CIvfPoint3d* point);

	/** Return position of object */
	virtual CIvfVec3d& getPosition();

	/**
	 * Object scale
	 *
	 * Sets the scaling of the object in x,y and z directions.
	 * @param xScale Scaling in the x-direction
	 * @param yScale Scaling in the y-direction
	 * @param zScale Scaling in the z-direction
	 */
	void setScale (const double xScale, const double yScale, const double zScale);
	
	/** Return object scale */
	void getScale (double &xScale, double &yScale, double &zScale);

	/** Set material for selection */
	void setSelectMaterial(CIvfMaterial* material);

	/** Enable/disable normalization of normals */
	void setNormalize(bool flag);
	
	// Dynamic methods beta

private:
	bool m_renderName;
	int m_selectState;
	THighlightState m_highlight;
	THighlightType  m_highlightType;
	CIvfMaterialPtr m_highlightMaterial;
	CIvfMaterialPtr m_selectMaterial;
	GLuint m_objectName;
	double	m_position[3];
	double	m_scale[3];
	double	m_rotation[3];
	double  m_rotQuat[4];
	CIvfMaterialPtr	m_material;
	CIvfTexturePtr    m_texture;
	bool m_normalize;
protected:
	/** Forces an object recalculation. */
	virtual void createGeometry();
	virtual void createSelect();
	virtual void endTransform();
	virtual void beginTransform();
	virtual void createMaterial();
};
#endif
