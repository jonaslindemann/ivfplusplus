#ifndef _CIvfCircularTube_h_
#define _CIvfCircularTube_h_

#include <ivf/IvfTubeExtrusion.h>

IvfSmartPointer(CIvfCircularTube);

class CIvfCircularTube : public CIvfTubeExtrusion {
private:
	double	m_radius;
	double	m_startAngle;
	double	m_endAngle;
	int		m_calcPoints;

	CIvfVec3d m_startVector;
	CIvfVec3d m_endVector;
	CIvfVec3d m_startPos;
	CIvfVec3d m_endPos;

	void initSpine();
public:
	CIvfCircularTube();
	virtual ~CIvfCircularTube();

	void setCalculationPoints(int numberOfPoints);
	int getCalculationPoints();

	void setEndAngle(double angle);
	double getEndAngle();

	void setStartAngle(double angle);
	double getStartAngle();

	void setCircleRadius(double radius);
	double getCircleRadius();

	CIvfVec3d& getEndVector();
	CIvfVec3d& getStartVector();

	CIvfVec3d& getEndPosition();
	CIvfVec3d& getStartPosition();

	virtual void refresh();
};

#endif 
