#include "CircularTube.h"

using namespace ivf;

#define R2D(a) a*360.0/M_PI/2.0
#define D2R(a) a*2.0*M_PI/360.0

CircularTube::CircularTube()
{
	m_radius = 1.0;
	m_startAngle = 0.0;
	m_endAngle = D2R(180.0);
	m_calcPoints = 20;

	refresh();
}

CircularTube::~CircularTube()
{

}

void CircularTube::refresh()
{
	initSpine();
	TubeExtrusion::refresh();
}

void CircularTube::initSpine()
{
	double angle, angleStep;
	double x, z;
	int i;

	setSpineSize(m_calcPoints+2);

	angleStep = (m_endAngle - m_startAngle)/(m_calcPoints-1);
	angle = m_startAngle-angleStep;
	i = 0;

	while (angle<(m_endAngle+angleStep)) 
	{
		x = m_radius*cos(angle);
		z = m_radius*sin(angle);

		if (angle==m_startAngle)
		{
			m_startVector.setComponents(z, 0.0, -x);
			m_startVector.normalize();
			m_startPos.setComponents(x, 0.0, z);
		}

		setSpineCoord(i++, x, 0.0, z);
		
		angle+=angleStep;
	}

	getSpineCoord(m_calcPoints, x, z, z);
	m_endVector.setComponents(-z, 0.0, x);
	m_endVector.normalize();
	m_endPos.setComponents(x, 0.0, z);

}

void CircularTube::setCircleRadius(double radius)
{
	m_radius = radius;
	refresh();
}

void CircularTube::setStartAngle(double angle)
{
	m_startAngle = D2R(angle);
	refresh();
}

void CircularTube::setEndAngle(double angle)
{
	m_endAngle = D2R(angle);
	refresh();
}

void CircularTube::setCalculationPoints(int numberOfPoints)
{
	m_calcPoints = numberOfPoints;
}

double CircularTube::getCircleRadius()
{
	return m_radius;
}

double CircularTube::getStartAngle()
{
	return R2D(m_startAngle);
}

double CircularTube::getEndAngle()
{
	return R2D(m_endAngle);
}

int CircularTube::getCalculationPoints()
{
	return m_calcPoints;
}

Vec3d& CircularTube::getStartVector()
{
	return m_startVector;
}

Vec3d& CircularTube::getEndVector()
{
	return m_endVector;
}

Vec3d& CircularTube::getStartPosition()
{
	return m_startPos;
}

Vec3d& CircularTube::getEndPosition()
{
	return m_endPos;
}
