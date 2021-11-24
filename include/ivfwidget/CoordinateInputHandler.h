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

#include <ivfwidget/HandlerBase.h>
#include <ivfwidget/WidgetBase.h>

#include <ivf/Scene.h>

#include <ivfmath/Ucs3d.h>

namespace ivf {

/**
 * 3D mouse down event class
 */
class IVFWIDGET_API MouseDown3dEvent : public EventBase 
{
public:
	/**
	 * onMouseDown3d method
	 *
	 * This method is called when the mouse is pressed,
	 * @param x position of 3d cursor.
	 * @param y position of 3d cursor.
	 * @param z position of 3d cursor.
	 */
	virtual void onMouseDown3d(double x, double y, double z) {};
};

/**
 * 3D mouse move event class
 */
class IVFWIDGET_API MouseMove3dEvent : public EventBase 
{
public:
	/**
	 * onMouseMove3d method
	 *
	 * This method is called when the mouse is moved,
	 * @param x position of 3d cursor.
	 * @param y position of 3d cursor.
	 * @param z position of 3d cursor.
	 */
	virtual void onMouseMove3d(double x, double y, double z) {};
};

/**
 * 3D mouse up event class
 */
class IVFWIDGET_API MouseUp3dEvent : public EventBase 
{
public:
	/**
	 * onMouseUp3d method
	 *
	 * This method is called when the mouse is released,
	 * @param x position of 3d cursor.
	 * @param y position of 3d cursor.
	 * @param z position of 3d cursor.
	 */
	virtual void onMouseUp3d(double x, double y, double z) {};
};

IvfSmartPointer(CoordinateInputHandler);

/**
 * 3D Coordinate input handler
 *
 * This class implements 3d coordinate input handler. The
 * class is used together with WidgetBase derived classes,
 * such as FltkBase, CIvfWin32Window, CIvfMfcBase etc.
 * The class needs a reference to the base widget and a CIvfCamera
 * derived class. The class installs 3 handlers for mouse movement
 * in the widget class.
 */
class IVFWIDGET_API CoordinateInputHandler : public HandlerBase,
	MouseMoveEvent,
	MouseDownEvent,
	MouseUpEvent 
{
private:
	WidgetBase*		m_widget;
	CameraPtr		m_camera;
	Ucs3dPtr		m_ucsXZ;
	Ucs3dPtr		m_ucsXY;
	Ucs3dPtr		m_ucsYZ;
	Vec3d			m_intersection;
	Vec3d			m_xzIntersection;
	Vec3d			m_temp;

	bool				m_lockXZ;
	bool				m_snap;

	int					m_mousePos[2];

	std::vector<MouseDown3dEvent*> m_mouseDown3dEvents;
	std::vector<MouseMove3dEvent*> m_mouseMove3dEvents;
	std::vector<MouseUp3dEvent*> m_mouseUp3dEvents;

	Vec3d& calcIntersection(int x, int y);
	void dispatchMouseDown3dEvent(double x, double y, double z);
	void dispatchMouseMove3dEvent(double x, double y, double z);
	void dispatchMouseUp3dEvent(double x, double y, double z);

	void doMouseUp(int x, int y);
	void doMouseMove(int x, int y);
	void doMouseDown(int x, int y);
	virtual void doLockXZ(bool flag);
public:
	/**
	 * Class constructor
	 *
	 * @paran widget Reference to a WidgetBase class. In most cases
	 * this parameter can be set to this.
	 * @param camera CIvfCamera derived class for calulcating the intersection
	 * with the coordinate plane.
	 */
	CoordinateInputHandler(WidgetBase* widget, Camera* camera);

	/** Class destructor */
	virtual ~CoordinateInputHandler();

	IvfClassInfo("CoordinateInputHandler",HandlerBase);

	/** Locks cursor in XY direction (without producing an event) */
	void setLockXZNoEvent(bool flag);

	/** Set position of the coordinate input plane. */
	void setPlanePosition(double x, double y, double z);

	/** Set position of the coordinate input plane. */
	void setPlanePosition(Vec3d& vec);

	/** Add mouse down event */
	void addMouseDown3dEvent(MouseDown3dEvent* event);

	/** Add mouse move event */
	void addMouseMove3dEvent(MouseMove3dEvent* event);

	/** Add mouse up event */
	void addMouseUp3dEvent(MouseUp3dEvent* event);

	/** Remove mouse down event */
	void removeMouseDown3dEvent(MouseDown3dEvent* event);

	/** Remove mouse move event */
	void removeMouseMove3dEvent(MouseMove3dEvent* event);

	/** Remove mouse up event */
	void removeMouseUp3dEvent(MouseUp3dEvent* event);

	/** Lock/unlock cursor in XZ plane */
	void setLockXZ(bool flag);

	/** Return cursor XZ lock state */
	bool getLockXZ();

	/** Return reference to current camera */
	Camera* getCamera();

	/** Return reference to widget */
	WidgetBase* getWidget();

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
};
/** \example handlers.cpp
 * This is an example of how handler classes can be used.
 */

}