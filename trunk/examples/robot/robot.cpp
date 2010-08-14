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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutBase {
private:
	CIvfCameraPtr		m_camera;
	CIvfCompositePtr  m_scene;
	CIvfLightPtr		m_light;

	CIvfTransformPtr  m_part1;
	CIvfTransformPtr  m_part2;
	CIvfTransformPtr  m_part3;
	CIvfTransformPtr  m_arm;

	double m_alfa;
	double m_beta;
	double m_gamma;
	double m_delta;

	void updateArm();
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfGlutBase(X, Y, W, H) {};


	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onDestroy();
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_alfa = 0.0;
	m_beta = -45.0;
	m_gamma = 90.0;
	m_delta = 75.0;

	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	CIvfMaterialPtr redMaterial = new CIvfMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	CIvfMaterialPtr greenMaterial = new CIvfMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);
	
	CIvfMaterialPtr blueMaterial = new CIvfMaterial();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	CIvfMaterialPtr yellowMaterial = new CIvfMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = new CIvfComposite();

	// Create robot base

	CIvfCylinderPtr lowerBase = new CIvfCylinder();
	lowerBase->setHeight(0.2);
	lowerBase->setMaterial(yellowMaterial);
	m_scene->addChild(lowerBase);

	CIvfCylinderPtr upperBase = new CIvfCylinder();
	upperBase->setHeight(0.3);
	upperBase->setRadius(0.5);
	upperBase->setPosition(0.0, 0.1 + 0.15, 0.0);
	upperBase->setMaterial(blueMaterial);
	m_scene->addChild(upperBase);

	// Create part1

	m_part1 = new CIvfTransform();

	CIvfSpherePtr sphere = new CIvfSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part1->addChild(sphere);

	CIvfCylinderPtr cylinder = new CIvfCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);

	m_part1->addChild(cylinder);
	m_part1->setPosition(0.0, 1.2, 0.0);
	m_part1->setRotationQuat(0.0, 0.0, 1.0, m_delta);

	// Create part2

	m_part2 = new CIvfTransform();

	sphere = new CIvfSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part2->addChild(sphere);

	cylinder = new CIvfCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part2->addChild(cylinder);
	m_part2->addChild(m_part1);
	m_part2->setPosition(0.0, 1.2, 0.0);
	m_part2->setRotationQuat(0.0, 0.0, 1.0, m_gamma);

	// Create part3

	m_part3 = new CIvfTransform();

	sphere = new CIvfSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part3->addChild(sphere);

	cylinder = new CIvfCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part3->addChild(cylinder);
	m_part3->addChild(m_part2);
	m_part3->setPosition(0.0, 0.0, 0.0);
	m_part3->setRotationQuat(0.0, 0.0, 1.0, m_beta);

	// Create complete arm

	m_arm = new CIvfTransform();
	m_arm->addChild(m_part3);
	m_arm->setPosition(0.0, 0.1+0.4, 0.0);
	m_arm->setRotationQuat(0.0, 1.0, 0.0, m_alfa);

	m_scene->addChild(m_arm);

	CIvfAxisPtr axis = new CIvfAxis();
	axis->setSize(1.5);
	m_scene->addChild(axis);
	
	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
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
	m_camera->render();
	m_scene->render();
}

// ------------------------------------------------------------
void CExampleWindow::onDestroy()
{
	delete m_camera;
	delete m_light;
	delete m_scene;
}

// ------------------------------------------------------------
void CExampleWindow::onKeyboard(int key, int x, int y)
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
void CExampleWindow::updateArm()
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

	CIvfGlutApplication* app = CIvfGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Robot example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
