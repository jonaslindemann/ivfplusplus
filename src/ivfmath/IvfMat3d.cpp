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

#include <ivfmath/IvfMat3d.h>

//
// From Stroustrup 11.6
//

const int ivfMaxTempMatrix = 16;

CIvfMat3d& ivfGetTempMatrix3d()
{
	static int nBuf = 0;
	static CIvfMat3d buf[ivfMaxTempMatrix];
	
	if (nBuf==ivfMaxTempMatrix) nBuf = 0;

	buf[nBuf].zero();

	return buf[nBuf++];
}


CIvfMat3d::CIvfMat3d()
{
	zero();	
}

CIvfMat3d::CIvfMat3d(CIvfMat3d& M)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = M.m_matrix[i][j];
}

CIvfMat3d::CIvfMat3d(double scalar)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = scalar;
}

CIvfMat3d::~CIvfMat3d()
{

}

void CIvfMat3d::zero()
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = 0.0;
}

void CIvfMat3d::identity()
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
		{
			if (i==j)
				m_matrix[i][j] = 1.0;
			else
				m_matrix[i][j] = 0.0;
		}
}

void CIvfMat3d::one()
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = 1.0;
}

void CIvfMat3d::setRow(int row, double v1, double v2, double v3)
{
	if ((row>=1)&&(row<=3))
	{
		m_matrix[row-1][0] = v1;
		m_matrix[row-1][1] = v2;
		m_matrix[row-1][2] = v3;
	}
}

void CIvfMat3d::setColumn(int col, double v1, double v2, double v3)
{
	if ((col>=1)&&(col<=3))
	{
		m_matrix[0][col-1] = v1;
		m_matrix[1][col-1] = v2;
		m_matrix[2][col-1] = v3;
	}
}

double CIvfMat3d::getComponent(int row, int col)
{
	if ((col>=0)&&(col<=3))
		if ((row>=0)&&(row<=3))
			return m_matrix[row][col];
		else
			return 0.0;
	else
		return 0.0;
}

void CIvfMat3d::setComponent(int row, int col, double value)
{
	if ((col>=0)&&(col<=3))
		if ((row>=0)&&(row<=3))
			m_matrix[row][col] = value;
}

#ifdef IVF_HAVE_IOSTREAM
void CIvfMat3d::print(std::ostream &out)
{
	int i, j;

	using namespace std;

	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			out << m_matrix[i][j] << " ";
		}
		out << endl;
	}
}
#else
void CIvfMat3d::print(ostream &out)
{
	int i, j;

	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			out << m_matrix[i][j] << " ";
		}
		out << endl;
	}
}
#endif


CIvfMat3d& CIvfMat3d::operator=(double a)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = a;

	return *this;
}

CIvfMat3d& CIvfMat3d::operator=(CIvfMat3d &A)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] = A.m_matrix[i][j];

	return *this;
}

CIvfMat3d& CIvfMat3d::operator+=(CIvfMat3d &A)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] += A.m_matrix[i][j];

	return *this;
}

CIvfMat3d& CIvfMat3d::operator-=(CIvfMat3d &A)
{
	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			m_matrix[i][j] -= A.m_matrix[i][j];

	return *this;
}

CIvfMat3d& CIvfMat3d::t()
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			R.m_matrix[j][i] = m_matrix[i][j];

	return R;
}

CIvfMat3d& CIvfMat3d::operator*(CIvfMat3d &A)
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	int i, j, k;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			for (k=0; k<3; k++)
				R.m_matrix[i][j] += m_matrix[i][k]*A.m_matrix[k][j];

	return R;
}

CIvfVec3d& CIvfMat3d::operator*(CIvfVec3d &v)
{
	CIvfVec3d& r = ivfGetTempVec3d();

	double vector[3];

	vector[0] = 0.0;
	vector[1] = 0.0;
	vector[2] = 0.0;

	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			vector[i] += m_matrix[i][j]*v[j];

	r.setComponents(vector);

	return r;
}

CIvfMat3d& CIvfMat3d::operator *(double scalar)
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			R.m_matrix[i][j] = m_matrix[i][j]*scalar;

	return R;
}

CIvfMat3d& CIvfMat3d::operator /(double scalar)
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			R.m_matrix[i][j] = m_matrix[i][j]/scalar;

	return R;
}

CIvfMat3d& CIvfMat3d::operator +(CIvfMat3d &A)
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	int i, j;

	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			R.m_matrix[i][j] = m_matrix[i][j] + A.m_matrix[i][j];

	return R;
}

CIvfMat3d& CIvfMat3d::inv()
{
	CIvfMat3d& R = ivfGetTempMatrix3d();

	double idetA = 1.0/(
		m_matrix[0][0]*(m_matrix[1][1]*m_matrix[2][2]-m_matrix[1][2]*m_matrix[2][1]) +
		m_matrix[0][1]*(m_matrix[1][2]*m_matrix[2][0]-m_matrix[1][0]*m_matrix[2][2]) + 
		m_matrix[0][2]*(m_matrix[1][0]*m_matrix[2][1]-m_matrix[1][1]*m_matrix[2][0]));
	
	R.setRow(1, 
		idetA*(m_matrix[1][1]*m_matrix[2][2]-m_matrix[1][2]*m_matrix[2][1]),
		idetA*(m_matrix[0][2]*m_matrix[2][1]-m_matrix[0][1]*m_matrix[2][2]),
		idetA*(m_matrix[0][1]*m_matrix[1][2]-m_matrix[0][2]*m_matrix[1][1])
	);

	R.setRow(2, 
		idetA*(m_matrix[1][2]*m_matrix[2][0]-m_matrix[1][0]*m_matrix[2][2]),
		idetA*(m_matrix[0][0]*m_matrix[2][2]-m_matrix[0][2]*m_matrix[2][0]),
		idetA*(m_matrix[0][2]*m_matrix[1][0]-m_matrix[0][0]*m_matrix[1][2])
	);

	R.setRow(3, 
		idetA*(m_matrix[1][0]*m_matrix[2][1]-m_matrix[1][1]*m_matrix[2][0]),
		idetA*(m_matrix[0][1]*m_matrix[2][0]-m_matrix[0][0]*m_matrix[2][1]),
		idetA*(m_matrix[0][0]*m_matrix[1][1]-m_matrix[0][1]*m_matrix[1][0])
	);

	return R;
}
