// ------------------------------------------------------------
//
// Ivf++ Robot arm example with movable camera
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
#include <ivf/IvfMaterial.h>
#include <ivf/IvfRasterization.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
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

	CCameraPtr		m_camera;
	CLightPtr		m_light;
	CCompositePtr  m_scene;

	// Robot state variables

	double m_alfa;
	double m_beta;
	double m_gamma;
	double m_delta;

	// Robot arm transforms

	CTransformPtr  m_part1;
	CTransformPtr  m_part2;
	CTransformPtr  m_part3;
	CTransformPtr  m_arm;

	// Routine for updating the arm

	void updateArm();
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};


	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onDestroy();
	virtual void onKeyboard(int key, int x, int y);
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
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

	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	// Initialize Ivf++ camera

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	CMaterialPtr redMaterial = new CMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	CMaterialPtr greenMaterial = new CMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);
	
	CMaterialPtr blueMaterial = new CMaterial();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	CMaterialPtr yellowMaterial = new CMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = new CComposite();

	// Create robot base

	CCylinderPtr lowerBase = new CCylinder();
	lowerBase->setHeight(0.2);
	lowerBase->setSlices(20);
	lowerBase->setMaterial(yellowMaterial);
	m_scene->addChild(lowerBase);

	CCylinderPtr upperBase = new CCylinder();
	upperBase->setHeight(0.3);
	upperBase->setRadius(0.5);
	upperBase->setSlices(20);
	upperBase->setPosition(0.0, 0.1 + 0.15, 0.0);
	upperBase->setMaterial(blueMaterial);
	m_scene->addChild(upperBase);

	// Create part1

	m_part1 = new CTransform();

	CSpherePtr sphere = new CSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part1->addChild(sphere);

	CCylinderPtr cylinder = new CCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);

	m_part1->addChild(cylinder);
	m_part1->setPosition(0.0, 1.2, 0.0);
	m_part1->setRotationQuat(0.0, 0.0, 1.0, m_delta);

	// Create part2

	m_part2 = new CTransform();

	sphere = new CSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part2->addChild(sphere);

	cylinder = new CCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part2->addChild(cylinder);
	m_part2->addChild(m_part1);
	m_part2->setPosition(0.0, 1.2, 0.0);
	m_part2->setRotationQuat(0.0, 0.0, 1.0, m_gamma);

	// Create part3

	m_part3 = new CTransform();

	sphere = new CSphere();
	sphere->setRadius(0.1);
	sphere->setMaterial(redMaterial);
	m_part3->addChild(sphere);

	cylinder = new CCylinder();
	cylinder->setRadius(0.1);
	cylinder->setHeight(1.0);
	cylinder->setMaterial(greenMaterial);
	cylinder->setPosition(0.0, 0.1 + 0.5, 0.0);
	m_part3->addChild(cylinder);
	m_part3->addChild(m_part2);
	m_part3->setPosition(0.0, 0.0, 0.0);
	m_part3->setRotationQuat(0.0, 0.0, 1.0, m_beta);

	// Create complete arm

	m_arm = new CTransform();
	m_arm->addChild(m_part3);
	m_arm->setPosition(0.0, 0.1+0.4, 0.0);
	m_arm->setRotationQuat(0.0, 1.0, 0.0, m_alfa);

	m_scene->addChild(m_arm);

	CAxisPtr axis = new CAxis();
	axis->setSize(1.5);
	m_scene->addChild(axis);
	
	// Create a light

	CLightingPtr lighting = CLighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	CRasterizationPtr rasterOps = CRasterization::getInstance();
	rasterOps->enableCullFace();
	rasterOps->setCullFace(CRasterization::CF_BACK);
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
		
		m_camera->rotatePositionY(m_angleX/100.0);
		m_camera->rotatePositionX(m_angleY/100.0);
		
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
		
		m_camera->moveSideways(m_moveX/100.0);
		m_camera->moveVertical(m_moveY/100.0);
		m_camera->moveDepth(m_zoomY/50.0);
		
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

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Robot example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
