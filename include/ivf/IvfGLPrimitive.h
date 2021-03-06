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

#ifndef _CIvfGLPrimitive_h_
#define _CIvfGLPrimitive_h_

#include <ivf/ivfconfig.h>

#include <ivf/IvfBase.h>
#include <ivf/IvfShape.h>
#include <ivf/IvfIndex.h>
#include <ivf/IvfColor.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfMaterialSet.h>

#include <ivfmath/IvfVec3d.h>
#include <ivfmath/IvfPoint3d.h>

IvfSmartPointer(CIvfGLPrimitive);

/**
 * OpenGL primitive base class
 *
 * Base class for all classes implemting OpenGL primtives.
 * This class maintains 4 main arrays for controlling the 
 * appearance of the geometry generated: a coordinate set, 
 * a color set, a normal set and a texture coordinate set. 
 * OpenGL primitives are created indirectly from these by
 * specifying indices with instances of CIvfIndex. A quad 
 * set is created using this principle in the following
 * code:
 *
 * \code
 *	CIvfQuadSet* quadSet = new CIvfQuadSet();
 *	
 *	quadSet->addCoord(0.0,0.0,2.0);
 *	quadSet->addCoord(1.0,0.3,2.0);
 *	quadSet->addCoord(2.0,0.0,2.0);
 *	quadSet->addCoord(0.0,0.3,1.0);
 *	quadSet->addCoord(1.0,0.5,1.0);
 *	quadSet->addCoord(2.0,0.3,1.0);
 *	quadSet->addCoord(0.0,0.0,0.0);
 *	quadSet->addCoord(1.0,0.3,0.0);
 *	quadSet->addCoord(2.0,0.0,0.0);
 * 
 *	coordIdx = new CIvfIndex();
 *	coordIdx->add(0,1,4,3);
 *	coordIdx->add(1,2,5,4);
 *	coordIdx->add(3,4,7,6);
 *	coordIdx->add(4,5,8,7);
 *
 *	quadSet->addCoordIndex(coordIdx);
 *
 *	quadSet->setMaterial(greenMaterial);
 *	quadSet->setUseVertexNormals(false);
 * \endcode
 */
class IVF_API CIvfGLPrimitive : public CIvfShape {
private:
	bool m_useVertexNormals;
protected:
	virtual void calcNormal(CIvfIndex* idx);
	std::vector<CIvfVec3d*>		m_coordSet;
	std::vector<CIvfColor*>		m_colorSet;
	std::vector<CIvfVec3d*>		m_normalSet;
	CIvfMaterialSetPtr			m_materialSet;
	std::vector<CIvfVec3d*>		m_textureCoordSet;

	std::vector<CIvfIndexPtr>		m_coordIndexSet;
	std::vector<CIvfIndexPtr>		m_colorIndexSet;
	std::vector<CIvfIndexPtr>		m_normalIndexSet;
	std::vector<CIvfIndexPtr>		m_textureIndexSet;
	std::vector<CIvfVec3d*>		m_vertexNormalSet;
	std::vector<CIvfIndexPtr>		m_vertexNormalIndexSet;
	std::vector<int>			m_materialIndexSet;
public:
	int getCoordIndexSetSize();
	/** CIvfGLPrimitive constructor */
	CIvfGLPrimitive();

	/** CIvfGLPrimitive destructor */
	virtual ~CIvfGLPrimitive();

	IvfClassInfo("CIvfGLPrimitive",CIvfShape);

	/** Clear all sets */
	void clear();

	/** Add coordinate to coordinate set */
	void addCoord(double x, double y, double z);

	/** Add coordinate to coordinate set */
	void addCoord(CIvfVec3d &coord);

	/** Set a coordinate at specified position in set */
	void setCoord(int pos, double x, double y, double z);

	/** Return coordinate at specified position */
	void getCoord(long pos, double &x, double &y, double &z);

	/** Return size of coordinate set */
	long getCoordSetSize();

	/** Clears coordinate set (deleting coordinates) */
	void clearCoord();

	/** Add a material to material set */
	void addMaterial(CIvfMaterial* material);

