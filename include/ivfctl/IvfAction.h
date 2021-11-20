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

#ifndef _CIvfAction_h_
#define _CIvfAction_h_

#include <ivf/IvfBase.h>

IvfSmartPointer(CAction);

class CController;

/**
 * Action class
 *
 * Defines an action used in the CActionController class.
 */
class IVFCTL_API CAction : public CBase {
private:
	double m_time;
	int m_actionType;
	double m_dParam;
	int m_iParam;
	CController* m_target;
public:
	/** CAction constructor */
	CAction();

	/** CAction destructor */
	virtual ~CAction();

	IvfClassInfo("CAction",CBase);

	/** Set type of action, \sa CController */
	void setActionType(int type);

	/** Return type of action \sa CController */
	int getActionType();

	/** Set integer parameter of action */
	void setIntParam(int param);

	/** Return integer parameter of action */
	int getIntParam();
	
	/** Set double parameter of action */
	void setDoubleParam(double param);

	/** Return double parameter of action */
	double getDoubleParam();
	
	/** 
	 * Set time of execution
	 * 
	 * Used by the CActionController class 
	 * \param time time when action is to be executed in seconds
	 */
	void setTime(double time);

	/** Return time of excecution */
	double getTime();

	/**
	 * Set action target controller
	 *
	 * This method sets the controller to received the action at the 
	 * specified execution time
	 */
	void setTarget(CController* controller);

	/** Return target controller */
	CController* getTarget();
};

#endif 
