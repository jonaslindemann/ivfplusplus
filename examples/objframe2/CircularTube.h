#ifndef _CIvfCircularTube_h_
#define _CIvfCircularTube_h_

#include <ivf/TubeExtrusion.h>

IvfSmartPointer(CircularTube);

class CircularTube : public ivf::TubeExtrusion {
private:
	double	m_radius;
	double	m_startAngle;
	double	m_endAngle;
	int		m_calcPoints;

	ivf::Vec3d m_startVector;
	ivf::Vec3d m_endVector;
	ivf::Vec3d m_startPos;
	ivf::Vec3d m_endPos;

	void initSpine();
public:
	CircularTube();
	virtual ~CircularTube();

	IvfClassInfo("CircularTube", Base);
	IvfStdFactory(CircularTube);

	void setCalculationPoints(int numberOfPoints);
	int getCalculationPoints();

	void setEndAngle(double angle);
	double getEndAngle();

	void setStartAngle(double angle);
	double getStartAngle();

	void setCircleRadius(double radius);
	double getCircleRadius();

	ivf::Vec3d& getEndVector();
	ivf::Vec3d& getStartVector();

	ivf::Vec3d& getEndPosition();
	ivf::Vec3d& getStartPosition();

	virtual void refresh();
};

#endif 
