// ------------------------------------------------------------
//
// Ivf++ colored cube example
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
#include <ivf/Lighting.h>
#include <ivf/Cube.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
private:
	CCameraPtr m_camera;
	CCubePtr   m_cube;
	CLightPtr  m_light;
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

	static CExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindowPtr CExampleWindow::create(int X, int Y, int W, int H)
{
	return CExampleWindowPtr(new CExampleWindow(X, Y, W, H));
}

void CExampleWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = CCamera::create();
	m_camera->setPosition(2.0, 2.0, 2.0);

	// Create a material

	auto material = CMaterial::create();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	// Create a cube
	
	m_cube = CCube::create();
	m_cube->setMaterial(material);
	
	// Create a light

	auto lighting = CLighting::getInstance();

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
	m_cube->render();
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	auto app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH);

	// Create a window

	auto window = CExampleWindow::create(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Cube example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
