// ------------------------------------------------------------
//
// Ivf++ Scene graph culling example
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
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCulling.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
private:

	// Camera movement state variables

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	// Ivf++ object declarations

	CCameraPtr			m_camera;
	CCameraPtr			m_externalCamera;
	CCompositePtr		m_scene;
	CCullingPtr			m_culling;
	CLightPtr			m_light;
	CCameraPtr			m_currentCamera;

	bool m_useCulling;

public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
	// Initialize variables

	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;

	// Initialise culling setting

	m_useCulling = true;

	// Create cameras

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 0.0, 10.0);
	m_camera->setPerspective(45.0, 0.5, 100.0);
	m_camera->setViewPort(width, height);

	m_externalCamera = new CCamera();
	m_externalCamera->setPosition(25.0, 50.0, 50.0);
	m_externalCamera->setPerspective(45.0, 0.5, 100.0);
	m_externalCamera->setViewPort(width, height);

	m_currentCamera = m_camera;

	// Create a materials

	CMaterialPtr material = new CMaterial();
	material->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	// Create scene

	m_scene = new CComposite();

	// Create a somewhat complex sphere 

	int i, j, k;
	int nNodes = 8;
	double distance = 20.0/(nNodes-1);

	// Geometry is reduced by reusing the same sphere.

	CSpherePtr sphere = new CSphere();
	sphere->setSlices(12);
	sphere->setStacks(12);
	sphere->setMaterial(material);
	sphere->setRadius(0.5);

	// Create a grid of spheres.

	CTransformPtr arrayXlt = new CTransform();
	CTransformPtr xlt;

	for (i=0; i<nNodes; i++)
		for (j=0; j<nNodes; j++)
			for (k=0; k<nNodes; k++)
			{
				xlt = new CTransform();
				xlt->setPosition(
					-10 + distance*i,
					-10 + distance*j,
					-10 + distance*k);
				xlt->addChild(sphere);
				arrayXlt->addChild(xlt);
			}

	arrayXlt->setPosition(0.0, 0.0, -15.0);
	arrayXlt->setRotationQuat(0.0, 0.0, 1.0, 45.0);

	m_scene->addChild(arrayXlt);

	// Create culling object

	m_culling = new CCulling();
    m_culling->setComposite(m_scene);
	m_culling->setCullView(m_camera);

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
	m_externalCamera->setViewPort(width, height);
	m_externalCamera->initialize();
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	m_currentCamera->initialize();

	if (m_useCulling)
		m_culling->cull();

	m_light->render();
	m_currentCamera->render();
	m_scene->render();

	cout << "Culled objects = " << m_culling->getCullCount() << endl;;
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
		if (getModifierKey() == CWidgetBase::MT_SHIFT)
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
void CExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case 'c':
		if (m_useCulling)
		{
			cout << "culling off" << endl;
			m_useCulling = false;
		}
		else
		{
			cout << "culling on" << endl;
			m_useCulling = true;
		}

		redraw();
		break;
	case 'x':
		if (m_currentCamera==m_camera)
		{
			m_currentCamera = m_externalCamera; 
		}
		else
		{
			m_currentCamera = m_camera;
		}
		redraw();
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

	window->setWindowTitle("Ivf++ Scene-graph culling example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
