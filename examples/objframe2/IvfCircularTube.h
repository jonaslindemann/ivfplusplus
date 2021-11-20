#ifndef _CIvfCircularTube_h_
#define _CIvfCircularTube_h_

#include <ivf/IvfTubeExtrusion.h>

IvfSmartPointer(CCircularTube);

class CCircularTube : public CTubeExtrusion {
private:
	double	m_radius;
	double	m_startAngle;
	double	m_endAngle;
	int		m_calcPoints;

	CVec3d m_startVector;
	CVec3d m_endVector;
	CVec3d m_startPos;
	CVec3d m_endPos;

	void initSpine();
public:
	CCircularTube();
	virtual ~CCircularTube();

	void setCalculationPoints(int numberOfPoints);
	int getCalculationPoints();

	void setEndAngle(double angle);
	double getEndAngle();

	void setStartAngle(double angle);
	double getStartAngle();

	void setCircleRadius(double radius);
	double getCircleRadius();

	CVec3d& getEndVector();
	CVec3d& getStartVector();

	CVec3d& getEndPosition();
	CVec3d& getStartPosition();

	virtual void refresh();
};

#endif 
