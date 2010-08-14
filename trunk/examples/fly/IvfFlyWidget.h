#ifndef _CIvfFlyWidget_h_
#define _CIvfFlyWidget_h_

#include <ivf/IvfCamera.h>
#include <ivf/IvfScene.h>

#include <ivfctl/IvfControllerGroup.h>

#include <ivfwidget/IvfFlyHandler.h>

#include <ivfglut/IvfGlutBase.h>

#include "StarField.h"
//#include "Joystick.h"

IvfSmartPointer(CIvfFlyWidget);

class CIvfFlyWidget : public CIvfGlutBase {
private:
	CIvfCameraPtr			m_camera;
	CIvfScenePtr			m_scene;
	CStarFieldPtr			m_starfield;
	CIvfControllerGroupPtr	m_controllers;
	CIvfFlyHandlerPtr		m_flyHandler;

	double m_t;
	double m_dt;
	bool m_firstFrame;
public:
	CIvfFlyWidget(int X, int Y, int W, int H, const char *L=0);
	virtual ~CIvfFlyWidget();

	virtual void onInit(int width, int height);
	virtual void onInitContext(int width, int height);
	virtual void onResize(int width, int height);

	virtual void onRender();
	virtual void onClear();
	virtual bool onAppLoop();

	virtual void onMouseUp(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseDown(int x, int y);
	virtual void onKeyboard(int key, int x, int y);
};

#endif 
