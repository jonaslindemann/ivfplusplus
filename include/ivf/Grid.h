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

#include <ivf/Shape.h>
#include <ivf/LineSet.h>
#include <ivf/QuadSet.h>
#include <ivf/Axis.h>

#include <ivfmath/Point3d.h>

namespace ivf {

IvfSmartPointer(Grid);

/**
 * Grid class
 *
 * This class implements a grid used in conjunction with
 * the construction plane class @see CIvfConstructionPlane
 */
class IVF_API Grid : public Shape {
private:
	Point3d m_p1;
	Point3d m_p2;

	LineSet* m_corners;
	LineSet* m_outline;
	LineSet* m_gridLines;
	QuadSet* m_surface;
	Axis*	 m_axis;

	MaterialPtr m_surfaceMaterial;
	float m_cornerColor[4];
	float m_outlineColor[4];
	float m_majorColor[4];
	float m_minorColor[4];

	double m_gridSpacing;

	bool m_useSurface;
	bool m_useGrid;
	bool m_useOutline;
	bool m_useCorners;
	bool m_useAxis;
	bool m_useAxisLabels;

	int m_gridInterval;

	void initGrid();
	bool isRoughly(double x, double value);
public:
	/** CIvfGrid constructor */
	Grid();

	/** CIvfGrid destructor */
	virtual ~Grid();

	IvfClassInfo("Grid",Shape);
	IvfStdFactory(Grid)

	/** 
	 * Set axis state flag 
	 * 
	 * if @param flag is set to true an axis indicator
	 * is shown at local coordinate (0,0,0).
	 */
	void setUseAxis(bool flag);

	/** 
	 * Set grid state flag (true = visible) 
	 * 
	 * if @param flag is set to true a grid with the spacing
	 * set with setGridSpacing is visible 
	 */
	void setUseGrid(bool flag);

	/** 
	 * Set outline state flag (true = visible) 
	 *
	 * if @param flag is set to true an outline is drawn around
	 * the grid extents.
	 */
	void setUseOutline(bool flag);

	/**
	 * Set corners state flag (true = visible)
	 *
	 * if @param flag is set to true special corners are drawn 
	 * on the max/min grid positions.
	 */
	void setUseCorners(bool flag);

	/**
	 * Set surface state flag (true = visible)
	 *
	 * if @param flag is set to true a quad is drawn over the
	 * grid extents.
	 */
	void setUseSurface(bool flag);

	/** Set the grid spacing */
	void setGridSpacing(double spacing);

	/** Set size of grid */
	void setSize(double width, double height);

	/** Set size of axis indicator */
	void setAxisSize(double size);

	/** Updates grid geometry */
	virtual void refresh();

	/** Updates grid bounding box */
	virtual void doUpdateBoundingSphere();

	/** Returns axis shape */
	Axis* getAxisShape();

	/** Set drawing interval of grid lines */
	void setGridInterval(int interval);

	void setCornerColor(float red, float green, float blue, float alpha);
	void setOutlineColor(float red, float green, float blue, float alpha);
	void setSurfaceMaterial(Material* material);
	void setMajorColor(float red, float green, float blue, float alpha);
	void setMinorColor(float red, float green, float blue, float alpha);
protected:
	virtual void doCreateGeometry();
};

}