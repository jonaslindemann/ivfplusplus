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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfLOD.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfCylinder.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
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

	CCameraPtr		m_camera;
	CLightPtr		m_light;
	CCompositePtr	m_scene;

	CLODPtr			m_lod1;
	CLODPtr			m_lod2;
	CLODPtr			m_lod3;
	CLODPtr			m_lod4;
	CSwitchPtr		m_switch;

	double m_lodNear;
	double m_lodFar;

	void updateLOD();
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

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

void CExampleWindow::onInit(int width, int height)
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

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 5.0, 20.0);

	// Create a materials

	CMaterialPtr material = new CMaterial();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	// Create scene

	m_scene = new CComposite();

	// Create four spheres with different complexity

	CSpherePtr sphereDetailed = new CSphere();
	sphereDetailed->setSlices(12);
	sphereDetailed->setStacks(12);
	
	CSpherePtr sphereNormal = new CSphere();
	sphereNormal->setSlices(10);
	sphereNormal->setStacks(8);

	CSpherePtr sphereSmall = new CSphere();
	sphereSmall->setSlices(8);
	sphereSmall->setStacks(6);
	
	CSpherePtr sphereTiny = new CSphere();
	sphereTiny->setSlices(6);
	sphereTiny->setStacks(4);

	// Create LOD objects

	m_lod1 = new CLOD();
	m_lod1->addChild(sphereDetailed);
	m_lod1->addChild(sphereNormal);
	m_lod1->addChild(sphereSmall);
	m_lod1->addChild(sphereTiny);
	m_lod1->setPosition(-5.0, 0.0, -5.0);
	m_lod1->setMaterial(material);
	m_lod1->setCamera(m_camera);
	m_lod1->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod1);
	
	m_lod2 = new CLOD();
	m_lod2->addChild(sphereDetailed);
	m_lod2->addChild(sphereNormal);
	m_lod2->addChild(sphereSmall);
	m_lod2->addChild(sphereTiny);
	m_lod2->setPosition(5.0, 0.0, -5.0);
	m_lod2->setMaterial(material);
	m_lod2->setCamera(m_camera);
	m_lod2->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod2);

	m_lod3 = new CLOD();
	m_lod3->addChild(sphereDetailed);
	m_lod3->addChild(sphereNormal);
	m_lod3->addChild(sphereSmall);
	m_lod3->addChild(sphereTiny);
	m_lod3->setPosition(-5.0, 0.0, 5.0);
	m_lod3->setMaterial(material);
	m_lod3->setCamera(m_camera);
	m_lod3->setLimits(m_lodNear, m_lodFar);
	m_scene->addChild(m_lod3);

	m_lod4 = new CLOD();
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

	CCubePtr cube = new CCube();
	cube->setMaterial(material);
	CCylinderPtr cylinder = new CCylinder();
	cylinder->setMaterial(material);
	
	m_switch = new CSwitch();
	m_switch->addChild(cube);
	m_switch->addChild(cylinder);

	m_scene->addChild(m_switch);
	
	// Create a light

	CLightingPtr lighting = CLighting::getInstance();
	
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
void CExampleWindow::onKeyboard(int key, int x, int y)
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
void CExampleWindow::updateLOD()
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

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ LOD/Switch example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
