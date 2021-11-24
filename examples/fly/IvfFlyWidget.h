#ifndef _CIvfFlyWidget_h_
#define _CIvfFlyWidget_h_

#include <ivf/Camera.h>
#include <ivf/Scene.h>

#include <ivfctl/ControllerGroup.h>

#include <ivfwidget/FlyHandler.h>

#include <ivfglut/GlutBase.h>

#include "StarField.h"
//#include "Joystick.h"

IvfSmartPointer(FlyWidget);

class FlyWidget : public ivf::GlutBase {
private:
	ivf::CameraPtr				m_camera;
	ivf::ScenePtr				m_scene;
	ivf::ControllerGroupPtr		m_controllers;
	ivf::FlyHandlerPtr			m_flyHandler;

	StarFieldPtr				m_starfield;

	double m_t;
	double m_dt;
	bool m_firstFrame;
public:
	FlyWidget(int X, int Y, int W, int H, const char *L=0);
	virtual ~FlyWidget();

	static FlyWidgetPtr create(int X, int Y, int W, int H);
	
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
