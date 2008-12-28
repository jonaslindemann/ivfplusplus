//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#ifndef _CIvfCamera_h_
#define _CIvfCamera_h_

#include <ivf/IvfView.h>
#include <ivf/IvfObject.h>

#include <ivfmath/IvfViewFrustum.h>
#include <ivfmath/IvfPoint3d.h>	
#include <ivfmath/IvfVec3d.h>

IvfSmartPointer(CIvfCamera);

/**
 * Camera/view class
 * 
 * CIvfCamera implements a virtual camera object supporting 
 * viewport definition, perspective transform, 
 * select functions and movement. To use the camera in OpenGL,
 * it must be rendered using the render() method, as the first 
 * object in the scene. See the following code sample.
 * 
 * \code
 * void DisplayFunc(void)
 * {
 *   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 *
 *   glPushMatrix();
 *
 *   camera->rotatePositionY(...);
 *   camera->rotatePositionX(...);
 *   camera->moveSideways(...);
 *   camera->moveVertical(...);
 *   camera->moveDepth(...);
 *   camera->render();
 *	
 *   // Render other objects
 *	
 *   xx->render();
 *   ...
 *   root->render();
 *   ...
 *
 *   glPopMatrix();
 *	   
 *   glutSwapBuffers();
 * }
 *
 * The camera must also be initialized before use this is often 
 * done in a GLUT reshape callback or a MFC OnSize event method.
 * The following code shows an example of a reshape callback.
 * 
 * void ReshapeFunc(int w, int h)
 * {
 *   camera->setPerspective(45.0, 0.1, 100.0);
 *   camera->setViewPort(w, h);
 *   camera->initialize();
 * }
 * \endcode
 * 
 * @author Jonas Lindemann
 */
class IVF_API CIvfCamera : public CIvfView {
public:
	enum TStereoEye {
		SE_LEFT,
		SE_RIGHT
	};
	enum TCameraType {
		CT_NORMAL,
		CT_FLY,
		CT_FREE_ORIENTATION
	};
private:
	double m_zFar;
	double m_zNear;
	double m_fov;

	double m_jitterX;
	double m_jitterY;

	double m_tileLeft;
	double m_tileRight;
	double m_tileBottom;
	double m_tileTop;

	TCameraType m_cameraType;

	bool m_freeOrientation;
	bool m_jitter;
	bool m_tileRendering;

	bool m_stereo;
	TStereoEye m_stereoEye;
	double m_eyeSeparation;

	CIvfVec3d m_up;
	CIvfVec3d m_sideways;
	CIvfVec3d m_forward;
	CIvfVec3d m_position;
	CIvfVec3d m_target;

public:
	/** CIvfCamera constructor. */
	CIvfCamera();

	/** CIvfCamera destructor */
	virtual ~CIvfCamera ();

	IvfClassInfo("CIvfCamera",CIvfView);

	/**
	 * Set camera position
	 * 
	 * Sets the camera position in the global coordinate system.
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	void setPosition(GLdouble x, GLdouble y, GLdouble z);

	/** Return camera position */
	void getPosition(GLdouble &x, GLdouble &y, GLdouble &z);
	
	/** Return camera position */
	CIvfVec3d& getPosition();

	/**
	 * Set camera target position
	 *
	 * The camera target defines the camera view direction.
	 * The position is defined in the global coordinate system.
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param z Z coordinate
	 */
	void setTarget(GLdouble x, GLdouble y, GLdouble z);

	/** Return camera target position */
	void getTarget(GLdouble &x, GLdouble &y, GLdouble &z);

	/** Returns camera up vector */
	void getUpVector(double* v);

	/**
	 * Set perspective parameters.
	 *
	 * CIvfCamera using the gluPerspective to create the perspective
	 * transform. This method uses the same parameters as this function. See GLU 
	 * documentation.
	 * @param fov The field of view angle, in degrees, in the y-direction. 
	 * @param zNear The distance from the viewer to the near clipping plane (always positive). 
	 * @param zFar The distance from the viewer to the far clipping plane (always positive). 
	 */
	void setPerspective(double fov, double zNear, double zFar);

