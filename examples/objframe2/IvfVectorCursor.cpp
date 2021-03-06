#include "IvfVectorCursor.h"

#include <ivf/IvfTubeExtrusion.h>
#include <ivf/IvfComposite.h>
#include <ivf/IvfExtrArrow.h>
#include <ivf/IvfBlendState.h>
#include <ivf/IvfDepthBufferState.h>
#include <ivf/IvfCompositeState.h>

#include <ivf/IvfSphere.h>

#define R2D(a) a*360.0/M_PI/2.0
#define D2R(a) a*2.0*M_PI/360.0

CIvfVectorCursor::CIvfVectorCursor()
{
	m_cursorType = CT_ALPHA;

	m_alpha = D2R(0.0);
	m_beta = D2R(-90.0);

	m_alfaRotate = new CIvfTransform();
	m_betaRotate = new CIvfTransform();

	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(1.0f, 1.0f, 0.0f, 1.0f);
	material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	material->setAmbientColor(0.5f, 0.5f, 0.0f, 1.0f);

	CIvfMaterialPtr vectorMaterial = new CIvfMaterial();
	vectorMaterial->setDiffuseColor(1.0f, 1.0f, 0.0f, 0.6f);
	vectorMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.8f);
	vectorMaterial->setAmbientColor(0.5f, 0.5f, 0.0f, 0.5f);

	CIvfMaterialPtr circleMaterial = new CIvfMaterial();
	circleMaterial->setDiffuseColor(0.8f, 0.8f, 0.8f, 0.6f);
	circleMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 0.8f);
	circleMaterial->setAmbientColor(0.2f, 0.2f, 0.2f, 0.5f);

	m_cursor = new CIvfTransform();
	m_nonRotatingCursor = new CIvfTransform();

	CIvfCompositePtr arrow = new CIvfComposite();

	CIvfCircularTubePtr circTube = new CIvfCircularTube();
	circTube->setCircleRadius(0.6);
	circTube->setRadius(0.025);
	circTube->setStartAngle(45.0);
	circTube->setEndAngle(180.0-45.0);

	m_circle1 = new CIvfCircularTube();
	m_circle1->setCircleRadius(0.5);
	m_circle1->setCalculationPoints(32);
	m_circle1->setRadius(0.015);
	m_circle1->setStartAngle(0.0);
	m_circle1->setEndAngle(360.0);
	m_circle1->setMaterial(circleMaterial);

	m_circle2 = new CIvfCircularTube();
	m_circle2->setCircleRadius(0.5);
	m_circle2->setCalculationPoints(32);
	m_circle2->setRadius(0.015);
	m_circle2->setStartAngle(0.0);
	m_circle2->setEndAngle(360.0);
	m_circle2->setRotationQuat(0.0, 0.0, 1.0, 90.0);
	m_circle2->setMaterial(circleMaterial);

	m_cursor->addChild(m_circle2);
	m_nonRotatingCursor->addChild(m_circle1);

	m_spheres1 = new CIvfTransform();
	m_spheres2 = new CIvfTransform();

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

		CIvfSpherePtr sphere1 = new CIvfSphere();
		sphere1->setRadius(sphereRadius);
		sphere1->setPosition(x, 0.0, z);
		sphere1->setMaterial(circleMaterial);

		CIvfSpherePtr sphere2 = new CIvfSphere();
		sphere2->setRadius(sphereRadius);
		sphere2->setPosition(0.0, x, z);
		sphere2->setMaterial(circleMaterial);

		m_spheres1->addChild(sphere1);
		m_spheres2->addChild(sphere2);


		angle+=angleStep;
	}

	m_nonRotatingCursor->addChild(m_spheres1);
	m_cursor->addChild(m_spheres2);

	CIvfExtrArrowPtr extrArrow1 = new CIvfExtrArrow();
	extrArrow1->setSize(0.25,0.25);
	extrArrow1->setRadius(0.05,0.025);
	extrArrow1->setOffset(0.0);
	extrArrow1->setDirection(circTube->getStartVector());
	extrArrow1->setPosition(circTube->getStartPosition());

	CIvfExtrArrowPtr extrArrow2 = new CIvfExtrArrow();
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

	CIvfBlendStatePtr blendState = new CIvfBlendState();
	blendState->setFunction(GL_ONE, GL_ONE);

	CIvfDepthBufferStatePtr depthState = new CIvfDepthBufferState();
	depthState->setState(CIvfDepthBufferState::DS_DISABLED);

	CIvfCompositeStatePtr compState = new CIvfCompositeState();
	compState->addChild(blendState);
	compState->addChild(depthState);

	double arrowSize = 0.5;

	m_vectorIndicator = new CIvfExtrArrow();
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

CIvfVectorCursor::~CIvfVectorCursor()
{

}

void CIvfVectorCursor::doCreateGeometry()
{
	m_cursor->render();
	m_nonRotatingCursor->render();
	m_vectorIndicator->render();
}


void CIvfVectorCursor::setType(TCursorType type)
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

void CIvfVectorCursor::setAlpha(double alpha)
{
	double temp = alpha;
	temp = (double)((long)((temp + (5.0/2)) / 5.0) * 5.0);
	m_alpha = D2R(temp);
	calcVector();
}

void CIvfVectorCursor::setBeta(double beta)
{
	double temp = beta;
	temp = (double)((long)((temp + (5.0/2)) / 5.0) * 5.0);
	m_beta = D2R(temp);
	calcVector();
}

void CIvfVectorCursor::calcVector()
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

double CIvfVectorCursor::getAlpha()
{
	return R2D(m_alpha);
}

double CIvfVectorCursor::getBeta()
{
	return R2D(m_beta);
}

void CIvfVectorCursor::reset()
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


CIvfVec3d& CIvfVectorCursor::getDirection()
{
	return m_vector;
}