	/** Clear material set */
	void clearMaterial();

	/** Return size of material set */
	int getMaterialSetSize();

	/** Return a specific material from material set */
	CIvfMaterial* getMaterialAt(int pos);

	/** Return material set */
	CIvfMaterialSet* getMaterialSet();

	/** Assign material set */
	void setMaterialSet(CIvfMaterialSet* materialSet);

	/** Add material index for element @param idx */
	void addMaterialIndex(int idx);

	/** Clear material index */
	void clearMaterialIndex();

	/** Return material index for element @param pos */
	int getMaterialIndex(int pos);

	/** 
	 * Adds a coordinate index 
	 * 
	 * All indices added are owned by CIvfGLPrimitive.
	 */
	void addCoordIndex(CIvfIndex* index);

	/** Returns a coordinate index */
	CIvfIndex* getCoordIndex(long pos);

	/** Clears coordinate index set (deleting indices) */
	void clearCoordIndex();

	/** Add a color to color set (alfa = 1.0) */
	void addColor(float red, float green, float blue);

	/** Add a color to color set */
	void addColor(float red, float green, float blue, float alfa);

	/** 
	 * Set a color at a specified position
	 * 
	 * Alpha value is unchanged.
	 */
	void setColor(int pos, float red, float green, float blue);

	/** Set color value at given position */
	void setColor(int pos, float red, float green, float blue, float alfa);

	/** Return color value at given position */
	void getColor(long pos, float &red, float &green, float &blue);

	/** Return color value and alpha value at given position */
	void getColor(int pos, float &red, float &green, float &blue, float &alfa);

	/** Set alfa value for given position */
	void setAlfa(int pos, float alfa);

	/** Return alfa value for given position */
	float getAlfa(int pos);

	/** Return size of color set */
	long getColorSetSize();

	/** 
	 * Add a color index 
	 * 
	 * All color indices are owned by CIvfGLPrimitive.
	 */
	void addColorIndex(CIvfIndex* index);

	/** Returns a color index at given position*/
	CIvfIndex* getColorIndex(long pos);

	/** Add normal to normal set */
	void addNormal(double n1, double n2, double n3);

	/** Return normal at given position */
	void getNormal(long pos, double &n1, double &n2, double &n3);

	/** Return size of normal set */
	long getNormalSetSize();

	/**
	 * Add normal index
	 *
	 * All normal indices are owned by CIvfGLPrimitive.
	 */
	void addNormalIndex(CIvfIndex* index);

	/** Return normal index at given position */
	CIvfIndex* getNormalIndex(long pos);

	/** Add Texture coordinate to texture coordinate set */
	void addTextureCoord(double s, double t);

	/** Set texture coordinate at specified position */
	void setTextureCoord(int pos, double s, double t);

	/** Return specified texture coordinate at given position */
	void getTextureCoord(long pos, double &s, double &t);

	/** Return size of texture coordinate set */
	long getTextureCoordSetSize();

	/** Clear texture coordinate set. (Deleting coordinates.) */
	void clearTextureCoord();

	/** 
	 * Add a texture coordinate index 
	 *
	 * All texture indices are owned by CIvfGLPrimitive 
	 */
	void addTextureIndex(CIvfIndex* index);

	/** Return texture coordinate index at given position */
	CIvfIndex* getTextureIndex(long pos);

	/** Recomputes normals if geometry has been modified */
	void refresh();

	/** Enables or disables the use of vertex normals */
	void setUseVertexNormals(bool flag);

	/** Returns state of vertex normal use */
	bool getUseVertexNormals();

	void invertNormals();

	/** 
	 * Updates vertex normals
	 * 
	 * Should be overidden by derived classes.
	 */
	virtual void updateVertexNormals();

	/** 
	 * Updates bounding sphere
	 *
	 * Upates the radius of the assigned bounding sphere, if any.
	 */
	void doUpdateBoundingSphere();
};
/** \example advgeom.cpp
 * This is an example of how to use the CIvfGLPrimitive 
 * derived classes.
 */
#endif 
