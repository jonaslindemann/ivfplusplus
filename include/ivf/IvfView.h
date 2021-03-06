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

#ifndef _CIvfView_h_
#define _CIvfView_h_
 
#include <ivf/IvfGLBase.h>
#include <ivf/IvfViewport.h>

#include <ivfmath/IvfViewFrustum.h>

IvfSmartPointer(CIvfView);

/**
 * View abstract class
 * 
 * The IvfView class is an abstract class for implementing different
 * cameras. The viewTransform and projectionTransform methods
 * should be overidden. 
 */
class IVF_API CIvfView : public CIvfGLBase  {
private:
	int m_viewX;
	int m_viewY;
	int m_viewHeight;
	int m_viewWidth;
	double m_viewAspect;
	
	CIvfViewportPtr m_viewport;

	bool m_selectTransform;
public:
	void setViewport(CIvfViewport* viewport);
	CIvfViewport* getViewport();
	/** CIvfView constructor */
	CIvfView();

	/** CIvfView destructor */
	virtual ~CIvfView();

	IvfClassInfo("CIvfView",CIvfGLBase);

	/**
	 * Initialize perspective and viewport
	 *
	 * This method must be called before the camera can be used to
	 * view a scene. Typically it is called in a GLUT reshape callback.
	 */
	virtual void initialize();

	/**
	 * Initialize selection
	 * 
	 * This method is used to setup the pick matrix used for object
	 * selection in OpenGL based on the current camera view.
	 * @param x X coordinate for selection point (pixels).
	 * @param y Y coordinate for selection point (pixels).
	 * @param w Width of selection box.
	 * @param h Height of selection box.
	 */
	virtual void initializeSelect(int x, int y, int w, int h);

	/** 
	 * Set viewport dimension.
	 *
	 * The viewport is defined in screen pixels. Viewport
	 * dimension is used to calculate the view aspect ratio.
	 * @param width View width in pixels
	 * @param height View height in pixels
	 */
	void setViewPort(int width, int height);

	/**
	 * Return view frustum
	 *
	 * This method should modify the frustum parameter
	 * to reflect the currently used view frustum.
	 * Method is overidden by descendant classes.
	 */
	virtual void getViewFrustum(CIvfViewFrustum* frustum);

	/** Returns view port dimension */
	void getViewPort(int &w, int &h);

	/** Returns view port dimension */
	void setViewPort(int tx, int ty, int tw, int th);

	/** Returns view port dimension */
	void getViewPort(int &tx, int &ty, int &tw, int &th);

	/** Return current aspect ratio. */
	double getAspect();

	// Class query

	void setSelectTransform(bool flag);
protected:

	// virtual methods for view and perspective transforms

	void doBeginTransform();
	virtual void viewTransform();
	virtual void projectionTransform();
};

#endif 
