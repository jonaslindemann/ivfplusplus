//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//

// Implementation of: public class CIvfCamera

#include <ivf/config.h>
#include <ivf/Camera.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

using namespace ivf;

/* accFrustum()
 * The first 6 arguments are identical to the glFrustum() call.
 *  
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accFrustum() calls glTranslatef().  You will 
 * probably want to insure that your ModelView matrix has been 
 * initialized to identity before calling accFrustum().
 */
void accFrustum(GLdouble left, GLdouble right, GLdouble bottom, 
   GLdouble top, GLdouble nnear, GLdouble ffar, GLdouble pixdx, 
   GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble xwsize, ywsize; 
   GLdouble dx, dy;
   GLint viewport[4];

   glGetIntegerv (GL_VIEWPORT, viewport);
	
   xwsize = right - left;
   ywsize = top - bottom;
	
   dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*nnear/focus);
   dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*nnear/focus);
	
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (left + dx, right + dx, bottom + dy, top + dy, nnear, ffar);
   //glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();
   //glTranslatef (-eyedx, -eyedy, 0.0);
}

/* accPerspective()
 * 
 * The first 4 arguments are identical to the gluPerspective() call.
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accPerspective() calls accFrustum().
 */
void accPerspective(GLdouble fovy, GLdouble aspect, 
   GLdouble nnear, GLdouble ffar, GLdouble pixdx, GLdouble pixdy, 
   GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble fov2,left,right,bottom,top;

   fov2 = ((fovy*M_PI) / 180.0) / 2.0;

   top = nnear / (cos(fov2) / sin(fov2));
   bottom = -top;

   right = top * aspect;
   left = -right;

   accFrustum (left, right, bottom, top, nnear, ffar,
               pixdx, pixdy, eyedx, eyedy, focus);
}

/* accPerspective()
 * 
 * The first 4 arguments are identical to the gluPerspective() call.
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accPerspective() calls accFrustum().
 */
void tilePerspective(GLdouble fovy, GLdouble aspect, 
   GLdouble nnear, GLdouble ffar, GLdouble pixdx, GLdouble pixdy, 
   GLdouble eyedx, GLdouble eyedy, GLdouble focus, GLdouble tileLeft, 
   GLdouble tileRight, GLdouble tileBottom, GLdouble tileTop)
{
   GLdouble fov2,left,right,bottom,top;

   fov2 = ((fovy*M_PI) / 180.0) / 2.0;

   top = nnear / (cos(fov2) / sin(fov2));
   bottom = -top;

   right = top * aspect;
   left = -right;

   accFrustum (right * tileLeft, right * tileRight, top * tileBottom, top * tileTop, nnear, ffar,
               pixdx, pixdy, eyedx, eyedy, focus);

   //accFrustum (left, right, bottom, top, nnear, ffar,
   //            pixdx, pixdy, eyedx, eyedy, focus);
}

// ------------------------------------------------------------
CCamera::CCamera ()
		:CView()
{
	m_position.setComponents(0.0, 0.0, -10.0);
	m_target.setComponents(0.0, 0.0, 0.0);
	setPerspective(45.0, 1.0, 50.0);

	m_freeOrientation = false;
	m_tileRendering = false;
	m_stereo = false;
	m_stereoEye = SE_LEFT;
	m_eyeSeparation = 1.0;

	m_cameraType = CT_NORMAL;

	m_jitter = false;
	m_jitterX = 0.0;
	m_jitterY = 0.0;

	m_tileLeft = -1.0;
	m_tileRight = 1.0;
	m_tileBottom = -1.0;
	m_tileTop = 1.0;
}

// ------------------------------------------------------------
CCamera::~CCamera ()
{

}

// ------------------------------------------------------------
void CCamera::setPosition(GLdouble x, GLdouble y, GLdouble z)
{
	m_position.setComponents(x, y, z);
	calcOrientation();
}

// ------------------------------------------------------------
void CCamera::setTarget(GLdouble x, GLdouble y, GLdouble z)
{
	m_target.setComponents(x, y, z);
	calcOrientation();
}

// ------------------------------------------------------------
void CCamera::getTarget(GLdouble & x, GLdouble & y, GLdouble & z)
{
	m_target.getComponents(x, y, z);
}

// ------------------------------------------------------------
void CCamera::getPosition(GLdouble & x, GLdouble & y, GLdouble & z)
{
	m_position.getComponents(x, y, z);
}

