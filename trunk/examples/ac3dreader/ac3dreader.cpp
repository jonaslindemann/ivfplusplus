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

#include <ivfdef/IvfDef.h>

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfLighting.h>

#include <ivffile/IvfAc3DReader.h>
#include <ivffile/IvfDxfWriter.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutBase {
private:
	CIvfCameraPtr		m_camera;
	CIvfCompositePtr	m_scene;
	CIvfLightPtr		m_light;

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
	CExampleWindow(int X, int Y, int W, int H, bool fullScreen);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual bool onTimeout0();
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H, bool fullScreen)
		:CIvfGlutBase(X, Y, W, H, fullScreen) 
{

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
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create a materials

	// Create scene

	m_scene = new CIvfComposite();

	// Create a file reader

	CIvfAc3DReaderPtr acReader = new CIvfAc3DReader();

	// Set parameters

	acReader->setFileName("models/ivf.ac");
	//acReader->setScaling(1.0);

	// Read file

	double max[3];
	double min[3];

	acReader->read();
	acReader->getSize(min, max);

	// Retrieve poly set

	CIvfShapePtr shape = acReader->getShape();
	shape->initBoundingSphere();
	std::cout << shape->getBoundingSphere()->getRadius() << std::endl;

	m_camera->setPosition(0.0, 0.0, shape->getBoundingSphere()->getRadius());

	m_scene->addChild(shape);


	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(0.0, 0.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	/*
	CIvfDxfWriterPtr dxfWriter = new CIvfDxfWriter();
	dxfWriter->setFileName("models/ac3dreader.dxf");
	dxfWriter->setShape(m_scene);
	dxfWriter->write();
	*/

	//enableTimeout(0.01, 0);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);	
	m_scene->setUselist(true);
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
		if (getModifierKey() == CIvfWidgetBase::MT_SHIFT)
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
bool CExampleWindow::onTimeout0()
{
	if (m_rotating)
	{
		m_angleX = 0.1;
		redraw();
	}
	return true;
}

// ------------------------------------------------------------
void CExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'r' :
		if (m_rotating)
			m_rotating = false;
		else
			m_rotating = true;
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

	CIvfGlutApplication* app = CIvfGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);
	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512, false);
	window->setModeString("1280x1024");

	// Set window title and show window

	//window->setWindowTitle("Ivf++ Ac3D file reader example");
	window->show();
	window->fullscreen();
	
	// Enter main application loop

	app->runAppLoop(window);

	return 0;
}
