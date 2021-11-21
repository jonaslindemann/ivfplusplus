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

#pragma once

#include <ivfmath/MathBase.h>
#include <ivfmath/Mat4d.h>

namespace ivf {

#define IVF_MATRIX_STACK_SIZE 32

IvfSmartPointer(CMatrixStack);

/**
 * Matrix stack class
 *
 * Implements a matrix stack used to calculated the global 
 * coordinates for rendered objects. Initial matrix is set
 * to identity.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CMatrixStack : public CMathBase {
private:

	int m_currentMatrix;

	CMat4d m_matrixStack[IVF_MATRIX_STACK_SIZE];
	CMat4d m_translateMatrix;
	CMat4d m_rotateMatrix;

	void initialize();
public:
	/** CMatrixStack constructor */
	CMatrixStack();

	/** CMatrixStack destructor */
	virtual ~CMatrixStack();

	IvfClassInfo("CMatrixStack",CMathBase);
	IvfStdFactory(CMatrixStack);

	/** Pushes the current matrix on to the stack */
	void pushMatrix();

	/** Pops the stack and replaces the current matrix */
	void popMatrix();

	/** Multiplies the current matrix with a translation matrix */
	void translate(double x, double y, double z);

	/** Multiplies the current matrix with a rotation matrix */
	void rotate(double vx, double vy, double vz, double theta);

	/** Prints the current matrix to stdout */
	void printCurrent();

	/** Zeros the current matrix */
	void zero();

	/** Sets the current matrix to identity */
	void identity();

	/** Returns the world coordinat of the transformed local system */
	void getWorldCoordinate(double lx, double ly, double lz, 
		double &wx, double &wy, double &wz);

};

}