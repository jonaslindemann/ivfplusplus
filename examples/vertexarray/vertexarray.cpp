// ------------------------------------------------------------
//
// Ivf++ Extrusion example
//
// ------------------------------------------------------------
//
// Author: Jonas Lindemann
//

// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#ifdef WIN32
#include <ivf/ivfwin32libs.h>
#endif

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfCube.h>

#include <ivf/IvfVertexElements.h>
#include <ivf/IvfPolyState.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfWindow {
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

public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfWindow(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
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

	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(0.0, 5.0, 5.0);

	// Create a materials

	CIvfMaterialPtr yellowMaterial = new CIvfMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create scene composite

	m_scene = new CIvfComposite();

	// First point
	
	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);
	
	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	CIvfVertexListPtr vertices = new CIvfVertexList();
	CIvfColorListPtr colors = new CIvfColorList();
	CIvfNormalListPtr normals = new CIvfNormalList();
	CIvfVertexIndexPtr indices = new CIvfVertexIndex();

	int nPoints = 500;
	int i, j;

	double x, y, w;

	w = 2.0/((double)nPoints-1.0);

	for (j=0; j<nPoints-1; j++)
		for (i=0; i<nPoints-1; i++)
		{
			x = -1.0 + 2.0*(double)i/(double)(nPoints-1);
			y = -1.0 + 2.0*(double)j/(double)(nPoints-1);

			vertices->add(x, y, 0.0);
			vertices->add(x+w, y, 0.0);
			vertices->add(x+w, y+w, 0.0);
			vertices->add(x, y+w, 0.0);

			colors->add(1.0*(double)i/(double)(nPoints-1), 1.0*(double)j/(double)(nPoints-1), 1.0f);
			colors->add(1.0*(double)i/(double)(nPoints-1), 1.0*(double)j/(double)(nPoints-1), 1.0f);
			colors->add(1.0*(double)i/(double)(nPoints-1), 1.0*(double)j/(double)(nPoints-1), 1.0f);
			colors->add(1.0*(double)i/(double)(nPoints-1), 1.0*(double)j/(double)(nPoints-1), 1.0f);

			normals->add(0.0, 0.0, 1.0);
			normals->add(0.0, 0.0, 1.0);
			normals->add(0.0, 0.0, 1.0);
			normals->add(0.0, 0.0, 1.0);
		}

	indices->createLinear(vertices->getSize());

	CIvfVertexElementsPtr vertexElements = new CIvfVertexElements();

	glEnable(GL_COLOR_MATERIAL);

	vertexElements->setPrimitive(CIvfVertexElements::PT_QUADS);
	vertexElements->setVertices( vertices );
	vertexElements->setIndices( indices );
	vertexElements->setColors( colors );
	vertexElements->setNormals( normals );

	CIvfPolyStatePtr polyState = new CIvfPolyState();
	polyState->setMode(CIvfPolyState::PM_FILL);
	vertexElements->setRenderState(polyState);

	m_scene->addChild(vertexElements);

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
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Extrusion example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
