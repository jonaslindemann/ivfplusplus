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

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase,
	CInitEvent,
	CTimeoutEvent,
	CGuiEvent
{
private:
	CCameraPtr		m_camera;
	CScenePtr		m_scene;
	CLightPtr		m_light;

	CMouseViewHandlerPtr m_mouseHandler;
	CSceneHandlerPtr		m_sceneHandler;
	CImUiHandlerPtr		m_uiHandler;

	bool m_rotating;

public:
	CExampleWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual bool onTimeout();
	virtual void onGui();
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
	:CGlutBase(X, Y, W, H) 
{
	addInitEvent(this);
	assignTimeoutEvent(0, this);
}

void CExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_rotating = false;

	// Initialize Ivf++ camera

	m_camera = new CCamera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new CScene();
	m_scene->setView(m_camera);

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

	// Create objects
	
	CCubePtr cube = new CCube();
	cube->setMaterial(redMaterial);
	cube->setPosition(2.0, 0.0, 2.0);
	m_scene->addChild(cube);

	CSpherePtr sphere = new CSphere();
	sphere->setMaterial(greenMaterial);
	sphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(sphere);

	CCylinderPtr cylinder = new CCylinder();
	cylinder->setMaterial(blueMaterial);
	cylinder->setPosition(-2.0, 0.0, -2.0);
	m_scene->addChild(cylinder);

	CConePtr cone = new CCone();
	cone->setMaterial(yellowMaterial);
	cone->setPosition(2.0, 0.0, -2.0);
	cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_scene->addChild(cone);

	CAxisPtr axis = new CAxis();
	m_scene->addChild(axis);

	// Create a light

	CLightingPtr lighting = CLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	// Create event handlers

	m_mouseHandler = new CMouseViewHandler(this, m_camera);
	m_mouseHandler->activate();
	
	m_sceneHandler = new CSceneHandler(this, m_scene);
	m_sceneHandler->activate();
	
	m_uiHandler = new CImUiHandler(this);
	m_uiHandler->setGuiEvent(this);
	m_uiHandler->activate();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
bool CExampleWindow::onTimeout()
{

	return true;
}

void CExampleWindow::onGui()
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

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_DEPTH|IVF_MULTISAMPLE|IVF_RGBA);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ event handler examples");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
