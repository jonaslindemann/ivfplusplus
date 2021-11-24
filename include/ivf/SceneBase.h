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

#include <ivf/Shape.h>
#include <ivf/Camera.h>
#include <ivf/Lighting.h>

#include <ivf/Culling.h>
#include <ivf/BufferSelection.h>

namespace ivf {

class IVF_API MultipassEvent {
public:
	/**
	 * onMultipass method
	 */
	virtual void onMultipass(int pass) {};
};

IvfSmartPointer(SceneBase);

/**
 * Base class for managing a scene 
 *
 * This class manages a scene with lights, cameras, culling and
 * selection. An example of use is shown below:
 *
 * \code
 * CExampleWindow::onInit(int width, int height)
 * {
 *	  // Create scene 
 *
 *	  m_scene = new SceneBase();
 *	  m_scene->getCamera()->setPosition(0.0, 0.0, 8.0);
 *
 *	  // Create a light
 *
 *	  OldLight* light = new OldLight();
 *	  light->setPosition(1.0, 1.0, 1.0, 0.0);
 *	  light->setAmbient(0.2f, 0.2f, 0.2f, 1.0f); 
 *
 *	  m_scene->getLightModel()->addLight(light);
 * }
 * 
 * CExampleWindow::onRender()
 * {
 *    m_scene->render();
 * }
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API SceneBase : public Shape {
public:
	enum TLightMode {
		LM_LOCAL,
		LM_WORLD
	};

	enum TStereoMode {
		SM_NONE,
		SM_ANAGLYPH,
		SM_QUAD_BUFFER
	};

	enum TAnaglyphColorPair {
		CP_RED_GREEN,
		CP_RED_BLUE,
		CP_RED_CYAN
	};
private:
	MultipassEvent* m_multipassEvent;
	CompositePtr m_preComposite;
	CompositePtr m_composite;
	CompositePtr m_postComposite;
	ViewPtr m_view;
	LightingPtr m_lighting;
	CullingPtr m_culling;
	BufferSelectionPtr m_selection;

	TLightMode m_lightMode;
	TStereoMode m_stereoMode;
	TAnaglyphColorPair m_colorPair;

	bool m_useCulling;
	bool m_dirty;

	bool m_multiPass;
	int m_nPasses;
    bool m_renderFlatShadow;
    double m_shadowColor[3];

public:
	void doResize(int width, int height);

	void defaultLocalLighting(int pass = 0);
	void defaultViewSetup(int pass = 0);
	void defaultWorldLighting(int pass = 0);
	void defaultSceneRender(int pass = 0);
	void defaultRendering();

	virtual void doLocalLighting(int pass);
	virtual void doViewSetup(int pass);
	virtual void doWorldLighting(int pass);
	virtual void doRender(int pass);
	virtual void doViewAndRender(int pass);
	virtual void doMultipass(int pass);

	/** SceneBase constructor */
	SceneBase();

	/** SceneBase destructor */
	virtual ~SceneBase();

	IvfClassInfo("SceneBase",Shape);
	IvfStdFactory(SceneBase);

	/** Set scene view (required) */
	virtual void setView(View* view);

	/** Returns current view */
	View* getView();

	/** Set camera (required) */
	void setCamera(Camera* camera);

	/**
	 * Set light mode
	 *
	 * @para mode - LM_LOCAL = light attached to viewer, 
	 * LM_WORLD = light positioned in world coordinate system.
	 */
	void setLightMode(TLightMode mode);

	/** Return light mode */
	TLightMode getLightMode();

	Composite* getPreComposite();
	Composite* getPostComposite();

	/** Return scene composite */
	Composite* getComposite();

	/** Set culling state */
	void setUseCulling(bool flag);

	/** Return culling state */
	bool getUseCulling();

	/** Initiate selection, @see CIvfBufferSelection */
	int pick(int x, int y);

	/** Update selection list, @see CIvfBufferSelection */
	void updateSelection();

	/** Return selected shape, @see CIvfBufferSelection */
	Shape* getSelectedShape();

	/** Clear scene, @see CIvfComposite */
	void clear();

	/** Delete all in scene, @see CIvfComposite */
	void deleteAll();

	/** Remove child from scene, @see CIvfComposite */
	Shape* removeChild(Shape* shape);

	/** Remove child from scene, @see CIvfComposite */
	Shape* removeChild(int idx);

	/** Add child to scene, @see CIvfComposite */
	void addChild(Shape* shape);

	Camera* getCamera();

	void setStereoMode(TStereoMode mode);
	void setAnaglyphColorPair(TAnaglyphColorPair colorPair);
	TAnaglyphColorPair getAnaglyphColorPair();

	SelectedShapesVector& getSelectedShapes();
	int getSelectionSize();
	Shape* getSelectedShape(int idx);
	Shape* removeShape(Shape* shape);

	void setMultipass(bool flag);
	bool getMultipass();

	void setPasses(int passes);
	int getPasses();

	void setMultipassEvent(MultipassEvent* evt);
    
    void setRenderFlatShadow(bool flag);
    void setShadowColor(double red, double green, double blue);

protected:
    virtual void doPostClear();
	virtual void doCreateGeometry();
};

}