// ------------------------------------------------------------
void CCamera::calcOrientation()
{
	double cx, cy, cz;
	double ax, ay, az;

	if (m_cameraType==CT_NORMAL)
		m_forward.setFromPoints(m_position, m_target);

	m_forward.normalize();
	m_forward.getComponents(cx, cy, cz);

	m_sideways.setComponents(cz, 0.0, -cx);
	m_sideways.normalize();

	m_forward.getComponents(ax,ay,az);
	m_sideways.getComponents(cx,cy,cz);

	m_up = m_forward * m_sideways; // cross product

	/*
	m_up.setComponents(ay*cz-az*cy,az*cx-ax*cz,ax*cy-ay*cx);
	*/

	m_up.normalize();

}

// ------------------------------------------------------------
void CCamera::moveSideways(double d)
{
	if (m_cameraType==CT_NORMAL)
		calcOrientation();

	CVec3d direction;
	direction = m_sideways * d;
	m_position += direction;
	m_target += direction; 
}

// ------------------------------------------------------------
void CCamera::moveVertical(double d)
{
	if (m_cameraType==CT_NORMAL)
		calcOrientation();
	
	CVec3d direction;
	direction = m_up * d;
	m_position += direction;
	m_target += direction; 
}

// ------------------------------------------------------------
void CCamera::moveDepth(double d)
{
	double dx, dy, dz;
	double xt, yt, zt;
	double xp, yp, zp;
	double distance;

	calcOrientation();
	m_forward.getComponents(dx, dy, dz);

	m_position.getComponents(xp, yp, zp);
	m_target.getComponents(xt, yt, zt);

	distance = sqrt(pow(xp-xt,2) + pow(yp-yt,2) + pow(zp-zt,2));

	if (d<distance)
		m_position.add(dx * d, dy * d, dz * d);
	else
	{
		//m_position.add(dx * d, dy * d, dz * d);
		m_target.add(dx * d, dy * d, dz * d);
	}
}

// ------------------------------------------------------------
void CCamera::rotatePositionX(double angle)
{
	double px, py, pz;
	double tx, ty, tz;
	double dx, dy, dz;
	double u, r, alfa, beta;

	m_position.getComponents(px, py, pz);
	m_target.getComponents(tx, ty, tz);

	dx = px - tx;
	dy = py - ty;
	dz = pz - tz;

	r = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	u = sqrt(pow(dx,2) + pow(dz,2));
	alfa = atan2(dy,u);
	beta = atan2(dz,dx);

	if (alfa+angle>M_PI/2.0)
		angle = 0.0;

	if (alfa+angle<-M_PI/2.0)
		angle = 0.0;

	//std::cout << alfa << ", " << beta << std::endl;
	
	u  = r * cos(angle + alfa);

	dy = r * sin(angle + alfa);
	dx = u * cos(beta);
	dz = u * sin(beta);

	px = dx + tx;
	py = dy + ty;
	pz = dz + tz;

	calcOrientation();

	m_position.setComponents(px, py, pz);
}

// ------------------------------------------------------------
void CCamera::rotatePositionY(double angle)
{
	double px, py, pz;
	double tx, ty, tz;
	double dx, dy, dz;
	double u, r, alfa, beta;

	m_position.getComponents(px, py, pz);
	m_target.getComponents(tx, ty, tz);

	dx = px - tx;
	dy = py - ty;
	dz = pz - tz;

	r = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	u = sqrt(pow(dx,2) + pow(dz,2));
	alfa = atan2(dy,u);
	beta = atan2(dz,dx);
	
	u  = r * cos(alfa);

	dy = r * sin(alfa);
	dx = u * cos(angle + beta);
	dz = u * sin(angle + beta);

	px = dx + tx;
	py = dy + ty;
	pz = dz + tz;
	
	calcOrientation();

	m_position.setComponents(px, py, pz);
}

// ------------------------------------------------------------
void CCamera::getForwardVector(double & vx, double & vy, double & vz)
{
	m_forward.getComponents(vx,vy,vz);	
}

