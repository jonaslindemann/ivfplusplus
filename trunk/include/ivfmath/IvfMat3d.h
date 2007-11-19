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

#ifndef _CIvfMat3d_h_
#define _CIvfMat3d_h_

#include <ivf/ivfconfig.h>
#include <ivfmath/IvfMathBase.h>
#include <ivfmath/IvfVec3d.h>

/**
 * [3x3] Matrix class
 *
 * Implements a simple [3x3] matrix.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfMat3d : public CIvfMathBase {
private:
	double m_matrix[3][3];
public:
	/** CIvfMat3d constructor */
	CIvfMat3d();

	/** CIvfMat3d copy constructor */
	CIvfMat3d(CIvfMat3d& M);

	/** CIvfMat3d scalar constructor */
	CIvfMat3d(double scalar);

	/** CIvfMat3d destructor */
	virtual ~CIvfMat3d();

	/** Initializes matrix with all ones (=1.0) */
	void one();

	/** Initializes matrix to identity I */
	void identity();

	/** Zeros the entire matrix */
	void zero();

#ifdef IVF_HAVE_IOSTREAM
	/** Prints matrix to std output */
	void print(std::ostream &out);
#else
	/** Prints matrix to std output */
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

	/** Assign a column the values v1, v2, v3 */
	void setColumn(int col, double v1, double v2, double v3);

	/** Assign a row the values v1, v2, v3 */
	void setRow(int row, double v1, double v2, double v3);

	/** Return the transpose of the matrix */
	CIvfMat3d& t();

	/** Return the inverse of the matrix */
	CIvfMat3d& inv();

	/** Assignment operator Matrix = Matrix */
	CIvfMat3d& operator=(CIvfMat3d& A);

	/** Assignment operator Matrix = scalar */
	CIvfMat3d& operator=(double a);

	/** += operator assignment Matrix += Matrix */
	CIvfMat3d& operator+=(CIvfMat3d& A);

	/** -= operator assignment Matrix -= Matrix */
	CIvfMat3d& operator-=(CIvfMat3d& A);

	/** Matrix/Matrix multiplication operator */
	CIvfMat3d& operator*(CIvfMat3d& A);

	/** Matrix/Vector multiplication operator */
	CIvfVec3d& operator*(CIvfVec3d& v);

	/** Matrix/Scalar multiplication operator */
	CIvfMat3d& operator*(double scalar);

	/** Elemental division operator */
	CIvfMat3d& operator/(double scalar);

	/** Matrix/Matrix addition operator */
	CIvfMat3d& operator+(CIvfMat3d& A);
};

IVFMATH_API CIvfMat3d& ivfGetTempMatrix3d();

/** \example math.cpp */

#endif 
