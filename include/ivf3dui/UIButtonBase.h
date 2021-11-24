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

#pragma once

#include <ivf3dui/UIInteractiveBase.h>

namespace ivf {

IvfSmartPointer(UIButtonBase);

class IVF3DUI_API UIButtonBase : public UIInteractiveBase {
public:
	enum TButtonType {
		BT_NORMAL,
		BT_TOGGLE,
		BT_GROUP
	};
	enum TButtonState {
		BS_DOWN,
		BS_UP,
		BS_UNDEFINED
	};
private:
	TButtonType m_type;
	TButtonState m_state;
public:
	void groupUpdateState(TButtonState state);
	UIButtonBase();
	virtual ~UIButtonBase();

	IvfClassInfo("UIButtonBase",UIInteractiveBase);
	IvfStdFactory(UIButtonBase);

	void setState(TButtonState state);
	TButtonType getType();

	void setType(TButtonType type);
	TButtonState getState();

	virtual void doStateChange();
};

}