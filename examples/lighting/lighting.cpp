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

#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include <ivf/Camera.h>
#include <ivf/Sphere.h>
#include <ivf/Composite.h>
#include <ivf/Material.h>
#include <ivf/Lighting.h>
#include <ivf/Mesh.h>
#include <ivf/LightingState.h>
#include <ivf/Rasterization.h>

#include <ivfmath/Vec3d.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase {
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

	CameraPtr		m_camera;
	CompositePtr	m_scene;
	LightPtr		m_light;
	LightingPtr		m_lighting;
	SpherePtr		m_lightSphere;

	Vec3d			m_direction;
	double				m_speed;
public:
	ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H) {};

	static ExampleWindowPtr create(int X, int Y, int W, int H);

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

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
	return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

void ExampleWindow::onInit(int width, int height)
{
    // State variables

    m_angleX = 0.0f;
    m_angleY = 0.0f;
    m_moveX = 0.0f;
    m_moveY = 0.0f;
    m_zoomX = 0.0f;
    m_zoomY = 0.0f;

	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPosition(0.0, 0.0, 9.0);
	m_camera->setPerspective(60.0, 0.1, 40.0);

	// Create a materials

	auto redMaterial = Material::create();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	auto greenMaterial = Material::create();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);
	
	auto blueMaterial = Material::create();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	auto yellowMaterial = Material::create();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	auto whiteMaterial = Material::create();
	whiteMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	whiteMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	whiteMaterial->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Create scene composite

	m_scene = Composite::create();

	// Create a sphere representing the light

	auto lightState = LightingState::create();
	lightState->setLighting(false);

	m_lightSphere = Sphere::create();
	m_lightSphere->setMaterial(whiteMaterial);
	m_lightSphere->setRadius(0.1);
	m_lightSphere->setRenderState(lightState);
	m_lightSphere->setPosition(-2.0, 0.0, 2.0);
	m_scene->addChild(m_lightSphere);

	// Create a sphere in the middle

	auto sphere = Sphere::create();
	sphere->setRadius(1.0);
	sphere->setMaterial(redMaterial);
	sphere->setStacks(12);
	sphere->setSlices(20);
	m_scene->addChild(sphere);

	// Create a room

	auto floor = Mesh::create();
	floor->setMeshType(Mesh::MT_ORDER_2);
	floor->setMeshResolution(30,30);
	floor->createMesh(10.0, 10.0);
	floor->setMaterial(redMaterial);
	floor->setPosition(0.0, -3.0, 0.0);

	m_scene->addChild(floor);
	
	auto roof = Mesh::create();
	roof->setMeshType(Mesh::MT_ORDER_2);
	roof->setMeshResolution(30,30);
	roof->createMesh(10.0, 10.0);
	roof->setMaterial(greenMaterial);
	roof->setPosition(0.0, 3.0, 0.0);
	roof->setRotationQuat(0.0, 0.0, 1.0, 180.0f);
	m_scene->addChild(roof);

	auto wall1 = Mesh::create();
	wall1->setMeshType(Mesh::MT_ORDER_2);
	wall1->setMeshResolution(30,30);
	wall1->createMesh(10.0, 6.0);
	wall1->setMaterial(blueMaterial);
	wall1->setPosition(0.0, 0.0, -5.0);
	wall1->setRotationQuat(1.0, 0.0, 0.0, 90.0f);
	m_scene->addChild(wall1);
	
	auto wall2 = Mesh::create();
	wall2->setMeshType(Mesh::MT_ORDER_2);
	wall2->setMeshResolution(30,30);
	wall2->createMesh(10.0, 6.0);
	wall2->setMaterial(blueMaterial);
	wall2->setPosition(0.0, 0.0, 5.0);
	wall2->setRotationQuat(1.0, 0.0, 0.0, -90.0f);
	m_scene->addChild(wall2);

	auto wall3 = Mesh::create();
	wall3->setMeshType(Mesh::MT_ORDER_2);
	wall3->setMeshResolution(30,30);
	wall3->createMesh(6.0, 10.0);
	wall3->setMaterial(yellowMaterial);
	wall3->setPosition(5.0, 0.0, 0.0);
	wall3->setRotationQuat(0.0, 0.0, 1.0, 90.0f);
	m_scene->addChild(wall3);

	auto wall4 = Mesh::create();
	wall4->setMeshType(Mesh::MT_ORDER_2);
	wall4->setMeshResolution(30,30);
	wall4->createMesh(6.0, 10.0);
	wall4->setMaterial(yellowMaterial);
	wall4->setPosition(-5.0, 0.0, 0.0);
	wall4->setRotationQuat(0.0, 0.0, 1.0, -90.0f);
	m_scene->addChild(wall4);

	// Create a light

	m_lighting = Lighting::getInstance();

	m_light = m_lighting->getLight(0);
	m_light->setType(Light::LT_POINT);
	m_light->setLightPosition(-2.0, 0.0, 2.0);
	m_light->setSpotCutoff(70.0f);
	m_light->setSpotExponent(20.0f);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	auto rasterOps = Rasterization::getInstance();
	rasterOps->enableCullFace();
	rasterOps->setCullFace(Rasterization::CF_BACK);

	// Setup moving light

	m_direction.setComponents(1.0, 1.0, 1.0);
	m_speed = 0.06;

	enableTimeout(0.01, 0);
}

// ------------------------------------------------------------
void ExampleWindow::onResize(int width, int height)
{
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
{
	m_camera->render();
	m_lighting->render();
	m_scene->render();
}

// ------------------------------------------------------------
void ExampleWindow::onMouseDown(int x, int y)
{
	m_beginX = x;
	m_beginY = y;
}

// ------------------------------------------------------------
void ExampleWindow::onMouseMove(int x, int y)
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
		if (this->getModifierKey()==WidgetBase::MT_SHIFT)
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
void ExampleWindow::onMouseUp(int x, int y)
{
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_moveX = 0.0;
	m_moveY = 0.0;
	m_zoomX = 0.0;
	m_zoomY = 0.0;
}

// ------------------------------------------------------------
void ExampleWindow::onKeyboard(int key, int x, int y)
{
	float angle;
	float exponent;

	switch (key) {
	case 'l':
		if (m_light->getType()==Light::LT_POINT)
			m_light->setType(Light::LT_SPOT);
		else
			m_light->setType(Light::LT_POINT);
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
bool ExampleWindow::onTimeout0()
{
	Vec3d pos;
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

	auto app = GlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	auto window = ExampleWindow::create(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Lighting example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
