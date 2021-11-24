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

IvfSmartPointer(MouseViewHandler);

/**
 * Mouse view handler
 *
 * This class implements basic mouse view handling.
 * Default mode is view rotation using the left mouse
 * button. Panning is done using the left mouse button
 * Zooming is done using the right mouse button and the
 * [Shift] key.
 */
class IVFWIDGET_API MouseViewHandler : public HandlerBase,
	MouseDownEvent,
	MouseMoveEvent,
	MouseUpEvent,
	ResizeEvent
{
public:
	enum TKeyHandling {
		KH_NORMAL,
		KH_MIDDLE_BUTTON
	};
private:
	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	int m_width;
	int m_height;

	double m_panScalefactor;
	double m_zoomScalefactor;
	double m_rotateScalefactor;

	double m_autoScaleBias;

	bool m_autoScale;

	CameraPtr m_camera;
	WidgetBase* m_widget;

	TKeyHandling m_keyHandling;

	bool isPan();
	bool isZoom();
	bool isRotating();
	bool isZoomOrPan();
	void calcAutoScale();
public:
	/**
	 * Class constructor
	 *
	 * @param widget reference to WidgetBase class.
	 * @param camera reference to a CIvfCamera to be manipulated.
	 */
	MouseViewHandler(WidgetBase* widget, Camera* camera);

	/** Class destructor */
	virtual ~MouseViewHandler();

	IvfClassInfo("MouseViewHandler",Base);

	static MouseViewHandlerPtr create(WidgetBase* widget, Camera* camera)
	{
		return MouseViewHandlerPtr(new MouseViewHandler(widget, camera));
	}

	/**
	 * Set panning scale factor
	 *
	 * Panning is calculated by: <br><br>
	 * View movement X = mouse_delta_x * factor
	 * View movement Y = mouse_delta_y * factor
	 */
	void setPanScalefactor(double factor);

	/** Return panning scale factor */
	double getPanScalefactor();

	/**
	 * Set zoom scale factor
	 *
	 * Zoom is calculated by: <br><br>
	 * Forward/backward movement = mouse_delta * factor
	 */
	void setZoomScalefactor(double factor);

	/** Return zoom scale factor */
	double getZoomScalefactor();

	/**
	 * Enable/disable auto scaling
	 *
	 * The different scale factor are calculated as follows: <br> <br>
	 * pan_scale_factor = auto_scale_bias*eye_target_distance/screen_width;
	 * zoom_scale_factor = auto_scale_bias*eye_target_distance/screen_width;
	 * rotate_factor = 4.0/screen_width;
	 */
	void setAutoScale(bool flag);

	/** Return auto scale state */
	bool getAutoScale();

	/**
	 * Set autoscale bias factor
	 *
	 * A factor to modify the calculated autoscale.
	 */
	void setAutoScaleBias(double bias);

	/** Return auto scale bias factor */
	double getAutoScaleBias();

	/**
	 * Set type of key handling.
	 * @param keyHandling Default set to MouseViewHandler::KH_NORMAL.
	 * this mode uses left/right buttons and the [Shift] key.
	 * MouseViewHandler::KH_MIDDLE_BUTTON uses the middle button (wheel)
	 * to handle zooming
	 */
	void setKeyHandling(TKeyHandling keyHandling);

	/** Return key handling mode */
	TKeyHandling getKeyHandling();

	virtual void doResize(int width, int height);
	virtual void doMouseDown(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseUp(int x, int y);

	// Implemented events

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
	virtual void onResize(int width, int height);

};

}