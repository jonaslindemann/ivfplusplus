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

#include <ivfctl/ActionController.h>

using namespace ivf;

ActionController::ActionController()
{
	m_actionIter = m_actionList.end();
	m_time = 0.0;
	m_tolerance = 1e-10;
}

ActionController::~ActionController()
{

}

void ActionController::addAction(Action *action)
{
	m_actionList.insert(action);
	action->addReference();
	m_actionIter = m_actionList.begin();
}

void ActionController::clear()
{
	CActionsIter ai;

	for (ai=m_actionList.begin(); ai!=m_actionList.end(); ai++)
	{
		(*ai)->deleteReference();
		if (!(*ai)->referenced())
			delete (*ai);
	}

	m_actionList.clear();
	m_actionIter = m_actionList.end();
}

void ActionController::doUpdate(double dt)
{
	if (m_actionIter!=m_actionList.end())
	{
		Action* nextAction = (*m_actionIter);
		Action* prevAction;

		if (nextAction!=nullptr)
		{
			if (m_time>=nextAction->getTime())
			{
				this->action(nextAction);

				prevAction = nextAction;
				m_actionIter++;

				if (m_actionIter!=m_actionList.end())
					nextAction = (*m_actionIter);
				else
					nextAction = nullptr;

				while ((nextAction!=nullptr)&&(nextAction->getTime()<=prevAction->getTime())) 
				{
					this->action(nextAction);
					m_actionIter++;
					if (m_actionIter!=m_actionList.end())
						nextAction = (*m_actionIter);
					else
						nextAction = nullptr;
				}
				//cout << endl;
			}
		}
	}
	
	ControllerGroup::doUpdate(dt);
	m_time += dt;
}

void ActionController::doReset()
{
	m_time = 0.0;
}

void ActionController::setTolerance(double tol)
{
	m_tolerance = tol;
}

void ActionController::doActivate()
{
	//m_time = 0.0;
}
