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

#include <ivfgle/IvfGleTwistExtrusion.h>

CGleTwistExtrusion::CGleTwistExtrusion()
{
	// Add construction code here
}

void CGleTwistExtrusion::doCreateGeometry()
{
	if (m_contour==NULL)
		return;


	if ((m_pointArray!=NULL)&&(m_twistArray!=NULL)&&(m_colorArray==NULL))
	{
		gleDouble (*temp1)[3];
		temp1 = (double (*)[3])m_pointArray->getData();

		gleDouble (*temp2)[2];
		temp2 = (double (*)[2])m_contour->getCoordData();

		gleDouble (*temp3)[2];
		temp3 = (double (*)[2])m_contour->getNormalData();

		gleTwistExtrusion(
			m_contour->getSize(),
			temp2,
			temp3,
			m_up,
			m_pointArray->getSize(),
			temp1,
			NULL,
			m_twistArray->getData()
		);
	}

	if ((m_pointArray!=NULL)&&(m_twistArray!=NULL)&&(m_colorArray!=NULL))
	{
		if (m_pointArray->getSize()==m_colorArray->getSize())
		{
			gleDouble (*temp1)[3];
			temp1 = (double (*)[3])m_pointArray->getData();

			gleDouble (*temp2)[2];
			temp2 = (double (*)[2])m_contour->getCoordData();

			gleDouble (*temp3)[2];
			temp3 = (double (*)[2])m_contour->getNormalData();

			float (*temp4)[3];
			temp4 = (float (*)[3])m_colorArray->getData();

			glEnable(GL_COLOR_MATERIAL);

			gleTwistExtrusion(
				m_contour->getSize(),
				temp2,
				temp3,
				m_up,
				m_pointArray->getSize(),
				temp1,
				temp4,
				m_twistArray->getData()
			);

			glDisable(GL_COLOR_MATERIAL);

		}
	}
}

void CGleTwistExtrusion::setPoints(CGleCoordArray *array)
{
	m_pointArray = array;
}

void CGleTwistExtrusion::setColors(CGleColorArray *array)
{
	m_colorArray = array;
}

void CGleTwistExtrusion::setContour(CGleContour *contour)
{
	m_contour = contour;
}

void CGleTwistExtrusion::setContourUp(double vx, double vy, double vz)
{
	m_up[0] = vx;
	m_up[1] = vy;
	m_up[2] = vz;
}

void CGleTwistExtrusion::setContourUp(CVec3d &vec)
{
	double vx, vy, vz;
	vec.getComponents(vx, vy, vz);
	m_up[0] = vx;
	m_up[1] = vy;
	m_up[2] = vz;

}

void CGleTwistExtrusion::setTwist(CGleScalarArray *array)
{
	m_twistArray = array;
}
