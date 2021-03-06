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

#ifndef _CIvfSelectComposite_h_
#define _CIvfSelectComposite_h_

/**@#-*/
#include <ivf/ivfconfig.h>

namespace std {};
using namespace std;

#include <ivf/IvfComposite.h>
#include <ivf/IvfCoordinateSystem.h>
#include <ivf/IvfView.h>
#include <ivf/IvfShape.h>
#include <ivf/IvfCursor.h>

#include <ivfmath/IvfPoint3d.h>

IvfSmartPointer(CIvfSelectComposite);

/**
 * SelectComposite class
 * 
 * This class is used to handle selection in Ivf++. 
 * @author Jonas Lindemann
 */
class IVF_API CIvfSelectComposite : public CIvfComposite {
public:
	/** Constructor */
	CIvfSelectComposite ();

	/** Destructor */
	virtual ~CIvfSelectComposite ();

	IvfClassInfo("CIvfSelectComposite",CIvfComposite);

	/**
	 * Set current camera object
	 *
	 * The camera is used to calculate the intersection 
	 * of a ray starting at the viewing plane and ending
	 * at the XY-plane of the coordinate system object.
	 * If the camera object is not referenced it will 
	 * be deleted.
	 */
	void setCamera(CIvfView* camera);

	/** Returns the current camera object */
	CIvfView* getCamera();

	/** Returns selected shape (NULL if none) */
	CIvfShape* getSelectedShape();

	void setRenderCamera(bool flag);
	void setUseCustomTransform(bool flag);

	/**
	 * Initiate pick operation
	 *
	 * The pick operation selects any shapes at the 
	 * position of the mouse cursor. 
	 * @param x mouse x position
	 * @param y mouse y position
	 * @returns 0 if no shapes where selected
	 * >0 if shapes where selected.
	 */
	GLint pick(int x, int y);

	/** Add child to scene */
	void addChild(CIvfShape* shape);

	/** Removes a specific child from the scene */
	CIvfShape* removeChild(int index);

	/** Removes a specific shape from the scene */
	CIvfShape* removeShape(CIvfShape *removeShape);

	/** Deletes a specific shape from the scene */
	void deleteChild(int index);

	void renameChildren();
private:
	CIvfShape* m_selectedShape;
	int m_childCount;
	GLuint m_selectBuf[512];
	CIvfView* m_camera;
	vector<CIvfShape*> m_allObjects;
	bool m_useCustomTransform;
	bool m_renderCamera;

	void compositeRemove(CIvfComposite* composite, CIvfShape* deleteChild);
	void nameChildren(CIvfShape* shape);
protected:
	void processHits(GLint hits, GLuint buffer[]);
	virtual void doEndTransform();
	virtual void doBeginTransform();
	virtual void doCreateGeometry();
	virtual void initializeSelect(int x, int y, int w, int h);
	virtual void customView();
};
#endif

