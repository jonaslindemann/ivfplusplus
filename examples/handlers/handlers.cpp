// ------------------------------------------------------------
//
// Ivf++ Ac3D file reader example
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
#include <ivf/Scene.h>
#include <ivf/Light.h>
#include <ivf/Cube.h>
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/SelectionHandler.h>
#include <ivfwidget/FlyHandler.h>
#include <ivfwidget/SceneHandler.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase,
	InitEvent,
	KeyboardEvent,
	TimeoutEvent,
	SingleSelectionEvent,
	MultipleSelectionEvent
{
private:
	CameraPtr		m_camera;
	ScenePtr		m_scene;
	LightPtr		m_light;

	MouseViewHandlerPtr m_mouseHandler;
	SelectionHandlerPtr m_selectionHandler;
	FlyHandlerPtr		m_flyHandler;
	SceneHandlerPtr		m_sceneHandler;

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
	ExampleWindow(int X, int Y, int W, int H);

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual bool onTimeout();
	virtual void onSelect(Shape* shape);
	virtual void onSelectMultiple(SelectedShapesVector& shapes);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
	return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

ExampleWindow::ExampleWindow(int X, int Y, int W, int H)
	:GlutBase(X, Y, W, H) 
{
	addInitEvent(this);
	addKeyboardEvent(this);
	assignTimeoutEvent(0, this);
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
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = Scene::create();
	m_scene->setView(m_camera);

	// Create a materials

	auto redMaterial = Material::create();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	auto greenMaterial = Material::create();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);
	
	auto blueMaterial = Material::create();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	auto yellowMaterial = Material::create();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create objects
	
	auto cube = Cube::create();
	cube->setMaterial(redMaterial);
	cube->setPosition(2.0, 0.0, 2.0);
	m_scene->addChild(cube);

	auto sphere = Sphere::create();
	sphere->setMaterial(greenMaterial);
	sphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(sphere);

	auto cylinder = Cylinder::create();
	cylinder->setMaterial(blueMaterial);
	cylinder->setPosition(-2.0, 0.0, -2.0);
	m_scene->addChild(cylinder);

	auto cone = Cone::create();
	cone->setMaterial(yellowMaterial);
	cone->setPosition(2.0, 0.0, -2.0);
	cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_scene->addChild(cone);

	auto axis = Axis::create();
	m_scene->addChild(axis);

	// Create a light

	auto lighting = Lighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	// Create event handlers

	m_mouseHandler = MouseViewHandler::create(this, m_camera);
	m_mouseHandler->deactivate();
	
	m_selectionHandler = SelectionHandler::create(this, m_scene);
	m_selectionHandler->setSingleSelectionEvent(this);
	m_selectionHandler->setMultipleSelectionEvent(this);
	m_selectionHandler->activate();
	
	m_flyHandler = FlyHandler::create(this, m_camera);
	m_flyHandler->deactivate();

	m_sceneHandler = SceneHandler::create(this, m_scene);
	m_sceneHandler->activate();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
bool ExampleWindow::onTimeout()
{
	if (m_flyHandler->isActive())
		redraw();

	m_flyHandler->update();

	return true;
}

// ------------------------------------------------------------
void ExampleWindow::onSelect(Shape* shape)
{
	if (shape!=nullptr)
		cout << shape->getClassName() << " selected." << endl;
	else
		cout << "Nothingness selected." << endl;
}

// ------------------------------------------------------------
void ExampleWindow::onSelectMultiple(SelectedShapesVector& shapes)
{
	SelectedShapesIterator it;

	cout << "Multiple selection:" << endl;
	for (it=shapes.begin(); it!=shapes.end(); it++)
	{
		auto shape = *it;
		cout << "\t" << shape->getClassName() << " selected." << endl;
	}
}

// ------------------------------------------------------------
void ExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case '1' :
		m_mouseHandler->activate();
		m_selectionHandler->deactivate();
		m_flyHandler->deactivate();
		break;
	case '2' :
		m_mouseHandler->deactivate();
		m_selectionHandler->activate();
		m_flyHandler->deactivate();
		break;
	case '3' :
		m_mouseHandler->deactivate();
		m_selectionHandler->deactivate();
		m_flyHandler->activate();
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

	window->setWindowTitle("Ivf++ event handler examples");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
