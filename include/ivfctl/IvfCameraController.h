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

#ifndef _CIvfCameraController_h_
#define _CIvfCameraController_h_

#include <ivfctl/IvfController.h>

#include <ivf/IvfCamera.h>
#include <ivfmath/IvfSpline3d.h>

IvfSmartPointer(CIvfCameraController);

/**
 * Camera controller
 *
 * Controller for controlling a camera along both a position path and
 * target path.
 */
class IVFCTL_API CIvfCameraController : public CIvfController {
public:
	enum TEndAction {
		EA_STOP,
		EA_REVERSE,
		EA_RESET,
		EA_DEACTIVATE
	};
	enum TStartAction {
		SA_STOP,
		SA_REVERSE,
		SA_RESET,
		SA_DEACTIVATE
	};
private:
	CIvfSpline3dPtr m_path;
	CIvfSpline3dPtr m_targetPath;
	
	double m_v0;
	double m_a0;
	double m_t0;
	double m_t;
	double m_v;

	double m_target_v0;
	double m_target_a0;
	double m_target_t0;
	double m_target_t;
	double m_target_v;

	bool m_useTargetPath;

	bool m_followPath;

	TEndAction m_endAction;
	TStartAction m_startAction;

	TEndAction m_targetEndAction;
	TStartAction m_targetStartAction;

	CIvfCameraPtr m_camera;
public:
	/** CIvfCameraController constructor */
	CIvfCameraController();

	/** CIvfCameraController destructor */
	virtual ~CIvfCameraController();

	/** Set path controlling camera position */
	void setPath(CIvfSpline3d* spline);

	/** Return path controlling camera position */
	CIvfSpline3d* getPath();

	/** 
	 * Set path controlling camera target 
	 *
	 * If target path is not set the camera target is used.
	 */
	void setTargetPath(CIvfSpline3d* spline);

	/** Return path controlling camera target */
	CIvfSpline3d* getTargetPath();
	
	/** Set camera to be controller */
	void setCamera(CIvfCamera* camera);

	/** Return controlled camera */
	CIvfCamera* getCamera();

	/** 
	 * Set initial position on curve
	 *
	 * \param t distance from curve start 0 <= t <= getSize()-1
	 */
	void setInitialPos(double t);

	/** 
	 * Set initial speed on curve
	 *
	 * \param v speed in units/second
	 */
	void setInitialSpeed(double v);

	/** 
	 * Set initial acceleration on curve
	 *
	 * \param a acceleration in units/second*second
	 */
	void setInitialAcceleration(double a);

	/** 
	 * Define what to do when returning to start position
	 *
	 * \param action SA_STOP stops movement, SA_REVERSE reverses 
	 * direction of movement, SA_RESET resets position to initial position
	 * SA_DEACTIVATE deactivates controller
	 */
	void setStartActionType(TStartAction action);

	/** 
	 * Define what to do when arriving at end position
	 *
	 * \param action EA_STOP stops movement, EA_REVERSE reverses 
	 * direction of movement, EA_RESET resets position to initial position
	 * EA_DEACTIVATE deactivates controller
	 */
	void setEndActionType(TEndAction action);

	/** 
	 * Set initial position on curve
	 *
	 * \param t distance from curve start 0 <= t <= getSize()-1
	 */
	void setInitialTargetAcceleration(double a);

	/** 
	 * Set initial speed on curve
	 *
	 * \param v speed in units/second
	 */
	void setInitialTargetSpeed(double v);

	/** 
	 * Set initial acceleration on curve
	 *
	 * \param a acceleration in units/second*second
	 */
	void setInitialTargetPos(double t);

	/** Enable/Disable use of target path */
	void setUseTargetPath(bool flag);

	/** Returns true if target path is used */
	bool getUseTargetPath();

	/** 
	 * Set follow path flag
	 *
	 * This flag determines if the camera target should follow the 
	 * path. If false the target from the initial camera object is
	 * used.
	 */
	void setFollowPath(bool flag);

	/** 
	 * Define what to do when arriving at end position
	 *
	 * \param action EA_STOP stops movement, EA_REVERSE reverses 
	 * direction of movement, EA_RESET resets position to initial position
	 * EA_DEACTIVATE deactivates controller
	 */
	void setTargetEndActionType(TEndAction action);

	/** 
	 * Define what to do when returning to start position
	 *
	 * \param action SA_STOP stops movement, SA_REVERSE reverses 
	 * direction of movement, SA_RESET resets position to initial position
	 * SA_DEACTIVATE deactivates controller
	 */
	void setTargetStartActionType(TStartAction action);

	virtual void doUpdate(double dt);
	virtual void doReset();
};

#endif 
