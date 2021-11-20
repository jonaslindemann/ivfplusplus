// ------------------------------------------------------------
//
// Ivf++ lighting example
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
#include <ivf/IvfSphere.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMesh.h>
#include <ivf/IvfLightingState.h>
#include <ivf/IvfRasterization.h>

#include <ivfmath/IvfVec3d.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase {
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

	CCameraPtr		m_camera;
	CCompositePtr	m_scene;
	CLightPtr		m_light;
	CLightingPtr		m_lighting;
	CSpherePtr		m_lightSphere;

	CVec3d			m_direction;
	double				m_speed;
public:
	CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();

	// Mouse event methods

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);

	// Keyboard events

	virtual void onKeyboard(int key, int x, int y);

	// Timer events

	virtual bool onTimeout0();
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

	m_camera = new CCamera();
	m_camera->setPosition(0.0, 0.0, 9.0);
	m_camera->setPerspective(60.0, 0.1, 40.0);

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

	CMaterialPtr whiteMaterial = new CMaterial();
	whiteMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	whiteMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	whiteMaterial->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Create scene composite

	m_scene = new CComposite();

	// Create a sphere representing the light

	CLightingStatePtr lightState = new CLightingState();
	lightState->setLighting(false);

	m_lightSphere = new CSphere();
	m_lightSphere->setMaterial(whiteMaterial);
	m_lightSphere->setRadius(0.1);
	m_lightSphere->setRenderState(lightState);
	m_lightSphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(m_lightSphere);

	// Create a sphere in the middle

	CSpherePtr sphere = new CSphere();
	sphere->setRadius(1.0);
	sphere->setMaterial(redMaterial);
	sphere->setStacks(12);
	sphere->setSlices(20);
	m_scene->addChild(sphere);

	// Create a room

	CMeshPtr floor = new CMesh();
	floor->setMeshType(CMesh::MT_ORDER_2);
	floor->setMeshResolution(30,30);
	floor->createMesh(10.0, 10.0);
	floor->setMaterial(redMaterial);
	floor->setPosition(0.0, -3.0, 0.0);

	m_scene->addChild(floor);
	
	CMeshPtr roof = new CMesh();
	roof->setMeshType(CMesh::MT_ORDER_2);
	roof->setMeshResolution(30,30);
	roof->createMesh(10.0, 10.0);
	roof->setMaterial(greenMaterial);
	roof->setPosition(0.0, 3.0, 0.0);
	roof->setRotationQuat(0.0, 0.0, 1.0, 180.0f);
	m_scene->addChild(roof);

	CMeshPtr wall1 = new CMesh();
	wall1->setMeshType(CMesh::MT_ORDER_2);
	wall1->setMeshResolution(30,30);
	wall1->createMesh(10.0, 6.0);
	wall1->setMaterial(blueMaterial);
	wall1->setPosition(0.0, 0.0, -5.0);
	wall1->setRotationQuat(1.0, 0.0, 0.0, 90.0f);
	m_scene->addChild(wall1);
	
	CMeshPtr wall2 = new CMesh();
	wall2->setMeshType(CMesh::MT_ORDER_2);
	wall2->setMeshResolution(30,30);
	wall2->createMesh(10.0, 6.0);
	wall2->setMaterial(blueMaterial);
	wall2->setPosition(0.0, 0.0, 5.0);
	wall2->setRotationQuat(1.0, 0.0, 0.0, -90.0f);
	m_scene->addChild(wall2);

	CMeshPtr wall3 = new CMesh();
	wall3->setMeshType(CMesh::MT_ORDER_2);
	wall3->setMeshResolution(30,30);
	wall3->createMesh(6.0, 10.0);
	wall3->setMaterial(yellowMaterial);
	wall3->setPosition(5.0, 0.0, 0.0);
	wall3->setRotationQuat(0.0, 0.0, 1.0, 90.0f);
	m_scene->addChild(wall3);

	CMeshPtr wall4 = new CMesh();
	wall4->setMeshType(CMesh::MT_ORDER_2);
	wall4->setMeshResolution(30,30);
	wall4->createMesh(6.0, 10.0);
	wall4->setMaterial(yellowMaterial);
	wall4->setPosition(-5.0, 0.0, 0.0);
	wall4->setRotationQuat(0.0, 0.0, 1.0, -90.0f);
	m_scene->addChild(wall4);

	// Create a light

	m_lighting = CLighting::getInstance();

	m_light = m_lighting->getLight(0);
	m_light->setType(CLight::LT_POINT);
	m_light->setLightPosition(-2.0, 0.0, 2.0);
	m_light->setSpotCutoff(70.0f);
	m_light->setSpotExponent(20.0f);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	CRasterizationPtr rasterOps = CRasterization::getInstance();
	rasterOps->enableCullFace();
	rasterOps->setCullFace(CRasterization::CF_BACK);

	// Setup moving light

	m_direction.setComponents(1.0, 1.0, 1.0);
	m_speed = 0.06;

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
void CExampleWindow::onResize(int width, int height)
{
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void CExampleWindow::onRender()
{
	m_camera->render();
	m_lighting->render();
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
		if (this->getModifierKey()==CWidgetBase::MT_SHIFT)
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
void CExampleWindow::onKeyboard(int key, int x, int y)
{
	float angle;
	float exponent;

	switch (key) {
	case 'l':
		if (m_light->getType()==CLight::LT_POINT)
			m_light->setType(CLight::LT_SPOT);
		else
			m_light->setType(CLight::LT_POINT);
		break;
	case 'a':
		angle = m_light->getSpotCutoff();
		angle += 5.0f;
		cout << "Angle = " << angle  << endl;
		m_light->setSpotCutoff(angle);
		break;
	case 'z':
		angle = m_light->getSpotCutoff();
		angle -= 5.0f;
		cout << "Angle = " << angle  << endl;
		m_light->setSpotCutoff(angle);
		break;
	case 's':
		exponent = m_light->getSpotExponent();
		exponent += 1.0f;
		cout << "Exponent = " << exponent  << endl;
		m_light->setSpotExponent(exponent);
		break;
	case 'x':
		exponent = m_light->getSpotExponent();
		exponent -= 1.0f;
		cout << "Exponent = " << exponent  << endl;
		m_light->setSpotExponent(exponent);
		break;
	case 'd':
		m_speed += 0.01;
		break;
	case 'c':
		m_speed -= 0.01;
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
bool CExampleWindow::onTimeout0()
{
	CVec3d pos;
	double x, y, z;
	double ex, ey, ez;
	
	pos = m_lightSphere->getPosition();
	pos = pos + m_direction * m_speed;

	pos.getComponents(x, y, z);
	m_direction.getComponents(ex, ey, ez);

	if ((x>4.8)||(x<-4.8))
		ex = -ex;

	if ((y>2.8)||(y<-2.8))
		ey = -ey;

	if ((z>4.8)||(z<-4.8))
		ez = -ez;

	m_direction.setComponents(ex, ey, ez);

	m_lightSphere->setPosition(pos);
	pos.getComponents(x, y, z);
	m_light->setPosition(x, y, z);
	m_light->setSpotDirection(ex, ey, ez);

	this->redraw();

	return true;
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

	window->setWindowTitle("Ivf++ Lighting example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
