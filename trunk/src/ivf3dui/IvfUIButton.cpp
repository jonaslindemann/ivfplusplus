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

#include <ivf3dui/IvfUIButton.h>

CIvfUIButton::CIvfUIButton()
{
	m_visualAction = VA_SCALED;
	m_movementDirection.setComponents(0.0, 0.0, 1.0);
	m_movementDistance = 0.2;

	m_movementXfm = new CIvfTransform();
	m_movementXfm->setUseName(false);

	this->addChild(m_movementXfm);
}

CIvfUIButton::~CIvfUIButton()
{
	
}

void CIvfUIButton::doControlOver(CIvfVec3d vec)
{
	switch (m_visualAction) {
	case VA_SCALED:
		
		break;
	case VA_MOVING:
		m_movementXfm->setHighlightComposite(CIvfShape::HS_ON);
		break;
	case VA_SHAPES:
		
		break;
	default:
		
		break;
	}
}

void CIvfUIButton::doControlLeave(CIvfVec3d vec)
{
	m_movementXfm->setHighlightComposite(CIvfShape::HS_OFF);
	switch (getType()) {
	case BT_NORMAL:
		setState(BS_UP);
		break;
	case BT_TOGGLE:

		break;
	default:
		visualRestore();
		break;
	}
}

void CIvfUIButton::doControlDown(CIvfVec3d vec, int button)
{
	switch (getType()) {
	case BT_NORMAL:
		setState(BS_DOWN);
		break;
	case BT_TOGGLE:
		if (getState()==BS_DOWN)
			setState(BS_UP);
		else
			setState(BS_DOWN);
		break;
	default:
		setState(BS_DOWN);
		break;
	}

}

void CIvfUIButton::doControlUp(CIvfVec3d vec)
{
	switch (getType()) {
	case BT_NORMAL:
		setState(BS_UP);
		break;
	case BT_TOGGLE:

		break;
	default:

		break;
	}
	
}

void CIvfUIButton::setUpShape(CIvfShape *shape)
{
	CIvfShapePtr oldShape = this->removeShape(m_upShape);
	m_upShape = shape;
	this->addChild(m_upShape);
	
	CIvfMaterialPtr material = m_upShape->getMaterial();
	
	if (material!=NULL)
	{
		CIvfMaterialPtr hlMaterial = new CIvfMaterial();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_upShape->setHighlightMaterial(hlMaterial);
	}
}

void CIvfUIButton::setDownShape(CIvfShape *shape)
{
	CIvfShapePtr oldShape = this->removeShape(m_downShape);
	m_downShape = shape;
	this->addChild(m_upShape);
	
	CIvfMaterialPtr material = m_downShape->getMaterial();
	
	if (material!=NULL)
	{
		CIvfMaterialPtr hlMaterial = new CIvfMaterial();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.2);
		hlMaterial->brightnessAmbient(0.2);
		m_downShape->setHighlightMaterial(hlMaterial);
	}
}

void CIvfUIButton::setVisualAction(TVisualAction action)
{
	m_visualAction = action;
	
	CIvfShapePtr oldShape = this->removeShape(m_downShape);
	oldShape = this->removeShape(m_upShape);
	oldShape = this->removeShape(m_movementXfm);
	
	switch (m_visualAction) {
	case VA_SCALED:
		this->addChild(m_movementXfm);
		break;
	case VA_MOVING:
		this->addChild(m_movementXfm);
		break;
	case VA_SHAPES:
		if ((m_downShape!=NULL)&&(m_upShape!=NULL))
		{
			this->addChild(m_downShape);
			this->addChild(m_upShape);
			m_downShape->setState(CIvfGLBase::OS_OFF);
			m_upShape->setState(CIvfGLBase::OS_ON);
		}
		break;
	default:
		
		break;
	}
}

void CIvfUIButton::setMovementDirection(CIvfVec3d vec)
{
	m_movementDirection = vec;
}

void CIvfUIButton::setShape(CIvfShape *shape)
{
	CIvfShapePtr oldShape = this->removeShape(m_downShape);
	oldShape = this->removeShape(m_upShape);
	m_shape = shape;
	m_shape->setUseName(false);
	m_shape->setRenderMaterial(false);

	m_movementXfm->deleteAll();
	m_movementXfm->addChild(m_shape);
	m_movementXfm->setMaterial(m_shape->getMaterial());

	CIvfMaterialPtr material = m_shape->getMaterial();
	
	if (material!=NULL)
	{
		CIvfMaterialPtr hlMaterial = new CIvfMaterial();
		hlMaterial->assignFrom(material);
		hlMaterial->brightnessDiffuse(0.5);
		hlMaterial->brightnessAmbient(0.5);
		m_movementXfm->setHighlightMaterial(hlMaterial);
	}
}

void CIvfUIButton::setMovementDistance(double dist)
{
	m_movementDistance = dist;
}

double CIvfUIButton::getMovementDistance()
{
	return m_movementDistance;
}

void CIvfUIButton::setMovementDirection(double vx, double vy, double vz)
{
	CIvfVec3d vec;
	vec.setComponents(vx, vy, vz);
	vec.normalize();
	setMovementDirection(vec);
}

void CIvfUIButton::visualDown()
{
	switch (m_visualAction) {
	case VA_SCALED:
		this->setNormalize(true);
		this->setScale(0.8, 0.8, 0.8);
		break;
	case VA_MOVING:
		if (m_shape!=NULL)
		{
			CIvfVec3d pos = m_movementXfm->getPosition();
			m_originalPosition = pos;
			pos = pos + m_movementDistance*m_movementDirection;
			m_movementXfm->setPosition(pos);
		}
		break;
	case VA_SHAPES:
		if ((m_downShape!=NULL)&&(m_upShape!=NULL))
		{
			m_downShape->setState(CIvfGLBase::OS_ON);
			m_upShape->setState(CIvfGLBase::OS_OFF);
		}
		break;
	default:
		
		break;
	}
}

void CIvfUIButton::visualUp()
{
	switch (m_visualAction) {
	case VA_SCALED:
		this->setScale(1.0, 1.0, 1.0);
		this->setNormalize(false);
		break;
	case VA_MOVING:
		if (m_shape!=NULL)
		{
			m_movementXfm->setPosition(m_originalPosition);
		}
		break;
	case VA_SHAPES:
		if ((m_downShape!=NULL)&&(m_upShape!=NULL))
		{
			m_downShape->setState(CIvfGLBase::OS_OFF);
			m_upShape->setState(CIvfGLBase::OS_ON);
		}
		break;
	default:
		
		break;
	}
}

void CIvfUIButton::visualRestore()
{
	switch (m_visualAction) {
	case VA_SCALED:
		this->setScale(1.0, 1.0, 1.0);
		this->setNormalize(false);
		break;
	case VA_MOVING:
		if (m_shape!=NULL)
		{
			m_movementXfm->setPosition(m_originalPosition);
		}
		break;
	case VA_SHAPES:
		if ((m_downShape!=NULL)&&(m_upShape!=NULL))
		{
			m_downShape->setState(CIvfGLBase::OS_OFF);
			m_upShape->setState(CIvfGLBase::OS_ON);
		}
		break;
	default:
		
		break;
	}	
}

void CIvfUIButton::doStateChange()
{
	if (getState()==BS_DOWN)
		visualDown();
	else
		visualUp();
}
