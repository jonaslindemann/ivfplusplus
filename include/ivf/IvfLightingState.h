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

#ifndef _CIvfLightingState_h_
#define _CIvfLightingState_h_

#include <ivf/IvfRenderState.h>
#include <ivf/IvfLighting.h>

IvfSmartPointer(CLightingState);

/**
 * Lighting state class
 *
 * Disables and enables the OpenGL lighting state
 *
 * @author Jonas Lindemann
 */
class IVF_API CLightingState : public CRenderState {
private:
	bool m_lightingState;
	CLighting* m_lighting;
public:
	/** CLightingState constructor */
	CLightingState();

	/** CLightingState destructor */
	virtual ~CLightingState();

	IvfClassInfo("CLightingState",CRenderState);

	/** Lighting flag (enabled = true) */
	void setLighting(bool flag);

	/** Return lighting flag */
	bool getLighting();

	virtual void doRestoreState();
	virtual void doSaveState();
	virtual void doState();
};

#endif 
