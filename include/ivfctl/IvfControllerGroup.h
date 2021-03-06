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

#ifndef _CIvfControllerGroup_h_
#define _CIvfControllerGroup_h_

#include <ivfctl/IvfController.h>

IvfSmartPointer(CIvfControllerGroup);

/**
 * Group controller class
 *
 * This controller handles a set of controllers as a single
 * controller
 */
class IVFCTL_API CIvfControllerGroup : public CIvfController {
private:
	std::vector<CIvfController*> m_controllers;
public:
	/** CIvfControllerGroup constructor */
	CIvfControllerGroup();

	/** CIvfControllerGroup destructor */
	virtual ~CIvfControllerGroup();

	IvfClassInfo("CIvfControllerGroup",CIvfController);

	/** Add a controller to the controller group */
	void addChild(CIvfController* controller);

	void clear();
	
	virtual void doUpdate(double dt);
	virtual void doAction(CIvfAction* action);
	virtual void doReset();
};

#endif 
