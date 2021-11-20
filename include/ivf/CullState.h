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

#include <ivf/RenderState.h>

namespace ivf {

IvfSmartPointer(CCullState);

/**
 * Culling state class
 * 
 * Sets the polygon culling state for objects
 */
class IVF_API CCullState : public CRenderState {
public:
	enum TCullFace {
		CF_FRONT,
		CF_BACK,
		CF_FRONT_AND_BACK
	};
private:
	bool m_cull;
	TCullFace m_cullFace;
public:
	/** CIvfCullState constructor */
	CCullState();

	/** CIvfCullState destructor */
	virtual ~CCullState();

	IvfClassInfo("CIvfCullState",CRenderState);

	/** Sets the face to be culled */
	void setCullFace(TCullFace face);

	/** Enable and disable polygon culling */
	void setCulling(bool flag);

	virtual void doSaveState();
	virtual void doState();
	virtual void doRestoreState();
};

}