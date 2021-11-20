// ------------------------------------------------------------
//
// Ivf++ Object selection example
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
#include <ivf/Cube.h>
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>
#include <ivf/Cone.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/BufferSelection.h>
#include <ivf/Lighting.h>
#include <ivf/Light.h>
#include <ivf/Material.h>

using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
private:

	// Ivf++ objects

	CCameraPtr				m_camera;
	CCompositePtr			m_scene;
	CLightPtr				m_light;

	CBufferSelectionPtr	m_selection;
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 4.0, 9.0);

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

	// Create scene composite

	m_scene = new CComposite();

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

	// Setup the selection algorithm,

	m_selection = new CBufferSelection();
	m_selection->setView(m_camera);
	m_selection->setComposite(m_scene);
	
	// Create a light

	CLightingPtr lighting = CLighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setType(CLight::LT_DIRECTIONAL);
	m_light->setDirection(1.0, 1.0, 1.0);
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
	m_camera->render();
	m_light->render();
	m_scene->render();
}

// ------------------------------------------------------------
void CExampleWindow::onMouseDown(int x, int y)
{
	int i;

	if (isLeftButtonDown())
	{
		m_selection->pick(x, y);
		if (m_selection->getSelectedShape()!=NULL)
		{
			m_selection->getSelectedShape()->setHighlight(CShape::HS_ON);
			redraw();
		}
		else
		{
			m_scene->setHighlightChildren(CShape::HS_OFF);
			redraw();
		}
	}
	
	if (isRightButtonDown())
	{
		m_selection->pick(x, y);
		if (m_selection->getSize()!=0)
		{
			for (i=0; i<m_selection->getSize(); i++)
				m_selection->getSelectedShape(i)->setHighlight(CShape::HS_ON);

			redraw();
		}
		else
		{
			m_scene->setHighlightChildren(CShape::HS_OFF);
			redraw();
		}
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

	window->setWindowTitle("Ivf++ Selection example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
