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

#ifdef WIN32
#include <ivf/ivfwin32libs.h>
#endif

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfTexture.h>
#include <ivf/IvfQuadPlane.h>

#include <ivfimage/IvfPngImage.h>

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfWindow {
private:
	CIvfCameraPtr		m_camera;
	CIvfCompositePtr  m_scene;
	CIvfLightPtr		m_light;

	CIvfTexturePtr	m_logoTexture;
	
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
	virtual void onDestroy();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onKeyboard(int key, int x, int y);
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
	m_camera->setPosition(0.0, 0.0, 6.0);
	
	// Create a materials
	
	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	
	// Create scene composite
	
	m_scene = new CIvfComposite();
	
	// Create images
	
	CIvfPngImagePtr logoImage = new CIvfPngImage();
	logoImage->setFileName("images/ivf.png");
	logoImage->read();
	
	// Create a texture
	
	m_logoTexture = new CIvfTexture();
	m_logoTexture->setImage(logoImage);
	m_logoTexture->setFilters(GL_NEAREST, GL_NEAREST);
	m_logoTexture->setGenerateMipmaps(true);

	// Create a quad plane
	
	CIvfQuadPlanePtr logo = new CIvfQuadPlane();
	logo->setSize(1.8, 1.8);;
	logo->setMaterial(material);
	logo->setTexture(m_logoTexture);
	
	m_scene->addChild(logo);
	
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

// ------------------------------------------------------------
void CExampleWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case '1':
		m_logoTexture->setFilters(GL_NEAREST, GL_NEAREST);
		break;
	case '2':
		m_logoTexture->setFilters(GL_LINEAR,  GL_LINEAR);
		break;
	case '3':
		m_logoTexture->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		break;
	case 'd':
		m_logoTexture->setMode(GL_DECAL);
		break;
	case 'b':
		m_logoTexture->setMode(GL_BLEND);
		break;
	case 'm':
		m_logoTexture->setMode(GL_MODULATE);
		break;
	default:

		break;
	}

	m_logoTexture->bind();

	redraw();
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
	
	window->setWindowTitle("Ivf++ Texture mapping example");
	window->show();
	
	// Enter main application loop
	
    app->run();
	
	return 0;
}


