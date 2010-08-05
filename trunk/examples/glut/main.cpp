
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif 

#include <string>

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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutWindow.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfCube.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutWindow {
private:
	CIvfCamera* m_camera;
	CIvfCube*   m_cube;
	CIvfLight*  m_light;
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfGlutWindow(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
	std::cout << "onInit" << std::endl;
	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(2.0, 2.0, 2.0);

	// Create a material

	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	// Create a cube
	
	m_cube = new CIvfCube();
	m_cube->setMaterial(material);
	
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
	std::cout << "onResize" << std::endl;
	std::cout << "width" << width << ", " << height << std::endl;
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	std::cout << "onRender" << std::endl;
	m_light->render();
	m_camera->render();
	m_cube->render();
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------


int main(int argc, char** argv)
{
	CIvfGlutApplication* application = CIvfGlutApplication::getInstance(&argc, argv);
	application->setDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create a window
	
	CExampleWindowPtr window = new CExampleWindow(50,50,400,300);

	// Set window title and show window

	//window->setWindowTitle("Ivf++ Cube example");
	//window->show();

	// Enter main application loop
	
	application->run();

	return 0;
}