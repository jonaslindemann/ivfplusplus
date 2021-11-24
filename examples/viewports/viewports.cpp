// ------------------------------------------------------------
//
// Ivf++ Viewport sample
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
#include <ivf/Cube.h>
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>
#include <ivf/Cone.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Lighting.h>
#include <ivf/Light.h>
#include <ivf/Material.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase {
private:

	// Camera movement state variables

	int m_beginX;
	int m_beginY;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	CameraPtr		m_camera;
	CompositePtr	m_scene;
	LightPtr		m_light;

	ViewportPtr		m_upperLeft;
	ViewportPtr		m_upperRight;
	ViewportPtr		m_lowerLeft;
	ViewportPtr		m_lowerRight;

public:
	ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H) {};

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();

	// Mouse event methods

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
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
    // State variables

    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_moveX = 0.0f;
    m_moveY = 0.0f;
    m_zoomX = 0.0f;
    m_zoomY = 0.0f;

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(0.0, 4.0, 9.0);
	m_camera->setPerspective(45.0, 0.1, 100.0);

	// Setup viewports

	m_upperLeft = Viewport::create();
	m_upperRight = Viewport::create();
	m_lowerLeft = Viewport::create();
	m_lowerRight = Viewport::create();

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

	// Create scene composite

	m_scene = Composite::create();

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

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
}

// ------------------------------------------------------------
void ExampleWindow::onResize(int width, int height)
{
	m_upperLeft->setPosition(0,height/2);
	m_upperLeft->setSize(width/2,height/2);
	m_upperRight->setPosition(width/2,height/2);
	m_upperRight->setSize(width/2,height/2);
	m_lowerLeft->setPosition(0,0);
	m_lowerLeft->setSize(width/2,height/2);
	m_lowerRight->setPosition(width/2,0);
	m_lowerRight->setSize(width/2,height/2);
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
{
	m_light->render();
	
	m_camera->setViewport(m_upperLeft);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();

	m_camera->setViewport(m_upperRight);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();
	
	m_camera->setViewport(m_lowerLeft);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();

	m_camera->setViewport(m_lowerRight);
	m_camera->initialize();
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
	
    if (this->isLeftButtonDown())
    {
        m_angleX = (x - m_beginX);
        m_angleY = (y - m_beginY);
        m_beginX = x;
        m_beginY = y;
		
		m_camera->rotatePositionY(m_angleX/100.0);
		m_camera->rotatePositionX(m_angleY/100.0);
		
        this->redraw();
    }

    if (this->isRightButtonDown())
    {
		if (this->getModifierKey()==WidgetBase::MT_SHIFT)
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

		m_camera->moveSideways(m_moveX/100.0);
		m_camera->moveVertical(m_moveY/100.0);
		m_camera->moveDepth(m_zoomY/50.0);
		
        this->redraw();
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

	window->setWindowTitle("Ivf++ viewport example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
