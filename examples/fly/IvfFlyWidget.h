#ifndef _CIvfFlyWidget_h_
#define _CIvfFlyWidget_h_

#include <ivf/IvfCamera.h>
#include <ivf/IvfScene.h>

#include <ivfctl/IvfControllerGroup.h>

#include <ivfwidget/IvfFlyHandler.h>

#include <ivfglut/IvfGlutBase.h>

#include "StarField.h"
//#include "Joystick.h"

IvfSmartPointer(CFlyWidget);

class CFlyWidget : public CGlutBase {
private:
	CCameraPtr			m_camera;
	CScenePtr			m_scene;
	CStarFieldPtr			m_starfield;
	CControllerGroupPtr	m_controllers;
	CFlyHandlerPtr		m_flyHandler;

	double m_t;
	double m_dt;
	bool m_firstFrame;
public:
	CFlyWidget(int X, int Y, int W, int H, const char *L=0);
	virtual ~CFlyWidget();

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
