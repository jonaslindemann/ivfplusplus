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

#include <ivf3dui/UISwitch.h>

#include <ivf/Sphere.h>

using namespace ivf;

UISwitch::UISwitch()
{
	MaterialPtr greenMaterial = new Material();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);

	MaterialPtr grayMaterial = new Material();
	grayMaterial->setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);

	SpherePtr sphereOn = new Sphere();
	SpherePtr sphereOff = new Sphere();

	sphereOn->setMaterial(greenMaterial);
	sphereOff->setMaterial(grayMaterial);

	this->setOnShape(sphereOn);
	this->setOffShape(sphereOff);

	m_onShape->setState(GLBase::OS_OFF);
}

UISwitch::~UISwitch()
{

}

void UISwitch::doStateChange(bool state)
{
	if (state)
	{
		if (m_onShape!=nullptr) m_onShape->setState(GLBase::OS_ON);
		if (m_offShape!=nullptr) m_offShape->setState(GLBase::OS_OFF);
	}
	else
	{
		if (m_onShape!=nullptr) m_onShape->setState(GLBase::OS_OFF);
		if (m_offShape!=nullptr) m_offShape->setState(GLBase::OS_ON);
	}
}

void UISwitch::setOnShape(Shape *shape)
{
	ShapePtr oldShape = this->removeShape(m_onShape);
	m_onShape = shape;
	this->addChild(m_onShape);

	MaterialPtr material = m_onShape->getMaterial();

	if (material!=nullptr)
	{
		MaterialPtr hlMaterial = new Material();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_onShape->setHighlightMaterial(hlMaterial);
	}

	doStateChange(getState());
}

void UISwitch::setOffShape(Shape *shape)
{
	ShapePtr oldShape = this->removeShape(m_offShape);
	m_offShape = shape;
	this->addChild(m_offShape);

	MaterialPtr material = m_offShape->getMaterial();

	if (material!=nullptr)
	{
		MaterialPtr hlMaterial = new Material();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_offShape->setHighlightMaterial(hlMaterial);
	}
	
	doStateChange(getState());
}

