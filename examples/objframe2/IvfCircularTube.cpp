#include "IvfCircularTube.h"

#define R2D(a) a*360.0/M_PI/2.0
#define D2R(a) a*2.0*M_PI/360.0

CCircularTube::CCircularTube()
{
	m_radius = 1.0;
	m_startAngle = 0.0;
	m_endAngle = D2R(180.0);
	m_calcPoints = 20;

	refresh();
}

CCircularTube::~CCircularTube()
{

}

void CCircularTube::refresh()
{
	initSpine();
	CTubeExtrusion::refresh();
}

void CCircularTube::initSpine()
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

void CCircularTube::setCircleRadius(double radius)
{
	m_radius = radius;
	refresh();
}

void CCircularTube::setStartAngle(double angle)
{
	m_startAngle = D2R(angle);
	refresh();
}

void CCircularTube::setEndAngle(double angle)
{
	m_endAngle = D2R(angle);
	refresh();
}

void CCircularTube::setCalculationPoints(int numberOfPoints)
{
	m_calcPoints = numberOfPoints;
}

double CCircularTube::getCircleRadius()
{
	return m_radius;
}

double CCircularTube::getStartAngle()
{
	return R2D(m_startAngle);
}

double CCircularTube::getEndAngle()
{
	return R2D(m_endAngle);
}

int CCircularTube::getCalculationPoints()
{
	return m_calcPoints;
}

CVec3d& CCircularTube::getStartVector()
{
	return m_startVector;
}

CVec3d& CCircularTube::getEndVector()
{
	return m_endVector;
}

CVec3d& CCircularTube::getStartPosition()
{
	return m_startPos;
}

CVec3d& CCircularTube::getEndPosition()
{
	return m_endPos;
}
