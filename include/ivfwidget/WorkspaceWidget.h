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

#include <ivfwidget/WidgetBase.h>

#include <ivf/Workspace.h>
#include <ivf/Camera.h>
#include <ivf/Light.h>
#include <ivf/Material.h>
#include <ivf/Sphere.h>
#include <ivf/ConstructionPlane.h>
#include <ivf/BufferSelection.h>

#include <ivf3dui/UIInteractiveBase.h>

namespace ivf {

IvfSmartPointer(WorkspaceWidget);

#define MODE_VIEW    0
#define MODE_CREATE  1

/**
 * Workspace widget class
 *
 * This class implements a widget with workspace functionality.
 * A 3d cursor is also implemented and construction planes using
 * the Workspace class. <br>
 * <br>
 * NOTE. Consider using event handlers instead of this class.
 */
class IVFWIDGET_API WorkspaceWidget : public WidgetBase {
public:
	enum TEditMode {
		EM_VIEW,
		EM_INTERACT,
		EM_CREATE_POINT,
        EM_CREATE_LINE,
        EM_CREATE_POLY
	};
private:
	CameraPtr		m_camera;
	WorkspacePtr	m_workspace;
	LightPtr		m_light;
    BufferSelectionPtr m_selection;

	ShapePtr		m_lastOver;

	int m_currentPlaneIdx;

	double m_angleX;
	double m_angleY;
	double m_moveX;
	double m_moveY;
	double m_zoomX;
	double m_zoomY;

	int m_beginX;
	int m_beginY;

	TEditMode m_editMode;

	bool m_selectOver;

	UIInteractiveBasePtr queryUIShape(Shape* shape);
public:
	/** Workspace constructor */
	WorkspaceWidget();

	/** Workspace destructor */
	virtual ~WorkspaceWidget();

	IvfClassInfo("WorkspaceWidget", WidgetBase);

	/**
	 * Adds a child to the workspace
	 *
	 * This is really just a convinience class. If more 
	 * functionality is wanted use the getWorkspace()
	 * method to retrieved the Workspace object.
	 */
	void addChild(Shape* shape);

	/**
	 * Sets the edit mode
	 *
	 * \param mode three modes are defined in this class
	 * EM_VIEW for viewing the workspace, EM_SELECT for selecting
	 * objects in the workspace and EM_CREATE for creating objects.
	 */
	void setEditMode(TEditMode mode);

	/** Return the current edit mode */
	TEditMode getEditMode();

	/** Return Workspace instance */
	Workspace* getWorkspace();

	/** Return current camera */
	Camera* getCamera();

	bool isShapeOverEnabled();
	void disableShapeOver();
	void enableShapeOver();

	virtual void doInit(int width, int height);
	virtual void doResize(int width, int height);
	virtual void doRender();
	virtual void doDestroy();
	virtual void doKeyboard(int key, int x, int y);
	virtual void doMouseDown(int x, int y);
	virtual void doMouseMove(int x, int y);
	virtual void doMouseUp(int x, int y);

	/** 
	 * 3D cursor event method
	 * 
	 * This method should be overridden in derived classes to 
	 * receive 3d cursor movement updates.
	 */
	virtual void onCursor(double x, double y, double z);

	virtual void onShapeDown(Shape* shape);
	virtual void onShapeClick(Shape* shape);
	virtual void onShapeUp(Shape* shape);
	virtual void onShapeOver(Shape* shape);
	virtual void onShapeLeave(Shape* shape);
	virtual void onShapeDrag(Shape* shape);

	virtual void onControlOver(UIInteractiveBase* uiControl);
	virtual void onControlLeave(UIInteractiveBase* uiControl);
	virtual void onControlDrag(UIInteractiveBase* uiControl);
	virtual void onControlUp(UIInteractiveBase* uiControl);
	virtual void onControlClick(UIInteractiveBase* uiControl);
	virtual void onControlDown(UIInteractiveBase* uiControl);
};

}