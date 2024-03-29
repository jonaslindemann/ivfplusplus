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

#pragma once

#include <ivfmath/Slerp.h>

#include <ivfctl/Controller.h>

namespace ivf {

IvfSmartPointer(SlerpController);

/**
 * Spherical linear rotation interpolater (SLERP) controller class
 *
 * Controls the rotation of an object using the rotations given by
 * a set of quaternions.
 */
class IVFCTL_API SlerpController : public Controller {
private:
public:
	enum TEndAction {
		EA_STOP,
		EA_REVERSE,
		EA_RESET,
		EA_DEACTIVATE
	};
	enum TStartAction {
		SA_STOP,
		SA_REVERSE,
		SA_RESET,
		SA_DEACTIVATE
	};
private:
	SlerpPtr m_slerp;
	Quat m_q;
	double m_v0;
	double m_a0;
	double m_t0;
	double m_t;
	double m_v;
	TEndAction m_endAction;
	TStartAction m_startAction;
public:
	/** SlerpController constructor */
	SlerpController();

	/** SlerpController destructor */
	virtual ~SlerpController();

	IvfClassInfo("SlerpController", Controller);
	IvfStdFactory(SlerpController);

	/** Set instance of slerp class used when interpolating */
	void setSlerp(Slerp* slerp);

	/** Return Slerp class */
	Slerp* getSlerp();
	/** 
	 * Set initial position on slerp
	 *
	 * \param t distance from curve start 0 <= t <= getSize()-1
	 */
	void setInitialPos(double value);

	/** 
	 * Set initial speed on slerp
	 *
	 * \param v speed in units/second
	 */
	void setInitialSpeed(double value);

	/** 
	 * Define what to do when returning to start position
	 *
	 * \param action SA_STOP stops movement, SA_REVERSE reverses 
	 * direction of movement, SA_RESET resets position to initial position
	 * SA_DEACTIVATE deactivates controller
	 */
	void setStartActionType(TStartAction action);

	/** 
	 * Define what to do when arriving at end position
	 *
	 * \param action EA_STOP stops movement, EA_REVERSE reverses 
	 * direction of movement, EA_RESET resets position to initial position
	 * EA_DEACTIVATE deactivates controller
	 */
	void setEndActionType(TEndAction action);
	
	virtual void doReset();
	virtual void doUpdate(double dt);
};

}