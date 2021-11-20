#ifndef _CIvfCircularTube_h_
#define _CIvfCircularTube_h_

#include <ivf/TubeExtrusion.h>

IvfSmartPointer(CCircularTube);

class CCircularTube : public ivf::CTubeExtrusion {
private:
	double	m_radius;
	double	m_startAngle;
	double	m_endAngle;
	int		m_calcPoints;

	ivf::CVec3d m_startVector;
	ivf::CVec3d m_endVector;
	ivf::CVec3d m_startPos;
	ivf::CVec3d m_endPos;

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

	ivf::CVec3d& getEndVector();
	ivf::CVec3d& getStartVector();

	ivf::CVec3d& getEndPosition();
	ivf::CVec3d& getStartPosition();

	virtual void refresh();
};

#endif 
