// ------------------------------------------------------------
//
// Ivf++ Texture mapping example
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
#include <ivf/IvfAxis.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfTexture.h>
#include <ivf/IvfQuadPlane.h>
#include <ivf/IvfSolidLine.h>

#include <ivfimage/IvfSgiImage.h>
#include <ivfimage/IvfJpegImage.h>
#include <ivfimage/IvfPngImage.h>

#include "perlinNoise.h"

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfGlutBase {
private:
	CIvfCameraPtr		m_camera;
	CIvfCompositePtr  m_scene;
	CIvfLightPtr		m_light;
	CIvfSolidLinePtr  m_solidLine;
	CIvfTexturePtr	m_coloredTexture;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	int m_textureMode;


public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfGlutBase(X, Y, W, H) {};

	void setTextureMode(int mode);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onDestroy();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onKeyboard(int key, int x, int y);
	float fnoise(float x, float y);
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

	m_textureMode = 0;

	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPosition(0.0, 0.0, 6.0);

	// Create a materials

	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Create scene composite

	m_scene = new CIvfComposite();

	// Create images

	CIvfSgiImagePtr image1 = new CIvfSgiImage();
	image1->setFileName("images/cars1.rgb");
	image1->read();

	CIvfJpegImagePtr image2 = new CIvfJpegImage();
	image2->setFileName("images/architecture8.jpg");
	image2->read();

	CIvfPngImagePtr image3 = new CIvfPngImage();
	image3->setFileName("images/ivf.png");
	image3->read();

	CIvfJpegImagePtr image4 = new CIvfJpegImage();
	image4->setFileName("images/road2.jpg");
	image4->read();

	CIvfJpegImagePtr image5 = new CIvfJpegImage();
	image5->setFileName("images/architecture9.jpg");
	image5->read();

	CIvfImagePtr image6 = new CIvfImage();
	image6->setSize(4,4);
    
    for (int i=0; i<4; i++)
    {
        image6->setPixel(i, 0, 255, 0, 0);
        image6->setPixel(i, 1, 0, 255, 0);
        image6->setPixel(i, 2, 0, 0, 255);
        image6->setPixel(i, 3, 255, 0, 255);
    }

	// Create a texture

	CIvfTexturePtr texture1 = new CIvfTexture();
	texture1->setImage(image1);
	texture1->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture1->setGenerateMipmaps(true);

	CIvfTexturePtr texture2 = new CIvfTexture();
	texture2->setImage(image2);
	texture2->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture2->setGenerateMipmaps(true);

	CIvfTexturePtr texture3 = new CIvfTexture();
	texture3->setImage(image3);
	texture3->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture3->setGenerateMipmaps(true);

	CIvfTexturePtr texture4 = new CIvfTexture();
	texture4->setImage(image4);
	texture4->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture4->setGenerateMipmaps(true);

	CIvfTexturePtr texture5 = new CIvfTexture();
	texture5->setImage(image6);
	texture5->setFilters(GL_NEAREST, GL_NEAREST);
	texture5->setGenerateMipmaps(false);

	m_coloredTexture = new CIvfTexture();
	m_coloredTexture->setFilters(GL_NEAREST, GL_NEAREST);
	m_coloredTexture->setImage(image6);

	CIvfNodePtr node1 = new CIvfNode();
	CIvfNodePtr node2 = new CIvfNode();

	node1->setPosition(-2.0, -0.5, 0.0);
	node2->setPosition( 2.0, 0.5, 0.0);

	m_solidLine = new CIvfSolidLine();
	m_solidLine->setNodes(node1, node2);
    m_solidLine->setSides(12);
	m_solidLine->setMaterial(material);
	m_solidLine->setTexture(m_coloredTexture);
    m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_MODEL_CYL);
    //m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_MODEL_CYL);
	m_coloredTexture->setTextureModifier(1.0, 1.0/m_solidLine->getLength(), 0.0);
	m_coloredTexture->setTextureMode(GL_MODULATE);

	m_scene->addChild(m_solidLine);

	setTextureMode(0);

	// Create a quad plane

	CIvfQuadPlanePtr quadPlane = new CIvfQuadPlane();
	quadPlane->setSize(1.8, 1.8);;
	quadPlane->setMaterial(material);

	// We reuse the quadset by adding it to
	// 5 different transforms and assigning
	// the textures to these instead.

	CIvfTransformPtr xfrm1 = new CIvfTransform();
	xfrm1->addChild(quadPlane);
	xfrm1->setPosition(-1.2, -1.2, 0.0);
	xfrm1->setTexture(texture1);

	CIvfTransformPtr xfrm2 = new CIvfTransform();
	xfrm2->addChild(quadPlane);
	xfrm2->setPosition(1.2, -1.2, 0.0);
	xfrm2->setTexture(texture2);

	CIvfTransformPtr xfrm3 = new CIvfTransform();
	xfrm3->addChild(quadPlane);
	xfrm3->setPosition(0.0, 0.0, -3.0);
	xfrm3->setTexture(texture3);

	CIvfTransformPtr xfrm4 = new CIvfTransform();
	xfrm4->addChild(quadPlane);
	xfrm4->setPosition(1.2, 1.2, 0.0);
	xfrm4->setTexture(texture4);

	CIvfTransformPtr xfrm5 = new CIvfTransform();
	xfrm5->addChild(quadPlane);
	xfrm5->setPosition(-1.2, 1.2, 0.0);
	xfrm5->setTexture(texture5);

	m_scene->addChild(xfrm1);
	m_scene->addChild(xfrm2);
	m_scene->addChild(xfrm3);
	m_scene->addChild(xfrm4);
	m_scene->addChild(xfrm5);

	// Create a light

	CIvfLightingPtr lighting = CIvfLighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setType(CIvfLight::LT_DIRECTIONAL);
	m_light->setDirection(1.0, 1.0, 1.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
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

	m_camera->rotatePositionY(m_angleX/100.0);
	m_camera->rotatePositionX(m_angleY/100.0);
	m_camera->moveSideways(m_moveX/100.0);
	m_camera->moveVertical(m_moveY/100.0);
	m_camera->moveDepth(m_zoomY/50.0);
	m_camera->render();

	m_scene->render();
}

