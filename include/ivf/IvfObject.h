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

#ifndef _CIvfObject_h_
#define _CIvfObject_h_

#include <ivf/IvfBase.h>
#include <ivfmath/IvfBoundingSphere.h>
#include <ivf/IvfRenderState.h>

#include <ivf/IvfGL.h>

IvfSmartPointer(CIvfObject);

static double g_nodeSize = 0.2;

/**
 * Root class "renderable" objects
 *
 * CIvfObject has virtual functions for handling drawing and transformations 
 * in OpenGL.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfObject : public CIvfBase {
public:
	enum TObjectState {
		OS_ON,
		OS_OFF
	};

	enum TSelectState {
		SS_ON,
		SS_OFF
	};
private:
	std::string m_objectName;
	long m_tag;
	TSelectState m_selectState;
	TObjectState m_state;
	CIvfBoundingSpherePtr m_boundSphere;
	GLuint m_displayList;
	bool m_useList;
	bool m_dynamic;
	bool m_useSelectShape;
	bool m_culled;
	bool m_renderMaterial;
	CIvfRenderStatePtr m_renderState;

public:
	bool getRenderMaterial();
	void setRenderMaterial(bool flag);
	bool isSelectEnabled();
	void disableSelect();
	void enableSelect();
	bool isEnabled();
	void disable();
	void enable();
	/** CIvfObject constructor */
	CIvfObject ();
	
	/** CIvfObject constructor */
	CIvfObject (const CIvfObject&);
	
	/** CIvfObject destructor */
	virtual ~CIvfObject ();
	CIvfObject& operator = (const CIvfObject &arg);

	IvfClassInfo("CIvfObject",CIvfBase);

	/**
	 * Sets the tag property of an object.
	 *
	 * The tag property can be used to assign special properties
	 * to the object.
	 */
	void setTag(long tag);
	
	/** Returns the tag property of an object. */
	long getTag();
	
	/**
	 * Sets the select property of an object.
	 * 
	 * This property is used to indicate if the object is 
	 * selected. If the select state is IVF_SELECT_ON the virtual 
	 * method createSelect is called to create the object that is
	 * used to indicate the select state. if the select state is 
	 * SS_OFF the object is rendered as normal. Default
	 * state is SS_OFF.
	 * @param selectState	SS_ON, object selected.
	 *						SS_OFF, object is not selected.
	 */
	void setSelect(TSelectState selectState);
	
	/** Returns the select state of an object.*/
	TSelectState getSelect();
	
	/**
	 * Sets object state.
	 * 
	 * The state of the object is used to determine if the object
	 * is to be drawn or not. OS_ON renders the object (default)
	 * OS_OFF turns the object off.
	 * @param state	OS_ON object is rendered.
	 *				OS_OFF object is not rendered.
	 */
	void setState(TObjectState state);
	
	/** Returns object state. */
	TObjectState getState();
	
	bool getUseSelectShape();
	void setUseSelectShape(bool flag);

	/** 
	 * Returns object bounding sphere.
	 * 
	 * To be implemented.
	 */
	CIvfBoundingSphere* getBoundingSphere();
	
	/**
	 * Sets the display list number.
	 *
	 * To be implemented.
	 */
	void setDisplayList(GLuint nList);
	
	/** 
	 * Return display list number. 
	 * 
	 * To be implemented.
	 */
	GLuint getDisplayList();
	
	/**
	 * Sets display list usage.
	 * 
	 * Determines if display lists are to be used when
	 * drawing the object. To be implemented.
	 */
	void setUselist(bool flag);
	
	/** Returns diusplay list usage. */
	bool getUselist();
	
	/**
	 * Renders the object
	 * 
	 * This method renders the object to screen. The code example 
	 * below shows which routines are called when the render() 
	 * method is called. To derive a new class based on CIvfObject
	 * the methods shown in the code example should be overridden.
	 *\code
	 * beginTransform();
	 * if (getSelectState() == IVF_SELECT_ON)
	 *   createSelect();
	 * createMaterial();
	 * createGeomtry();
	 * endTransform();
	 *\endcode
	 */
	virtual void	render ();

	/**
	 * Initializes use of bounding sphere
	 *
	 * If no bounding sphere is assigned to object, one is 
	 * created. The updateBoundingSphere is called to update
	 * the size of the bounding sphere.
	 */
	void initBoundingSphere();

	/**
	 * Updates the size of bounding sphere
	 *
	 * Classes used in cull operations should override
	 * this method and update the size of the bounding sphere
	 * this method should be called if the object has changed size
	 * and is also called when the initBoundingSphere is called when
	 * culling is used.
	 */
	virtual void updateBoundingSphere();

	/**
	 * Sets the cull flag
	 *
	 * If set to true the objects has been culled.
	 * The cull flag is set to false after each call to render.
	 */
	void setCulled(bool flag);

	/** Returns cull status */
	bool getCulled();

	/** Returns object name (default=NULL) */
	const char* getName();

	/** Gives the object a name */
	void setName(const char* name);

	/** 
	 * Set object render state 
	 *
	 * The render state encapsulates an OpenGL state change
	 * using the glEnable()/glDisable() pair. The state will
	 * be applied in the beginning of the render() method and
	 * removed before exiting the render() method. The render
	 * state should be handled like a material or texture.
	 */
	void setRenderState(CIvfRenderState* state);

	/** Return assigned render state */
	CIvfRenderState* getRenderState();

protected:
	virtual void preGeometry();
	virtual void postGeometry();

	/**
	 * Compiles display list.
	 *
	 * To be implemented.
	 */
	virtual void compileList();
	
	/**
	 * Creates the select object.
	 *
	 * When the object is selected. This method is called
	 * to create the object respresenting the select state.
	 */
	virtual void createSelect();

	/**
	 * Creates the geometry of the object.
	 *
	 * This method is called from the render() method when geometry
	 * rendering is to be done. Objects derived from CIvfObject
	 * should implement rendering routines from this method using
	 * OpenGL.
	 */
	virtual void createGeometry ();

	/**
	 * Creates the material of the object.
	 *
	 * This method is called from the render() method when material
	 * rendering is to be done. Objects derived from CIvfObject 
	 * should implement material routines from this method using
	 * OpenGL.
	 */
	virtual void createMaterial ();

	/**
	 * Start transform
	 *
	 * Implements a OpenGL transform. This routine should 
	 * call a glPushMatrix() and then call OpenGL routines
	 * transforming the object. The method is the first
	 * routine to be called in the render() method.
	 */
	virtual void beginTransform ();

	/**
	 * End transform
	 * 
	 * This is the last method called in the render() method.
	 * if a glPushMatrix() is called in the beginTransform() a
	 * glPopMatrix() should be called in this routine.
	 */
	virtual void endTransform ();

};
#endif
