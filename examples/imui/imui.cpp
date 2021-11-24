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
#include <ivfwidget/ImUiHandler.h>
#include <ivfwidget/SceneHandler.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase,
	InitEvent,
	TimeoutEvent,
	GuiEvent
{
private:
	CameraPtr		m_camera;
	ScenePtr		m_scene;
	LightPtr		m_light;

	MouseViewHandlerPtr m_mouseHandler;
	SceneHandlerPtr		m_sceneHandler;
	ImUiHandlerPtr		m_uiHandler;

	bool m_rotating;

public:
	ExampleWindow(int X, int Y, int W, int H);

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual bool onTimeout();
	virtual void onGui();
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
	assignTimeoutEvent(0, this);
}

void ExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_rotating = false;

	// Initialize Ivf++ camera

	m_camera = new Camera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new Scene();
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
	m_mouseHandler->activate();
	
	m_sceneHandler = SceneHandler::create(this, m_scene);
	m_sceneHandler->activate();
	
	m_uiHandler = ImUiHandler::create(this);
	m_uiHandler->setGuiEvent(this);
	m_uiHandler->activate();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
bool ExampleWindow::onTimeout()
{

	return true;
}

void ExampleWindow::onGui()
{
	std::cout << "onGui()" << std::endl;
	
	//glRasterPos2i(50,50);
	//glutBitmapString(GLUT_BITMAP_9_BY_15, "Hello, World!");
}


// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	auto app = GlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_DEPTH|IVF_MULTISAMPLE|IVF_RGBA);

	// Create a window

	auto window = ExampleWindow::create(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ event handler examples");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
