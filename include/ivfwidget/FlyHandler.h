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
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivf/Base.h>
#include <ivf/Camera.h>

#include <ivfwidget/WidgetBase.h>
#include <ivfwidget/HandlerBase.h>

namespace ivf {

IvfSmartPointer(FlyHandler);

/**
 * Fly view handler
 *
 * This handler implements a fly navigation mode that can
 * be used together with WidgetBase derived classes,
 * such as FltkBase, CIvfWin32Window, CIvfMfcBase etc.
 * The class take a reference to the active widget and a
 * CIvfCamera derived class.
 * <br><br>
 * Controls:
 * <br><br>
 * w - Increase velocity<br>
 * s - Decrease velocity<br>
 * x - Sets velocity to zero.<br>
 * q - Increase vertical velocity<br>
 * a - Decrease vertical velocity<br>
 */
class IVFWIDGET_API FlyHandler : public HandlerBase,
	KeyboardEvent,
	MouseUpEvent,
	MouseMoveEvent,
	MouseDownEvent
{
private:
	void doRedraw();
	int m_mouseDownPos[2];
	int	m_mouseDelta[2];
	double m_mouseSensitivity;
	double m_speed;
	double m_maxSpeed;
	double m_speedStep;
	double m_t;
	double m_dt;
	double m_verticalSpeed;
	double m_horisontalSpeed;

	bool m_firstFrame;
	bool m_doRedraw;

	CameraPtr m_camera;
	WidgetBase* m_widget;
public:
	/**
	 * Class constructor
	 *
	 * @paran widget Reference to a WidgetBase class. In most cases
	 * this parameter can be set to this.
	 * @param camera CIvfCamera derived class which will be modified.
	 */
	FlyHandler(WidgetBase* widget, Camera* camera);
	virtual ~FlyHandler();

	IvfClassInfo("FlyHandler",HandlerBase);

	static FlyHandlerPtr create(WidgetBase* widget, Camera* camera)
	{
		return FlyHandlerPtr(new FlyHandler(widget, camera));
	}

	/**
	 * View update routine
	 *
	 * The fly handler needs to be updated regularly.
	 * This routine updates the state variables and the camera
	 * depending on the elapsed time. Typically this routine
	 * should be called from a timer or an application loop.
	 */
	void update();

	/**
	 * Set mouse sensistivity
	 *
	 * The mouse sensitivity is used determine the turning/rotate
	 * speed when the mouse is moved:
	 * mouse_delta_x * sensitivity * dt
	 * mouse_delta_y * sensitivity * dt
	 */
	void setMouseSensitivity(double value);

	/** Return mouse sensitivity */
	double getMouseSensitivity();

	/**
	 * Set speed step
	 *
	 * The speed step value determines how much the speed increases
	 * or decreases when the user presses the forward/back button (W/S)
	 * to increase/decrease the forward velocity.
	 */
	void setSpeedStep(double step);

	/** Return current speed step value */
	double getSpeedStep();

	/** Set maximum speed */
	void setMaxSpeed(double speed);

	/** Return max speed */
	double getMaxSpeed();

	/**
	 * Set redraw handling
	 *
	 * The fly handler can handle screen redrawing automatically (flag=true)
	 * Sometimes it can be effective to control redrawing manually (flag=false)
	 */
	void setRedraw(bool flag);

	virtual void onKeyboard(int key, int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);

	void doKeyboard(int key, int x, int y);
	void doMouseUp(int x, int y);
	void doMouseMove(int x, int y);
	void doMouseDown(int x, int y);
protected:
	virtual void doDeactivate();
	virtual void doActivate();
};
/** \example fly.cpp
 * This is an example of how the FlyHandler can be used.
 */
/** \example handlers.cpp
 * This is an example of how handler HandlerBase derived
 * classes can be used.
 */

}
