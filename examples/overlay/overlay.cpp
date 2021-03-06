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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivfwidget/IvfMouseViewHandler.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfCube.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutBase,
	CIvfInitEvent,
	CIvfRenderEvent,
	CIvfOverlayEvent,
	CIvfResizeEvent
{
private:
	CIvfCameraPtr		m_camera;
	CIvfCompositePtr	m_scene;
	CIvfLightPtr		m_light;
	
	CIvfMouseViewHandlerPtr m_mouseViewHandler;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

public:
	CExampleWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onRender();
	virtual void onOverlay();
	virtual void onResize(int width, int height);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
:CIvfGlutBase(X, Y, W, H)
{
	this->addInitEvent(this);
	this->addRenderEvent(this);
	this->addOverlayEvent(this);
	this->addResizeEvent(this);
}

void CExampleWindow::onResize(int width, int height)
{
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

void CExampleWindow::onInit(int width, int height)
{	
	// Initialize variables

	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	CIvfMaterialPtr yellowMaterial = new CIvfMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = new CIvfComposite();

	CIvfCubePtr cube = new CIvfCube();
	
	cube->setMaterial(yellowMaterial);
	m_scene->addChild(cube);

	// First point
	
	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);
	
	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();

	m_light = new CIvfLight();
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	this->setUseOverlay(true);
	
	m_mouseViewHandler = new CIvfMouseViewHandler(this, m_camera);
	m_mouseViewHandler->activate();
	
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	m_light->render();
	m_camera->render();
	m_scene->render();
}

// ------------------------------------------------------------
void CExampleWindow::onOverlay()
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

	CIvfGlutApplication* app = CIvfGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_DEPTH|IVF_MULTISAMPLE|IVF_RGBA);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Overlay example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
