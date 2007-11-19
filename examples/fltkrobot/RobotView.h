#ifndef _CRobotView_h_
#define _CRobotView_h_

// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivffltk/IvfFltkBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfScene.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CRobotView);

class CRobotView: public CIvfFltkBase, 
	CIvfInitEvent,
	CIvfDestroyEvent,
	CIvfKeyboardEvent
{
private:
	CIvfCameraPtr		m_camera;
	CIvfScenePtr		m_scene;
	CIvfLightPtr		m_light;

	CIvfTransformPtr  m_part1;
	CIvfTransformPtr  m_part2;
	CIvfTransformPtr  m_part3;
	CIvfTransformPtr  m_arm;

	CIvfMouseViewHandlerPtr m_mouseViewHandler;
	CIvfSceneHandlerPtr m_sceneHandler;

	double m_alfa;
	double m_beta;
	double m_gamma;
	double m_delta;

	void updateArm();

public:
	CRobotView(int X, int Y, int W, int H, const char* L=0);

	void onInit(int width, int height);
	void onKeyboard(int key, int x, int y);
};

#endif