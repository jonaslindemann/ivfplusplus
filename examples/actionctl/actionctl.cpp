// ------------------------------------------------------------
//
// Ivf++ Controller example
//
// ------------------------------------------------------------
//
// Author: Jonas Lindemann
//

// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include <ivf/Camera.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Lighting.h>
#include <ivf/Cube.h>

#include <ivffile/Ac3DReader.h>

#include <ivfctl/ControllerGroup.h>
#include <ivfctl/ActionController.h>
#include <ivfctl/RotateController.h>
#include <ivfctl/CameraController.h>
#include <ivfctl/SlerpController.h>
#include <ivfctl/PathController.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
private:
	CCameraPtr		m_camera;
	CCompositePtr	m_scene;
	CLightPtr		m_light;

	CActionControllerPtr m_controllers;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	bool m_rotating;

public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual bool onTimeout0();
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	m_rotating = false;

	int i, j;

	// Initialize Ivf++ camera

	m_camera = new CCamera();
	m_camera->setPosition(8.0, 8.0, 8.0);
	m_camera->setTarget(0.0, 0.0, 0.0);

	// Create a materials

	CMaterialPtr material = new CMaterial();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	// Create scene

	m_scene = new CComposite();

	CCubePtr cube = new CCube();
	cube->setMaterial(material);

	CAxisPtr axis = new CAxis();
	axis->setPosition(0.0, 0.0, 0.0);
	axis->setSize(2.0);
	m_scene->addChild(axis);

	// Create controllers

	m_controllers = new CActionController();
	m_controllers->activate();

	for (i=0; i<5; i++)
		for (j=0; j<5; j++)
		{
			CTransformPtr tf = new CTransform();
			tf->addChild(cube);
			tf->setPosition(-5.0+2.5*(double)i, 0.0, -5.0+2.5*(double)j);
			m_scene->addChild(tf);

			CRotateControllerPtr ctl = new CRotateController();
			ctl->setShape(tf);
			ctl->setRotationSpeed(45.0*3.0/10.0);
			ctl->activate();

			CSpline3dPtr path = new CSpline3d();
			path->setSize(2);
			path->getPoint(0)->setComponents(-5.0+2.5*(double)i, 0.0, -5.0+2.5*(double)j);
			path->getPoint(1)->setComponents(-5.0+2.5*(double)i, 3.0, -5.0+2.5*(double)j);
			path->update();

			CPathControllerPtr pathCtl = new CPathController();
			pathCtl->setPath(path);
			pathCtl->setShape(tf);
			pathCtl->setInitialSpeed(0.1);
			pathCtl->activate();

			CControllerGroupPtr groupCtl = new CControllerGroup();
			groupCtl->addChild(ctl);
			groupCtl->addChild(pathCtl);
			//groupCtl->activate();

			m_controllers->addChild(groupCtl);

			CActionPtr action = new CAction();
			action->setActionType(IVF_ACTION_ACTIVATE);
			action->setTarget(groupCtl);
			action->setTime(1.0+1.0*(double)(i*5+j));

			m_controllers->addAction(action);
			
			action = new CAction();
			action->setActionType(IVF_ACTION_DEACTIVATE);
			action->setTarget(ctl);
			action->setTime(1.0+1.0*(double)(i*5+j)+10.0);

			m_controllers->addAction(action);
		}

	// Create a light

	CLightingPtr lighting = CLighting::getInstance();
	
	m_light = lighting->getLight(0);;
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
void CExampleWindow::onResize(int width, int height)
{
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	m_light->render();

	m_camera->rotatePositionY(m_angleX/100.0);
	m_camera->rotatePositionX(m_angleY/100.0);
	m_camera->moveSideways(m_moveX/100.0);
	m_camera->moveVertical(m_moveY/100.0);
	m_camera->moveDepth(m_zoomY/50.0);
	m_camera->render();

	m_scene->render();
}

// ------------------------------------------------------------
void CExampleWindow::onMouseDown(int x, int y)
{
	m_beginX = x;
	m_beginY = y;
}

// ------------------------------------------------------------
void CExampleWindow::onMouseMove(int x, int y)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	if (isLeftButtonDown())
	{
		m_angleX = (x - m_beginX);
		m_angleY = (y - m_beginY);
		m_beginX = x;
		m_beginY = y;
		redraw();
	}

	if (isRightButtonDown())
	{
		if (getModifierKey() == CWidgetBase::MT_SHIFT)
		{
			m_zoomX = (x - m_beginX);
			m_zoomY = (y - m_beginY);
		}
		else
		{
			m_moveX = (x - m_beginX);
			m_moveY = (y - m_beginY);
		}
		m_beginX = x;
		m_beginY = y;

		redraw();
	}
}

// ------------------------------------------------------------
void CExampleWindow::onMouseUp(int x, int y)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
}

// ------------------------------------------------------------
bool CExampleWindow::onTimeout0()
{
	m_controllers->update(0.01);
	redraw();
	return true;
}

// ------------------------------------------------------------
void CExampleWindow::onKeyboard(int key, int x, int y)
{
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGBA|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Controller example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
