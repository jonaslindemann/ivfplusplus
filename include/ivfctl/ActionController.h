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

#pragma once

#include <ivfctl/ControllerGroup.h>
#include <ivfctl/Action.h>

namespace ivf {

struct SActionCmp {
    bool operator() (CAction* a1, CAction* a2) const {
        return (a1->getTime()<a2->getTime());
    }
};

typedef std::multiset<CAction*, SActionCmp> CActions;
typedef std::multiset<CAction*, SActionCmp>::iterator CActionsIter;

IvfSmartPointer(CActionController);

/**
 * Action controller class
 *
 * Class for sequencing actions for controlling dynamic scenes
 */
class IVFCTL_API CActionController : public CControllerGroup {
private:
	std::multiset<CAction*, SActionCmp> m_actionList;
	std::multiset<CAction*, SActionCmp>::iterator m_actionIter;
	std::vector<double> m_test;

	double m_time;
	double m_tolerance;
public:
	/** CActionController constructor */
	CActionController();

	/** CActionController destructor */
	virtual ~CActionController();

	IvfClassInfo("CActionController",CControllerGroup);

	/** 
	 * Add action to controller
	 *
	 * Actions will be sorted and executed in order
	 */
	void addAction(CAction* action);

	/** Clear (delete) all actions in controller */
	void clear();

	/** Set time tolerance for accepting an action */
	void setTolerance(double tol);

	virtual void doUpdate(double dt);
	virtual void doActivate();
	virtual void doReset();
};
/** \example actionctl.cpp
 * This is an example of how to use the CAction class, using the 
 * CActionController class
 */
}