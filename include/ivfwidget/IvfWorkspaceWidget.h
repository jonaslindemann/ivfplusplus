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

#ifndef _CIvfWorkplaceWidget_h_
#define _CIvfWorkplaceWidget_h_

#include <ivfwidget/IvfWidgetBase.h>

#include <ivf/IvfWorkspace.h>
#include <ivf/IvfCamera.h>
#include <ivf/IvfLight.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfConstructionPlane.h>
#include <ivf/IvfBufferSelection.h>

#include <ivf3dui/IvfUIInteractiveBase.h>

IvfSmartPointer(CIvfWorkspaceWidget);

#define MODE_VIEW    0
#define MODE_CREATE  1

/**
 * Workspace widget class
 *
 * This class implements a widget with workspace functionality.
 * A 3d cursor is also implemented and construction planes using
 * the CIvfWorkspace class. <br>
 * <br>
 * NOTE. Consider using event handlers instead of this class.
 */
class IVFWIDGET_API CIvfWorkspaceWidget : public CIvfWidgetBase {
public:
	enum TEditMode {
		EM_VIEW,
		EM_INTERACT,
		EM_CREATE_POINT,
        EM_CREATE_LINE,
        EM_CREATE_POLY
	};
private:
	CIvfCameraPtr		m_camera;
	CIvfWorkspacePtr	m_workspace;
	CIvfLightPtr		m_light;
    CIvfBufferSelectionPtr m_selection;

	CIvfShapePtr		m_lastOver;

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

	CIvfUIInteractiveBasePtr queryUIShape(CIvfShape* shape);
public:
	/** CIvfWorkspace constructor */
	CIvfWorkspaceWidget();

	/** CIvfWorkspace destructor */
	virtual ~CIvfWorkspaceWidget();

	IvfClassInfo("CIvfWorkspaceWidget", CIvfWidgetBase);

	/**
	 * Adds a child to the workspace
	 *
	 * This is really just a convinience class. If more 
	 * functionality is wanted use the getWorkspace()
	 * method to retrieved the CIvfWorkspace object.
	 */
	void addChild(CIvfShape* shape);

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

	/** Return CIvfWorkspace instance */
	CIvfWorkspace* getWorkspace();

	/** Return current camera */
	CIvfCamera* getCamera();

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

	virtual void onShapeDown(CIvfShape* shape);
	virtual void onShapeClick(CIvfShape* shape);
	virtual void onShapeUp(CIvfShape* shape);
	virtual void onShapeOver(CIvfShape* shape);
	virtual void onShapeLeave(CIvfShape* shape);
	virtual void onShapeDrag(CIvfShape* shape);

	virtual void onControlOver(CIvfUIInteractiveBase* uiControl);
	virtual void onControlLeave(CIvfUIInteractiveBase* uiControl);
	virtual void onControlDrag(CIvfUIInteractiveBase* uiControl);
	virtual void onControlUp(CIvfUIInteractiveBase* uiControl);
	virtual void onControlClick(CIvfUIInteractiveBase* uiControl);
	virtual void onControlDown(CIvfUIInteractiveBase* uiControl);
};

#endif 
