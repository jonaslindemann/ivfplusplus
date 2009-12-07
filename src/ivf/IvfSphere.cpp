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

// Implementation of: public class CIvfSphere

#include <ivf/ivfconfig.h>
#include <ivf/IvfSphere.h>

// ------------------------------------------------------------
CIvfSphere::CIvfSphere ()
		:CIvfShape()
		//TODO: check and complete member initialisation list!
{
	m_radius = 1.0;
	m_slices = 8;
	m_stacks = 8;
	m_selectionBox = new CIvfSelectionBox();
	m_selectionBox->setUseName(false);
	m_qobj = gluNewQuadric();
	gluQuadricDrawStyle(m_qobj, GLU_FILL);
	gluQuadricTexture(m_qobj, GL_TRUE);
	gluQuadricOrientation(m_qobj, GLU_OUTSIDE);
	gluQuadricNormals(m_qobj, GLU_SMOOTH);
}

// ------------------------------------------------------------
CIvfSphere::~CIvfSphere ()
{
	gluDeleteQuadric(m_qobj);
	delete m_selectionBox;
}

// ------------------------------------------------------------
void CIvfSphere::setRadius (const double radius)
{
	m_radius = radius;
	updateSelectBox();
}

// ------------------------------------------------------------
double CIvfSphere::getRadius ()
{
	return m_radius;
}

// ------------------------------------------------------------
void CIvfSphere::doCreateGeometry()
{
	glPushMatrix();
		glRotated(90, 1.0, 0.0, 0.0);
		gluSphere(m_qobj, m_radius, m_slices, m_stacks);
		//glutSolidSphere(m_radius,m_slices,m_stacks);
	glPopMatrix();
}

// ------------------------------------------------------------
void CIvfSphere::setSlices(const int slices)
{
	m_slices = slices;
}

// ------------------------------------------------------------
int CIvfSphere::getSlices()
{
	return m_slices;
}

// ------------------------------------------------------------
void CIvfSphere::setStacks(const int stacks)
{
	m_stacks = stacks;
}

// ------------------------------------------------------------
int CIvfSphere::getStacks()
{
	return m_stacks;
}

// ------------------------------------------------------------
void CIvfSphere::updateSelectBox()
{
	m_selectionBox->setSize(m_radius*2.0, m_radius*2.0, m_radius*2.0);
}

// ------------------------------------------------------------
void CIvfSphere::doCreateSelect()
{
	m_selectionBox->render();
}

// ------------------------------------------------------------
void CIvfSphere::doUpdateBoundingSphere()
{
	// This is very simple

	if (getBoundingSphere()!=NULL)
		getBoundingSphere()->setRadius(m_radius);
}
