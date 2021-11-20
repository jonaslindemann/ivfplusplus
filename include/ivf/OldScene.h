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

#pragma once

#include <ivf/config.h>

#include <ivf/SelectComposite.h>
#include <ivf/CoordinateSystem.h>
#include <ivf/Camera.h>
#include <ivf/Shape.h>
#include <ivf/Cursor.h>
#include <ivf/Lighting.h>

#include <ivfmath/Point3d.h>

namespace ivf {

IvfSmartPointer(COldScene);

#define IVF_TEMP_SOLID 0
#define IVF_TEMP_WIREFRAME 1

#define IVF_LIGHT_LOCAL 0
#define IVF_LIGHT_WORLD 1

/**
 * Scene class
 * 
 * This class is used to manage a scene in Ivf++. 
 * The class handles low-level cursor movement, light management,
 * and selection. 
 * @author Jonas Lindemann
 */
class IVF_API COldScene : public CSelectComposite {
public:
	/** Constructor */
	COldScene ();

	/** Destructor */
	virtual ~COldScene ();

	IvfClassInfo("COldScene",CSelectComposite);

	void setCamera(CCamera * camera);
	CCamera* getCamera();
	
	CView* getView();
	void setView(CView* view);

	/** 
	 * Set current coordinate system object
	 *
	 * The coordinate system object is used to calculate
	 * the position of the 3D cursor.
	 * If the coordiate system object is not referenced
	 * by any other objects it will be deleted by IvfScene.
	 */
	void setWorldSystem(CCoordinateSystem* world);

	/** Return coordinate system object */
	CCoordinateSystem* getWorldSystem();

	/**
	 * Set cursor object
	 *
	 * The cursor is a 3D representation of a mouse
	 * cursor. If the cursor is not referenced it will
	 * be deleted.
	 */
	void setCursor(CCursor* cursor);
	CCursor* getCursor();

	//changes Calle Lejdfors 990803

	void setLightMode(int mode);
	int getLightMode();

	/** 
	 * Set temporary shape
	 *
	 * The temporary shape is not included in the 
	 * scene graph and can be used to draw outlines of 
	 * objects that is to be placed in the scene.
	 */
	void setTempShape(CShape* shape);

	/** Return temporary shape */
	CShape* getTempShape();

	/** 
	 * Set temporary drawing style
	 *
	 * The temporary object can be drawn in 
	 * to ways:
	 *
	 * \param IVF_TEMP_SOLID solid shape
	 * \param IVF_TEMP_WIRE wire frame representation
	 */
	void setTempRenderStyle(int style);

	/** Return temporary render style */
	int getTempRenderStyle();

	/** 
	 * Update 3D cursor
	 *
	 * Projects a ray from the viewing plane to the
	 * current coordinate system XY-plane and moves
	 * the 3D cursor to this position.
	 * @param x mouse x position
	 * @param y mouse y position
	 */
	void updateCursor(int x, int y);

	/** Not supported */
	void setNodeSize(double size);

	/** Not supported */
	void setObjectSize(double size);

	/** 
	 * Locks 3D cursor in XY-direction 
	 * 
	 * When this method is called the 3D cursor can only
	 * be moved in the Z-direction of the current coordinate
	 * system.
	 */
	void lockCursor();

	/** Unlocks the 3D cursor */
	void unlockCursor();

	/** Returns true if cursor is locked otherwise false */
	int isCursorLocked();

	/** Enable/disable grid snap */
	void setSnapToGrid(bool flag);

	/** Return grid snap state */
	bool getSnapToGrid();

	/** 
	 * Set the workspace size 
	 *
	 * Sets the size of the workspace and scales corresponding
	 * interaction objects.
	 */
	void setWorkspace(double size);

	/**
	 * Set transparent shape 
	 *
	 * The transparent shape is rendered last in the 
	 * rendering stage of the scene and is not contained
	 * in the scene graph.
	 */
	void setTransparentShape(CShape* shape);

	/** Enable/disable cursor */
	void setShowCursor(bool flag);

	/** Return cursor state */
	bool getShowCursor();

	/** Enable/disable grid */
	void setShowGrid(bool flag);

	/** Return grid state */
	bool getShowGrid();

private:
	CPoint3d m_pointOnPlane;
	double m_objectSize;
	int m_cursorStartY;
	int m_cursorStartX;
	int m_cursorY;
	int m_cursorX;
	int m_tempRenderStyle;
	bool m_showGrid;
	bool m_showCursor;
	bool m_cursorLocked;
	bool m_snapToGrid;
	CShape* m_tempShape;
	CShape* m_transparentShape;
	CCursor* m_cursor;
	CCoordinateSystem* m_world;
	CLighting* m_lighting;
	int m_lightMode;
	//end changes

protected:
	void doEndTransform();
	void doBeginTransform();
	void doCreateGeometry();
};
}