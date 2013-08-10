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

#include <ivfmath/IvfMat4d.h>

//
// From Stroustrup 11.6
//

const int ivfMaxTempMatrix = 16;

//
// LU_Decompose and LU_Backsub are from Steve Hollasch, Microsoft
//

int  CIvfMat4d::LU_Decompose  (double matrix[4][4], int* index)
{
#ifdef WIN32
    register int    i, j, k;		/* Loop Indices */
    register int    imax;		/* Maximum i Index */
    double big;		/* Non-Zero Largest Element */
    double sum;		/* Accumulator */
    double temp;		/* Scratch Variable */
    double vv[4];		/* Implicit Scaling of Each Row */
#else
    int    i, j, k;		/* Loop Indices */
    int    imax;		/* Maximum i Index */
    double big;		/* Non-Zero Largest Element */
    double sum;		/* Accumulator */
    double temp;		/* Scratch Variable */
    double vv[4];		/* Implicit Scaling of Each Row */
#endif

    for (i=0;  i < 4;  ++i)
    {   big = 0;
	for (j=0;  j < 4;  ++j)
	    if ((temp = fabs (matrix[i][j])) > big)
		big = temp;
	if (big == 0.0)
	    return 0;
	vv[i] = 1 / big;
    }

    for (j=0;  j < 4;  ++j)
    {   for (i=0;  i < j;  ++i)
	{   sum = matrix[i][j];
	    for (k=0;  k < i;  ++k)
		sum -= matrix[i][k] * matrix[k][j];
	    matrix[i][j] = sum;
	}

	big = 0;
	for (i=j;  i < 4;  ++i)
	{   sum = matrix[i][j];
	    for (k=0;  k < j;  ++k)
		sum -= matrix[i][k] * matrix[k][j];
	    matrix[i][j] = sum;
	    temp = vv[i] * fabs(sum);
	    if (temp >= big)
	    {   big = temp;
		imax = i;
	    }
	}

	if (j != imax)
	{   for (k=0;  k < 4;  ++k)
	    {   temp = matrix[imax][k];
		matrix[imax][k] = matrix[j][k];
		matrix[j][k] = temp;
	    }
	    vv[imax] = vv[j];
	}

	index[j] = imax;
	if (matrix[j][j] == 0.0)
	    return 0;
	
	if (j != (4-1))
	{   temp = 1 / matrix[j][j];
	    for (i=j+1;  i < 4;  ++i)
		matrix[i][j] *= temp;
	}
    }

    return 1;
}

void  CIvfMat4d::LU_Backsub  (double matrix[4][4], int* index, double* B)
{
#ifdef WIN32
    register int    i,j;
    int    ii;
    int    ip;
    double sum;
#else
    int    i,j;
    int    ii;
    int    ip;
    double sum;
#endif

    ii = -1;

    for (i=0;  i < 4;  ++i)
    {   ip = index[i];
	sum = B[ip];
	B[ip] = B[i];
	if (ii >= 0)
	    for (j=ii;  j <= i-1;  ++j)
		sum -= matrix[i][j] * B[j];
	else if (sum)
	    ii = i;
	B[i] = sum;
    }

    for (i=(4-1);  i >= 0;  --i)
    {   sum = B[i];
	for (j=i+1;  j < 4;  ++j)
	    sum -= matrix[i][j] * B[j];
	B[i] = sum / matrix[i][i];
    }
}

CIvfMat4d& ivfGetTempMatrix4d()
{
	static int nBuf = 0;
	static CIvfMat4d buf[ivfMaxTempMatrix];
	
	if (nBuf==ivfMaxTempMatrix) nBuf = 0;

	buf[nBuf].zero();

	return buf[nBuf++];
}


CIvfMat4d::CIvfMat4d()
{
	zero();	
}

CIvfMat4d::CIvfMat4d(CIvfMat4d& M)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = M.m_matrix[i][j];
}

CIvfMat4d::CIvfMat4d(double scalar)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = scalar;
}

CIvfMat4d::~CIvfMat4d()
{

}

void CIvfMat4d::zero()
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = 0.0;
}

void CIvfMat4d::identity()
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
		{
			if (i==j)
				m_matrix[i][j] = 1.0;
			else
				m_matrix[i][j] = 0.0;
		}
}

