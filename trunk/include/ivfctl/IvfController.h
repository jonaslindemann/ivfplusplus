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

#ifndef _CIvfController_h_
#define _CIvfController_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfShape.h>
#include <ivf/IvfLight.h>

#include <ivfctl/IvfAction.h>

#define IVF_ACTION_DEACTIVATE		0
#define IVF_ACTION_ACTIVATE			1
#define IVF_ACTION_RESET			2

IvfSmartPointer(CIvfController);

/**
 * Controller class
 *
 * Base class for the different controllers in Ivf++
 */
class IVFCTL_API CIvfController : public CIvfBase {
private:
	CIvfShape* m_controlledShape;	

	bool m_active;

	double	m_initialPosition[3];
	double	m_initialScale[3];
	double	m_initialRotation[3];
	double  m_initialRotQuat[4];

	CIvfAction* m_endAction;
	CIvfAction* m_startAction;

	CIvfLight* m_light;
public:
	/** CIvfController constructor */
	CIvfController();

	/** CIvfController destructor */
	virtual ~CIvfController();

	IvfClassInfo("CIvfController",CIvfBase);

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
	void action(CIvfAction* action);

	/** Set shape to be controller */
	void setShape(CIvfShape* shape);

	/** Return controlled shape */
	CIvfShape* getShape(); 

	/** Set light to be controlled */
	void setLight(CIvfLight* light);

	/** Retrieve controlled light */
	CIvfLight* getLight();

	/** 
	 * Set start action
	 *
	 * This action is performed when the controller has returned to
	 * the starting state
	 */
	void setStartAction(CIvfAction* action);

	/**
	 * Set end action
	 *
	 * This action is performed when the controller has reached the
	 * end position
	 */
	void setEndAction(CIvfAction* action);

	/** 
	 * Virtual update routine
	 * 
	 * This method should be overridden in derived classes to
	 * handle incremental time update
	 * \param dt time since last call in seconds.
	 */
	virtual void doUpdate(double dt);
	virtual void doReset();
	virtual void doAction(CIvfAction* action);
	virtual void doDeactivate();
	virtual void doActivate();
	virtual void doEndAction();
	virtual void doStartAction();
};
/** \example controllers.cpp
 * This is an example of how to use controllers
 */

#endif 
