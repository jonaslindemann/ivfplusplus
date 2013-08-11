// ------------------------------------------------------------
//
// Ivf++ Advanced geometry example
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
#include <ivf/IvfCylinder.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h> 
#include <ivf/IvfMaterial.h>
#include <ivf/IvfTexture.h>

// CIvfGLPrimitive derived classes

#include <ivf/IvfPointSet.h>
#include <ivf/IvfLineSet.h>
#include <ivf/IvfLineStripSet.h>
#include <ivf/IvfTriSet.h>
#include <ivf/IvfTriStripSet.h>
#include <ivf/IvfQuadSet.h>

// From the image library

#include <ivfimage/IvfPngImage.h>

// From the file library

#include <ivffile/IvfDxfWriter.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutBase {
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
		:CIvfGlutBase(X, Y, W, H) {};

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
	m_camera->setPosition(-6.6, 9.2, 10.5);
	m_camera->setTarget(1.1, 0.1, 0.6);

	// Create a materials

	CIvfMaterialPtr greenMaterial = new CIvfMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.2f, 0.0f, 1.0f);
	
	CIvfMaterialPtr redMaterial = new CIvfMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.2f, 0.0f, 0.0f, 1.0f);

	// Create textures

	CIvfPngImagePtr logoImage = new CIvfPngImage();
	logoImage->setFileName("images/ivf.png");
	logoImage->read();

	CIvfTexturePtr logoTexture = new CIvfTexture();
	logoTexture->setImage(logoImage);

	// Create scene composite

	m_scene = new CIvfComposite();

	// Create a PointSet

	CIvfPointSetPtr pointSet = new CIvfPointSet();

	pointSet->addCoord(0.0, 0.0, 0.0);
	pointSet->addCoord(0.0, 0.0, 1.0);
	pointSet->addCoord(1.0, 0.5, 0.0);
	pointSet->addCoord(1.0, 0.5, 1.0);
	pointSet->addCoord(2.0, 0.5, 0.0);
	pointSet->addCoord(2.0, 0.5, 1.0);
	pointSet->addCoord(3.0, 0.0, 0.0);
	pointSet->addCoord(3.0, 0.0, 1.0);
	
	CIvfIndexPtr coordIdx = new CIvfIndex();
	coordIdx->createLinear(8);

	pointSet->addCoordIndex(coordIdx);

	pointSet->addColor(0.0, 0.0, 1.0);
	pointSet->addColor(0.0, 1.0, 0.0);
	pointSet->addColor(0.0, 1.0, 1.0);
	pointSet->addColor(1.0, 0.0, 0.0);
	pointSet->addColor(1.0, 0.0, 1.0);
	pointSet->addColor(1.0, 1.0, 0.0);
	pointSet->addColor(1.0, 1.0, 1.0);
	pointSet->addColor(0.0, 0.0, 1.0);

	CIvfIndexPtr colorIdx = new CIvfIndex();
	colorIdx->createLinear(8);
	pointSet->addColorIndex(colorIdx);
	pointSet->setUseColor(true);

	pointSet->setPosition(-3.0, 0.0, 3.0);
	pointSet->setPointSize(3);
	
	m_scene->addChild(pointSet);

	// Create a LineSet

	CIvfLineSetPtr lineSet = new CIvfLineSet();	
	
	lineSet->addCoord(0.0, 0.0, 0.0);
	lineSet->addCoord(0.0, 0.0, 1.0);
	lineSet->addCoord(1.0, 0.5, 0.0);
	lineSet->addCoord(1.0, 0.5, 1.0);
	lineSet->addCoord(2.0, 0.5, 0.0);
	lineSet->addCoord(2.0, 0.5, 1.0);
	lineSet->addCoord(3.0, 0.0, 0.0);
	lineSet->addCoord(3.0, 0.0, 1.0);
	
	coordIdx = new CIvfIndex();
	coordIdx->createLinear(8);

	lineSet->addCoordIndex(coordIdx);

	lineSet->addColor(0.0, 0.0, 1.0);
	lineSet->addColor(0.0, 1.0, 0.0);
	lineSet->addColor(0.0, 1.0, 1.0);
	lineSet->addColor(1.0, 0.0, 0.0);
	lineSet->addColor(1.0, 0.0, 1.0);
	lineSet->addColor(1.0, 1.0, 0.0);
	lineSet->addColor(1.0, 1.0, 1.0);
	lineSet->addColor(0.0, 0.0, 1.0);

	colorIdx = new CIvfIndex();
	colorIdx->createLinear(8);

	lineSet->addColorIndex(colorIdx);

	lineSet->setPosition(1.5, 0.0, 3.0);
	lineSet->setMaterial(redMaterial);
	lineSet->setUseColor(true);
	lineSet->setLineWidth(2);

	m_scene->addChild(lineSet);

	// Create a LineStripSet
	
	CIvfLineStripSetPtr lineStripSet = new CIvfLineStripSet();

	lineStripSet->addCoord(0.0, 0.0, 0.0);
	lineStripSet->addCoord(0.0, 0.0, 1.0);
	lineStripSet->addCoord(1.0, 0.5, 0.0);
	lineStripSet->addCoord(1.0, 0.5, 1.0);
	lineStripSet->addCoord(2.0, 0.5, 0.0);
	lineStripSet->addCoord(2.0, 0.5, 1.0);
	lineStripSet->addCoord(3.0, 0.0, 0.0);
	lineStripSet->addCoord(3.0, 0.0, 1.0);
	
	coordIdx = new CIvfIndex();
	coordIdx->createLinear(8);

	lineStripSet->addCoordIndex(coordIdx);

	lineStripSet->addColor(0.0, 0.0, 1.0);
	lineStripSet->addColor(0.0, 1.0, 0.0);
	lineStripSet->addColor(0.0, 1.0, 1.0);
	lineStripSet->addColor(1.0, 0.0, 0.0);
	lineStripSet->addColor(1.0, 0.0, 1.0);
	lineStripSet->addColor(1.0, 1.0, 0.0);
	lineStripSet->addColor(1.0, 1.0, 1.0);
	lineStripSet->addColor(0.0, 0.0, 1.0);

	colorIdx = new CIvfIndex();
	colorIdx->createLinear(8);

	lineStripSet->addColorIndex(colorIdx);

	lineStripSet->setPosition(1.5, 3.0, 3.0);
	lineStripSet->setUseColor(true);
	lineStripSet->setLineWidth(2);

	m_scene->addChild(lineStripSet);

	// Create a TriSet

	CIvfTriSetPtr triSet = new CIvfTriSet();
	
	triSet->addCoord(0.0,0.0,2.0);
	triSet->addCoord(1.0,0.3,2.0);
	triSet->addCoord(2.0,0.0,2.0);
	triSet->addCoord(0.0,0.3,1.0);
	triSet->addCoord(1.0,0.5,1.0);
	triSet->addCoord(2.0,0.3,1.0);
	triSet->addCoord(0.0,0.0,0.0);
	triSet->addCoord(1.0,0.3,0.0);
	triSet->addCoord(2.0,0.0,0.0);

	coordIdx = new CIvfIndex();
	coordIdx->add(0,1,4);
	coordIdx->add(0,4,3);
	coordIdx->add(1,2,5);
	coordIdx->add(1,5,4);
	coordIdx->add(3,4,7);
	coordIdx->add(3,7,6);
	coordIdx->add(4,5,8);
	coordIdx->add(4,8,7);

	triSet->addCoordIndex(coordIdx);

	triSet->addTextureCoord(0.0,0.0);
	triSet->addTextureCoord(0.5,0.0);
	triSet->addTextureCoord(1.0,0.0);
	triSet->addTextureCoord(0.0,0.5);
	triSet->addTextureCoord(0.5,0.5);
	triSet->addTextureCoord(1.0,0.5);
	triSet->addTextureCoord(0.0,1.0);
	triSet->addTextureCoord(0.5,1.0);
	triSet->addTextureCoord(1.0,1.0);

	CIvfIndexPtr textureIdx = new CIvfIndex();
	textureIdx->assignFrom(coordIdx);

	triSet->addTextureIndex(textureIdx);

	triSet->setMaterial(greenMaterial);
	triSet->setTexture(logoTexture);
	//triSet->setUseVertexNormals(true);
	triSet->setPosition(-3.0, 0.0, -3.0);

	m_scene->addChild(triSet);

	// Create a TriStripSet

	CIvfTriStripSetPtr triStripSet = new CIvfTriStripSet();

	triStripSet->addCoord(0.0, 0.0, 0.0);
	triStripSet->addCoord(0.0, 0.0, 1.0);
	triStripSet->addCoord(1.0, 0.5, 0.0);
	triStripSet->addCoord(1.0, 0.5, 1.0);
	triStripSet->addCoord(2.0, 0.5, 0.0);
	triStripSet->addCoord(2.0, 0.5, 1.0);
	triStripSet->addCoord(3.0, 0.0, 0.0);
	triStripSet->addCoord(3.0, 0.0, 1.0);

	triStripSet->addCoord(0.0, 1.0, 0.0);
	triStripSet->addCoord(0.0, 1.0, 1.0);
	triStripSet->addCoord(1.0, 1.5, 0.0);
	triStripSet->addCoord(1.0, 1.5, 1.0);
	triStripSet->addCoord(2.0, 1.5, 0.0);
	triStripSet->addCoord(2.0, 1.5, 1.0);
	triStripSet->addCoord(3.0, 1.0, 0.0);
	triStripSet->addCoord(3.0, 1.0, 1.0);

	coordIdx = new CIvfIndex();
	coordIdx->createLinear(8);

	triStripSet->addCoordIndex(coordIdx);

	coordIdx = new CIvfIndex();
	coordIdx->createLinear(8,8);

	triStripSet->addCoordIndex(coordIdx);

	triStripSet->addColor(0.0, 0.0, 1.0);
	triStripSet->addColor(0.0, 1.0, 0.0);
	triStripSet->addColor(0.0, 1.0, 1.0);
	triStripSet->addColor(1.0, 0.0, 0.0);
	triStripSet->addColor(1.0, 0.0, 1.0);
	triStripSet->addColor(1.0, 1.0, 0.0);
	triStripSet->addColor(1.0, 1.0, 1.0);
	triStripSet->addColor(0.0, 0.0, 1.0);

	colorIdx = new CIvfIndex();
	colorIdx->createLinear(8);

	triStripSet->addColorIndex(colorIdx);

	triStripSet->setMaterial(redMaterial);
	triStripSet->setUseColor(true);
	triStripSet->setUseVertexNormals(true);
	triStripSet->setPosition(1.5, 0.0, -3.0);

	m_scene->addChild(triStripSet);

	// Create a QuadSet

	CIvfQuadSetPtr quadSet = new CIvfQuadSet();
	
	quadSet->addCoord(0.0,0.0,1.0);
	quadSet->addCoord(1.0,0.0,1.0);
	quadSet->addCoord(1.0,0.0,0.0);
	quadSet->addCoord(0.0,0.0,0.0);
	quadSet->addCoord(0.0,1.0,1.0);
	quadSet->addCoord(1.0,1.0,1.0);
	quadSet->addCoord(1.0,1.0,0.0);
	quadSet->addCoord(0.0,1.0,0.0);

	coordIdx = new CIvfIndex();
	coordIdx->add(0,1,5,4);
	coordIdx->add(1,2,6,5);
	coordIdx->add(2,3,7,6);
	coordIdx->add(3,0,4,7);
	coordIdx->add(4,5,6,7);
	coordIdx->add(0,3,2,1);

	quadSet->addCoordIndex(coordIdx);

	quadSet->addColor(0.0, 0.0, 1.0);
	quadSet->addColor(0.0, 1.0, 0.0);
	quadSet->addColor(0.0, 1.0, 1.0);
	quadSet->addColor(1.0, 0.0, 0.0);
	quadSet->addColor(1.0, 0.0, 1.0);
	quadSet->addColor(1.0, 1.0, 0.0);
	quadSet->addColor(1.0, 1.0, 1.0);
	quadSet->addColor(0.0, 0.0, 1.0);
	
	colorIdx = new CIvfIndex();
	colorIdx->assignFrom(coordIdx);
	
	quadSet->addColorIndex(colorIdx);

	quadSet->setUseColor(true);
	quadSet->setPosition(-3.0, 3.0, -3.0);

	m_scene->addChild(quadSet);

	CIvfAxisPtr axis = new CIvfAxis();
	axis->setSize(1.5);
	m_scene->addChild(axis);
	
	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();

	// Export all to a DXF file

	/*
	CIvfDxfWriterPtr dxfWriter = new CIvfDxfWriter();
	dxfWriter->setFileName("advgeom.dxf");
	dxfWriter->setShape(m_scene);
	dxfWriter->write();
	*/
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

	CIvfGlutApplication* app = CIvfGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Advanced geometry");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
