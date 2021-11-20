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

#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include <ivfwidget/MouseViewHandler.h>

#include <ivf/Camera.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Cube.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase,
	CInitEvent,
	CRenderEvent,
	COverlayEvent,
	CResizeEvent
{
private:
	CCameraPtr		m_camera;
	CCompositePtr	m_scene;
	CLightPtr		m_light;
	
	CMouseViewHandlerPtr m_mouseViewHandler;

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
:CGlutBase(X, Y, W, H)
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

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	CMaterialPtr yellowMaterial = new CMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = new CComposite();

	CCubePtr cube = new CCube();
	
	cube->setMaterial(yellowMaterial);
	m_scene->addChild(cube);

	// First point
	
	CAxisPtr axis = new CAxis();
	m_scene->addChild(axis);
	
	// Create a light

	CLightingPtr lighting = CLighting::getInstance();

	m_light = new CLight();
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	this->setUseOverlay(true);
	
	m_mouseViewHandler = new CMouseViewHandler(this, m_camera);
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

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
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
