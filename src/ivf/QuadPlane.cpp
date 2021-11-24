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

#include <ivf/QuadPlane.h>

using namespace ivf;

QuadPlane::QuadPlane()
:QuadSet()
{
	m_width = 1.0;
	m_height = 1.0;
	m_ratio = 1.0;
	m_planeOrientation = PO_XY;
	m_textureRepeat[0] = 1.0;
	m_textureRepeat[1] = 1.0;
	this->initPlane();
}

QuadPlane::~QuadPlane()
{

}

void QuadPlane::setSize(double width, double height)
{
	m_width = width;
	m_height = height;
	m_ratio = width / height;

	this->initPlane();
}

void QuadPlane::setTexture(Texture *texture)
{
	int width, height;
	Shape::setTexture(texture);
	texture->getSize(width, height);
	if ((width>0)&&(height>0))
	{
		m_ratio = (double)width / (double)height;
		this->setSize(m_width, m_width/m_ratio);
	}
}

void QuadPlane::updateTextureCoords()
{

}


void QuadPlane::setWidth(double width)
{
	this->setSize(width, width/m_ratio);
}

void QuadPlane::flipVert()
{
	this->setTextureCoord(0, 0.0,m_textureRepeat[1]);
	this->setTextureCoord(1, m_textureRepeat[0],m_textureRepeat[1]);
	this->setTextureCoord(2, m_textureRepeat[0],0.0);
	this->setTextureCoord(3, 0.0,0.0);
}

void QuadPlane::initPlane()
{
	this->clear();
	
	switch (m_planeOrientation) {
	case PO_XY:
		this->addCoord(-m_width/2.0, -m_height/2.0, 0.0);
		this->addCoord(m_width/2.0, -m_height/2.0, 0.0);
		this->addCoord(m_width/2.0, m_height/2.0, 0.0);
		this->addCoord(-m_width/2.0, m_height/2.0, 0.0);
		break;
	case PO_XZ:
		this->addCoord(-m_width/2.0, 0.0, -m_height/2.0);
		this->addCoord(-m_width/2.0, 0.0, m_height/2.0);
		this->addCoord(m_width/2.0, 0.0, m_height/2.0);
		this->addCoord(m_width/2.0, 0.0, -m_height/2.0);
		break;
	case PO_YZ:
		this->addCoord(0.0, -m_width/2.0, -m_height/2.0);
		this->addCoord(0.0, m_width/2.0, -m_height/2.0);
		this->addCoord(0.0, m_width/2.0, m_height/2.0);
		this->addCoord(0.0, -m_width/2.0, m_height/2.0);
		break;
	default:
		this->addCoord(-m_width/2.0, -m_height/2.0, 0.0);
		this->addCoord(m_width/2.0, -m_height/2.0, 0.0);
		this->addCoord(m_width/2.0, m_height/2.0, 0.0);
		this->addCoord(-m_width/2.0, m_height/2.0, 0.0);
		break;
	};
	
	IndexPtr idx = new Index();
	idx->createLinear(4);
	this->addCoordIndex(idx);

	this->addTextureCoord(0.0,0.0);
	this->addTextureCoord(m_textureRepeat[0],0.0);
	this->addTextureCoord(m_textureRepeat[0],m_textureRepeat[1]);
	this->addTextureCoord(0.0,m_textureRepeat[1]);

	idx = new Index();
	idx->createLinear(4);
	this->addTextureIndex(idx);
}

void QuadPlane::setOrientation(TPlaneOrientation orientation)
{
	m_planeOrientation = orientation;
	this->initPlane();
}

void QuadPlane::setTextureRepeat(double xrepeat, double yrepeat)
{
	m_textureRepeat[0] = xrepeat;
	m_textureRepeat[1] = yrepeat;

	this->setTextureCoord(0, 0.0,m_textureRepeat[1]);
	this->setTextureCoord(1, m_textureRepeat[0],m_textureRepeat[1]);
	this->setTextureCoord(2, m_textureRepeat[0],0.0);
	this->setTextureCoord(3, 0.0,0.0);
}
