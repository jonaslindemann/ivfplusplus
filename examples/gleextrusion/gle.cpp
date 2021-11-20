// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

#include <ivf/IvfScene.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfSwitch.h>
#include <ivf/IvfAxis.h>

#include <ivfgle/IvfGleCoordArray.h>
#include <ivfgle/IvfGleColorArray.h>
#include <ivfgle/IvfGleScalarArray.h>
#include <ivfgle/IvfGlePolyCylinder.h>
#include <ivfgle/IvfGlePolyCone.h>
#include <ivfgle/IvfGle.h>
#include <ivfgle/IvfGleExtrusion.h>
#include <ivfgle/IvfGleContour.h>
#include <ivfgle/IvfGleTwistExtrusion.h>
#include <ivfgle/IvfGleSpiral.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CGlutBase,
	CInitEvent,
	CInitContextEvent,
	CResizeEvent,
	CRenderEvent,
	CClearEvent,
	CKeyboardEvent
{
private:

	CScenePtr m_scene;

	CSwitchPtr m_gleShapes;

	CLightingPtr m_lighting;

	CMouseViewHandlerPtr m_mouseViewHandler;
	CSceneHandlerPtr m_sceneHandler;

public:
	CExampleWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onInitContext(int width, int height);
	virtual void onClear();
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CExampleWindow::CExampleWindow(int X, int Y, int W, int H)
	:CGlutBase(X, Y, W, H)
{
	addInitEvent(this);
	addInitContextEvent(this);
	addResizeEvent(this);
	addRenderEvent(this);
	addClearEvent(this);
}

double rnd()
{
	return (double)rand()/(double)RAND_MAX;
}

void CExampleWindow::onInit(int width, int height)
{
	// Setup a simple scene

	m_scene = new CScene();
	m_scene->getCamera()->setPosition(3.0, 3.0, 3.0);

	m_gleShapes = new CSwitch();

	CMaterialPtr material = new CMaterial();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setColorMaterial(false);

	////////////////////////////////////////////////////////////////////
	// Testing GLE classes

	CGlePtr gle = CGle::getInstance();
	gle->setNumSides(20);
	gle->setJoinStyle(TUBE_JN_ANGLE|TUBE_NORM_EDGE);

	////////////////////////////////////////////////////////////////////
	// Testing poly cylinder

	int nPoints = 100;
	double x = -2*M_PI;
	double y;
	double deltaX = 4*M_PI/(nPoints-1);
	int i;

	CGleColorArrayPtr colorArray = new CGleColorArray(nPoints+2);
	CGleCoordArrayPtr coordArray = new CGleCoordArray(nPoints+2);
	CGleScalarArrayPtr radiusArray = new CGleScalarArray(nPoints+2);
	CGleScalarArrayPtr twistArray = new CGleScalarArray(nPoints+2);

	for (i=0; i<nPoints; i++)
	{
		y = sin(x);
		coordArray->setCoord(i+1, x, y, 0.0);
		colorArray->setColor(i+1, rnd(), rnd(), rnd());
		radiusArray->setValue(i+1, rnd()*0.2);
		twistArray->setValue(i+1, 10.0 - 20.0*rnd());
		x += deltaX;
	}
	
	coordArray->calcFirstAndLast();

	CGlePolyCylinderPtr polyCylinder = new CGlePolyCylinder();
	polyCylinder->setPoints(coordArray);
	polyCylinder->setColors(colorArray);
	polyCylinder->setMaterial(material);

	m_gleShapes->addChild(polyCylinder);

	////////////////////////////////////////////////////////////////////
	// Testing poly cone

	CGlePolyConePtr polyCone = new CGlePolyCone();
	polyCone->setPoints(coordArray);
	polyCone->setColors(colorArray);
	polyCone->setRadius(radiusArray);
	polyCone->setMaterial(material);

	m_gleShapes->addChild(polyCone);

	////////////////////////////////////////////////////////////////////
	// Test extrusion

	CGleContourPtr contourArray = new CGleContour(5);
	contourArray->setCoord(0, -0.2, -0.2);
	contourArray->setCoord(1,  0.2, -0.2);
	contourArray->setCoord(2,  0.2,  0.2);
	contourArray->setCoord(3, -0.2,  0.2);
	contourArray->setCoord(4, -0.2, -0.2);
	contourArray->calcNormals();

	CGleExtrusionPtr extrusion = new CGleExtrusion();
	extrusion->setPoints(coordArray);
	extrusion->setColors(colorArray);
	extrusion->setContour(contourArray);
	extrusion->setContourUp(0.0, 1.0, 0.0);
	extrusion->setMaterial(material);

	m_gleShapes->addChild(extrusion);
	
	////////////////////////////////////////////////////////////////////
	// Test twist extrusion

	CGleTwistExtrusionPtr twistExtrusion = new CGleTwistExtrusion();
	twistExtrusion->setPoints(coordArray);
	twistExtrusion->setColors(colorArray);
	twistExtrusion->setContour(contourArray);
	twistExtrusion->setContourUp(0.0, 1.0, 0.0);
	twistExtrusion->setTwist(twistArray);
	twistExtrusion->setMaterial(material);

	m_gleShapes->addChild(twistExtrusion);

	////////////////////////////////////////////////////////////////////
	// Test spiral

	CGleSpiralPtr spiral = new CGleSpiral();
	spiral->setContour(contourArray);
	spiral->setContourUp(0.0, 1.0, 0.0);
	spiral->setMaterial(material);

	m_gleShapes->addChild(spiral);

	m_scene->addChild(m_gleShapes);

	CAxisPtr axis = new CAxis();
	m_scene->addChild(axis);

	// Setup OpenGL

	m_lighting = CLighting::getInstance();
	m_lighting->enable();
	m_lighting->getLight(0)->enable();

	// Create handlers

	m_mouseViewHandler = new CMouseViewHandler(this, m_scene->getCamera());
	m_sceneHandler = new CSceneHandler(this, m_scene);
}

void CExampleWindow::onKeyboard(int key, int x, int y)
{
	m_gleShapes->cycleForward();
	redraw();
}

void CExampleWindow::onInitContext(int width, int height)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CExampleWindow::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------


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
    
	window->setWindowTitle("Ivf++ event handler examples");
	window->show();
    
	// Enter main application loop
    
	app->run();
    
	return 0;
}