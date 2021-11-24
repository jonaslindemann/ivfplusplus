// ------------------------------------------------------------
//
// Ivf++ LOD/Switch example
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
#include <ivf/Light.h>
#include <ivf/Material.h>
#include <ivf/Sphere.h>
#include <ivf/LOD.h>
#include <ivf/Cube.h>
#include <ivf/Cylinder.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase {
private:

	// Camera movement state variables

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	// Ivf++ object declarations

	CameraPtr		m_camera;
	LightPtr		m_light;
	CompositePtr	m_scene;

	LODPtr			m_lod1;
	LODPtr			m_lod2;
	LODPtr			m_lod3;
	LODPtr			m_lod4;
	SwitchPtr		m_switch;

	double m_lodNear;
	double m_lodFar;

	void updateLOD();
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

	// Initialise LOD limits

	m_lodNear = 2.0;
	m_lodFar = 20.0;

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(0.0, 5.0, 20.0);

	// Create a materials

	auto material = Material::create();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	// Create scene

	m_scene = Composite::create();

	// Create four spheres with different complexity

	auto sphereDetailed = Sphere::create();
	sphereDetailed->setSlices(12);
	sphereDetailed->setStacks(12);
	
	auto sphereNormal = Sphere::create();
	sphereNormal->setSlices(10);
	sphereNormal->setStacks(8);

	auto sphereSmall = Sphere::create();
	sphereSmall->setSlices(8);
	sphereSmall->setStacks(6);
	
	auto sphereTiny = Sphere::create();
	sphereTiny->setSlices(6);
	sphereTiny->setStacks(4);

	// Create LOD objects

	m_lod1 = LOD::create();
	m_lod1->addChild(sphereDetailed);
	m_lod1->addChild(sphereNormal);
	m_lod1->addChild(sphereSmall);
	m_lod1->addChild(sphereTiny);
	m_lod1->setPosition(-5.0, 0.0, -5.0);
	m_lod1->setMaterial(material);
	m_lod1->setCamera(m_camera);
	m_lod1->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod1);
	
	m_lod2 = LOD::create();
	m_lod2->addChild(sphereDetailed);
	m_lod2->addChild(sphereNormal);
	m_lod2->addChild(sphereSmall);
	m_lod2->addChild(sphereTiny);
	m_lod2->setPosition(5.0, 0.0, -5.0);
	m_lod2->setMaterial(material);
	m_lod2->setCamera(m_camera);
	m_lod2->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod2);

	m_lod3 = LOD::create();
	m_lod3->addChild(sphereDetailed);
	m_lod3->addChild(sphereNormal);
	m_lod3->addChild(sphereSmall);
	m_lod3->addChild(sphereTiny);
	m_lod3->setPosition(-5.0, 0.0, 5.0);
	m_lod3->setMaterial(material);
	m_lod3->setCamera(m_camera);
	m_lod3->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod3);

	m_lod4 = LOD::create();
	m_lod4->addChild(sphereDetailed);
	m_lod4->addChild(sphereNormal);
	m_lod4->addChild(sphereSmall);
	m_lod4->addChild(sphereTiny);
	m_lod4->setPosition(5.0, 0.0, 5.0);
	m_lod4->setCamera(m_camera);
	m_lod4->setMaterial(material);
	m_lod4->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod4);

	// Create a switch object

	auto cube = Cube::create();
	cube->setMaterial(material);
	auto cylinder = Cylinder::create();
	cylinder->setMaterial(material);
	
	m_switch = new Switch();
	m_switch->addChild(cube);
	m_switch->addChild(cylinder);

	m_scene->addChild(m_switch);
	
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
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
{
	m_light->render();
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

		m_camera->rotatePositionY(m_angleX/100.0);
		m_camera->rotatePositionX(m_angleY/100.0);

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

		m_camera->moveSideways(m_moveX/100.0);
		m_camera->moveVertical(m_moveY/100.0);
		m_camera->moveDepth(m_zoomY/50.0);

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
void ExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'a':
		m_lodNear += 0.5;
		updateLOD();
		redraw();
		break;
	case 'z':
		m_lodNear -= 0.5;
		updateLOD();
		redraw();
		break;
	case 's':
		m_lodFar += 0.5;
		updateLOD();
		redraw();
		break;
	case 'x':
		m_lodFar -= 0.5;
		updateLOD();
		redraw();
		break;
	case 'd':
		m_switch->cycleForward();
		redraw();
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
void ExampleWindow::updateLOD()
{
	m_lod1->setLimits(m_lodNear, m_lodFar);
	m_lod2->setLimits(m_lodNear, m_lodFar);
	m_lod3->setLimits(m_lodNear, m_lodFar);
	m_lod4->setLimits(m_lodNear, m_lodFar);

	cout << "Lod limits set to: " << m_lodNear << ", " << m_lodFar << endl;
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

	window->setWindowTitle("Ivf++ LOD/Switch example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