// ------------------------------------------------------------
void CExampleWindow::onDestroy()
{
	delete m_camera;
	delete m_light;
	delete m_scene;
}

void CExampleWindow::onKeyboard(int key, int x, int y)
{
	if (key=='m')
	{
		if (m_textureMode<11)
			m_textureMode++;
		else
			m_textureMode = 0;

		setTextureMode(m_textureMode);

		redraw();
	}

	if (key=='a')
	{
		cout << "Texture activated." << endl;
		m_coloredTexture->activate();
		redraw();
	}

	if (key=='d')
	{
		cout << "Texture deactivated." << endl;
		m_coloredTexture->deactivate();
		redraw();
	}	
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
void CExampleWindow::setTextureMode(int mode)
{
	switch (m_textureMode) {
case 0:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_FLAT); break;
case 1:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_FLAT); break;
case 2:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_CYL); break;
case 3:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_CYL); break;
case 4:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_SPH); break;
case 5:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_SPH); break;
case 6:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_MODEL_FLAT); break;
case 7:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_MODEL_FLAT); break;
case 8:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_MODEL_CYL); break;
case 9:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_MODEL_CYL); break;
case 10:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_VERTEX_MODEL_SPH); break;
case 11:m_solidLine->setTextureMode(GLE_TEXTURE_ENABLE | GLE_TEXTURE_NORMAL_MODEL_SPH); break;
	}
	switch (m_textureMode) {
case 0:cout << "GLE_TEXTURE_VERTEX_FLAT" << endl; break;
case 1:cout << "GLE_TEXTURE_NORMAL_FLAT" << endl; break;
case 2:cout << "GLE_TEXTURE_VERTEX_CYL" << endl; break;
case 3:cout << "GLE_TEXTURE_NORMAL_CYL" << endl; break;
case 4:cout << "GLE_TEXTURE_VERTEX_SPH" << endl; break;
case 5:cout << "GLE_TEXTURE_NORMAL_SPH" << endl; break;
case 6:cout << "GLE_TEXTURE_VERTEX_MODEL_FLAT" << endl; break;
case 7:cout << "GLE_TEXTURE_NORMAL_MODEL_FLAT" << endl; break;
case 8:cout << "GLE_TEXTURE_VERTEX_MODEL_CYL" << endl; break;
case 9:cout << "GLE_TEXTURE_NORMAL_MODEL_CYL" << endl; break;
case 10:cout << "GLE_TEXTURE_VERTEX_MODEL_SPH" << endl; break;
case 11:cout << "GLE_TEXTURE_NORMAL_MODEL_SPH" << endl; break;
	}
}

float CExampleWindow::fnoise(float x, float y)
{
  int i;
  float amplitude = 1.0;
  float result = 0.0;
  int noiseMaxOctaves = 10;
  float noisePersistance = 0.3;

  for (i=0; i<noiseMaxOctaves; i++) {
    result += perlinNoise::noise(x, y) * amplitude;
    amplitude *= noisePersistance;
    x *= 2.0; y *= 2.0;
  }
  return result;
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

	window->setWindowTitle("Ivf++ Texture mapping example");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}


