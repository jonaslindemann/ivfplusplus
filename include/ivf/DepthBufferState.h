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

#include <ivf/RenderState.h>

namespace ivf {

IvfSmartPointer(DepthBufferState);

/**
 * Depth buffer render state class
 *
 * Render state class for enabling/disabling
 * depth buffer test state. Default enabled.
 */
class IVF_API DepthBufferState : public RenderState {
public:
	enum TDepthBufferState {
		DS_ENABLED,
		DS_DISABLED
	};
private:
	TDepthBufferState m_depthBufferState;
	TDepthBufferState m_oldState;
public:
	/** Class constructor */
	DepthBufferState();

	/** Class destructor */
	virtual ~DepthBufferState();

	IvfClassInfo("DepthBufferState",RenderState);
	IvfStdFactory(DepthBufferState);

	/** Set depth test state, see TDepthBufferState */
	void setState(TDepthBufferState state);

	/** Return depth test state */
	TDepthBufferState getState();

	virtual void doState();
	virtual void doRestoreState();
	virtual void doSaveState();

};

}