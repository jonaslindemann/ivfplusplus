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

#include <ivf/config.h>
#include <ivfmath/MathBase.h>
#include <ivfmath/Vec3d.h>

namespace ivf {

/**
 * [3x3] Matrix class
 *
 * Implements a simple [3x3] matrix.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API Mat3d : public MathBase {
private:
	double m_matrix[3][3];
public:
	/** Mat3d constructor */
	Mat3d();

	/** Mat3d copy constructor */
	Mat3d(Mat3d& M);

	/** Mat3d scalar constructor */
	Mat3d(double scalar);

	/** Mat3d destructor */
	virtual ~Mat3d();

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
	Mat3d& t();

	/** Return the inverse of the matrix */
	Mat3d& inv();

	/** Assignment operator Matrix = Matrix */
	Mat3d& operator=(Mat3d& A);

	/** Assignment operator Matrix = scalar */
	Mat3d& operator=(double a);

	/** += operator assignment Matrix += Matrix */
	Mat3d& operator+=(Mat3d& A);

	/** -= operator assignment Matrix -= Matrix */
	Mat3d& operator-=(Mat3d& A);

	/** Matrix/Matrix multiplication operator */
	Mat3d& operator*(Mat3d& A);

	/** Matrix/Vector multiplication operator */
	Vec3d& operator*(Vec3d& v);

	/** Matrix/Scalar multiplication operator */
	Mat3d& operator*(double scalar);

	/** Elemental division operator */
	Mat3d& operator/(double scalar);

	/** Matrix/Matrix addition operator */
	Mat3d& operator+(Mat3d& A);
};

IVFMATH_API Mat3d& ivfGetTempMatrix3d();

/** \example math.cpp */

}