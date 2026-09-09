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
#include <ivf/MeshBuffer.h>

namespace ivf {

IvfSmartPointer(Mesh);

/**
 * Mesh class
 *
 * Implements a Bezier patch over a grid of control points -- bilinear for
 * MT_ORDER_2 and bicubic for MT_ORDER_4.
 *
 * The fixed-function path evaluates the patch with glMap2d/glEvalMesh2. Those
 * evaluators do not exist in a core profile, so the modern path evaluates the
 * same patch on the CPU, at the same grid resolution and with the same analytic
 * normals GL_AUTO_NORMAL produced, and draws the result as an indexed triangle
 * mesh. Both paths are fed from the same control points and are meant to be
 * indistinguishable.
 */
class IVF_API Mesh : public Shape {
public:
	enum TMeshType {
		MT_ORDER_2,
		MT_ORDER_4
	};
	enum TMeshOrientation {
		MO_CW,
		MO_CCW
	};
private:
	GLdouble* m_controlPoints;
	int m_xSize;
	int m_ySize;
	bool m_initDone;
	TMeshType m_meshType;
	TMeshOrientation m_meshOrientation;
	int m_meshRows;
	int m_meshCols;

	MeshBuffer m_buffer;
	bool m_meshDirty;

	/** Evaluates the patch into m_buffer at the current grid resolution. */
	void updateGeometry();

	/** Surface point and analytic normal at (u, v), both in 0..1. */
	void evalPatch(double u, double v, glm::vec3 &position, glm::vec3 &normal) const;
public:
	/** Mesh constructor */
	Mesh();

	/** Mesh destructor */
	virtual ~Mesh();

	IvfClassInfo("Mesh",Shape);
	IvfStdFactory(Mesh);

	void createMesh(double width, double height);

	/** Set mesh size */
	void setSize(int xSize, int ySize);

	/** Return mesh size */
	void getSize(int &xSize, int &ySize);

	/** Set mesh control point */
	void setControlPoint(int i, int j, double x, double y, double z);

	/** Return mesh control point */
	void getControlPoint(int i, int j, double &x, double &y, double &z);

	/** 
	 * Mesh orientation
	 *
	 * Determines the orientation of the primitives generated.
	 * MO_CW = clockwise orientation MO_CCW = counter clockwise.
	 */
	void setMeshOrientation(TMeshOrientation meshOrientation);

	/** 
	 * Set mesh resolution 
	 * 
	 * Resolution of mesh when evaluating the surface mesh.
	 */
	void setMeshResolution(int rows, int cols);

	/**
	 * Set mesh type
	 *
	 * Set mesh function order. MT_ORDER_2 = linear
	 * MT_ORDER_4 = quadratic.
	 */
	void setMeshType(TMeshType meshType);

	/** Set position of control point */
	void setControlPoint(int i, int j, double y);
	virtual bool hasModernPath() override;
protected:
	void doCreateGeometry();
	void initialize();
};

}