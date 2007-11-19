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

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

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

class CExampleWindow: public CIvfWindow,
	CIvfInitEvent,
	CIvfKeyboardEvent,
	CIvfTimeoutEvent,
	CIvfSingleSelectionEvent,
	CIvfMultipleSelectionEvent
{
private:
	CIvfCameraPtr		m_camera;
	CIvfScenePtr		m_scene;
	CIvfLightPtr		m_light;

	CIvfMouseViewHandlerPtr m_mouseHandler;
	CIvfSelectionHandlerPtr m_selectionHandler;
	CIvfFlyHandlerPtr		m_flyHandler;
	CIvfSceneHandlerPtr		m_sceneHandler;

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
	virtual void onSelect(CIvfShape* shape);
	virtual void onSelectMultiple(CIvfSelectedShapesVector& shapes);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
	:CIvfWindow(X, Y, W, H) 
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

	m_camera = new CIvfCamera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new CIvfScene();
	m_scene->setView(m_camera);

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

	// Create objects
	
	CIvfCubePtr cube = new CIvfCube();
	cube->setMaterial(redMaterial);
	cube->setPosition(2.0, 0.0, 2.0);
	m_scene->addChild(cube);

	CIvfSpherePtr sphere = new CIvfSphere();
	sphere->setMaterial(greenMaterial);
	sphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(sphere);

	CIvfCylinderPtr cylinder = new CIvfCylinder();
	cylinder->setMaterial(blueMaterial);
	cylinder->setPosition(-2.0, 0.0, -2.0);
	m_scene->addChild(cylinder);

	CIvfConePtr cone = new CIvfCone();
	cone->setMaterial(yellowMaterial);
	cone->setPosition(2.0, 0.0, -2.0);
	cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_scene->addChild(cone);

	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);

	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	// Create event handlers

	m_mouseHandler = new CIvfMouseViewHandler(this, m_camera);
	m_mouseHandler->deactivate();
	
	m_selectionHandler = new CIvfSelectionHandler(this, m_scene);
	m_selectionHandler->setSingleSelectionEvent(this);
	m_selectionHandler->setMultipleSelectionEvent(this);
	m_selectionHandler->activate();
	
	m_flyHandler = new CIvfFlyHandler(this, m_camera);
	m_flyHandler->deactivate();

	m_sceneHandler = new CIvfSceneHandler(this, m_scene);
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
void CExampleWindow::onSelect(CIvfShape* shape)
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
		CIvfShapePtr shape = *it;
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

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ event handler examples");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
