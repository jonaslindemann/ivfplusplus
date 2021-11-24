#ifndef _CIvfFlyCamera_h_
#define _CIvfFlyCamera_h_

#include <ivf/View.h>
#include <ivfmath/Vec3d.h>

IvfSmartPointer(FlyCamera);

class FlyCamera : public ivf::View {
private:
	double m_zFar;
	double m_zNear;
	double m_fov;

	double m_position[3];

	double m_heading;
	double m_pitch;
	double m_bank;

	ivf::Vec3d m_forward;
	ivf::Vec3d m_sideways;
	ivf::Vec3d m_up;
public:
	FlyCamera();
	virtual ~FlyCamera();

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
