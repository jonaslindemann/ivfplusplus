// ------------------------------------------------------------
//
// Ivf++ 3D UI interaction example
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
#include <ivf/Brick.h>
#include <ivf/Cube.h>
#include <ivf/Sphere.h>
#include <ivf/Cylinder.h>
#include <ivf/Cone.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Light.h>
#include <ivf/Material.h>
#include <ivf/Lighting.h>

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/SceneHandler.h>
#include <ivfwidget/InteractionHandler.h>

#include <ivf3dui/UIButton.h>
#include <ivf3dui/UISwitch.h>
#include <ivf3dui/UIButtonGroup.h>
#include <ivf3dui/UIComposite.h>

using namespace std;
using namespace ivf;

#define VIEW_MODE			100
#define INTERACTION_MODE	101

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase,
	InitEvent,
	ShapeOverEvent,
	ShapeLeaveEvent,
	ShapeEnterEvent,
	ShapeDownEvent,
	ShapeUpEvent,
	ShapeClickEvent,
	ControlOverEvent,
	ControlLeaveEvent,
	ControlEnterEvent,
	ControlDownEvent,
	ControlUpEvent,
	ControlClickEvent,
	TimeoutEvent
{
private:

	// Camera movement state variables

	CameraPtr		m_camera;
	ScenePtr		m_scene;
	LightPtr		m_light;

	CubePtr			m_cube;
	SpherePtr		m_sphere;
	CylinderPtr		m_cylinder;
	ConePtr			m_cone;

	TransformPtr	m_xfm;

	ShapePtr		m_rotateShape;

	MouseViewHandlerPtr		m_mouseHandler;
	SceneHandlerPtr			m_sceneHandler;
	InteractionHandlerPtr	m_interactionHandler;
public:
	ExampleWindow(int X, int Y, int W, int H);

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual bool onTimeout();

	virtual void onShapeDown(Shape* shape);
	virtual void onShapeClick(Shape* shape);
	virtual void onShapeUp(Shape* shape);
	virtual void onShapeEnter(Shape* shape);
	virtual void onShapeOver(Shape* shape);
	virtual void onShapeLeave(Shape* shape);
	virtual void onShapeDrag(Shape* shape);

	virtual void onControlClick(UIInteractiveBase* uiControl);
	virtual void onControlDrag(UIInteractiveBase* uiControl);
	virtual void onControlUp(UIInteractiveBase* uiControl);
	virtual void onControlDown(UIInteractiveBase* uiControl);
	virtual void onControlEnter(UIInteractiveBase* uiControl);
	virtual void onControlOver(UIInteractiveBase* uiControl);
	virtual void onControlLeave(UIInteractiveBase* uiControl);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
	return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

ExampleWindow::ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H)
{
	addInitEvent(this);
	assignTimeoutEvent(0, this);
}

void ExampleWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = Camera::create();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 6.0, 10.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = Scene::create();
	m_scene->setView(m_camera);

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

	// Create objects

	m_xfm = Transform::create();

	m_scene->addChild(m_xfm);
	
	m_cube = Cube::create();
	m_cube->setMaterial(redMaterial);
	m_cube->setPosition(2.0, 0.0, 2.0);
	m_xfm->addChild(m_cube);

	m_sphere = Sphere::create();
	m_sphere->setMaterial(greenMaterial);
	m_sphere->setPosition(-2.0, 0.0, 2.0);
	m_xfm->addChild(m_sphere);

	m_cylinder = Cylinder::create();
	m_cylinder->setMaterial(blueMaterial);
	m_cylinder->setPosition(-2.0, 0.0, -2.0);
	m_xfm->addChild(m_cylinder);

	m_cone = Cone::create();
	m_cone->setMaterial(yellowMaterial);
	m_cone->setPosition(2.0, 0.0, -2.0);
	m_cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_xfm->addChild(m_cone);

	auto buttonShape = Brick::create();
	buttonShape->setSize(0.3, 0.1, 0.3);
	buttonShape->setMaterial(yellowMaterial);

	// Create a light

	auto lighting = Lighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	auto buttonGroup = UIButtonGroup::create();
	buttonGroup->setPosition(0.0, 0.0, 0.0);
	//buttonGroup->setRotationQuat(1.0, 0.0, 0.0, 0.0);

	int i, j;

	UIButtonPtr button;

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			button = UIButton::create();
			button->setVisualAction(UIButton::VA_MOVING);
			button->setId(1001+j+i*2);
			button->setMovementDirection(0.0, -1.0, 0.0);
			button->setMovementDistance(0.2);
			button->setPosition(i*0.4 - 0.2, 0.0, j*0.4 - 0.2);
			button->setShape(buttonShape);
			buttonGroup->addChild(button);
		}
	}

	m_scene->addChild(buttonGroup);

	button = UIButton::create();
	button->setVisualAction(UIButton::VA_MOVING);
	button->setId(1005);
	button->setMovementDirection(0.0, -1.0, 0.0);
	button->setMovementDistance(0.2);
	button->setPosition(0.0, 0.0, 1.0);
	button->setShape(buttonShape);

	m_scene->addChild(button);

	// Create event handlers

	m_mouseHandler = MouseViewHandler::create(this, m_camera);
	m_mouseHandler->activate();

	m_interactionHandler = InteractionHandler::create(this, m_scene);
	m_interactionHandler->activate();
	m_interactionHandler->setShapeEnterEvent(this);
	m_interactionHandler->setShapeOverEvent(this);
	m_interactionHandler->setShapeLeaveEvent(this);
	m_interactionHandler->setShapeDownEvent(this);
	m_interactionHandler->setShapeUpEvent(this);
	m_interactionHandler->setShapeClickEvent(this);
	m_interactionHandler->setControlEnterEvent(this);
	m_interactionHandler->setControlOverEvent(this);
	m_interactionHandler->setControlLeaveEvent(this);
	m_interactionHandler->setControlDownEvent(this);
	m_interactionHandler->setControlUpEvent(this);
	m_interactionHandler->setControlClickEvent(this);
	
	m_sceneHandler = SceneHandler::create(this, m_scene);
	m_sceneHandler->activate();

	enableTimeout(0.01, 0);
}

bool ExampleWindow::onTimeout()
{
	if (m_rotateShape!=nullptr)
	{
		double v1, v2, v3, angle;
		m_rotateShape->getRotationQuat(v1, v2, v3, angle);
		angle+=0.1;
		m_rotateShape->setRotationQuat(0.0, 0.0, 1.0, angle);

		redraw();
	}
	return true;
}

void ExampleWindow::onShapeDown(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeDown: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeClick(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeClick: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeUp(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeUp: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeEnter(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeEnter: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeOver(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeOver: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeLeave(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeLeave: " << shape->getClassName() << endl;
}

void ExampleWindow::onShapeDrag(Shape* shape)
{
	if (shape!=nullptr)
		cout << "onShapeDrag: " << shape->getClassName() << endl;
}

void ExampleWindow::onControlClick(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
	{
		switch (uiControl->getId()) {
		case 1001:
			m_rotateShape = m_cylinder;
			break;
		case 1002: 
			m_rotateShape = m_sphere;
			break;
		case 1003:
			m_rotateShape = m_cone;
			break;
		case 1004:
			m_rotateShape = m_cube;
			break;
		default:
			break;
		}
		cout << "onControlClick: " << uiControl->getClassName()
		<< ", " << uiControl->getId() << endl;
	}
}

void ExampleWindow::onControlOver(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
		cout << "onControlOver: " << uiControl->getClassName() 
		<< ", " << uiControl->getId() << endl;
}

void ExampleWindow::onControlDown(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
	{
		if (uiControl->getId()==1005)
			m_rotateShape = m_xfm;

		cout << "onControlDown: " << uiControl->getClassName()
		<< ", " << uiControl->getId() << endl;
	}
}

void ExampleWindow::onControlUp(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
	{
		if (uiControl->getId()==1005)
			m_rotateShape = nullptr;

		cout << "onControlUp: " << uiControl->getClassName()
		<< ", " << uiControl->getId() << endl;
	}
}

void ExampleWindow::onControlDrag(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
		cout << "onControlDrag: " << uiControl->getClassName() << endl;
}

void ExampleWindow::onControlLeave(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
		cout << "onControlLeave: " << uiControl->getClassName() << endl;
}

void ExampleWindow::onControlEnter(UIInteractiveBase *uiControl)
{
	if (uiControl!=nullptr)
		cout << "onControlEnter: " << uiControl->getClassName() << endl;
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

	window->setWindowTitle("Ivf++ 3D user interface interaction");
	window->show();

	// Enter main application loop

    app->run();

	cout << "window refcount = " << window->getRefCount() << endl;

	return 0;
}


