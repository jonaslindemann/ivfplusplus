// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/SceneHandler.h>

#include <ivf/Scene.h>
#include <ivf/Cube.h>
#include <ivf/Material.h>
#include <ivf/Lighting.h>
#include <ivf/Switch.h>
#include <ivf/Axis.h>

#include <ivfgle/GleCoordArray.h>
#include <ivfgle/GleColorArray.h>
#include <ivfgle/GleScalarArray.h>
#include <ivfgle/GlePolyCylinder.h>
#include <ivfgle/GlePolyCone.h>
#include <ivfgle/Gle.h>
#include <ivfgle/GleExtrusion.h>
#include <ivfgle/GleContour.h>
#include <ivfgle/GleTwistExtrusion.h>
#include <ivfgle/GleSpiral.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase,
	InitEvent,
	InitContextEvent,
	ResizeEvent,
	RenderEvent,
	ClearEvent,
	KeyboardEvent
{
private:

	ScenePtr m_scene;

	SwitchPtr m_gleShapes;

	LightingPtr m_lighting;

	MouseViewHandlerPtr m_mouseViewHandler;
	SceneHandlerPtr m_sceneHandler;

public:
	ExampleWindow(int X, int Y, int W, int H);

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onInitContext(int width, int height);
	virtual void onClear();
	virtual void onKeyboard(int key, int x, int y);
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
	addInitContextEvent(this);
	addResizeEvent(this);
	addRenderEvent(this);
	addClearEvent(this);
}

double rnd()
{
	return (double)rand()/(double)RAND_MAX;
}

void ExampleWindow::onInit(int width, int height)
{
	// Setup a simple scene

	m_scene = Scene::create();
	m_scene->getCamera()->setPosition(3.0, 3.0, 3.0);

	m_gleShapes = Switch::create();

	auto material = Material::create();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setColorMaterial(false);

	////////////////////////////////////////////////////////////////////
	// Testing GLE classes

	auto gle = Gle::getInstance();
	gle->setNumSides(20);
	gle->setJoinStyle(TUBE_JN_ANGLE|TUBE_NORM_EDGE);

	////////////////////////////////////////////////////////////////////
	// Testing poly cylinder

	int nPoints = 100;
	double x = -2*M_PI;
	double y;
	double deltaX = 4*M_PI/(nPoints-1);
	int i;

	GleColorArrayPtr colorArray = GleColorArray::create(nPoints+2);
	GleCoordArrayPtr coordArray = GleCoordArray::create(nPoints+2);
	GleScalarArrayPtr radiusArray = GleScalarArray::create(nPoints+2);
	GleScalarArrayPtr twistArray = GleScalarArray::create(nPoints+2);

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

	auto polyCylinder = GlePolyCylinder::create();
	polyCylinder->setPoints(coordArray);
	polyCylinder->setColors(colorArray);
	polyCylinder->setMaterial(material);

	m_gleShapes->addChild(polyCylinder);

	////////////////////////////////////////////////////////////////////
	// Testing poly cone

	auto polyCone = GlePolyCone::create();
	polyCone->setPoints(coordArray);
	polyCone->setColors(colorArray);
	polyCone->setRadius(radiusArray);
	polyCone->setMaterial(material);

	m_gleShapes->addChild(polyCone);

	////////////////////////////////////////////////////////////////////
	// Test extrusion

	auto contourArray = GleContour::create(5);
	contourArray->setCoord(0, -0.2, -0.2);
	contourArray->setCoord(1,  0.2, -0.2);
	contourArray->setCoord(2,  0.2,  0.2);
	contourArray->setCoord(3, -0.2,  0.2);
	contourArray->setCoord(4, -0.2, -0.2);
	contourArray->calcNormals();

	auto extrusion = GleExtrusion::create();
	extrusion->setPoints(coordArray);
	extrusion->setColors(colorArray);
	extrusion->setContour(contourArray);
	extrusion->setContourUp(0.0, 1.0, 0.0);
	extrusion->setMaterial(material);

	m_gleShapes->addChild(extrusion);
	
	////////////////////////////////////////////////////////////////////
	// Test twist extrusion

	auto twistExtrusion = GleTwistExtrusion::create();
	twistExtrusion->setPoints(coordArray);
	twistExtrusion->setColors(colorArray);
	twistExtrusion->setContour(contourArray);
	twistExtrusion->setContourUp(0.0, 1.0, 0.0);
	twistExtrusion->setTwist(twistArray);
	twistExtrusion->setMaterial(material);

	m_gleShapes->addChild(twistExtrusion);

	////////////////////////////////////////////////////////////////////
	// Test spiral

	auto spiral = GleSpiral::create();
	spiral->setContour(contourArray);
	spiral->setContourUp(0.0, 1.0, 0.0);
	spiral->setMaterial(material);

	m_gleShapes->addChild(spiral);

	m_scene->addChild(m_gleShapes);

	auto axis = Axis::create();
	m_scene->addChild(axis);

	// Setup OpenGL

	m_lighting = Lighting::getInstance();
	m_lighting->enable();
	m_lighting->getLight(0)->enable();

	// Create handlers

	m_mouseViewHandler = MouseViewHandler::create(this, m_scene->getCamera());
	m_sceneHandler = SceneHandler::create(this, m_scene);
}

void ExampleWindow::onKeyboard(int key, int x, int y)
{
	m_gleShapes->cycleForward();
	redraw();
}

void ExampleWindow::onInitContext(int width, int height)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void ExampleWindow::onClear()
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
    
	auto app = GlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);
    
	// Create a window
    
	auto window = ExampleWindow::create(0, 0, 512, 512);
    
	// Set window title and show window
    
	window->setWindowTitle("Ivf++ event handler examples");
	window->show();
    
	// Enter main application loop
    
	app->run();
    
	return 0;
}