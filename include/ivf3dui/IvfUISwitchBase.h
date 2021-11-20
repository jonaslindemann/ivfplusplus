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

#ifndef _CIvfUISwitchBase_h_
#define _CIvfUISwitchBase_h_

#include <ivf3dui/IvfUIInteractiveBase.h>

IvfSmartPointer(CUISwitchBase);

class IVF3DUI_API CUISwitchBase : public CUIInteractiveBase {
private:
	bool m_state;
public:
	CUISwitchBase();
	virtual ~CUISwitchBase();

	void on();
	void off();

	bool isOn();
	bool isOff();

	void setState(bool state);
	bool getState();

	IvfClassInfo("CUISwitchBase",CUIInteractiveBase);

	virtual void doControlClick(CVec3d vec, int button);
	virtual void doControlOver(CVec3d vec);
	virtual void doControlLeave(CVec3d vec);
	virtual void doControlUp(CVec3d vec);
	virtual void doControlDown(CVec3d vec, int button);
	virtual void doStateChange(bool state);
};

#endif 
