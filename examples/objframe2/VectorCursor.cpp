#include "VectorCursor.h"

#include <ivf/TubeExtrusion.h>
#include <ivf/Composite.h>
#include <ivf/ExtrArrow.h>
#include <ivf/BlendState.h>
#include <ivf/DepthBufferState.h>
#include <ivf/CompositeState.h>

#include <ivf/Sphere.h>

#define R2D(a) a*360.0/M_PI/2.0
#define D2R(a) a*2.0*M_PI/360.0

using namespace ivf;

VectorCursor::VectorCursor()
{
	m_cursorType = CT_ALPHA;

	m_alpha = D2R(0.0);
	m_beta = D2R(-90.0);

	m_alfaRotate = new Transform();
	m_betaRotate = new Transform();

	auto material = Material::create();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	auto vectorMaterial = Material::create();
	vectorMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 0.6f);
	vectorMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.8f);
	vectorMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 0.5f);

	auto circleMaterial = Material::create();
	circleMaterial->setDiffuseColor(0.8f, 0.8f, 0.8f, 0.6f);
	circleMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.8f);
	circleMaterial->setAmbientColor(0.2f, 0.2f, 0.2f, 0.5f);

	m_cursor = Transform::create();
	m_nonRotatingCursor = Transform::create();

	auto arrow = Composite::create();

	auto circTube = CircularTube::create();
	circTube->setCircleRadius(0.6);
	circTube->setRadius(0.025);
	circTube->setStartAngle(45.0);
	circTube->setEndAngle(180.0-45.0);

	m_circle1 = CircularTube::create();
	m_circle1->setCircleRadius(0.5);
	m_circle1->setCalculationPoints(32);
	m_circle1->setRadius(0.015);
	m_circle1->setStartAngle(0.0);
	m_circle1->setEndAngle(360.0);
	m_circle1->setMaterial(circleMaterial);

	m_circle2 = CircularTube::create();
	m_circle2->setCircleRadius(0.5);
	m_circle2->setCalculationPoints(32);
	m_circle2->setRadius(0.015);
	m_circle2->setStartAngle(0.0);
	m_circle2->setEndAngle(360.0);
	m_circle2->setRotationQuat(0.0, 0.0, 1.0, 90.0);
	m_circle2->setMaterial(circleMaterial);

	m_cursor->addChild(m_circle2);
	m_nonRotatingCursor->addChild(m_circle1);

	m_spheres1 = Transform::create();
	m_spheres2 = Transform::create();

	double sphereRadius = 0.04;

	double angle, angleStep;
	double x, z;
	int i;

	angleStep = D2R(45.0);
	angle = 0.0;
	i = 0;

	while (angle<360.0)
	{
		x = 0.5*cos(angle);
		z = 0.5*sin(angle);

		auto sphere1 = Sphere::create();
		sphere1->setRadius(sphereRadius);
		sphere1->setPosition(x, 0.0, z);
		sphere1->setMaterial(circleMaterial);

		auto sphere2 = Sphere::create();
		sphere2->setRadius(sphereRadius);
		sphere2->setPosition(0.0, x, z);
		sphere2->setMaterial(circleMaterial);

		m_spheres1->addChild(sphere1);
		m_spheres2->addChild(sphere2);


		angle+=angleStep;
	}

	m_nonRotatingCursor->addChild(m_spheres1);
	m_cursor->addChild(m_spheres2);

	auto extrArrow1 = ExtrArrow::create();
	extrArrow1->setSize(0.25,0.25);
	extrArrow1->setRadius(0.05,0.025);
	extrArrow1->setOffset(0.0);
	extrArrow1->setDirection(circTube->getStartVector());
	extrArrow1->setPosition(circTube->getStartPosition());

	auto extrArrow2 = ExtrArrow::create();
	extrArrow2->setSize(0.25,0.25);
	extrArrow2->setRadius(0.05,0.025);
	extrArrow2->setOffset(0.0);
	extrArrow2->setDirection(circTube->getEndVector());
	extrArrow2->setPosition(circTube->getEndPosition());

	arrow->addChild(circTube);
	arrow->addChild(extrArrow1);
	arrow->addChild(extrArrow2);
	arrow->setMaterial(material);

	m_alfaRotate->addChild(arrow);

	m_betaRotate->setRotationQuat(0.0, 0.0, 1.0, 90.0);
	m_betaRotate->addChild(arrow);

	// Vector indicator

	auto blendState = BlendState::create();
	blendState->setFunction(GL_ONE, GL_ONE);

	auto depthState = DepthBufferState::create();
	depthState->setState(DepthBufferState::DS_DISABLED);

	auto compState = CompositeState::create();
	compState->addChild(blendState);
	compState->addChild(depthState);

	double arrowSize = 0.5;

	m_vectorIndicator = ExtrArrow::create();
	m_vectorIndicator->setSize(arrowSize*1.0, arrowSize*0.4);
	m_vectorIndicator->setRadius(arrowSize*0.1, arrowSize*0.05);
	m_vectorIndicator->setOffset(-0.7);
	m_vectorIndicator->setDirection(0.0, -1.0, 0.0);
	m_vectorIndicator->setMaterial(vectorMaterial);
	m_vectorIndicator->setRenderState(compState);

	m_cursor->addChild(m_alfaRotate);
	m_cursor->addChild(m_betaRotate);


	setType(CT_VECTOR);
	calcVector();
}

