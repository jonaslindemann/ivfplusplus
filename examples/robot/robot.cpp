// ------------------------------------------------------------
//
// Ivf++ Robot arm example
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
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
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
	CameraPtr		m_camera;
	CompositePtr  m_scene;
	LightPtr		m_light;

	TransformPtr  m_part1;
	TransformPtr  m_part2;
	TransformPtr  m_part3;
	TransformPtr  m_arm;

	double m_alfa;
	double m_beta;
	double m_gamma;
	double m_delta;

	void updateArm();
public:
	ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H) {};

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onDestroy();
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

	m_alfa = 0.0;
	m_beta = -45.0;
	m_gamma = 90.0;
	m_delta = 75.0;

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(0.0, 5.0, 5.0);

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

	// Create robot base

	auto lowerBase = Cylinder::create();
	lowerBase->setHeight(0.2);
	lowerBase->setMaterial(yellowMaterial);
	m_scene->addChild(lowerBase);

	auto upperBase = Cylinder::create();
	upperBase->setHeight(0.3);
	upperBase->setRadius(0.5);
	upperBase->setPosition(0.0, 0.1 + 0.15, 0.0);
	upperBase->setMaterial(blueMaterial);
	m_scene->addChild(upperBase);

	// Create part1

	m_part1 = Transform::create();

	auto sphere = Sphere::create();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part1->addChild(sphere);

	auto cylinder = Cylinder::create();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);

	m_part1->addChild(cylinder);
	m_part1->setPosition(0.0, 1.2, 0.0);
	m_part1->setRotationQuat(0.0, 0.0, 1.0, m_delta);

	// Create part2

	m_part2 = Transform::create();

	sphere = Sphere::create();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part2->addChild(sphere);

	cylinder = Cylinder::create();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part2->addChild(cylinder);
	m_part2->addChild(m_part1);
	m_part2->setPosition(0.0, 1.2, 0.0);
	m_part2->setRotationQuat(0.0, 0.0, 1.0, m_gamma);

	// Create part3

	m_part3 = Transform::create();

	sphere = Sphere::create();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part3->addChild(sphere);

	cylinder = Cylinder::create();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part3->addChild(cylinder);
	m_part3->addChild(m_part2);
	m_part3->setPosition(0.0, 0.0, 0.0);
	m_part3->setRotationQuat(0.0, 0.0, 1.0, m_beta);

	// Create complete arm

	m_arm = Transform::create();
	m_arm->addChild(m_part3);
	m_arm->setPosition(0.0, 0.1+0.4, 0.0);
	m_arm->setRotationQuat(0.0, 1.0, 0.0, m_alfa);

	m_scene->addChild(m_arm);

	auto axis = Axis::create();
	axis->setSize(1.5);
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
void ExampleWindow::onDestroy()
{
	delete m_camera;
	delete m_light;
	delete m_scene;
}

// ------------------------------------------------------------
void ExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'a':
		m_alfa += 5.0;
		updateArm();
		break;
	case 'z':
		m_alfa -= 5.0;
		updateArm();
		break;
	case 's':
		m_beta += 5.0;
		updateArm();
		break;
	case 'x':
		m_beta -= 5.0;
		updateArm();
		break;
	case 'd':
		m_gamma += 5.0;
		updateArm();
		break;
	case 'c':
		m_gamma -= 5.0;
		updateArm();
		break;
	case 'f':
		m_delta += 5.0;
		updateArm();
		break;
	case 'v':
		m_delta -= 5.0;
		updateArm();
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
void ExampleWindow::updateArm()
{
	m_arm->setRotationQuat(0.0, 1.0, 0.0, m_alfa);
	m_part1->setRotationQuat(0.0, 0.0, 1.0, m_delta);
	m_part2->setRotationQuat(0.0, 0.0, 1.0, m_gamma);
	m_part3->setRotationQuat(0.0, 0.0, 1.0, m_beta);

	redraw();
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

	window->setWindowTitle("Ivf++ Robot example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