void CIvfMat4d::one()
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = 1.0;
}

void CIvfMat4d::setRow(int row, double v1, double v2, double v3, double v4)
{
	if ((row>=1)&&(row<=4))
	{
		m_matrix[row-1][0] = v1;
		m_matrix[row-1][1] = v2;
		m_matrix[row-1][2] = v3;
		m_matrix[row-1][3] = v4;
	}
}

void CIvfMat4d::setColumn(int col, double v1, double v2, double v3, double v4)
{
	if ((col>=1)&&(col<=4))
	{
		m_matrix[0][col-1] = v1;
		m_matrix[1][col-1] = v2;
		m_matrix[2][col-1] = v3;
		m_matrix[3][col-1] = v4;
	}
}

double CIvfMat4d::getComponent(int row, int col)
{
	if ((col>=0)&&(col<=4))
		if ((row>=0)&&(row<=4))
			return m_matrix[row][col];
		else
			return 0.0;
	else
		return 0.0;
}

void CIvfMat4d::setComponent(int row, int col, double value)
{
	if ((col>=0)&&(col<=4))
		if ((row>=0)&&(row<=4))
			m_matrix[row][col] = value;
}

#ifdef IVF_HAVE_IOSTREAM
void CIvfMat4d::print(std::ostream &out)
{
	int i, j;

	using namespace std;

	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			out << m_matrix[i][j] << " ";
		}
		out << endl;
	}
}
#else
void CIvfMat4d::print(ostream &out)
{
	int i, j;

	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			out << m_matrix[i][j] << " ";
		}
		out << endl;
	}
}
#endif


CIvfMat4d& CIvfMat4d::operator=(double a)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = a;

	return *this;
}

CIvfMat4d& CIvfMat4d::operator=(CIvfMat4d &A)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] = A.m_matrix[i][j];

	return *this;
}

CIvfMat4d& CIvfMat4d::operator+=(CIvfMat4d &A)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] += A.m_matrix[i][j];

	return *this;
}

CIvfMat4d& CIvfMat4d::operator-=(CIvfMat4d &A)
{
	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			m_matrix[i][j] -= A.m_matrix[i][j];

	return *this;
}

CIvfMat4d& CIvfMat4d::t()
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[j][i] = m_matrix[i][j];

	return R;
}

CIvfMat4d& CIvfMat4d::operator*(CIvfMat4d &A)
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	int i, j, k;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			for (k=0; k<4; k++)
				R.m_matrix[i][j] += m_matrix[i][k]*A.m_matrix[k][j];

	return R;
}

CIvfVec4d& CIvfMat4d::operator*(CIvfVec4d &v)
{
	CIvfVec4d& r = ivfGetTempVec4d();

	double vector[4];

	vector[0] = 0.0;
	vector[1] = 0.0;
	vector[2] = 0.0;
	vector[3] = 0.0;

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			vector[i] += m_matrix[i][j]*v[j];

	r.setComponents(vector);

	return r;
}

/*
CIvfVec4d CIvfMat4d::operator*(CIvfVec4d &v)
{
	CIvfVec4d r;

	double vector[4];

	vector[0] = 0.0;
	vector[1] = 0.0;
	vector[2] = 0.0;
	vector[3] = 0.0;

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			vector[i] += m_matrix[i][j]*v[j];

	r.setComponents(vector);

	return r;
}
*/

CIvfMat4d& CIvfMat4d::operator *(double scalar)
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[i][j] = m_matrix[i][j]*scalar;

	return R;
}

CIvfMat4d& CIvfMat4d::operator /(double scalar)
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[i][j] = m_matrix[i][j]/scalar;

	return R;
}

CIvfMat4d& CIvfMat4d::operator +(CIvfMat4d &A)
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	int i, j;

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[i][j] = m_matrix[i][j] + A.m_matrix[i][j];

	return R;
}

