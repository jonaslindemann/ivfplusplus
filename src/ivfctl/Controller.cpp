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

#include <ivfctl/Controller.h>

using namespace ivf;

CController::CController()
{
	m_active = false;
	m_controlledShape = NULL;
	m_startAction = NULL;
	m_endAction = NULL;
	m_light = NULL;
}

CController::~CController()
{
	if (m_startAction!=NULL)
	{
		m_startAction->deleteReference();
		if (!m_startAction->referenced())
			delete m_startAction;
	}

	if (m_endAction!=NULL)
	{
		m_endAction->deleteReference();
		if (!m_endAction->referenced())
			delete m_endAction;
	}
}

void CController::activate()
{
	m_active = true;
	doActivate();
}

void CController::deactivate()
{
	m_active = false;
	doDeactivate();
}

bool CController::isActive()
{
	return m_active;
}

void CController::setShape(CShape *shape)
{
	m_controlledShape = shape;

	// Store initial shape state

	m_controlledShape->getPosition(
		m_initialPosition[0],
		m_initialPosition[1],
		m_initialPosition[2]
	);

	m_controlledShape->getScale(
		m_initialScale[0],
		m_initialScale[1],
		m_initialScale[2]
	);

	m_controlledShape->getRotation(
		m_initialRotation[0],
		m_initialRotation[1],
		m_initialRotation[2]
	);

	m_controlledShape->getRotationQuat(
		m_initialRotQuat[0],
		m_initialRotQuat[1],
		m_initialRotQuat[2],
		m_initialRotQuat[3]
	);
}

CShape* CController::getShape()
{
	return m_controlledShape;
}

void CController::reset()
{
	if (m_controlledShape!=NULL)
	{
		
		// Reset controller to initial position
		
		m_controlledShape->setPosition(
			m_initialPosition[0],
			m_initialPosition[1],
			m_initialPosition[2]
			);
		
		m_controlledShape->setScale(
			m_initialScale[0],
			m_initialScale[1],
			m_initialScale[2]
			);
		
		m_controlledShape->setRotation(
			m_initialRotation[0],
			m_initialRotation[1],
			m_initialRotation[2]
			);
		
		m_controlledShape->setRotationQuat(
			m_initialRotQuat[0],
			m_initialRotQuat[1],
			m_initialRotQuat[2],
			m_initialRotQuat[3]
			);

		if (m_light!=NULL)
		{
			m_light->setPosition(
				m_initialPosition[0],
				m_initialPosition[1],
				m_initialPosition[2]
				);
		}
	}

	doReset();
}

void CController::update(double dt)
{
	if (m_active)
		doUpdate(dt);
}

void CController::doUpdate(double dt)
{

}

void CController::doReset()
{

}

void CController::action(CAction* action)
{
	this->doAction(action);
}

void CController::doAction(CAction* action)
{
	CController* target = action->getTarget();
	if ((target==this)||(target==NULL))
	{
		switch (action->getActionType()) {
		case IVF_ACTION_ACTIVATE:
			this->activate();
			break;
		case IVF_ACTION_DEACTIVATE:
			this->deactivate();
			break;
		case IVF_ACTION_RESET:
			this->reset();
			break;
		default:
			break;
		}
	}
}

void CController::doActivate()
{

}

void CController::doDeactivate()
{

}

void CController::setStartAction(CAction *action)
{
	if (m_startAction!=NULL)
	{
		m_startAction->deleteReference();
		if (!m_startAction->referenced())
			delete m_startAction;
	}
	m_startAction = action;
	m_startAction->addReference();
}

void CController::setEndAction(CAction *action)
{
	if (m_endAction!=NULL)
	{
		m_endAction->deleteReference();
		if (!m_endAction->referenced())
			delete m_endAction;
	}
	m_endAction = action;
	m_endAction->addReference();
}

void CController::doStartAction()
{
	if (m_startAction!=NULL)
	{
		CController* target = m_startAction->getTarget();
		target->action(m_startAction);
	}
}

void CController::doEndAction()
{
	if (m_endAction!=NULL)
	{
		CController* target = m_endAction->getTarget();
		target->action(m_endAction);
	}
}

void CController::setLight(CLight *light)
{
	m_light = light;
}

CLight* CController::getLight()
{
	return m_light;
}
