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

#ifndef _CIvfMat4d_h_
#define _CIvfMat4d_h_

#include <ivf/ivfconfig.h>
#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec4d.h>

/**
 * [4x4] Matrix class
 *
 * Implements a simple [4x4] matrix.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CMat4d : public CMathBase {
private:
	double m_matrix[4][4];
	int LU_Decompose  (double matrix[4][4], int* index);
	void LU_Backsub  (double matrix[4][4], int* index, double* B);
public:
	/** CMat4d constructor */
	CMat4d();

	/** CMat4d copy constructor */
	CMat4d(CMat4d& M);

	/** CMat4d scalar copy constructor */
	CMat4d(double scalar);

	/** CMat4d destructor */
	virtual ~CMat4d();

	/** Assigns all elements in matrix to one (=1.0) */
	void one();

	/** Assigns the matrix the identity matrix I */
	void identity();

	/** Zeros all element sin matrix */
	void zero();

	/** Create a translation matrix */
	void translate(double tx, double ty, double tz);

	/** 
	 * Create a rotation matrix
	 *
	 * Creates a rotation matrix that rotates a vector/point
	 * @param angle around the x-axis.
	 */
	void rotateX(double angle);

	/** 
	 * Create a rotation matrix
	 *
	 * Creates a rotation matrix that rotates a vector/point
	 * @param angle around the y-axis.
	 */
	void rotateY(double angle);

	/** 
	 * Create a rotation matrix
	 *
	 * Creates a rotation matrix that rotates a vector/point
	 * @param angle around the z-axis.
	 */
	void rotateZ(double angle);

	/** 
	 * Create a rotation matrix
	 *
	 * Creates a rotation matrix that rotates a vector/point
	 * @param angle around an arbitrary axis.
	 */
	void rotate(double vx, double vy, double vz, double theta);

	/** 
	 * Create a scaling matrix
	 *
	 * Creates a scaling matrix that scales a vector/point
	 */
	void scaling(double sx, double sy, double sz);

#ifdef IVF_HAVE_IOSTREAM
	/** Print matrix to a output stream */
	void print(std::ostream &out);
#else
	/** Print matrix to a output stream */
	void print(ostream &out);
#endif

	/** 
	 * Sets matrix component 
	 *
	 * Assigns matrix element (row, col) to value.
	 */
	void setComponent(int row, int col, double value);

	/** Returns value of matrix element (row, col) */
	double getComponent(int row, int col);

	/** Assign a column the values v1, v2, v3, v4 */
	void setColumn(int col, double v1, double v2, double v3, double v4);

	/** Assign a row the values v1, v2, v3 */
	void setRow(int row, double v1, double v2, double v3, double v4);

	/** Returns a transpose of the matrix */
	CMat4d& t();

	/** Returns the inverse of the matrix */
	CMat4d& inv();

	/** Assignment operator Matrix = Matrix */
	CMat4d& operator=(CMat4d& A);

	/** Assignment operator Matrix = scalar */
	CMat4d& operator=(double a);
	
	/** += operator assignment Matrix += Matrix */
	CMat4d& operator+=(CMat4d& A);
	
	/** -= operator assignment Matrix -= Matrix */
	CMat4d& operator-=(CMat4d& A);
	
	/** Matrix/Matrix multiplication operator */
	CMat4d& operator*(CMat4d& A);
	
	/** Matrix/Vector multiplication operator */
	CVec4d& operator*(CVec4d& v);
	
	/** Matrix/Vector multiplication operator */
	//CVec4d operator*(CVec4d& v);

	/** Matrix/Scalar multiplication operator */
	CMat4d& operator*(double scalar);
	
	/** Elemental division operator */
	CMat4d& operator/(double scalar);
	
	/** Matrix/Matrix addition operator */
	CMat4d& operator+(CMat4d& A);
};

IVFMATH_API CMat4d& ivfGetTempMatrix4d();

/** \example math.cpp */

#endif 