VectorCursor::~VectorCursor()
{

}

void VectorCursor::doCreateGeometry()
{
	m_cursor->render();
	m_nonRotatingCursor->render();
	m_vectorIndicator->render();
}


void VectorCursor::setType(TCursorType type)
{
	m_cursorType = type;

	switch (m_cursorType) {
	case CT_VECTOR:
		m_alfaRotate->disable();
		m_betaRotate->disable();
		m_spheres1->disable();
		m_spheres2->disable();
		m_circle1->disable();
		m_circle2->disable();
		break;
	case CT_ALPHA:
		m_alfaRotate->enable();
		m_betaRotate->disable();
		m_spheres1->enable();
		m_spheres2->disable();
		m_circle1->enable();
		m_circle2->enable();
		break;
	case CT_BETA:
		m_alfaRotate->disable();
		m_betaRotate->enable();
		m_spheres1->disable();
		m_spheres2->enable();
		m_circle1->enable();
		m_circle2->enable();
		break;
	default:

		break;
	}
}

void VectorCursor::setAlpha(double alpha)
{
	double temp = alpha;
	temp = (double)((long)((temp + (5.0/2)) / 5.0) * 5.0);
	m_alpha = D2R(temp);
	calcVector();
}

void VectorCursor::setBeta(double beta)
{
	double temp = beta;
	temp = (double)((long)((temp + (5.0/2)) / 5.0) * 5.0);
	m_beta = D2R(temp);
	calcVector();
}

void VectorCursor::calcVector()
{
	double x, y, z, t;

	t = cos(m_beta);
	y = sin(m_beta);
	x = t*cos(-m_alpha-D2R(90.0));
	z = t*sin(-m_alpha-D2R(90.0));

	m_vector.setComponents(x, y, z);

	m_vectorIndicator->setDirection(m_vector);

	m_cursor->setRotationQuat(0.0, 1.0, 0.0, R2D(m_alpha));
}

double VectorCursor::getAlpha()
{
	return R2D(m_alpha);
}

double VectorCursor::getBeta()
{
	return R2D(m_beta);
}

void VectorCursor::reset()
{
	m_alpha = D2R(0.0);
	m_beta = D2R(-90.0);

	m_alfaRotate->disable();
	m_betaRotate->disable();
	m_spheres1->disable();
	m_spheres2->disable();
	m_circle1->disable();
	m_circle2->disable();

	calcVector();
}


Vec3d& VectorCursor::getDirection()
{
	return m_vector;
}
