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

#include <ivf/Base.h>
#include <ivf/Composite.h>
#include <ivf/View.h>

#include <ivfmath/MatrixStack.h>
#include <ivfmath/ViewFrustum.h>

namespace ivf {

IvfSmartPointer(CCulling);

/**
 * Scene culling class
 *
 * Implements a scene culling algorithm using bounding
 * spheres. The following code example illustrates using 
 * the CIvfCulling class:
 *
 * \code
 * void CMyWindow::onInit()
 * {
 *     m_culling = new CIvfCulling();
 *     m_scene = new CIvfComposite();
 *     m_camera = new CIvfCamera();
 *     .
 *     .
 *     m_culling->setComposite(m_scene);
 *     m_culling->setCullView(m_camera);
 *     .
 *     .
 * }
 * 
 * void CMyWindow::onRender()
 * {
 *     m_culling->cull();
 *     m_camera->render();
 *     m_scene->render();
 * }
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CCulling : public CBase {
private:
	CCompositePtr m_composite;
	CMatrixStackPtr m_matrixStack;
	CViewFrustumPtr m_frustum;
	CViewPtr m_cullView;
	int m_cullCount;
	bool intersectFrustum(CBoundingSphere* bSphere);
	void cullChildren(CShape* shape);
public:
	/** CIvfCulling constructor */
	CCulling();

	/** CIvfCulling destructor */
	virtual ~CCulling();

	IvfClassInfo("CIvfCulling",CBase);

	/** 
	 * Initiates the cull operation
	 *
	 * Culls the assigned composite objects against the 
	 * the view frustum of the assigned view. When an object
	 * has been culled the culled flag is set to true, @see setCulled.
	 * If the flag is set to true the culled object is not rendered
	 * the int the next render() operation. The cull flag is 
	 * automatically set to false after a render() method.
	 */
	void cull();
	
	/** Sets the composite to be culled (required) */
	void setComposite(CComposite* composite);

	/** Sets the view used to cull the composite (required) */
	void setCullView(CView* view);

	/** Returns the number of objects culled */
	int getCullCount();
};
/** \example culling.cpp
 * This is an example of how to cull a scene graph using the
 * CIvfCulling class.
 */

}