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

// Implementation of: public class CIvfBrick

#include <ivf/config.h>
#include <ivf/Brick.h>

using namespace ivf;

// ------------------------------------------------------------
CBrick::CBrick ()
		:CQuadSet()
{
	m_size[0] = 1.0;
	m_size[1] = 1.0;
	m_size[2] = 1.0;
	initBrick();
}

// ------------------------------------------------------------
CBrick::~CBrick ()
{
}

// ------------------------------------------------------------
void CBrick::initBrick()
{
	double ox, oy, oz;

	ox = -m_size[0]/2.0;
	oy = -m_size[1]/2.0;
	oz = -m_size[2]/2.0;

	addCoord(ox, oy, oz);
	addCoord(ox + m_size[0], oy, oz);
	addCoord(ox + m_size[0], oy, oz + m_size[2]);
	addCoord(ox, oy, oz + m_size[2]);
	addCoord(ox, oy + m_size[1], oz);
	addCoord(ox + m_size[0], oy + m_size[1], oz);
	addCoord(ox + m_size[0], oy + m_size[1], oz + m_size[2]);
	addCoord(ox, oy + m_size[1], oz + m_size[2]);

	addTextureCoord(0.0, 0.0);
	addTextureCoord(1.0, 0.0);
	addTextureCoord(1.0, 1.0);
	addTextureCoord(0.0, 1.0);

	CIndex* idx;

	idx = new CIndex();
	idx->add(1, 2, 3, 0);
	idx->add(7, 6, 5, 4);
	idx->add(4, 5, 1, 0);
	idx->add(5, 6, 2, 1);
	idx->add(6, 7, 3, 2);
	idx->add(3, 7, 4, 0);

	addCoordIndex(idx);

	idx = new CIndex();
	idx->add(0, 1, 2, 3);
	idx->add(0, 1, 2, 3);
	idx->add(0, 1, 2, 3);
	idx->add(0, 1, 2, 3);
	idx->add(0, 1, 2, 3);
	idx->add(0, 1, 2, 3);

	addTextureIndex(idx);
}

// ------------------------------------------------------------
void CBrick::setSize (const double width, const double height, const double depth)
{
	m_size[0] = width;
	m_size[1] = height;
	m_size[2] = depth;
	updateBrick();
	doUpdateBoundingSphere();
}

// ------------------------------------------------------------
void CBrick::getSize (double &width, double &height, double &depth)
{
	width = m_size[0];
	height = m_size[1];
	depth = m_size[2];
}

// ------------------------------------------------------------
void CBrick::setSize(CPoint3d* p1, CPoint3d* p2)
{
	double ax, ay, az;
	double bx, by, bz;

	p1->getComponents(ax, ay, az);
	p2->getComponents(bx, by, bz);

	m_size[0] = fabs(bx - ax);
	m_size[1] = fabs(by - ay);
	m_size[2] = fabs(bz - az);

	setPosition((ax + bx)/2.0, (ay + by)/2.0, (az + bz)/2.0);
	doUpdateBoundingSphere();
	updateBrick();
}

// ------------------------------------------------------------
void CBrick::doCreateSelect()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPushMatrix();
			//glScaled(m_size[0]*1.05,m_size[1]*1.05,m_size[2]*1.05);
			glScaled(1.05, 1.05, 1.05);
			CMaterial* hMaterial = this->getHightlightMaterial();
			hMaterial->render();
			CQuadSet::doCreateGeometry();
		glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ------------------------------------------------------------
void CBrick::updateBrick()
{
	double ox, oy, oz;

	ox = -m_size[0]/2.0;
	oy = -m_size[1]/2.0;
	oz = -m_size[2]/2.0;

	setCoord(0, ox, oy, oz);
	setCoord(1, ox + m_size[0], oy, oz);
	setCoord(2, ox + m_size[0], oy, oz + m_size[2]);
	setCoord(3, ox, oy, oz + m_size[2]);
	setCoord(4, ox, oy + m_size[1], oz);
	setCoord(5, ox + m_size[0], oy + m_size[1], oz);
	setCoord(6, ox + m_size[0], oy + m_size[1], oz + m_size[2]);
	setCoord(7, ox, oy + m_size[1], oz + m_size[2]);
}

