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

#include <ivfdef/Pointer.h>
#include <ivfdef/Def.h>

#include <ivf/Composite.h>

namespace ivf {

IvfSmartPointer(CUIBase)

/**
 * Base user interface class
 *
 * Base class for deriving 3D ui functionality. This will
 * be implemented in the widget classes soon.
 *
 * @author Jonas Lindemann
 */
class IVF3DUI_API CUIBase : public CComposite {
public:

private:
	int m_id;
	bool m_enabled;
	CUIBasePtr m_parent;
public:
	/** CUIBase constructor */
	CUIBase();

	/** CUIBase destructor */
	virtual ~CUIBase();

	IvfClassInfo("CUIBase",CComposite);
	IvfStdFactory(CUIBase);

	void enable();
	void disable();
	bool isEnabled();

	/** Set button id */
	void setId(int id);

	/** Return button id */
	int getId();

	void setParentControl(CUIBase* parent);

	bool haveParent();
	CUIBase* getParentControl();

	virtual void doChangeState(bool enabled);
};

}