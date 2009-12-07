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

#ifndef _CIvfWorkspace_h_
#define _CIvfWorkspace_h_

#include <ivf/IvfSceneBase.h>
#include <ivf/IvfConstructionPlane.h>

IvfSmartPointer(CIvfWorkspace);

/**
 * Workspace class
 *
 * The CIvfWorkspace class implements a complete scene
 * class with construction plane management. Use updateCursor()
 * and updateSizes() to update cursor and gui element sizes in 
 * response to mouse moves and camera changes.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfWorkspace : public CIvfSceneBase {
private:
	std::vector<CIvfConstructionPlane*> m_workplanes;
	CIvfConstructionPlane* m_currentPlane;
	CIvfConstructionPlane* m_rootPlane;
	int m_currentPlaneIdx;
	bool m_usePlanes;
	bool m_useCursor;
public:
	void disableCursor();
	void enableCursor();
	double getSize();
	bool getUseCursor();
	void setUseCursor(bool flag);
	/** CIvfWorkspace constructor */
	CIvfWorkspace();

	/** CIvfWorkspace destructor */
	virtual ~CIvfWorkspace();

	IvfClassInfo("CIvfWorkspace",CIvfSceneBase);

	/** Add a construction plane */
	void addPlane(CIvfConstructionPlane* plane);

	/** Clear all construction planes */
	void clearPlanes();

	/** 
	 * Update cursor on current construction plane 
	 * 
	 * This method should be called whenever the mouse is
	 * moved in the window. The (x, y) mouse position is then
	 * mapped to the current constructionplane.
	 */
	void updateCursor(int x, int y);

	/** 
	 * Update relative cursor and axis sizes 
	 *
	 * This method should be called when the camera view is moved
	 * closer or further away from the construction plane.
	 */
	void updateSizes();

	/** Set current construction plane */
	void setCurrentPlane(int idx);

	/** Switch to next construction plane */
	void nextPlane();

	/** Switch to previous construction plane */
	void prevPlane();

	/** Lock cursor in XZ plane */
	void lockCursor();

	/** Lock cursor in Y direction */
	void unlockCursor();

	/** Query cursor lock state */
	bool isCursorLocked();

	/** Set view used (required) */
	virtual void setView(CIvfView* view);

	/** Set size of workspace */
	void setWorkspaceSize(double size);

	/** Set relative axis size */
	void setRelativeAxisSize(double size);

	/** Set relative cursor size */
	void setRelativeCursorSize(double size);

	/** Return current construction plane */
	CIvfConstructionPlane* getCurrentPlane();

	/** Return global cursor position */
	CIvfVec3d& getCursorPosition();

	bool getUsePlanes();
	void setUsePlanes(bool flag);

	double getWorkspaceSize();

  	/** No descriptions */
	void setSize(double size);

protected:
	virtual void doCreateGeometry();
};

#endif 
