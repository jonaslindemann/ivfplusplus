#include "IvfFlyWidget.h"

#include <ivf/Cube.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Transform.h>

#include <ivfctl/RotateController.h>

#include <ivffile/Ac3DReader.h>

//#include <Mmsystem.h>

#include "StarField.h"

using namespace ivf;

FlyWidget::FlyWidget(int X, int Y, int W, int H, const char *L)
:GlutBase(X, Y, W, H)
{
}

FlyWidget::~FlyWidget()
{
}

FlyWidgetPtr FlyWidget::create(int X, int Y, int W, int H)
{
	return FlyWidgetPtr(new FlyWidget(X, Y, W, H));
}

void FlyWidget::onInit(int width, int height)
{
	Vec3d forward;

	forward.setComponents(0.0, 0.0, 1.0);

	m_camera = new Camera();
	m_camera->setType(Camera::CT_FLY);
	m_camera->setPosition(0.0, 0.0, 0.0);
	m_camera->setPerspective(45.0, 1.0, 200.0);
	m_camera->setForwardVector(forward);
	
	m_scene = new Scene();
	m_scene->setCamera(m_camera);
	m_scene->setLightMode(SceneBase::LM_WORLD);

	auto lighting = Lighting::getInstance();
	lighting->enable();
	lighting->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);

	auto light = lighting->getLight(0);
	light->setPosition(1.0f, 1.0f, 1.0f);
	light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	light->enable();

	m_flyHandler = new FlyHandler(this, m_camera);
	m_flyHandler->setRedraw(false);

	m_starfield = new StarField(m_camera);

	m_scene->addChild(m_starfield);

	auto redMaterial = Material::create();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);

	int i;
	double x, y, z;
	double vx, vy, vz;

	m_controllers = new ControllerGroup();
	m_controllers->activate();

	auto fileReader = Ac3DReader::create();
	fileReader->setFileName("data/models/asteroid01.ac");
	fileReader->read();

	ShapePtr asteroid = fileReader->getShape();
	
	for (i=0; i<50; i++)
	{
		x = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		y = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		z = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		vx = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vy = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vz = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		auto asteroidXf = Transform::create();
		asteroidXf->addChild(asteroid);
		asteroidXf->setPosition(x, y, z);
		asteroidXf->setRotationQuat(vx, vy, vz, 0.0);
		m_scene->addChild(asteroidXf);

		auto rotateController = RotateController::create();
		rotateController->setRotationSpeed(40);
		rotateController->setShape(asteroidXf);
		rotateController->activate();

		m_controllers->addChild(rotateController);
	}

	m_firstFrame = true;
	m_t = 0.0;
	m_dt = 0.0;
}

void FlyWidget::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void FlyWidget::onResize(int width, int height)
{
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

void FlyWidget::onRender()
{
	m_scene->render();
}

void FlyWidget::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FlyWidget::onAppLoop()
{
	m_flyHandler->update();

	if (m_firstFrame)
	{
		m_t = elapsedTime();
		m_firstFrame = false;
	}
	else
	{
		m_dt = elapsedTime()-m_t;
		m_t = elapsedTime();
		m_controllers->update(m_dt);
	}

	this->redraw();

	return true;
}

void FlyWidget::onMouseDown(int x, int y)
{
	m_flyHandler->doMouseDown(x, y);
}

void FlyWidget::onMouseMove(int x, int y)
{
	m_flyHandler->doMouseMove(x, y);
}

void FlyWidget::onMouseUp(int x, int y)
{
	m_flyHandler->doMouseUp(x, y);
}

void FlyWidget::onKeyboard(int key, int x, int y)
{
	m_flyHandler->doKeyboard(key, x, y);
}