CIvfMat4d& CIvfMat4d::inv()
{
	CIvfMat4d& R = ivfGetTempMatrix4d();

	//
	// Code from Steve Hollasch, Microsoft
	//
#ifdef WIN32
    register int         i,j;
    int         index[4];
    double      column[4];
    double		 inverse[4][4];
#else
    int         i,j;
    int         index[4];
    double      column[4];
    double		 inverse[4][4];
#endif

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[i][j] = m_matrix[i][j];

    if (!LU_Decompose (R.m_matrix, index))
    {   
		printf ("InvertMatrix:  Singular matrix.\n");
		return R;
    }

    for (j=0;  j < 4;  ++j)
    {   
		for (i=0;  i < 4;  ++i)
			column[i] = 0;
		column[j] = 1;
		LU_Backsub (R.m_matrix, index, column);
		for (i=0;  i < 4;  ++i)
			inverse[i][j] = column[i];
    }

	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			R.m_matrix[i][j] = inverse[i][j];

	return R;
}

void CIvfMat4d::translate(double tx, double ty, double tz)
{
	zero();
	m_matrix[0][0] = 1.0;
	m_matrix[1][1] = 1.0;
	m_matrix[2][2] = 1.0;
	m_matrix[3][3] = 1.0;
	m_matrix[0][3] = tx;
	m_matrix[1][3] = ty;
	m_matrix[2][3] = tz;
}

void CIvfMat4d::rotateX(double angle)
{
	double s, c;

	s = sin(angle);
	c = cos(angle);

	m_matrix[0][0] = 1.0;
	m_matrix[3][3] = 1.0;

	m_matrix[1][1] = c;
	m_matrix[1][2] = -s;
	m_matrix[2][1] = s;
	m_matrix[2][2] = c;
}

void CIvfMat4d::rotateY(double angle)
{
	double s, c;
	
	s = sin(angle);
	c = cos(angle);

	m_matrix[1][1] = 1.0;
	m_matrix[3][3] = 1.0;

	m_matrix[0][0] = c;
	m_matrix[0][2] = s;
	m_matrix[2][0] = -s;
	m_matrix[2][2] = c;
}

void CIvfMat4d::rotateZ(double angle)
{
	double s, c;
	
	s = sin(angle);
	c = cos(angle);

	m_matrix[2][2] = 1.0;
	m_matrix[3][3] = 1.0;

	m_matrix[0][0] = c;
	m_matrix[0][1] = -s;
	m_matrix[1][0] = s;
	m_matrix[1][1] = c;
}

void CIvfMat4d::scaling(double sx, double sy, double sz)
{
	m_matrix[3][3] = 1.0;
	m_matrix[0][0] = sx;
	m_matrix[1][1] = sy;
	m_matrix[2][2] = sz;
}

/*
  Let s = sin(t), c = cos(t), c_1 = 1 - c, and precompute xy*c_1, xz*c_1, yz*c_1, xs, ys, and zs (for the sake of efficiency.) 

   [ P.x' ]   [ xx*c_1+c   xy*c_1-zs  xz*c_1+ys ]   [ P.x ]
   [ P.y' ] = [ xy*c_1+zs  yy*c_1+c   yz*c_1-xs ] * [ P.y ]
   [ P.z' ]   [ xz*c_1-ys  yz*c_1+xs  zz*c_1+c  ]   [ P.z ]
*/

void CIvfMat4d::rotate(double vx, double vy, double vz, double theta)
{
	double s, c;
	double c_1;
	double xxc, yyc, zzc, xyc, xzc, yzc, xs, ys, zs;

	zero();

	s = sin(theta);
	c = cos(theta);

	c_1 = 1 - c;

	xxc = vx*vx*c_1;
	yyc = vy*vy*c_1;
	zzc = vz*vz*c_1;
	xyc = vx*vy*c_1;
	xzc = vx*vz*c_1;
	yzc = vy*vz*c_1;
	xs  = vx*s;
	ys  = vy*s;
	zs  = vz*s;

	m_matrix[3][3] = 1.0;

	m_matrix[0][0] = xxc + c;
	m_matrix[0][1] = xyc - zs;
	m_matrix[0][2] = xzc + ys;
	m_matrix[1][0] = xyc + zs;
	m_matrix[1][1] = yyc + c;
	m_matrix[1][2] = yzc - xs;
	m_matrix[2][0] = xzc - ys;
	m_matrix[2][1] = yzc + xs;
	m_matrix[2][2] = zzc + c;
}
