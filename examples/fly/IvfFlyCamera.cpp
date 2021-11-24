#include "IvfFlyCamera.h"

using namespace ivf;

FlyCamera::FlyCamera()
{
	setPosition(0.0, 0.0, 0.0);
	setDirection(0.0, 0.0, 1.0);
	setPerspective(45.0, 1.0, 200.0);
}

FlyCamera::~FlyCamera()
{
}

void FlyCamera::viewTransform()
{
	gluLookAt(
		m_position[0], m_position[1], m_position[2],
		m_position[0]+m_forward[0],m_position[1]+m_forward[1],m_position[2]+m_forward[2],
		m_up[0], m_up[1], m_up[2]
	);
}

void FlyCamera::projectionTransform()
{
	gluPerspective(m_fov, getAspect(), m_zNear, m_zFar);	
}

void FlyCamera::setPerspective(double fov, double zNear, double zFar)
{
	m_fov = fov;
	m_zNear = zNear;
	m_zFar = zFar;
}

void FlyCamera::setPosition(double x, double y, double z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
}

void FlyCamera::setDirection(double heading, double pitch, double bank)
{
	m_heading = heading;
	m_pitch = pitch;
	m_bank = bank;
	calcOrientation();
}

void FlyCamera::calcOrientation()
{
	double ex, ey, ez;

	double l = cos(m_pitch*2*M_PI*0.002777777777777);
	ez = -l*cos(m_heading*2*M_PI*0.002777777777777);
	ex =  l*sin(m_heading*2*M_PI*0.002777777777777);
	ey =   -sin(m_pitch*2*M_PI*0.002777777777777);

	m_forward.setComponents(ex, ey, ez);
	m_forward.normalize();

	m_sideways.setComponents(-ez, 0.0, ex);
	m_sideways.normalize();

	m_up = m_sideways * m_forward; // Cross product
	m_up.normalize();
}

void FlyCamera::addDirection(double dHeading, double dPitch, double dBank)
{
	m_heading += dHeading;
	m_pitch   += dPitch;
	m_bank    += dBank;
	calcOrientation();
}

void FlyCamera::moveForward(double d)
{
	m_position[0] += m_forward[0]*d;
	m_position[1] += m_forward[1]*d;
	m_position[2] += m_forward[2]*d;
}


void FlyCamera::turn(double dx, double dy, double dz)
{
	m_forward.rotate(m_up, -dy);
	m_sideways.rotate(m_up, -dy);

	m_forward.rotate(m_sideways, -dx);
	m_up.rotate(m_sideways, -dx);

	m_up.rotate(m_forward, -dz);
	m_sideways.rotate(m_forward, -dz);
}

void FlyCamera::getPosition(double &x, double &y, double &z)
{
	x = m_position[0];
	y = m_position[1];
	z = m_position[2];
}