	/** Return perspective parameters */
	void getPerspective(double &fov, double &zNear, double &zFar);

	/** 
	 * Return the current view frustum 
	 *
	 * The function does not instantiate a view frustum class, but
	 * updates the input frustum with the correct values for the 
	 * current camera view.
	 */
	void getViewFrustum(CIvfViewFrustum* frustum);

	/** 
	 * Retrieve pick vector
	 *
	 * Calculates a vector starting at the eye position and
	 * intersecting the camera plane.
	 * @param sx Viewport x coordinate.
	 * @param sy Viewport y coordinate.
	 * @param vx Pick vector x component
	 * @param vy Pick vector y component
	 * @param vz Pick vector z component
	 */
	void pickVector(int sx, int sy, double & vx, double & vy, double & vz);
	CIvfVec3d& pickVector(int x, int y);

	/**
	 * Return forward vector.
	 *
	 * Returns the normalized forward vector.
	 * @param vx Forward vector x component.
	 * @param vy Forward vector y component.
	 * @param vz Forward vector z component.
	 */
	void getForwardVector(double &vx, double &vy, double &vz);

	/**
	 * Rotate target point
	 * 
	 * Rotate target point around camera position. (Not yet implemented)
	 * @param angle Rotation angle around View X-axis.
	 */
	void rotateTargetY(double angle);

	/**
	 * Rotate target point
	 * 
	 * Rotate target point around camera position. (Not yet implemented)
	 * @param angle Rotation angle around View Y-axis.
	 */
	void rotateTargetX(double angle);

	/**
	 * Rotate camera position
	 * 
	 * Rotate camera point around target position. 
	 * @param angle Rotation angle around View Y-axis.
	 */
	void rotatePositionY(double angle);

	/**
	 * Rotate camera position
	 * 
	 * Rotate camera point around target position. 
	 * @param angle Rotation angle around View Y-axis.
	 */
	void rotatePositionX(double angle);
	
	/**
	 * Move camera forward/backwards.
	 * 
	 * Move camera position forward along the forward vector.
	 * @param d Distance to move camera. Positive forward.
	 */
	void moveDepth(double d);

	/** 
	 * Move camera vertical
	 *
	 * Move camera vertical parallel to view plane.
	 * @param d Distance to move camera. Positive up.
	 */
	void moveVertical(double d);

	/**
	 * Move camera sideways
	 *
	 * Move camera sideways parallel to view plane.
	 * @param d Distance to move camera. Positive right.
	 */
	void moveSideways(double d);

	/**
	 * Set camera forward vector
	 *
	 * This method is used when int CT_FLY or 
	 * CT_FREE_ORIENTATION mode, to set the forward camera 
	 * direction.
	 */
	void setForwardVector(CIvfVec3d& vec);

	void moveForward(double d);
	void turn(double dx, double dy, double dz);

	void setEyeSeparation(double separation);
	double getEyeSeparation();
	void setStereoEye(TStereoEye eye);

	void setStereo(bool flag);
	bool getStereo();
	
	void setType(TCameraType type);
	TCameraType getType();

	void setTileRendering(bool flag);
	bool getTileRendering();

	void setJitter(bool flag);
	bool getJitter();

	void setJitterPixels(double dx, double dy);
	void getJitterPixels(double &x, double &y);

	CIvfVec3d& getForwardVector();
	CIvfVec3d& getTarget();

	void setTileRect(double left, double right, double bottom, double top);
	void getTileRect(double &left, double &right, double &bottom, double &top);

	// Unsupported

	void getAbsoluteRotation(double &alfa, double &beta);
	void rotateAbsolute(double alfa, double beta);
	void moveAbsoluteDepth(double d);

protected:
	void calcOrientation();
	void projectionTransform();
	void viewTransform();
	void stereoTransform();
};
#endif
