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

#ifndef _CIvfGenericButton_h_
#define _CIvfGenericButton_h_

#include <ivf/IvfSwitch.h>

IvfSmartPointer(CIvfSwitch);

/**
 * Base button class
 *
 * Base class for deriving button functionality. This will
 * be implemented in the widget classes soon.
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfGenericButton : public CIvfSwitch {
public:
	enum TButtonState {
		BS_NORMAL,
		BS_PRESSED,
		BS_DISABLED,
		BS_CHECKED,
		BS_HIDDEN
	};
private:
	TButtonState m_state;
	int m_id;
public:
	/** CIvfGenericButton constructor */
	CIvfGenericButton();

	/** CIvfGenericButton destructor */
	virtual ~CIvfGenericButton();

	IvfClassInfo("CIvfGenericButton",CIvfSwitch);

	/** Set button id */
	void setId(int id);

	/** Return button id */
	int getId();

	/** Set button state */
	void setButtonState(TButtonState state);
};

#endif 