// ------------------------------------------------------------
void CCamera::pickVector(int sx, int sy, double & vx, double & vy, double & vz)
{
	double xx, yy;
	double px, py, pz;
	double cx, cy, cz;
	double viewWidth, viewHeight;
	int x, y, w, h;
	double fov;

	CVec3d  planePoint;
	CVec3d  pickVector;

	fov = m_fov * 2.0 * M_PI / 360.0;

	viewWidth = 2 * tan (fov/2.0) * getAspect();
	viewHeight = 2 * tan (fov/2.0);

	getViewPort(x, y, w, h);

	xx = ((double)(sx-x) * viewWidth / w) - viewWidth / 2.0;
	yy = ((double)(sy-y) * viewHeight / h) - viewHeight / 2.0;

	m_position.getComponents(cx, cy, cz);
	planePoint.setComponents(cx, cy, cz);

	m_forward.getComponents(px, py, pz);
	planePoint.add(px, py, pz);

	m_sideways.getComponents(px, py, pz);
	planePoint.add( -xx * px, -xx * py, -xx * pz );

	m_up.getComponents(px, py, pz);
	planePoint.add( -yy * px, -yy * py, -yy * pz );

	planePoint.getComponents(px, py, pz);
	pickVector.setComponents(px-cx, py-cy, pz-cz);
	pickVector.getComponents(vx, vy, vz);
}

// ------------------------------------------------------------
void CCamera::setPerspective(double fov, double zNear, double zFar)
{
	m_fov = fov;
	m_zNear = zNear;
	m_zFar = zFar;
}

// ------------------------------------------------------------
void CCamera::getPerspective(double & fov, double & zNear, double & zFar)
{
	fov = m_fov;
	zNear = m_zNear;
	zFar = m_zFar;
}

// ------------------------------------------------------------
void CCamera::rotateAbsolute(double alfa, double beta)
{
	double px, py, pz;
	double tx, ty, tz;
	double dx, dy, dz;
	double u, r;

	m_position.getComponents(px, py, pz);
	m_target.getComponents(tx, ty, tz);

	dx = px - tx;
	dy = py - ty;
	dz = pz - tz;

	r = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	u = sqrt(pow(dx,2) + pow(dz,2));

	u  = r * cos(alfa);

	dy = r * sin(alfa);
	dx = u * cos(beta);
	dz = u * sin(beta);

	px = dx + tx;
	py = dy + ty;
	pz = dz + tz;

	m_position.setComponents(px, py, pz);
}

// ------------------------------------------------------------
void CCamera::getAbsoluteRotation(double &alfa, double &beta)
{
	double px, py, pz;
	double tx, ty, tz;
	double dx, dy, dz;
	double u, r;

	m_position.getComponents(px, py, pz);
	m_target.getComponents(tx, ty, tz);

	dx = px - tx;
	dy = py - ty;
	dz = pz - tz;

	r = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));
	u = sqrt(pow(dx,2) + pow(dz,2));
	alfa = atan2(dy,u);
	beta = atan2(dz,dx);
}

// ------------------------------------------------------------
void CCamera::getUpVector(double *v)
{
	m_up.getComponents(v[0],v[1],v[2]);	
}

// ------------------------------------------------------------
void CCamera::projectionTransform()
{
	if (m_stereo)
		stereoTransform();
	else
	{
		if (m_tileRendering)
		{
			// CHECK THIS CODE. Selection fails using the code below....
			if (!m_jitter)
				tilePerspective(m_fov, getAspect(), m_zNear, m_zFar, 
					0.0, 0.0, 0.0, 0.0, 1.0, 
					m_tileLeft, m_tileRight, m_tileBottom, m_tileTop);	
			else
				tilePerspective(m_fov, getAspect(), m_zNear, m_zFar, m_jitterX, m_jitterY, 
					0.0, 0.0, 0.1, 
					m_tileLeft, m_tileRight, m_tileBottom, m_tileTop);
		}
		else
		{
			if (!m_jitter)
				gluPerspective(m_fov, getAspect(), m_zNear, m_zFar);
			else
				accPerspective(m_fov, getAspect(), m_zNear, m_zFar, m_jitterX, m_jitterY, 0.0, 0.0, 0.1);
		}
	}
}

