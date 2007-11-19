#ifndef _CIvfFlyCamera_h_
#define _CIvfFlyCamera_h_

#include <ivf/IvfView.h>
#include <ivfmath/IvfVec3d.h>

IvfSmartPointer(CIvfFlyCamera);

class CIvfFlyCamera : public CIvfView {
private:
	double m_zFar;
	double m_zNear;
	double m_fov;

	double m_position[3];

	double m_heading;
	double m_pitch;
	double m_bank;

	CIvfVec3d m_forward;
	CIvfVec3d m_sideways;
	CIvfVec3d m_up;
public:
	CIvfFlyCamera();
	virtual ~CIvfFlyCamera();

	void addDirection(double dHeading, double dPitch, double dBank);
	void moveForward(double d);

	void getPosition(double &x, double &y, double &z);
	void turn(double dx, double dy, double dz);

	void setPerspective(double fov, double zNear, double zFar);
	void setDirection(double heading, double pitch, double bank);
	void setPosition(double x, double y, double z);
protected:
	void calcOrientation();
	virtual void projectionTransform();
	virtual void viewTransform();
};

#endif 
