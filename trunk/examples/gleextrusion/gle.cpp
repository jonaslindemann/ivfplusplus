// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

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

IvfSmartPointer(CDemosysWindow);

class CDemosysWindow: public CIvfWindow,
	CIvfInitEvent,
	CIvfInitContextEvent,
	CIvfResizeEvent,
	CIvfRenderEvent,
	CIvfClearEvent,
	CIvfKeyboardEvent
{
private:

	CIvfScenePtr m_scene;

	CIvfSwitchPtr m_gleShapes;

	CIvfLightingPtr m_lighting;

	CIvfMouseViewHandlerPtr m_mouseViewHandler;
	CIvfSceneHandlerPtr m_sceneHandler;

public:
	CDemosysWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onInitContext(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onClear();
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CDemosysWindow::CDemosysWindow(int X, int Y, int W, int H)
	:CIvfWindow(X, Y, W, H)
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

void CDemosysWindow::onInit(int width, int height)
{
	// Setup a simple scene

	m_scene = new CIvfScene();
	m_scene->getCamera()->setPosition(3.0, 3.0, 3.0);

	m_gleShapes = new CIvfSwitch();

	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setColorMaterial(false);

	////////////////////////////////////////////////////////////////////
	// Testing GLE classes

	CIvfGlePtr gle = CIvfGle::getInstance();
	gle->setNumSides(20);
	gle->setJoinStyle(TUBE_JN_ANGLE|TUBE_NORM_EDGE);

	////////////////////////////////////////////////////////////////////
	// Testing poly cylinder

	int nPoints = 100;
	double x = -2*M_PI;
	double y;
	double deltaX = 4*M_PI/(nPoints-1);
	int i;

	CIvfGleColorArrayPtr colorArray = new CIvfGleColorArray(nPoints+2);
	CIvfGleCoordArrayPtr coordArray = new CIvfGleCoordArray(nPoints+2);
	CIvfGleScalarArrayPtr radiusArray = new CIvfGleScalarArray(nPoints+2);
	CIvfGleScalarArrayPtr twistArray = new CIvfGleScalarArray(nPoints+2);

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

	CIvfGlePolyCylinderPtr polyCylinder = new CIvfGlePolyCylinder();
	polyCylinder->setPoints(coordArray);
	polyCylinder->setColors(colorArray);
	polyCylinder->setMaterial(material);

	m_gleShapes->addChild(polyCylinder);

	////////////////////////////////////////////////////////////////////
	// Testing poly cone

	CIvfGlePolyConePtr polyCone = new CIvfGlePolyCone();
	polyCone->setPoints(coordArray);
	polyCone->setColors(colorArray);
	polyCone->setRadius(radiusArray);
	polyCone->setMaterial(material);

	m_gleShapes->addChild(polyCone);

	////////////////////////////////////////////////////////////////////
	// Test extrusion

	CIvfGleContourPtr contourArray = new CIvfGleContour(5);
	contourArray->setCoord(0, -0.2, -0.2);
	contourArray->setCoord(1,  0.2, -0.2);
	contourArray->setCoord(2,  0.2,  0.2);
	contourArray->setCoord(3, -0.2,  0.2);
	contourArray->setCoord(4, -0.2, -0.2);
	contourArray->calcNormals();

	CIvfGleExtrusionPtr extrusion = new CIvfGleExtrusion();
	extrusion->setPoints(coordArray);
	extrusion->setColors(colorArray);
	extrusion->setContour(contourArray);
	extrusion->setContourUp(0.0, 1.0, 0.0);
	extrusion->setMaterial(material);

	m_gleShapes->addChild(extrusion);
	
	////////////////////////////////////////////////////////////////////
	// Test twist extrusion

	CIvfGleTwistExtrusionPtr twistExtrusion = new CIvfGleTwistExtrusion();
	twistExtrusion->setPoints(coordArray);
	twistExtrusion->setColors(colorArray);
	twistExtrusion->setContour(contourArray);
	twistExtrusion->setContourUp(0.0, 1.0, 0.0);
	twistExtrusion->setTwist(twistArray);
	twistExtrusion->setMaterial(material);

	m_gleShapes->addChild(twistExtrusion);

	////////////////////////////////////////////////////////////////////
	// Test spiral

	CIvfGleSpiralPtr spiral = new CIvfGleSpiral();
	spiral->setContour(contourArray);
	spiral->setContourUp(0.0, 1.0, 0.0);
	spiral->setMaterial(material);

	m_gleShapes->addChild(spiral);

	m_scene->addChild(m_gleShapes);

	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);

	// Setup OpenGL

	m_lighting = CIvfLighting::getInstance();
	m_lighting->enable();
	m_lighting->getLight(0)->enable();

	// Create handlers

	m_mouseViewHandler = new CIvfMouseViewHandler(this, m_scene->getCamera());
	m_sceneHandler = new CIvfSceneHandler(this, m_scene);
}

void CDemosysWindow::onKeyboard(int key, int x, int y)
{
	m_gleShapes->cycleForward();
	redraw();
}

void CDemosysWindow::onInitContext(int width, int height)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CDemosysWindow::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

void CDemosysWindow::onResize(int width, int height)
{

}

void CDemosysWindow::onRender()
{

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

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CDemosysWindowPtr window = new CDemosysWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Cube example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
