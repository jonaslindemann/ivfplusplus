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

#include <ivf/GL.h>
#include <ivf/RenderState.h>

namespace ivf {

IvfSmartPointer(BlendState);

/**
 * Blending state class
 *
 * Encapsulates a blending state. 
 */
class IVF_API BlendState : public RenderState {
private:
	GLenum m_sfactor;
	GLenum m_dfactor;
	GLboolean m_oldState;
public:
	/** CIvfBlendState constructor */
	BlendState();

	/** CIvfBlendState constructor */
	BlendState(GLenum sfactor, GLenum dfactor);

	/** CIvfBlendState destructor */
	virtual ~BlendState();

	IvfClassInfo("BlendState",RenderState);
	IvfStdFactory(BlendState);

	/** Sets the blend function factors */
	void setFunction(GLenum sfactor, GLenum dfactor);

	virtual void doRestoreState();
	virtual void doSaveState();
	virtual void doState();
};

}