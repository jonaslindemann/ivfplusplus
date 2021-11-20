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

CFlyWidget::CFlyWidget(int X, int Y, int W, int H, const char *L)
:CGlutBase(X, Y, W, H)
{
}

CFlyWidget::~CFlyWidget()
{
}

void CFlyWidget::onInit(int width, int height)
{
	CVec3d forward;

	forward.setComponents(0.0, 0.0, 1.0);

	m_camera = new CCamera();
	m_camera->setType(CCamera::CT_FLY);
	m_camera->setPosition(0.0, 0.0, 0.0);
	m_camera->setPerspective(45.0, 1.0, 200.0);
	m_camera->setForwardVector(forward);
	
	m_scene = new CScene();
	m_scene->setCamera(m_camera);
	m_scene->setLightMode(CSceneBase::LM_WORLD);

	CLightingPtr lighting = CLighting::getInstance();
	lighting->enable();
	lighting->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);

	CLightPtr light = lighting->getLight(0);
	light->setPosition(1.0f, 1.0f, 1.0f);
	light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	light->enable();

	m_flyHandler = new CFlyHandler(this, m_camera);
	m_flyHandler->setRedraw(false);

	m_starfield = new CStarField(m_camera);

	m_scene->addChild(m_starfield);

	CMaterial* redMaterial = new CMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);

	int i;
	double x, y, z;
	double vx, vy, vz;

	m_controllers = new CControllerGroup();
	m_controllers->activate();

	CAc3DReader* fileReader = new CAc3DReader();
	fileReader->setFileName("data/models/asteroid01.ac");
	fileReader->read();

	CShapePtr asteroid = fileReader->getShape();
	
	for (i=0; i<50; i++)
	{
		x = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		y = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		z = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		vx = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vy = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vz = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		CTransformPtr asteroidXf = new CTransform();
		asteroidXf->addChild(asteroid);
		asteroidXf->setPosition(x, y, z);
		asteroidXf->setRotationQuat(vx, vy, vz, 0.0);
		m_scene->addChild(asteroidXf);

		CRotateControllerPtr rotateController = new CRotateController();
		rotateController->setRotationSpeed(40);
		rotateController->setShape(asteroidXf);
		rotateController->activate();

		m_controllers->addChild(rotateController);
	}

	m_firstFrame = true;
	m_t = 0.0;
	m_dt = 0.0;
}

void CFlyWidget::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CFlyWidget::onResize(int width, int height)
{
	m_camera->setViewPort(width, height);
	m_camera->initialize();
}

void CFlyWidget::onRender()
{
	m_scene->render();
}

void CFlyWidget::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool CFlyWidget::onAppLoop()
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

void CFlyWidget::onMouseDown(int x, int y)
{
	m_flyHandler->doMouseDown(x, y);
}

void CFlyWidget::onMouseMove(int x, int y)
{
	m_flyHandler->doMouseMove(x, y);
}

void CFlyWidget::onMouseUp(int x, int y)
{
	m_flyHandler->doMouseUp(x, y);
}

void CFlyWidget::onKeyboard(int key, int x, int y)
{
	m_flyHandler->doKeyboard(key, x, y);
}
