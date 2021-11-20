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

#include <ivf/Base.h>
#include <ivf/Shape.h>
#include <ivf/Light.h>

#include <ivfctl/Action.h>

namespace ivf {

#define IVF_ACTION_DEACTIVATE		0
#define IVF_ACTION_ACTIVATE			1
#define IVF_ACTION_RESET			2

IvfSmartPointer(CController);

/**
 * Controller class
 *
 * Base class for the different controllers in Ivf++
 */
class IVFCTL_API CController : public CBase {
private:
	CShape* m_controlledShape;	

	bool m_active;

	double	m_initialPosition[3];
	double	m_initialScale[3];
	double	m_initialRotation[3];
	double  m_initialRotQuat[4];

	CAction* m_endAction;
	CAction* m_startAction;

	CLight* m_light;
public:
	/** CController constructor */
	CController();

	/** CController destructor */
	virtual ~CController();

	IvfClassInfo("CController",CBase);

	/** Make the controller active */
	void activate();

	/** Make the controller inactive (default)*/
	void deactivate();

	/** Return true if controller is active */
	bool isActive();

	/** Reset controller to initial state */
	void reset();

	/**
	 * Incremental update of controller
	 *
	 * \param dt time passed since last call of update in seconds
	 */
	void update(double dt);

	/** 
	 * Perform specified action 
	 *
	 * \param action Action to be performed on the controller. Calls doAction on derived classes.
	 */
	void action(CAction* action);

	/** Set shape to be controller */
	void setShape(CShape* shape);

	/** Return controlled shape */
	CShape* getShape(); 

	/** Set light to be controlled */
	void setLight(CLight* light);

	/** Retrieve controlled light */
	CLight* getLight();

	/** 
	 * Set start action
	 *
	 * This action is performed when the controller has returned to
	 * the starting state
	 */
	void setStartAction(CAction* action);

	/**
	 * Set end action
	 *
	 * This action is performed when the controller has reached the
	 * end position
	 */
	void setEndAction(CAction* action);

	/** 
	 * Virtual update routine
	 * 
	 * This method should be overridden in derived classes to
	 * handle incremental time update
	 * \param dt time since last call in seconds.
	 */
	virtual void doUpdate(double dt);
	virtual void doReset();
	virtual void doAction(CAction* action);
	virtual void doDeactivate();
	virtual void doActivate();
	virtual void doEndAction();
	virtual void doStartAction();
};
/** \example controllers.cpp
 * This is an example of how to use controllers
 */

}