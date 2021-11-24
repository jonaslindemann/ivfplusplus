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

#include <ivf/GLBase.h>
#include <ivf/Material.h>
#include <ivf/Texture.h>

#include <ivfmath/Point3d.h>
#include <ivfmath/Vec3d.h>
#include <ivfmath/Quat.h>

namespace ivf {

IvfSmartPointer(Shape);

#define IVF_NONAME			999999
#define IVF_NO_ENUM         999998

// Modified JL

/**
 * Abstract shape class with material and position.
 *
 * Shape implements a CIvfGLBase with a material and a position.
 * Methods for moving, and rotation are implemented. The Shape
 * also has a special material and state for a highlight effect.
 * Shape should be considered an abstract class. Classes derived 
 * from Shape should override the doCreateGeometry, 
 * doCreateSelect methods.
 *
 * @author Jonas Lindemann
 */
class IVF_API Shape : public GLBase {
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
	/** Shape constructor */
	Shape ();

	/** Shape destructor */
	virtual ~Shape ();

	IvfClassInfo("Shape",GLBase);
	IvfStdFactory(Shape);

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
	void assignPointTo(Point3d* point);

	/**
	 * Assigns the position of the object to a 
	 * Shape instance.
	 * @param shape Reference to a Shape instance.
	 */
	void assignPositionTo(Shape* shape);

	/**
	 * Assign highlight material
	 *
	 * Assigns the material used to highlight the object. 
	 * The Shape class increases the reference count of the
	 * material when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the material is not referenced it is deleted.
	 * @param material Reference of a Material instance.
	 */
	void setHighlightMaterial(Material* material);
	
	/** Returns the used highlight material */
	Material* getHightlightMaterial();

	/**
	 * Assign material
	 *
	 * Assigns the default material to the object. 
	 * The Shape class increases the reference count of the
	 * material when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the material is not referenced it is deleted.
	 * @param material Reference of a Material instance.
	 */
	void setMaterial (Material* material);

	/** Returns the used material */
	virtual Material* getMaterial ();
	
	/**
	 * Assign texture
	 *
	 * Assigns the default texture to the object. 
	 * The Shape class increases the reference count of the
	 * texture when it is assigned to the class. When the
	 * destructor of the class is called the reference count is 
	 * decreased. If the texture is not referenced it is deleted.
	 * @param texture Reference of a Texture instance.
	 */
	void setTexture(Texture* texture);

	/** Returns used texture */
	Texture* getTexture();

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

	/** Sets the rotation of the object specified by an Quat. */
	void setRotation(Quat& q);

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
	virtual void setPosition (Shape* shape);
	
	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param point Point used to retrieve position from.
	 */
	virtual void setPosition (Point3d* point);

	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param pos Point used to retrieve position from.
	 */
	virtual void setPosition(Point3d& pos);

	/**
	 * Position of object
	 *
	 * Sets the position of the object int the global coordinate system.
	 * @param pos Point used to retrieve position from.
	 */
	virtual void setPosition(Vec3d& pos);

	/** Return position of object */
	virtual void getPosition (double &x, double &y, double &z);

	/** Return position of object */
	virtual void getPosition (Shape* shape);

	/** Return position of object */
	virtual void getPosition (Point3d* point);

	/** Return position of object */
	virtual Vec3d& getPosition();

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
	void setSelectMaterial(Material* material);

	/** Enable/disable normalization of normals */
	void setNormalize(bool flag);
	
	// Dynamic methods beta

private:
	bool m_renderName;
	int m_selectState;
	THighlightState m_highlight;
	THighlightType  m_highlightType;
	MaterialPtr m_highlightMaterial;
	MaterialPtr m_selectMaterial;
	GLuint m_objectName;
	double	m_position[3];
	double	m_scale[3];
	double	m_rotation[3];
	double  m_rotQuat[4];
	MaterialPtr	m_material;
	TexturePtr    m_texture;
	bool m_normalize;
protected:
	/** Forces an object recalculation. */
	virtual void doCreateGeometry();
	virtual void doCreateSelect();
	virtual void doEndTransform();
	virtual void doBeginTransform();
	virtual void doCreateMaterial();
};

}