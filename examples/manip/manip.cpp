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

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfBrick.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>
#include <ivf/IvfCone.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfLighting.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>
#include <ivfwidget/IvfInteractionHandler.h>

#include <ivf3dui/IvfUIButton.h>
#include <ivf3dui/IvfUISwitch.h>
#include <ivf3dui/IvfUIButtonGroup.h>
#include <ivf3dui/IvfUIComposite.h>

using namespace std;

#define VIEW_MODE			100
#define INTERACTION_MODE	101

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase,
	CInitEvent,
	CShapeOverEvent,
	CShapeLeaveEvent,
	CShapeEnterEvent,
	CShapeDownEvent,
	CShapeUpEvent,
	CShapeClickEvent,
	CControlOverEvent,
	CControlLeaveEvent,
	CControlEnterEvent,
	CControlDownEvent,
	CControlUpEvent,
	CControlClickEvent,
	CTimeoutEvent
{
private:

	// Camera movement state variables

	CCameraPtr		m_camera;
	CScenePtr		m_scene;
	CLightPtr		m_light;

	CCubePtr			m_cube;
	CSpherePtr		m_sphere;
	CCylinderPtr		m_cylinder;
	CConePtr			m_cone;

	CTransformPtr	m_xfm;

	CShapePtr		m_rotateShape;

	CMouseViewHandlerPtr		m_mouseHandler;
	CSceneHandlerPtr			m_sceneHandler;
	CInteractionHandlerPtr	m_interactionHandler;
public:
	CExampleWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual bool onTimeout();

	virtual void onShapeDown(CShape* shape);
	virtual void onShapeClick(CShape* shape);
	virtual void onShapeUp(CShape* shape);
	virtual void onShapeEnter(CShape* shape);
	virtual void onShapeOver(CShape* shape);
	virtual void onShapeLeave(CShape* shape);
	virtual void onShapeDrag(CShape* shape);

	virtual void onControlClick(CUIInteractiveBase* uiControl);
	virtual void onControlDrag(CUIInteractiveBase* uiControl);
	virtual void onControlUp(CUIInteractiveBase* uiControl);
	virtual void onControlDown(CUIInteractiveBase* uiControl);
	virtual void onControlEnter(CUIInteractiveBase* uiControl);
	virtual void onControlOver(CUIInteractiveBase* uiControl);
	virtual void onControlLeave(CUIInteractiveBase* uiControl);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
		:CGlutBase(X, Y, W, H)
{
	addInitEvent(this);
	assignTimeoutEvent(0, this);
}

void CExampleWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = new CCamera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 6.0, 10.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new CScene();
	m_scene->setView(m_camera);

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

	// Create objects

	m_xfm = new CTransform();

	m_scene->addChild(m_xfm);
	
	m_cube = new CCube();
	m_cube->setMaterial(redMaterial);
	m_cube->setPosition(2.0, 0.0, 2.0);
	m_xfm->addChild(m_cube);

	m_sphere = new CSphere();
	m_sphere->setMaterial(greenMaterial);
	m_sphere->setPosition(-2.0, 0.0, 2.0);
	m_xfm->addChild(m_sphere);

	m_cylinder = new CCylinder();
	m_cylinder->setMaterial(blueMaterial);
	m_cylinder->setPosition(-2.0, 0.0, -2.0);
	m_xfm->addChild(m_cylinder);

	m_cone = new CCone();
	m_cone->setMaterial(yellowMaterial);
	m_cone->setPosition(2.0, 0.0, -2.0);
	m_cone->setRotationQuat(0.0, 0.0, 1.0, 45.0);
	m_xfm->addChild(m_cone);

	CBrickPtr buttonShape = new CBrick();
	buttonShape->setSize(0.3, 0.1, 0.3);
	buttonShape->setMaterial(yellowMaterial);

	// Create a light

	CLightingPtr lighting = CLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	CUIButtonGroupPtr buttonGroup = new CUIButtonGroup();
	buttonGroup->setPosition(0.0, 0.0, 0.0);
	//buttonGroup->setRotationQuat(1.0, 0.0, 0.0, 0.0);

	int i, j;

	CUIButtonPtr button;

	for (i=0; i<2; i++)
	{
		for (j=0; j<2; j++)
		{
			button = new CUIButton();
			button->setVisualAction(CUIButton::VA_MOVING);
			button->setId(1001+j+i*2);
			button->setMovementDirection(0.0, -1.0, 0.0);
			button->setMovementDistance(0.2);
			button->setPosition(i*0.4 - 0.2, 0.0, j*0.4 - 0.2);
			button->setShape(buttonShape);
			buttonGroup->addChild(button);
		}
	}

	m_scene->addChild(buttonGroup);

	button = new CUIButton();
	button->setVisualAction(CUIButton::VA_MOVING);
	button->setId(1005);
	button->setMovementDirection(0.0, -1.0, 0.0);
	button->setMovementDistance(0.2);
	button->setPosition(0.0, 0.0, 1.0);
	button->setShape(buttonShape);

	m_scene->addChild(button);

	// Create event handlers

	m_mouseHandler = new CMouseViewHandler(this, m_camera);
	m_mouseHandler->activate();

	m_interactionHandler = new CInteractionHandler(this, m_scene);
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
	
	m_sceneHandler = new CSceneHandler(this, m_scene);
	m_sceneHandler->activate();

	enableTimeout(0.01, 0);
}

bool CExampleWindow::onTimeout()
{
	if (m_rotateShape!=NULL)
	{
		double v1, v2, v3, angle;
		m_rotateShape->getRotationQuat(v1, v2, v3, angle);
		angle+=0.1;
		m_rotateShape->setRotationQuat(0.0, 0.0, 1.0, angle);

		redraw();
	}
	return true;
}

void CExampleWindow::onShapeDown(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeDown: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeClick(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeClick: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeUp(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeUp: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeEnter(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeEnter: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeOver(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeOver: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeLeave(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeLeave: " << shape->getClassName() << endl;
}

void CExampleWindow::onShapeDrag(CShape* shape)
{
	if (shape!=NULL)
		cout << "onShapeDrag: " << shape->getClassName() << endl;
}

void CExampleWindow::onControlClick(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
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

void CExampleWindow::onControlOver(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
		cout << "onControlOver: " << uiControl->getClassName() 
		<< ", " << uiControl->getId() << endl;
}

void CExampleWindow::onControlDown(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
	{
		if (uiControl->getId()==1005)
			m_rotateShape = m_xfm;

		cout << "onControlDown: " << uiControl->getClassName()
		<< ", " << uiControl->getId() << endl;
	}
}

void CExampleWindow::onControlUp(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
	{
		if (uiControl->getId()==1005)
			m_rotateShape = NULL;

		cout << "onControlUp: " << uiControl->getClassName()
		<< ", " << uiControl->getId() << endl;
	}
}

void CExampleWindow::onControlDrag(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
		cout << "onControlDrag: " << uiControl->getClassName() << endl;
}

void CExampleWindow::onControlLeave(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
		cout << "onControlLeave: " << uiControl->getClassName() << endl;
}

void CExampleWindow::onControlEnter(CUIInteractiveBase *uiControl)
{
	if (uiControl!=NULL)
		cout << "onControlEnter: " << uiControl->getClassName() << endl;
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

	window->setWindowTitle("Ivf++ 3D user interface interaction");
	window->show();

	// Enter main application loop

    app->run();

	cout << "window refcount = " << window->getRefCount() << endl;

	return 0;
}


