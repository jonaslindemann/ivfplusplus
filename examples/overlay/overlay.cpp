// ------------------------------------------------------------
//
// Ivf++ Extrusion example
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

#include <ivfwidget/MouseViewHandler.h>

#include <ivf/Camera.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Cube.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase,
	InitEvent,
	RenderEvent,
	OverlayEvent,
	ResizeEvent
{
private:
	CameraPtr		m_camera;
	CompositePtr	m_scene;
	LightPtr		m_light;
	
	MouseViewHandlerPtr m_mouseViewHandler;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

public:
	ExampleWindow(int X, int Y, int W, int H);

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onRender();
	virtual void onOverlay();
	virtual void onResize(int width, int height);
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
	this->addInitEvent(this);
	this->addRenderEvent(this);
	this->addOverlayEvent(this);
	this->addResizeEvent(this);
}

void ExampleWindow::onResize(int width, int height)
{
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

void ExampleWindow::onInit(int width, int height)
{	
	// Initialize variables

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	auto yellowMaterial = Material::create();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = Composite::create();

	auto cube = Cube::create();
	
	cube->setMaterial(yellowMaterial);
	m_scene->addChild(cube);

	// First point
	
	auto axis = Axis::create();
	m_scene->addChild(axis);
	
	// Create a light

	auto lighting = Lighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	this->setUseOverlay(true);
	
	m_mouseViewHandler = MouseViewHandler::create(this, m_camera);
	m_mouseViewHandler->activate();
	
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
{
	m_light->render();
	m_camera->render();
	m_scene->render();
}

// ------------------------------------------------------------
void ExampleWindow::onOverlay()
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2i(50,50);
	glVertex2i(100,100);
	glEnd();
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

	auto window = new ExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Overlay example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
