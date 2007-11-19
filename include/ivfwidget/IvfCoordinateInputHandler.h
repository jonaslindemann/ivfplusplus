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

#ifndef _CIvfCoordinateInputHandler_h_
#define _CIvfCoordinateInputHandler_h_

#include <ivfwidget/IvfHandlerBase.h>
#include <ivfwidget/IvfWidgetBase.h>

#include <ivf/IvfScene.h>

#include <ivfmath/IvfUcs3d.h>

/**
 * 3D mouse down event class
 */
class IVFWIDGET_API CIvfMouseDown3dEvent : public CIvfEventBase 
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
class IVFWIDGET_API CIvfMouseMove3dEvent : public CIvfEventBase 
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
class IVFWIDGET_API CIvfMouseUp3dEvent : public CIvfEventBase 
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

IvfSmartPointer(CIvfCoordinateInputHandler);

/**
 * 3D Coordinate input handler
 *
 * This class implements 3d coordinate input handler. The
 * class is used together with CIvfWidgetBase derived classes,
 * such as CIvfFltkBase, CIvfWin32Window, CIvfMfcBase etc.
 * The class needs a reference to the base widget and a CIvfCamera
 * derived class. The class installs 3 handlers for mouse movement
 * in the widget class.
 */
class IVFWIDGET_API CIvfCoordinateInputHandler : public CIvfHandlerBase,
	CIvfMouseMoveEvent,
	CIvfMouseDownEvent,
	CIvfMouseUpEvent 
{
private:
	CIvfWidgetBase*		m_widget;
	CIvfCameraPtr		m_camera;
	CIvfUcs3dPtr		m_ucsXZ;
	CIvfUcs3dPtr		m_ucsXY;
	CIvfUcs3dPtr		m_ucsYZ;
	CIvfVec3d			m_intersection;
	CIvfVec3d			m_xzIntersection;
	CIvfVec3d			m_temp;

	bool				m_lockXZ;
	bool				m_snap;

	int					m_mousePos[2];

	std::vector<CIvfMouseDown3dEvent*> m_mouseDown3dEvents;
	std::vector<CIvfMouseMove3dEvent*> m_mouseMove3dEvents;
	std::vector<CIvfMouseUp3dEvent*> m_mouseUp3dEvents;

	CIvfVec3d& calcIntersection(int x, int y);
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
	 * @paran widget Reference to a CIvfWidgetBase class. In most cases
	 * this parameter can be set to this.
	 * @param camera CIvfCamera derived class for calulcating the intersection
	 * with the coordinate plane.
	 */
	CIvfCoordinateInputHandler(CIvfWidgetBase* widget, CIvfCamera* camera);

	/** Class destructor */
	virtual ~CIvfCoordinateInputHandler();

	IvfClassInfo("CIvfCoordinateInputHandler",CIvfHandlerBase);

	/** Locks cursor in XY direction (without producing an event) */
	void setLockXZNoEvent(bool flag);

	/** Set position of the coordinate input plane. */
	void setPlanePosition(double x, double y, double z);

	/** Set position of the coordinate input plane. */
	void setPlanePosition(CIvfVec3d& vec);

	/** Add mouse down event */
	void addMouseDown3dEvent(CIvfMouseDown3dEvent* event);

	/** Add mouse move event */
	void addMouseMove3dEvent(CIvfMouseMove3dEvent* event);

	/** Add mouse up event */
	void addMouseUp3dEvent(CIvfMouseUp3dEvent* event);

	/** Remove mouse down event */
	void removeMouseDown3dEvent(CIvfMouseDown3dEvent* event);

	/** Remove mouse move event */
	void removeMouseMove3dEvent(CIvfMouseMove3dEvent* event);

	/** Remove mouse up event */
	void removeMouseUp3dEvent(CIvfMouseUp3dEvent* event);

	/** Lock/unlock cursor in XZ plane */
	void setLockXZ(bool flag);

	/** Return cursor XZ lock state */
	bool getLockXZ();

	/** Return reference to current camera */
	CIvfCamera* getCamera();

	/** Return reference to widget */
	CIvfWidgetBase* getWidget();

	virtual void onMouseDown(int x, int y);
	virtual void onMouseMove(int x, int y);
	virtual void onMouseUp(int x, int y);
};
/** \example handlers.cpp
 * This is an example of how handler classes can be used.
 */

#endif