// ------------------------------------------------------------
void CCamera::viewTransform()
{
	GLdouble eyeX, eyeY, eyeZ;
	GLdouble centerX, centerY, centerZ;
	GLdouble upX, upY, upZ;

	if (m_stereo)
	{
		CVec3d pos;
		CVec3d target;

		pos = m_position;
		target = m_target;

		if (m_cameraType!=CT_NORMAL)
			target = m_position + m_forward;

		if (m_stereoEye == SE_LEFT)
		{
			pos = pos + m_sideways*(m_eyeSeparation/2.0);
			target = target + m_sideways*(m_eyeSeparation/2.0);
		}
		else
		{
			pos = pos - m_sideways*(m_eyeSeparation/2.0);
			target = target - m_sideways*(m_eyeSeparation/2.0);
		}

		pos.getComponents(eyeX, eyeY, eyeZ);
		target.getComponents(centerX, centerY, centerZ);
	}
	else
	{
		m_position.getComponents(eyeX, eyeY, eyeZ);
		
		if (m_cameraType==CT_NORMAL)
			m_target.getComponents(centerX, centerY, centerZ);
		else
		{
			CVec3d center = m_position + m_forward;
			center.getComponents(centerX, centerY, centerZ);
		}
	}
	
	m_up.getComponents(upX, upY, upZ);

	/*
	upX = 0.0;
	upY = 1.0;
	upZ = 0.0;
	*/

	//std::cout << upX << ", " << upY << ", " << upZ << std::endl;

	gluLookAt(eyeX, eyeY, eyeZ, 
		centerX, centerY, centerZ, 
		upX, upY, upZ);

}

// ------------------------------------------------------------
void CCamera::getViewFrustum(CViewFrustum *frustum)
{
	double x, y, z;
	double v1[3];
	double v2[3];
	double v3[3];
	double v4[3];
	double forward[3];
	double sideways[3];
	double up[3];
	double fov;
	double viewWidth;
	double viewHeight;

	// Get all planes

	CPlane* nearPlane   = frustum->getNearPlane();
	CPlane* farPlane    = frustum->getFarPlane();
	CPlane* leftPlane   = frustum->getLeftPlane();
	CPlane* rightPlane  = frustum->getRightPlane();
	CPlane* topPlane    = frustum->getTopPlane();
	CPlane* bottomPlane = frustum->getBottomPlane();

	// Get orientation vectors

	m_position.getComponents(x, y, z);
	m_forward.getComponents(forward[0], forward[1], forward[2]);
	m_sideways.getComponents(sideways[0], sideways[1], sideways[2]);
	m_up.getComponents(up[0], up[1], up[2]);

	// Calculate far plane

	farPlane->setNormal(forward[0], forward[1], forward[2]);
	farPlane->distanceFromPoint(x+forward[0]*m_zFar, y+forward[1]*m_zFar, z+forward[2]*m_zFar);

	// Calculate near plane

	nearPlane->setNormal(-forward[0], -forward[1], -forward[2]);
	nearPlane->distanceFromPoint(x+forward[0]*m_zNear, y+forward[1]*m_zNear, z+forward[2]*m_zNear);

	// Calculate other planes

	fov = m_fov * 2.0 * M_PI / 360.0;

	viewHeight = tan(fov/2.0);	
	viewWidth = viewHeight * getAspect();

	v1[0] = v2[0] = v3[0] = v4[0] = forward[0];
	v1[1] = v2[1] = v3[1] = v4[1] = forward[1];
	v1[2] = v2[2] = v3[2] = v4[2] = forward[2];

	v1[0] -= sideways[0]*viewWidth; v4[0] = v1[0];
	v1[1] -= sideways[1]*viewWidth; v4[1] = v1[1];
	v1[2] -= sideways[2]*viewWidth; v4[2] = v1[2];

	v2[0] += sideways[0]*viewWidth; v3[0] = v2[0];
	v2[1] += sideways[1]*viewWidth; v3[1] = v2[1];
	v2[2] += sideways[2]*viewWidth; v3[2] = v2[2];

	v1[0] += up[0]*viewHeight; 
	v1[1] += up[1]*viewHeight; 
	v1[2] += up[2]*viewHeight; 

	v2[0] += up[0]*viewHeight;
	v2[1] += up[1]*viewHeight;
	v2[2] += up[2]*viewHeight;

	v3[0] -= up[0]*viewHeight;
	v3[1] -= up[1]*viewHeight;
	v3[2] -= up[2]*viewHeight;

	v4[0] -= up[0]*viewHeight;
	v4[1] -= up[1]*viewHeight;
	v4[2] -= up[2]*viewHeight;

	topPlane->normalFromVectors(v1, v2);
	topPlane->distanceFromPoint(x, y, z);

	rightPlane->normalFromVectors(v2, v3);
	rightPlane->distanceFromPoint(x, y, z);

	bottomPlane->normalFromVectors(v3, v4);
	bottomPlane->distanceFromPoint(x, y, z);

	leftPlane->normalFromVectors(v4, v1);
	leftPlane->distanceFromPoint(x, y, z);
}


