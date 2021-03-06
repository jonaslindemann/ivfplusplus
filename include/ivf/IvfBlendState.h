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

#ifndef _CIvfBlendState_h_
#define _CIvfBlendState_h_

#include <ivf/IvfGL.h>
#include <ivf/IvfRenderState.h>

IvfSmartPointer(CIvfBlendState);

/**
 * Blending state class
 *
 * Encapsulates a blending state. 
 */
class IVF_API CIvfBlendState : public CIvfRenderState {
private:
	GLenum m_sfactor;
	GLenum m_dfactor;
	GLboolean m_oldState;
public:
	/** CIvfBlendState constructor */
	CIvfBlendState();

	/** CIvfBlendState constructor */
	CIvfBlendState(GLenum sfactor, GLenum dfactor);

	/** CIvfBlendState destructor */
	virtual ~CIvfBlendState();

	IvfClassInfo("CIvfBlendState",CIvfRenderState);

	/** Sets the blend function factors */
	void setFunction(GLenum sfactor, GLenum dfactor);

	virtual void doRestoreState();
	virtual void doSaveState();
	virtual void doState();
};

#endif 
