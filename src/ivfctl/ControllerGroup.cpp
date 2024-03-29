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

#include <ivfctl/ControllerGroup.h>

using namespace ivf;

ControllerGroup::ControllerGroup()
{

}

ControllerGroup::~ControllerGroup()
{
	clear();
}

void ControllerGroup::doUpdate(double dt)
{
	using namespace std;

	vector<Controller*>::iterator vi;

	for (vi=m_controllers.begin(); vi!=m_controllers.end(); vi++)
		(*vi)->update(dt);
}

void ControllerGroup::addChild(Controller *controller)
{
	controller->addReference();
	m_controllers.push_back(controller);
}

void ControllerGroup::clear()
{
	using namespace std;

	vector<Controller*>::iterator vi;

	for (vi=m_controllers.begin(); vi!=m_controllers.end(); vi++)
	{
		Controller* controller = (*vi);

		controller->deleteReference();
		if (!controller->referenced())
			delete controller;
	}

	m_controllers.clear();
}

void ControllerGroup::doAction(Action *action)
{
	Controller* target = action->getTarget();
	if ((target==this)||(target==nullptr))
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
	else
	{
		using namespace std;
		
		vector<Controller*>::iterator vi;
		
		for (vi=m_controllers.begin(); vi!=m_controllers.end(); vi++)
			(*vi)->doAction(action);
	}
}

void ControllerGroup::doReset()
{
	using namespace std;

	vector<Controller*>::iterator vi;

	for (vi=m_controllers.begin(); vi!=m_controllers.end(); vi++)
		(*vi)->reset();
}

