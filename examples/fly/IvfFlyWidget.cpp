#include "IvfFlyWidget.h"

#include <ivf/IvfCube.h>
#include <ivf/IvfLighting.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfTransform.h>

#include <ivfctl/IvfRotateController.h>

#include <ivffile/IvfAc3DReader.h>

#include <FL/x.H>
//#include <Mmsystem.h>

#include "StarField.h"

CIvfFlyWidget::CIvfFlyWidget(int X, int Y, int W, int H, const char *L)
:CIvfWindow(X, Y, W, H)
{
}

CIvfFlyWidget::~CIvfFlyWidget()
{
}

void CIvfFlyWidget::onInit(int width, int height)
{
	CIvfVec3d forward;

	forward.setComponents(0.0, 0.0, 1.0);

	m_camera = new CIvfCamera();
	m_camera->setType(CIvfCamera::CT_FLY);
	m_camera->setPosition(0.0, 0.0, 0.0);
	m_camera->setPerspective(45.0, 1.0, 200.0);
	m_camera->setForwardVector(forward);
	
	m_scene = new CIvfScene();
	m_scene->setCamera(m_camera);
	m_scene->setLightMode(CIvfSceneBase::LM_WORLD);

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();
	lighting->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);

	CIvfLightPtr light = lighting->getLight(0);
	light->setPosition(1.0f, 1.0f, 1.0f);
	light->setAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	light->enable();

	m_flyHandler = new CIvfFlyHandler(this, m_camera);
	m_flyHandler->setRedraw(false);

	m_starfield = new CStarField(m_camera);

	m_scene->addChild(m_starfield);

	CIvfMaterial* redMaterial = new CIvfMaterial();
	redMaterial->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);

	int i;
	double x, y, z;
	double vx, vy, vz;

	m_controllers = new CIvfControllerGroup();
	m_controllers->activate();

	CIvfAc3DReader* fileReader = new CIvfAc3DReader();
	fileReader->setFileName("models/asteroid01.ac");
	fileReader->read();

	CIvfShapePtr asteroid = fileReader->getShape();
	
	for (i=0; i<50; i++)
	{
		x = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		y = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		z = 20.0*(1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		vx = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vy = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);
		vz = (1.0 - 2.0*(double)(rand())/(double)RAND_MAX);

		CIvfTransformPtr asteroidXf = new CIvfTransform();
		asteroidXf->addChild(asteroid);
		asteroidXf->setPosition(x, y, z);
		asteroidXf->setRotationQuat(vx, vy, vz, 0.0);
		m_scene->addChild(asteroidXf);

		CIvfRotateControllerPtr rotateController = new CIvfRotateController();
		rotateController->setRotationSpeed(40);
		rotateController->setShape(asteroidXf);
		rotateController->activate();

		m_controllers->addChild(rotateController);
	}

	m_firstFrame = true;
	m_t = 0.0;
	m_dt = 0.0;
}

void CIvfFlyWidget::onInitContext(int width, int height)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CIvfFlyWidget::onResize(int width, int height)
{
	m_camera->setViewPort(w(), h());
	m_camera->initialize();
}

void CIvfFlyWidget::onRender()
{
	m_scene->render();
}

void CIvfFlyWidget::onClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool CIvfFlyWidget::onAppLoop()
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

void CIvfFlyWidget::onMouseDown(int x, int y)
{
	m_flyHandler->doMouseDown(x, y);
}

void CIvfFlyWidget::onMouseMove(int x, int y)
{
	m_flyHandler->doMouseMove(x, y);
}

void CIvfFlyWidget::onMouseUp(int x, int y)
{
	m_flyHandler->doMouseUp(x, y);
}

void CIvfFlyWidget::onKeyboard(int key, int x, int y)
{
	m_flyHandler->doKeyboard(key, x, y);
}
