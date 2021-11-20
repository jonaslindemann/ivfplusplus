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

#pragma once

#include <ivf/Base.h>

namespace ivf {

IvfSmartPointer(CHandlerBase);

/**
 * Handler base class
 *
 * Ivf++ 0.9.x introduces a new UI concept called
 * handlers. A handler is a class which implements
 * user interface functionality by automatically
 * attaching event handlers to CWidgetBase derived
 * classes.
 */
class IVFWIDGET_API CHandlerBase : public CBase {
private:
	bool m_active;
public:
	/** Class constructor */
	CHandlerBase();

	/** Class destructor */
	virtual ~CHandlerBase();

	IvfClassInfo("CHandlerBase",CBase);

	/** Return active state */
	bool isActive();

	/**
	 * Activate handler
	 *
	 * When an handler is inactive it _not_ handle
	 * events. Deactivating a handler will call the
	 * virtual method doDeactivate() for derived
	 * classes to handle special conditions when
	 * the handler changes it state.
	 */
	void deactivate();

	/**
	 * Activate handler
	 *
	 * When an handler is active it will handle
	 * events. Activating a handler will call the
	 * virtual method doActivate() for derived
	 * classes to handle special conditions when
	 * the handler changes it state.
	 */
	void activate();
protected:
	virtual void doDeactivate();
	virtual void doActivate();
};
/** \example handlers.cpp
 * This is an example of how to use CHandlerBase derived
 * handlers.
 */
}