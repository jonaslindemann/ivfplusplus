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

#include <ivffile/Ac3DReader.h>

#include <ivfctl/ControllerGroup.h>
#include <ivfctl/RotateController.h>
#include <ivfctl/CameraController.h>
#include <ivfctl/SlerpController.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase {
private:
	CameraPtr		m_camera;
	CompositePtr	m_scene;
	LightPtr		m_light;

	ControllerGroupPtr m_controllers;
	RotateControllerPtr m_propCtl;
	RotateControllerPtr m_headCtl;
	CameraControllerPtr m_cameraCtl;

	SlerpControllerPtr m_slerpCtl;

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
	ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H) {};

	static ExampleWindowPtr create(int X, int Y, int W, int H);

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

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
	return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

void ExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	m_rotating = false;

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(10.0, 20.0, 20.0);
	m_camera->setTarget(0.0, 8.0, 0.0);

	// Create a materials

	auto material = Material::create();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	// Create scene

	m_scene = Composite::create();

	// Create a file reader

	auto acReader = Ac3DReader::create();

	// Set parameters

	acReader->setFileName("data/models/controllers.ac");
	acReader->setScaling(1.0);

	// Read file

	acReader->read();

	// Retrieve poly set

	auto model = (Composite*)acReader->getShape();

	auto propeller = model->findShape("propeller");
	propeller->setRotationQuat(0.0, 0.0, 1.0, 0.0);
	auto head = model->findShape("head");
	head->setRotationQuat(0.0, 1.0, 0.0, 0.0);

	m_propCtl = RotateController::create();
	m_propCtl->setShape(propeller);
	m_propCtl->setRotationSpeed(100.0);
	m_propCtl->activate();

	m_headCtl = RotateController::create();
	m_headCtl->setShape(head);
	m_headCtl->setRotationSpeed(10.0);
	m_headCtl->activate();

	auto positionSpline = Spline3d::create();
	positionSpline->setSize(5);
	positionSpline->getPoint(0)->setComponents(15.0, 10.0, 15.0);
	positionSpline->getPoint(1)->setComponents(-15.0,  5.0, 15.0);
	positionSpline->getPoint(2)->setComponents(-15.0, 10.0, -15.0);
	positionSpline->getPoint(3)->setComponents( 15.0, 5.0, -15.0);
	positionSpline->getPoint(4)->setComponents( 15.0, 10.0, 15.0);
	positionSpline->update();

	/*
	Spline3d* targetSpline = new Spline3d();
	targetSpline->setSize(5);
	targetSpline->getPoint(0)->setComponents(15.0, 10.0, 15.0);
	targetSpline->getPoint(1)->setComponents(-15.0,  5.0, 15.0);
	targetSpline->getPoint(2)->setComponents(-15.0, 10.0, -15.0);
	targetSpline->getPoint(3)->setComponents( 15.0, 5.0, -15.0);
	targetSpline->getPoint(4)->setComponents( 15.0, 10.0, 15.0);
	targetSpline->update();
	*/

	m_cameraCtl = CameraController::create();
	m_cameraCtl->setCamera(m_camera);
	m_cameraCtl->setPath(positionSpline);
	m_cameraCtl->setFollowPath(false);
	//m_cameraCtl->setTargetPath(targetSpline);
	m_cameraCtl->setInitialSpeed(0.2);
	//m_cameraCtl->setInitialTargetSpeed(0.2);
	m_cameraCtl->setEndActionType(CameraController::EA_RESET);
	m_cameraCtl->setStartActionType(CameraController::SA_RESET);
	//m_cameraCtl->setTargetEndAction(CameraController::EA_REVERSE);
	//m_cameraCtl->setTargetStartAction(CameraController::SA_REVERSE);
	m_cameraCtl->deactivate();

	auto axis = Axis::create();
	axis->setPosition(3.0, 3.0, 3.0);
	axis->setSize(2.0);
	m_scene->addChild(axis);

	auto slerp = Slerp::create();
	slerp->setSize(4);
	slerp->setQuatAxisAngle(0, 0.0, 0.0, 1.0, 10.0);
	slerp->setQuatAxisAngle(1, 0.0, 0.0, 1.0, 90.0);
	slerp->setQuatAxisAngle(2, 1.0, 0.0, 0.0, 45.0);
	slerp->setQuatAxisAngle(3, 0.0, 1.0, 0.0, 30.0);
	slerp->update();

	auto action1 = Action::create();
	action1->setActionType(IVF_ACTION_ACTIVATE);
	action1->setTarget(m_cameraCtl);

	auto action2 = Action::create();
	action2->setActionType(IVF_ACTION_DEACTIVATE);
	action2->setTarget(m_cameraCtl);

	m_slerpCtl = SlerpController::create();
	m_slerpCtl->setSlerp(slerp);
	m_slerpCtl->setShape(axis);
	m_slerpCtl->setInitialSpeed(1.0);
	m_slerpCtl->setEndActionType(SlerpController::EA_REVERSE);
	m_slerpCtl->setEndAction(action1);
	m_slerpCtl->setStartActionType(SlerpController::SA_REVERSE);
	m_slerpCtl->setStartAction(action2);
	m_slerpCtl->activate();

	m_controllers = ControllerGroup::create();
	m_controllers->addChild(m_propCtl);
	m_controllers->addChild(m_headCtl);
	m_controllers->addChild(m_cameraCtl);
	m_controllers->addChild(m_slerpCtl);
	m_controllers->activate();

	m_scene->addChild(model);

	// Create a light

	auto lighting = Lighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
void ExampleWindow::onResize(int width, int height)
{
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
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
void ExampleWindow::onMouseDown(int x, int y)
{
	m_beginX = x;
	m_beginY = y;
}

// ------------------------------------------------------------
void ExampleWindow::onMouseMove(int x, int y)
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
		if (getModifierKey() == WidgetBase::MT_SHIFT)
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
void ExampleWindow::onMouseUp(int x, int y)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
}

// ------------------------------------------------------------
bool ExampleWindow::onTimeout0()
{
	m_controllers->update(0.01);
	redraw();
	return true;
}

// ------------------------------------------------------------
void ExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'p' :
		if (m_propCtl->isActive())
			m_propCtl->deactivate();
		else
			m_propCtl->activate();
		break;
	case 'h' :
		if (m_headCtl->isActive())
			m_headCtl->deactivate();
		else
			m_headCtl->activate();
		break;
	case 'c' :
		if (m_cameraCtl->isActive())
			m_cameraCtl->deactivate();
		else
			m_cameraCtl->activate();
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	auto app = GlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	auto window = ExampleWindow::create(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Controller example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
