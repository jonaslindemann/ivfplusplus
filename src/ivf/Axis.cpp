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

// Implementation of: public class CIvfAxis

#include <ivf/Axis.h>

using namespace ivf;

// ------------------------------------------------------------
CAxis::CAxis ()
		:CComposite()
		//TODO: check and complete member initialisation list!
{
	m_axisType = IVF_AXIS_STD;
	m_xArrow = new CArrow();
	m_yArrow = new CArrow();
	m_zArrow = new CArrow();

	this->setSize(1.0);

	m_xArrow->setRotationQuat(0.0, 0.0, 1.0, -90.0);
	m_zArrow->setRotationQuat(1.0, 0.0, 0.0, 90.0);

	m_xMaterial = new CMaterial();
	m_xMaterial->setSpecularColor(1.0, 1.0, 1.0, 1.0);
	m_xMaterial->setDiffuseColor(1.0, 0.0, 0.0, 1.0);
	m_xMaterial->setAmbientColor(0.5, 0.0, 0.0, 1.0);
	m_xArrow->setMaterial(m_xMaterial);

	m_yMaterial = new CMaterial();
	m_yMaterial->setSpecularColor(1.0, 1.0, 1.0, 1.0);
	m_yMaterial->setDiffuseColor(0.0, 1.0, 0.0, 1.0);
	m_yMaterial->setAmbientColor(0.0, 0.5, 0.0, 1.0);
	m_yArrow->setMaterial(m_yMaterial);

	m_zMaterial = new CMaterial();
	m_zMaterial->setSpecularColor(1.0, 1.0, 1.0, 1.0);
	m_zMaterial->setDiffuseColor(0.0, 0.0, 1.0, 1.0);
	m_zMaterial->setAmbientColor(0.0, 0.0, 0.5, 1.0);
	m_zArrow->setMaterial(m_zMaterial);
	
	addChild(m_xArrow);
	addChild(m_yArrow);
	addChild(m_zArrow);
}

// ------------------------------------------------------------
CAxis::~CAxis ()
{
}

// ------------------------------------------------------------
void CAxis::setAxisType(int type)
{
	m_axisType = type;

	switch (m_axisType) {
	case IVF_AXIS_STD:
		m_xArrow->setRotationQuat(0.0, 0.0, 1.0, -90.0);
		m_xArrow->setPosition(m_size/2.0, 0.0, 0.0);
		break;
	case IVF_AXIS_FLIPX:
		m_xArrow->setRotationQuat(0.0, 0.0, 1.0, 90.0);
		m_xArrow->setPosition(-m_size/2.0, 0.0, 0.0);
		break;
	default:
		m_xArrow->setRotationQuat(0.0, 0.0, 1.0, -90.0);
		m_xArrow->setPosition(m_size/2.0, 0.0, 0.0);
		break;
	}
}


// ------------------------------------------------------------
void CAxis::setSize(double size)
{
	m_size = size;
	m_xArrow->setPosition(size/2.0, 0.0, 0.0);
	m_xArrow->setSize(size,0.2*size);
	m_xArrow->setRadius(0.05*size,0.025*size);
	m_yArrow->setPosition(0.0, size/2.0, 0.0);
	m_yArrow->setSize(size,0.2*size);
	m_yArrow->setRadius(0.05*size,0.025*size);
	m_zArrow->setPosition(0.0, 0.0, size/2.0);
	m_zArrow->setSize(size,0.2*size);
	m_zArrow->setRadius(0.05*size,0.025*size);
	this->setAxisType(m_axisType);
}

// ------------------------------------------------------------
void CAxis::setArrowMaterial(CMaterial *material)
{
	m_xArrow->setMaterial(material);
	m_yArrow->setMaterial(material);
	m_zArrow->setMaterial(material);
}
