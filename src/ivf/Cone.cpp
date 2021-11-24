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

#include <ivf/Cone.h>

#include <ivf/config.h>

#define PI 3.141592653589793

using namespace ivf;

// ------------------------------------------------------------
Cone::Cone ()
		:Shape()
		//TODO: check and complete member initialisation list!
{
	m_bottomRadius = 1.0;
	m_topRadius = 0.0;
	m_height = 2.0;
	m_slices = 8;
	m_stacks = 1;
	m_selectionBox = new SelectionBox();
	updateSelectBox();
}

// ------------------------------------------------------------
Cone::~Cone ()
{
	delete m_selectionBox;
}

// ------------------------------------------------------------
void Cone::setTopRadius (const double radius)
{
	m_topRadius = radius;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getTopRadius ()
{
	return m_topRadius;
	updateSelectBox();
}

// ------------------------------------------------------------
void Cone::setBottomRadius (const double radius)
{
	m_bottomRadius = radius;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getBottomRadius ()
{
	return m_bottomRadius;
}

// ------------------------------------------------------------
void Cone::setHeight (const double height)
{
	m_height = height;
	doUpdateBoundingSphere();
	updateSelectBox();
}

// ------------------------------------------------------------
double Cone::getHeight ()
{
	return m_height;
}

void Cone::setSlices(int slices)
{
	m_slices = slices;
}

int Cone::getSlices()
{
	return m_slices;
}

void Cone::setStacks(int stacks)
{
	m_stacks = stacks;
}

int Cone::getStacks()
{
	return m_stacks;
}

void Cone::doCreateGeometry()
{
	glPushMatrix();
		glPushMatrix();
			glTranslated(0.0,-getHeight()/2.0,0.0);
			glRotated(-90,1.0,0.0,0.0);

			GLUquadricObj* cylinder = gluNewQuadric();
			gluQuadricNormals(cylinder,GLU_SMOOTH);
			gluQuadricTexture(cylinder,GL_TRUE);
			gluQuadricDrawStyle(cylinder,GLU_FILL);
			gluCylinder(cylinder,
				getBottomRadius(),getTopRadius(),getHeight(),
				getSlices(),getStacks());
			gluDeleteQuadric(cylinder);

			GLUquadricObj* bottom = gluNewQuadric();
			gluQuadricNormals(bottom,GLU_SMOOTH);
			gluQuadricTexture(bottom,GL_TRUE);
			gluQuadricDrawStyle(bottom,GLU_FILL);
			gluQuadricOrientation(bottom,GLU_INSIDE);
			gluDisk(bottom,
				0.0, getBottomRadius(),
				getSlices(),1);
			gluDeleteQuadric(bottom);
		glPopMatrix();

		if (getTopRadius()>0.0)
		{
			glPushMatrix();
				glTranslated(0.0,getHeight()/2.0,0.0);
				glRotated(-90,1.0,0.0,0.0);

				GLUquadricObj* top = gluNewQuadric();
				gluQuadricNormals(top,GLU_SMOOTH);
				gluQuadricTexture(top,GL_TRUE);
				gluQuadricDrawStyle(top,GLU_FILL);
				gluDisk(top,
					0.0, getTopRadius(),
					getSlices(),1);
				gluDeleteQuadric(top);
			glPopMatrix();
		}

	glPopMatrix();
}


void Cone::updateSelectBox()
{
	if (m_bottomRadius>m_topRadius)
		m_selectionBox->setSize(m_bottomRadius*2.0,m_height,m_bottomRadius*2.0);
	else
		m_selectionBox->setSize(m_topRadius*2.0,m_height,m_topRadius*2.0);
}

void Cone::doCreateSelect()
{
	m_selectionBox->render();
}

void Cone::doUpdateBoundingSphere()
{
	if (getBoundingSphere()!=nullptr)
	{
		double radius;
		if (m_topRadius>=m_bottomRadius)
			radius = sqrt(pow(m_height/2.0,2) + pow(m_topRadius,2));
		else
			radius = sqrt(pow(m_height/2.0,2) + pow(m_bottomRadius,2));
		getBoundingSphere()->setRadius(radius);
	}
}
