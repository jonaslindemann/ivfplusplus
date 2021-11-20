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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfScene.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSelectionHandler.h>
#include <ivfwidget/IvfFlyHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase,
	CInitEvent,
	CKeyboardEvent,
	CTimeoutEvent,
	CSingleSelectionEvent,
	CMultipleSelectionEvent
{
private:
	CCameraPtr		m_camera;
	CScenePtr		m_scene;
	CLightPtr		m_light;

	CMouseViewHandlerPtr m_mouseHandler;
	CSelectionHandlerPtr m_selectionHandler;
	CFlyHandlerPtr		m_flyHandler;
	CSceneHandlerPtr		m_sceneHandler;

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
	CExampleWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
	virtual bool onTimeout();
	virtual void onSelect(CShape* shape);
	virtual void onSelectMultiple(CIvfSelectedShapesVector& shapes);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
	:CGlutBase(X, Y, W, H) 
{
	addInitEvent(this);
	addKeyboardEvent(this);
	assignTimeoutEvent(0, this);
}

void CExampleWindow::onInit(int width, int height)
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
	m_mouseHandler->deactivate();
	
	m_selectionHandler = new CSelectionHandler(this, m_scene);
	m_selectionHandler->setSingleSelectionEvent(this);
	m_selectionHandler->setMultipleSelectionEvent(this);
	m_selectionHandler->activate();
	
	m_flyHandler = new CFlyHandler(this, m_camera);
	m_flyHandler->deactivate();

	m_sceneHandler = new CSceneHandler(this, m_scene);
	m_sceneHandler->activate();

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
bool CExampleWindow::onTimeout()
{
	if (m_flyHandler->isActive())
		redraw();

	m_flyHandler->update();

	return true;
}

// ------------------------------------------------------------
void CExampleWindow::onSelect(CShape* shape)
{
	if (shape!=NULL)
		cout << shape->getClassName() << " selected." << endl;
	else
		cout << "Nothingness selected." << endl;
}

// ------------------------------------------------------------
void CExampleWindow::onSelectMultiple(CIvfSelectedShapesVector& shapes)
{
	CIvfSelectedShapesIterator it;

	cout << "Multiple selection:" << endl;
	for (it=shapes.begin(); it!=shapes.end(); it++)
	{
		CShapePtr shape = *it;
		cout << "\t" << shape->getClassName() << " selected." << endl;
	}
}

// ------------------------------------------------------------
void CExampleWindow::onKeyboard(int key, int x, int y)
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

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ event handler examples");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
