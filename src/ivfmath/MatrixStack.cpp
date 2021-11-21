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

#include <ivfmath/MatrixStack.h>

using namespace ivf;

CMatrixStack::CMatrixStack()
{
	m_currentMatrix = 0;
	initialize();
}

void CMatrixStack::initialize()
{
	m_translateMatrix.identity();
	identity();
}

CMatrixStack::~CMatrixStack()
{

}

void CMatrixStack::translate(double x, double y, double z)
{
	m_translateMatrix.translate(x, y, z);
	m_matrixStack[m_currentMatrix] = 
		m_matrixStack[m_currentMatrix] * m_translateMatrix;
}

void CMatrixStack::rotate(double vx, double vy, double vz, double theta)
{
	m_rotateMatrix.rotate(vx, vy, vz, theta*2*M_PI/360.0);
	m_matrixStack[m_currentMatrix] = 
		m_matrixStack[m_currentMatrix] * m_rotateMatrix;
}

void CMatrixStack::pushMatrix()
{
	if (m_currentMatrix<IVF_MATRIX_STACK_SIZE)
	{
		m_currentMatrix++;
		m_matrixStack[m_currentMatrix] = m_matrixStack[m_currentMatrix-1];
	}
}

void CMatrixStack::popMatrix()
{
	if (m_currentMatrix!=0)
		m_currentMatrix--;
}

void CMatrixStack::identity()
{
	m_matrixStack[m_currentMatrix].identity();
}

void CMatrixStack::zero()
{
	m_matrixStack[m_currentMatrix].zero();
}

void CMatrixStack::printCurrent()
{
#ifdef IVF_HAVE_IOSTREAM
	m_matrixStack[m_currentMatrix].print(std::cout);
#else
	m_matrixStack[m_currentMatrix].print(cout);
#endif
}

void CMatrixStack::getWorldCoordinate(double lx, double ly, double lz, 
										 double &wx, double &wy, double &wz)
{
	CVec4d l;
	CVec4d w;
	CVec4d z(0.0,0.0,0.0);

	l.setComponents(lx, ly, lz, 1.0);
	w = m_matrixStack[m_currentMatrix] * l;
	w.getComponents(wx, wy, wz);
}


/*
void CMatrixStack::applyToVector(double *v)
{
	CVec4d l;
	CVec4d w;

	l.setComponents(lx, ly, lz, 1.0);
	w = m_matrixStack[m_currentMatrix] * l;
	w.getComponents(wx, wy, wz);

	ColVector<double> l(4,1.0);
	ColVector<double> w(4,0.0);

	l[0] = v[0];
	l[1] = v[1];
	l[2] = v[2];

	w = m_matrixStack[m_currentMatrix] * l;

	v[0] = w[0];
	v[1] = w[1];
	v[2] = w[2];
}
*/
