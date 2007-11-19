// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

#include <ivf/IvfCamera.h>
#include <ivf/IvfAxis.h>
#include <ivf/IvfScene.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfCube.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCylinder.h>

#include <ivfwidget/IvfMouseViewHandler.h>
#include <ivfwidget/IvfSceneHandler.h>

#include <ivffont/IvfExtrudedFont.h>
#include <ivffont/IvfPolygonalFont.h>
#include <ivffont/IvfOutlineFont.h>
#include <ivffont/IvfTextureFont.h>
#include <ivffont/IvfText.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CFontsWindow);

class CFontsWindow: public CIvfWindow,
	CIvfInitEvent,
	CIvfKeyboardEvent
{
private:
	CIvfCameraPtr		m_camera;
	CIvfScenePtr		m_scene;
	CIvfLightPtr		m_light;

	CIvfExtrudedFontPtr			m_extrudedFont;
	CIvfPolygonalFontPtr		m_polygonalFont;
	CIvfOutlineFontPtr			m_outlineFont;
	CIvfTextureFontPtr			m_textureFont;

	CIvfTextPtr					m_text;

	CIvfMouseViewHandlerPtr m_mouseHandler;
	CIvfSceneHandlerPtr	m_sceneHandler;

public:
	CFontsWindow(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

CFontsWindow::CFontsWindow(int X, int Y, int W, int H)
	:CIvfWindow(X, Y, W, H)
{
	addInitEvent(this);
	addKeyboardEvent(this);
}

void CFontsWindow::onInit(int width, int height)
{
	// Initialize Ivf++ camera

	m_camera = new CIvfCamera();
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setPosition(-0.0, 3.0, 5.0);
	m_camera->setTarget(-0.0, 0.0, 0.0);

	// Create scene

	m_scene = new CIvfScene();
	m_scene->setView(m_camera);

	// Create a materials

	CIvfMaterialPtr redMaterial = new CIvfMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	redMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	redMaterial->setAmbientColor(0.5f, 0.0f, 0.0f, 1.0f);

	CIvfMaterialPtr greenMaterial = new CIvfMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	greenMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	greenMaterial->setAmbientColor(0.0f, 0.5f, 0.0f, 1.0f);

	CIvfMaterialPtr blueMaterial = new CIvfMaterial();
	blueMaterial->setDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	blueMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	blueMaterial->setAmbientColor(0.0f, 0.0f, 0.5f, 1.0f);

	CIvfMaterialPtr yellowMaterial = new CIvfMaterial();
	yellowMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	yellowMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	yellowMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	// Create fonts

#ifdef WIN32
	m_extrudedFont = new CIvfExtrudedFont("C:\\Windows\\Fonts\\Tahoma.ttf");
	m_polygonalFont = new CIvfPolygonalFont("C:\\Windows\\Fonts\\Tahoma.ttf");
	m_outlineFont = new CIvfOutlineFont("C:\\Windows\\Fonts\\Tahoma.ttf");
#else
        m_extrudedFont = new CIvfExtrudedFont("/usr/share/fonts/bitstream-vera/Vera.ttf");
        m_polygonalFont = new CIvfPolygonalFont("/usr/share/fonts/bitstream-vera/Vera.ttf");
        m_outlineFont = new CIvfOutlineFont("/usr/share/fonts/bitstream-vera/Vera.ttf");
#endif 

	glEnable(GL_TEXTURE_2D);

	//m_textureFont = new CIvfTextureFont("C:\\Windows\\Fonts\\trebuc.ttf");
	//m_textureFont->setRealFaceSize(1.0);
	//m_textureFont->setResolution(144);

	// Create text object

	m_text = new CIvfText();
	m_text->setFont(m_extrudedFont);
	m_text->setText("Hello, World!");
	m_text->setMaterial(yellowMaterial);

	m_scene->addChild(m_text);

	CIvfAxisPtr axis = new CIvfAxis();
	m_scene->addChild(axis);

	// Initialize lighting

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	m_light = lighting->getLight(0);
	m_light->setLightPosition(1.0, 1.0, 1.0, 0.0);
	m_light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_light->enable();


	// Create event handlers

	m_mouseHandler = new CIvfMouseViewHandler(this, m_camera);
	m_mouseHandler->activate();

	m_sceneHandler = new CIvfSceneHandler(this, m_scene);
	m_sceneHandler->activate();
}

void CFontsWindow::onKeyboard(int key, int x, int y)
{
	switch (key) {
	case '1' :
		m_text->setFont(m_extrudedFont);
		this->redraw();
		break;
	case '2' :
		m_text->setFont(m_polygonalFont);
		this->redraw();
		break;
	case '3' :
		m_text->setFont(m_outlineFont);
		this->redraw();
		break;
	case '4' :
		//m_text->setFont(m_textureFont);
		this->redraw();
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv)
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CFontsWindowPtr window = new CFontsWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Fonts");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}