// ------------------------------------------------------------
void CCamera::moveAbsoluteDepth(double d)
{
	double dx, dy, dz;
	double xt, yt, zt;
	double xp, yp, zp;

	calcOrientation();
	m_forward.getComponents(dx, dy, dz);
	m_position.getComponents(xp, yp, zp);
	m_target.getComponents(xt, yt, zt);
	m_position.setComponents(xt - dx*d, yt - dy*d, zt - dz*d);
}

// ------------------------------------------------------------
CVec3d& CCamera::pickVector(int x, int y)
{
	double cx, cy, cz;
	CVec3d& v = ivfGetTempVec3d();

	pickVector(x, y, cx, cy, cz);
	v.setComponents(cx, cy, cz);

	return v;
}

// ------------------------------------------------------------
CVec3d& CCamera::getPosition()
{
	return m_position;
}

// ------------------------------------------------------------
void CCamera::setForwardVector(CVec3d &vec)
{
	m_forward = vec;
	m_forward.normalize();
	calcOrientation();
}

// Code based on:
// 3D Stereo Rendering Using OpenGL (and GLUT)
// By Paul Bourke
// http://astronomy.swin.edu.au/~pbourke/opengl/stereogl/

void CCamera::stereoTransform()
{

	double znear, zfar, fov;

	double DTOR = 0.0174532925;

	double left, right, top, bottom;
	double ratio = getAspect();
	
	CVec3d r;

	getPerspective(fov, znear, zfar);

	double radians = DTOR * fov / 2;
	double wd2     = znear * tan(radians);
	double ndfl    = znear / zfar;

	r = m_sideways;
	r.normalize();
	r = r * (m_eyeSeparation / 2.0);

	if (m_stereoEye == SE_LEFT)
	{
		left  = - ratio * wd2 - 0.5 * m_eyeSeparation * ndfl;
		right =   ratio * wd2 - 0.5 * m_eyeSeparation * ndfl;
	}
	else
	{
		left  = - ratio * wd2 + 0.5 * m_eyeSeparation * ndfl;
		right =   ratio * wd2 + 0.5 * m_eyeSeparation * ndfl;
	}
	
	top    =   wd2;
	bottom = - wd2;

    glFrustum(left,right,bottom,top,znear,zfar);
}

void CCamera::setStereo(bool flag)
{
	m_stereo = flag;
}

bool CCamera::getStereo()
{
	return m_stereo;
}

void CCamera::setStereoEye(TStereoEye eye)
{
	m_stereoEye = eye;
}

void CCamera::setEyeSeparation(double separation)
{
	m_eyeSeparation = separation;
}

double CCamera::getEyeSeparation()
{
	return m_eyeSeparation;
}

void CCamera::setType(TCameraType type)
{
	m_cameraType = type;
}

CCamera::TCameraType CCamera::getType()
{
	return m_cameraType;
}

void CCamera::turn(double dx, double dy, double dz)
{
	m_forward.rotate(m_up, -dy);
	m_sideways.rotate(m_up, -dy);

	m_forward.rotate(m_sideways, -dx);
	m_up.rotate(m_sideways, -dx);

	m_up.rotate(m_forward, -dz);
	m_sideways.rotate(m_forward, -dz);
}

void CCamera::moveForward(double d)
{
	m_position = m_position + m_forward*d;
}

CVec3d& CCamera::getTarget()
{
	return m_target;
}

CVec3d& CCamera::getForwardVector()
{
	return m_forward;
}

void CCamera::setTileRendering(bool flag)
{
	m_tileRendering = flag;
}

bool CCamera::getTileRendering()
{
	return m_tileRendering;
}

void CCamera::setJitter(bool flag)
{
	m_jitter = flag;
}

bool CCamera::getJitter()
{
	return m_jitter;
}

void CCamera::setJitterPixels(double dx, double dy)
{
	m_jitterX = dx;
	m_jitterY = dy;
}

void CCamera::getJitterPixels(double &dx, double &dy)
{
	dx = m_jitterX;
	dy = m_jitterY;
}

void CCamera::setTileRect(double left, double right, double bottom, double top)
{
	m_tileLeft = left;
	m_tileRight = right;
	m_tileBottom = bottom;
	m_tileTop = top;
}

void CCamera::getTileRect(double &left, double &right, double &bottom, double &top)
{
	left = m_tileLeft;
	right = m_tileRight;
	bottom = m_tileBottom;
	top = m_tileTop;
}
