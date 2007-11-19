// ------------------------------------------------------------
//
// Ivf++ Viewport sample
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
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfCone.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfWindow {
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

	CIvfCameraPtr		m_camera;
	CIvfCompositePtr	m_scene;
	CIvfLightPtr		m_light;

	CIvfViewportPtr		m_upperLeft;
	CIvfViewportPtr		m_upperRight;
	CIvfViewportPtr		m_lowerLeft;
	CIvfViewportPtr		m_lowerRight;

public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfWindow(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();

	// Mouse event methods

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
    // State variables

    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_moveX = 0.0f;
    m_moveY = 0.0f;
    m_zoomX = 0.0f;
    m_zoomY = 0.0f;

	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(0.0, 4.0, 9.0);
	m_camera->setPerspective(45.0, 0.1, 100.0);

	// Setup viewports

	m_upperLeft = new CIvfViewport();
	m_upperRight = new CIvfViewport();
	m_lowerLeft = new CIvfViewport();
	m_lowerRight = new CIvfViewport();

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

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
}

// ------------------------------------------------------------
void CExampleWindow::onResize(int width, int height)
{
	m_upperLeft->setPosition(0,height/2);
	m_upperLeft->setSize(width/2,height/2);
	m_upperRight->setPosition(width/2,height/2);
	m_upperRight->setSize(width/2,height/2);
	m_lowerLeft->setPosition(0,0);
	m_lowerLeft->setSize(width/2,height/2);
	m_lowerRight->setPosition(width/2,0);
	m_lowerRight->setSize(width/2,height/2);
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	m_light->render();
	
	m_camera->setViewport(m_upperLeft);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();

	m_camera->setViewport(m_upperRight);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();
	
	m_camera->setViewport(m_lowerLeft);
	m_camera->initialize();
	m_camera->render();
	m_scene->render();

	m_camera->setViewport(m_lowerRight);
	m_camera->initialize();
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
	
    if (this->isLeftButtonDown())
    {
        m_angleX = (x - m_beginX);
        m_angleY = (y - m_beginY);
        m_beginX = x;
        m_beginY = y;
		
		m_camera->rotatePositionY(m_angleX/100.0);
		m_camera->rotatePositionX(m_angleY/100.0);
		
        this->redraw();
    }

    if (this->isRightButtonDown())
    {
		if (this->getModifierKey()==CIvfWidgetBase::MT_SHIFT)
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
		
        this->redraw();
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
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ viewport example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
