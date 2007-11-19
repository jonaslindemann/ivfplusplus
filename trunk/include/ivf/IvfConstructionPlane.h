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

#ifndef _CIvfConstructionPlane_h_
#define _CIvfConstructionPlane_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfGrid.h>
#include <ivf/IvfCursor.h>
#include <ivf/IvfCamera.h>

#include <ivfmath/IvfUcs3d.h>

IvfSmartPointer(CIvfConstructionPlane);

/**
 * Construction plane class
 *
 * This class implements a visual coordinate system plane, 
 * with a cursor and a grid. The cursor is allways aligned
 * with the defined coordinate system and can also be snapped
 * to a specified grid.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfConstructionPlane : public CIvfShape {
private:
	CIvfUcs3d* m_ucs;
	CIvfGrid* m_grid;
	CIvfCursor* m_cursor;
	CIvfCamera* m_camera;

	CIvfVec3d m_last;

	double m_size[2];
	bool m_cursorLocked;
	bool m_snapToGrid;
	double m_gridSpacing;

	double m_v[3];
	double m_theta;

	double m_cursorX;
	double m_cursorY;
	double m_cursorStartX;
	double m_cursorStartY;
	double m_relativeCursorSize;
	double m_relativeAxisSize;

	bool m_active;
	bool m_updateCursorSize;
	bool m_updateAxisSize;
	
	void initPlane();
	void updateAxisSize();
	void updateCursorSize();
public:
	/** CIvfConstructionPlane constructor */
	CIvfConstructionPlane();

	/** CIvfConstructionPlane destructor */
	virtual ~CIvfConstructionPlane();

	IvfClassInfo("CIvfConstructionPlane",CIvfShape);

	/** 
	 * Updates 3D cursor position
	 *
	 * This routine maps the 2D mouse coordinates to 3d coordinates
	 * used with the 3D cursor. The routine should be called in 
	 * response to mouse move events.
	 *
	 * @param x - x position of mouse in current viewport
	 * @param y - y position of mouse in current viewport
	 */
	void updateCursor(int x, int y);

	/**
	 * Updates sizes of axis and cursor objects
	 *
	 * To be able to allways see the cursor and axis objects, 
	 * the sizes can be automatically updated relative to the
	 * distance to the camera. This method should be called 
	 * whenever the camera is moved.
	 */ 
	void updateSizes();

	/** 
	 * Sets the construction plane in an active state
	 *
	 * In the active state the grid spacing, cursor and 
	 * axis are visible.
	 */
	void activate();

	/** 
	 * Sets the construction plane in an inactive state
	 *
	 * In the active state the grid spacing, cursor and 
	 * axis are invisible.
	 */
	void deactivate();

	/** Queries active state */
	bool isActive();

	/** 
	 * Locks cursor
	 *
	 * This method locks cursor movement in the local XZ plane.
	 * Enables movement in the local Y direction.
	 */
	void lockCursor();

	/**
	 * Unlocks cursor
	 *
	 * Movement in the XZ plane is enabled. Movement in the 
	 * local Y direction is disabled.
	 */
	void unlockCursor();

	/** Queries cursor lock state */
	bool isCursorLocked();

	void enableCursor();
	void disableCursor();
	bool isCursorEnabled();

	/** Set size of construction plane in the XZ plane */
	void setSize(double width, double height);

	/** 
	 * Sets the current camera (required)
	 *
	 * The camera is used to map the x/y mouse coordinates to 
	 * the coordinates on the construction plane. 
	 */
	void setCamera(CIvfCamera* camera);

	/**
	 * Set position of construction plane
	 *
	 * Sets the position of the construction plane and the
	 * local coordinate system used when calculating 3d 
	 * coordinates.
	 */
	virtual void setPosition(const double x, const double y, const double z);

	/**
	 * Set rotation of construction plane
	 *
	 * Sets the rotation of the construction plane and the
	 * local coordinate system used when calculating 3d 
	 * coordinates.
	 */
	void setRotationQuat(double vx, double vy, double vz, double theta);

	/** Set cursor used */
	void setCursor(CIvfCursor* cursor);

	/**
	 * Set relative axis size
	 *
	 * @param size - relative axis size calulatied as follows: 
	 * axis_size = camera_distance * size
	 */
	void setRelativeAxisSize(double size);

	/** Return relative axis size */
	double getRelativeAxisSize();

	/**
	 * Set relative cursor size
	 *
	 * @param size - relative cursor size calulatied as follows: 
	 * cursor_size = camera_distance * size
	 */
	void setRelativeCursorSize(double size);

	/** Return relative cursor size */
	double getRelativeCursorSize();

	/**
	 * Update cursor flag
	 *
	 * @param flag - true = cursor size is updated, when updateSizes() is called.
	 */
	void setUpdateCursorSize(bool flag);

	/**
	 * Update cursor flag
	 *
	 * @param flag - true = axis size is updated, when updateSizes() is called.
	 */
	void setUpdateAxisSize(bool flag);

	/** Returns the current cursor position in global coordinates */
	CIvfVec3d& getCursorPosition();

	/** Returns position of construction plane */
	void getPosition(double &x, double &y, double &z);

	/** Returns used defined coordinates system */
	CIvfUcs3d* getUcs();

	/** Sets grid spacing */
	void setGridSpacing(double spacing);

	/** Updates bounding sphere for construction plane */
	virtual void updateBoundingSphere();

	CIvfGrid* getGrid();
	double getDepth();
	double getWidth();
protected:
	virtual void createGeometry();
};

#endif 
