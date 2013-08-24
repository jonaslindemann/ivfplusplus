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

#ifndef _CIvfSceneBase_h_
#define _CIvfSceneBase_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfCamera.h>
#include <ivf/IvfLighting.h>

#include <ivf/IvfCulling.h>
#include <ivf/IvfBufferSelection.h>

class IVF_API CIvfMultipassEvent {
public:
	/**
	 * onMultipass method
	 */
	virtual void onMultipass(int pass) {};
};

IvfSmartPointer(CIvfSceneBase);

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
 *	  m_scene = new CIvfSceneBase();
 *	  m_scene->getCamera()->setPosition(0.0, 0.0, 8.0);
 *
 *	  // Create a light
 *
 *	  CIvfOldLight* light = new CIvfOldLight();
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
class IVF_API CIvfSceneBase : public CIvfShape {
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
	CIvfMultipassEvent* m_multipassEvent;
	CIvfCompositePtr m_preComposite;
	CIvfCompositePtr m_composite;
	CIvfCompositePtr m_postComposite;
	CIvfViewPtr m_view;
	CIvfLightingPtr m_lighting;
	CIvfCullingPtr m_culling;
	CIvfBufferSelectionPtr m_selection;

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

	/** CIvfSceneBase constructor */
	CIvfSceneBase();

	/** CIvfSceneBase destructor */
	virtual ~CIvfSceneBase();

	IvfClassInfo("CIvfSceneBase",CIvfShape);

	/** Set scene view (required) */
	virtual void setView(CIvfView* view);

	/** Returns current view */
	CIvfView* getView();

	/** Set camera (required) */
	void setCamera(CIvfCamera* camera);

	/**
	 * Set light mode
	 *
	 * @para mode - LM_LOCAL = light attached to viewer, 
	 * LM_WORLD = light positioned in world coordinate system.
	 */
	void setLightMode(TLightMode mode);

	/** Return light mode */
	TLightMode getLightMode();

	CIvfComposite* getPreComposite();
	CIvfComposite* getPostComposite();

	/** Return scene composite */
	CIvfComposite* getComposite();

	/** Set culling state */
	void setUseCulling(bool flag);

	/** Return culling state */
	bool getUseCulling();

	/** Initiate selection, @see CIvfBufferSelection */
	int pick(int x, int y);

	/** Update selection list, @see CIvfBufferSelection */
	void updateSelection();

	/** Return selected shape, @see CIvfBufferSelection */
	CIvfShape* getSelectedShape();

	/** Clear scene, @see CIvfComposite */
	void clear();

	/** Delete all in scene, @see CIvfComposite */
	void deleteAll();

	/** Remove child from scene, @see CIvfComposite */
	CIvfShape* removeChild(CIvfShape* shape);

	/** Remove child from scene, @see CIvfComposite */
	CIvfShape* removeChild(int idx);

	/** Add child to scene, @see CIvfComposite */
	void addChild(CIvfShape* shape);

	CIvfCamera* getCamera();

	void setStereoMode(TStereoMode mode);
	void setAnaglyphColorPair(TAnaglyphColorPair colorPair);
	TAnaglyphColorPair getAnaglyphColorPair();

	CIvfSelectedShapesVector& getSelectedShapes();
	int getSelectionSize();
	CIvfShape* getSelectedShape(int idx);
	CIvfShape* removeShape(CIvfShape* shape);

	void setMultipass(bool flag);
	bool getMultipass();

	void setPasses(int passes);
	int getPasses();

	void setMultipassEvent(CIvfMultipassEvent* evt);
    
    void setRenderFlatShadow(bool flag);
    void setShadowColor(double red, double green, double blue);

protected:
	virtual void doCreateGeometry();
};

#endif 
