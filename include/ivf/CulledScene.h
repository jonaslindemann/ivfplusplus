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

#include <ivf/Scene.h>
#include <ivf/View.h>

#include <ivfmath/MatrixStack.h>
#include <ivfmath/ViewFrustum.h>

namespace ivf {

IvfSmartPointer(CulledScene);

/**
 * Culled scene class (obsolete use SceneBase derived classes instead)
 *
 * Implements a scene with culling against a view frustum.
 *
 * @author Jonas Lindemann
 */
class IVF_API CulledScene : public Scene {
private:
	void cullChildren(Shape* shape);

	MatrixStack* m_matrixStack;
	ViewFrustum* m_frustum;
	View* m_cullView;
	int m_cullCount;
	bool m_useCulling;

public:
	/** CIvfCulledScene constructor */
	CulledScene();

	/** CIvfCulledScene destructor */
	virtual ~CulledScene();

	IvfClassInfo("CulledScene",Scene);
	IvfStdFactory(CulledScene);

	/** 
	 * Adds a child to scene
	 *
	 * Each child added to the scene will be assigned
	 * a bounding sphere using the initBoundingSphere
	 * method of the CIvfGLBase class.
	 */
	void addChild(Shape* shape);

	/**
	 * Sets the view used for culling 
	 *
	 * If not set, the scene camera will be used to 
	 * cull the scene. This method can be used to 
	 * support custom culling routines.
	 */
	void setCullView(View* view);

	/** Returns the view used for culling */
	View* getCullView();

	/**
	 * Enables or disables culling
	 *
	 * The default value is true. If set to false
	 * normal scene rendering will be used.
	 */
	void setUseCulling(bool flag);

	/** Returns the culling state */
	bool getUseCulling();

	/** Returns the number of objects culled */
	int getCullCount();

protected:
	bool intersectFrustum(BoundingSphere* bSphere);
	void cull();
	virtual void doCreateGeometry();
};

}