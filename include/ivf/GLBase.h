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

#include <ivf/Base.h>
#include <ivfmath/BoundingSphere.h>
#include <ivf/RenderState.h>

#include <ivf/GL.h>

namespace ivf {

IvfSmartPointer(GLBase);

//static double g_nodeSize = 0.2;

/**
 * Root class "renderable" objects
 *
 * CIvfGLBase has virtual functions for handling drawing and transformations
 * in OpenGL.
 *
 * @author Jonas Lindemann
 */
class IVF_API GLBase : public Base {
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
	BoundingSpherePtr m_boundSphere;
	GLuint m_displayList;
	bool m_useList;
	bool m_listDirty;
	bool m_dynamic;
	bool m_useSelectShape;
	bool m_culled;
	bool m_renderMaterial;
	RenderStatePtr m_renderState;

public:
	bool getRenderMaterial();
	void setRenderMaterial(bool flag);
	bool isSelectEnabled();
	void disableSelect();
	void enableSelect();
	bool isEnabled();
	void disable();
	void enable();
	/** CIvfGLBase constructor */
	GLBase ();

	/** CIvfGLBase constructor */
	GLBase (const GLBase&);

	/** CIvfGLBase destructor */
	virtual ~GLBase ();
	GLBase& operator = (const GLBase &arg);

	IvfClassInfo("GLBase",Base);
	IvfStdFactory(GLBase);

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
	 * method doCreateSelect is called to create the object that is
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
	BoundingSphere* getBoundingSphere();

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
	 * When enabled the object's geometry is compiled into a display list and
	 * replayed on subsequent frames instead of being regenerated. This is worth
	 * doing for anything whose doCreateGeometry() is expensive -- tessellated or
	 * swept geometry in particular.
	 *
	 * Compilation is deferred to the next render(), so this is safe to call from a
	 * constructor or before an OpenGL context exists.
	 *
	 * The list captures the object's transform, its material, and its geometry.
	 * Any change to those invalidates it, so mutators must call markListDirty().
	 * The stock geometry classes do this for their own properties, but a material
	 * mutated behind the object's back is not detected -- mark the object dirty
	 * from wherever that happens.
	 *
	 * Objects whose geometry changes every frame should use setDynamic(true)
	 * rather than turning lists off, so the choice can be reversed cheaply.
	 */
	void setUselist(bool flag);

	/** Returns display list usage. */
	bool getUselist();

	/**
	 * Marks the object's cached representation as out of date.
	 *
	 * For an object using display lists the list is recompiled on the next
	 * render(). Objects that cache their geometry some other way -- in a vertex
	 * buffer, say -- override this to invalidate that instead, so a caller can
	 * say "this object changed behind your back" without knowing which strategy
	 * the object uses. Cheap to call redundantly.
	 */
	virtual void markListDirty();

	/** Returns true when a compiled display list is awaiting recompilation. */
	bool isListDirty();

	/**
	 * Marks the object as changing every frame.
	 *
	 * A dynamic object never replays a display list, because recompiling one per
	 * frame costs more than regenerating the geometry directly. Use this to
	 * suspend list usage during animation or interactive dragging, then clear it
	 * when the object comes to rest.
	 */
	void setDynamic(bool flag);

	/** Returns the dynamic flag. */
	bool getDynamic();

	/**
	 * Renders the object
	 *
	 * This method renders the object to screen. The code example
	 * below shows which routines are called when the render()
	 * method is called. To derive a new class based on CIvfGLBase
	 * the methods shown in the code example should be overridden.
	 *\code
	 * doBeginTransform();
	 * if (getSelectState() == IVF_SELECT_ON)
	 *   doCreateSelect();
	 * doCreateMaterial();
	 * createGeomtry();
	 * doEndTransform();
	 *\endcode
	 */
	virtual void	render ();

	/**
	 * Renders the object without going through its display list.
	 *
	 * Used when the object has to be drawn in a state the compiled list does not
	 * describe -- highlighted or selected, for instance -- and by render() itself
	 * whenever lists are disabled or the object is dynamic.
	 */
	void renderImmediate();

	/**
	 * Initializes use of bounding sphere
	 *
	 * If no bounding sphere is assigned to object, one is
	 * created. The doUpdateBoundingSphere is called to update
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
	virtual void doUpdateBoundingSphere();

	/**
	 * Sets the cull flag
	 *
	 * If set to true the objects has been culled.
	 * The cull flag is set to false after each call to render.
	 */
	void setCulled(bool flag);

	/** Returns cull status */
	bool getCulled();

	/** Returns object name (default=nullptr) */
	const std::string getName();

	/** Gives the object a name */
	void setName(const std::string& name);

	/**
	 * Set object render state
	 *
	 * The render state encapsulates an OpenGL state change
	 * using the glEnable()/glDisable() pair. The state will
	 * be applied in the beginning of the render() method and
	 * removed before exiting the render() method. The render
	 * state should be handled like a material or texture.
	 */
	void setRenderState(RenderState* state);

	/** Return assigned render state */
	RenderState* getRenderState();

protected:
	virtual void doPreGeometry();
	virtual void doPostGeometry();

	/**
	 * Decides whether render() may replay a display list this frame.
	 *
	 * The base implementation allows it whenever lists are enabled and the object
	 * is not dynamic. Override to add conditions under which the compiled list
	 * does not describe the object's current appearance.
	 */
	virtual bool useDisplayList();

	/**
	 * Compiles the display list.
	 *
	 * Called from render() when the list is missing or has been marked dirty.
	 */
	virtual void compileList();

	/**
	 * Creates the select object.
	 *
	 * When the object is selected. This method is called
	 * to create the object respresenting the select state.
	 */
	virtual void doCreateSelect();

	/**
	 * Creates the geometry of the object.
	 *
	 * This method is called from the render() method when geometry
	 * rendering is to be done. Objects derived from CIvfGLBase
	 * should implement rendering routines from this method using
	 * OpenGL.
	 */
	virtual void doCreateGeometry ();

	/**
	 * Creates the material of the object.
	 *
	 * This method is called from the render() method when material
	 * rendering is to be done. Objects derived from CIvfGLBase
	 * should implement material routines from this method using
	 * OpenGL.
	 */
	virtual void doCreateMaterial ();

	/**
	 * Start transform
	 *
	 * Implements a OpenGL transform. This routine should
	 * call a glPushMatrix() and then call OpenGL routines
	 * transforming the object. The method is the first
	 * routine to be called in the render() method.
	 */
	virtual void doBeginTransform ();

	/**
	 * End transform
	 *
	 * This is the last method called in the render() method.
	 * if a glPushMatrix() is called in the doBeginTransform() a
	 * glPopMatrix() should be called in this routine.
	 */
	virtual void doEndTransform ();

};

}