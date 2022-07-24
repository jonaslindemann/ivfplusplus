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

#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include <ivf/Camera.h>
#include <ivf/Axis.h>
#include <ivf/Composite.h>
#include <ivf/Transform.h>
#include <ivf/Lighting.h>
#include <ivf/Light.h>
#include <ivf/Material.h>
#include <ivf/Texture.h>
#include <ivf/QuadPlane.h>
#include <ivf/SolidLine.h>
#include <ivf/BitmapFont.h>
#include <ivf/TextLabel.h>

#include <ivfimage/SgiImage.h>
#include <ivfimage/JpegImage.h>
#include <ivfimage/PngImage.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

float random()
{
	return ((double)(rand()) / (double)RAND_MAX);
}

IvfSmartPointer(ExampleWindow);

class ExampleWindow: public GlutBase {
private:
	CameraPtr		m_camera;
	CompositePtr  m_scene;
	LightPtr		m_light;

	BitmapFontPtr m_font;
	TextLabelPtr m_textLabel;

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
	ExampleWindow(int X, int Y, int W, int H)
		:GlutBase(X, Y, W, H) {};

	static ExampleWindowPtr create(int X, int Y, int W, int H);

	virtual void onInit(int width, int height);
	virtual void onResize(int width, int height);
	virtual void onRender();
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
	return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

void ExampleWindow::onInit(int width, int height)
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

	m_camera = new Camera();
	m_camera->setPosition(0.0, 0.0, 10.0);
	m_camera->setPerspective(45.0, 0.2, 10000.0);

	// Create a materials

	auto material = Material::create();
	material->setDiffuseColor(1.0f, 0.5f, 0.5f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	
	// Create scene composite

	m_scene = Composite::create();

	m_font = BitmapFont::create("data/fonts/white_font.fnt");

	m_textLabel = TextLabel::create();
	m_textLabel->setFont(m_font);
	m_textLabel->setText("Ivf++", 2.0);
 	m_textLabel->setCamera(m_camera);

	auto texts = ivf::Composite::create();

	for (auto i = 0; i < 20; i++)
	{
		auto text = TextLabel::create();

		double x = 10.0 - 20.0 * random();
		double y = 10.0 - 20.0 * random();
		double z = 10.0 - 20.0 * random();

		text->setFont(m_font);
		text->setText("A");
		text->setPosition(x, y, z);
		text->setCamera(m_camera);
		text->setBillboardType(IVF_BILLBOARD_XY);
		texts->addChild(text);
	}

	auto quad = QuadPlane::create();
	quad->setMaterial(material);
	quad->setSize(20.0, 20.0);
	quad->setPosition(0.0, 0.0, -5.0);

	auto axis = Axis::create();

	m_scene->addChild(quad);
	m_scene->addChild(axis);
	m_scene->addChild(m_textLabel);
	m_scene->addChild(texts);

	// Create a light

	auto lighting = Lighting::getInstance();

	m_light = lighting->getLight(0);
	m_light->setType(Light::LT_DIRECTIONAL);
	m_light->setDirection(1.0, 1.0, 1.0);
	m_light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f); 
	m_light->enable();
}

// ------------------------------------------------------------
void ExampleWindow::onResize(int width, int height)
{
	m_camera->setPerspective(45.0, 0.1, 100.0);
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

// ------------------------------------------------------------
void ExampleWindow::onRender()
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

void ExampleWindow::onKeyboard(int key, int x, int y)
{
	if (key=='1')
		m_textLabel->setAlignment(HorizontalAlignment::Right, VerticalAlignment::Top);
	if (key == '2')
		m_textLabel->setAlignment(HorizontalAlignment::Right, VerticalAlignment::Middle);
	if (key == '3')
		m_textLabel->setAlignment(HorizontalAlignment::Right, VerticalAlignment::Bottom);
	if (key == '4')
		m_textLabel->setAlignment(HorizontalAlignment::Center, VerticalAlignment::Top);
	if (key == '5')
		m_textLabel->setAlignment(HorizontalAlignment::Center, VerticalAlignment::Middle);
	if (key == '6')
		m_textLabel->setAlignment(HorizontalAlignment::Center, VerticalAlignment::Bottom);
	if (key == '7')
		m_textLabel->setAlignment(HorizontalAlignment::Left, VerticalAlignment::Top);
	if (key == '8')
		m_textLabel->setAlignment(HorizontalAlignment::Left, VerticalAlignment::Middle);
	if (key == '9')
		m_textLabel->setAlignment(HorizontalAlignment::Left, VerticalAlignment::Bottom);

	this->redraw();
}

// ------------------------------------------------------------
void ExampleWindow::onMouseDown(int x, int y)
{
	m_beginX = x;
	m_beginY = y;
}

// ------------------------------------------------------------
void ExampleWindow::onMouseMove(int x, int y)
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
		if (getModifierKey() == WidgetBase::MT_SHIFT)
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
void ExampleWindow::onMouseUp(int x, int y)
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

	auto app = GlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGB|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	auto window = ExampleWindow::create(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Texture mapping example");
	window->show();

	// Enter main application loop

	app->run();

	return 0;
}


