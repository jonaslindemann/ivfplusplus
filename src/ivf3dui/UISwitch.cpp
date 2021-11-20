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

#include <ivf3dui/UISwitch.h>

#include <ivf/Sphere.h>

using namespace ivf;

CUISwitch::CUISwitch()
{
	CMaterialPtr greenMaterial = new CMaterial();
	greenMaterial->setDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);

	CMaterialPtr grayMaterial = new CMaterial();
	grayMaterial->setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);

	CSpherePtr sphereOn = new CSphere();
	CSpherePtr sphereOff = new CSphere();

	sphereOn->setMaterial(greenMaterial);
	sphereOff->setMaterial(grayMaterial);

	this->setOnShape(sphereOn);
	this->setOffShape(sphereOff);

	m_onShape->setState(CGLBase::OS_OFF);
}

CUISwitch::~CUISwitch()
{

}

void CUISwitch::doStateChange(bool state)
{
	if (state)
	{
		if (m_onShape!=NULL) m_onShape->setState(CGLBase::OS_ON);
		if (m_offShape!=NULL) m_offShape->setState(CGLBase::OS_OFF);
	}
	else
	{
		if (m_onShape!=NULL) m_onShape->setState(CGLBase::OS_OFF);
		if (m_offShape!=NULL) m_offShape->setState(CGLBase::OS_ON);
	}
}

void CUISwitch::setOnShape(CShape *shape)
{
	CShapePtr oldShape = this->removeShape(m_onShape);
	m_onShape = shape;
	this->addChild(m_onShape);

	CMaterialPtr material = m_onShape->getMaterial();

	if (material!=NULL)
	{
		CMaterialPtr hlMaterial = new CMaterial();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_onShape->setHighlightMaterial(hlMaterial);
	}

	doStateChange(getState());
}

void CUISwitch::setOffShape(CShape *shape)
{
	CShapePtr oldShape = this->removeShape(m_offShape);
	m_offShape = shape;
	this->addChild(m_offShape);

	CMaterialPtr material = m_offShape->getMaterial();

	if (material!=NULL)
	{
		CMaterialPtr hlMaterial = new CMaterial();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_offShape->setHighlightMaterial(hlMaterial);
	}
	
	doStateChange(getState());
}

