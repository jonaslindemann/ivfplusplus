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

#include <ivf/config.h>
#include <ivf/Base.h>

namespace ivf {

IvfSmartPointer(RenderState);

/**
 * Render state class
 *
 * This class manages a OpenGL state change. This class
 * is an abstract class, and should not be used directly.
 */
class IVF_API RenderState : public Base {
private:

public:
	/** RenderState constructor */
	RenderState();

	/** RenderState destructor */
	virtual ~RenderState();

	IvfClassInfo("RenderState",Base);
	IvfStdFactory(RenderState);

	/** 
	 * Applies state
	 * 
	 * This method first calls the virtual doSaveState() 
	 * method to save the current state. Then it calls 
	 * doState() to apply the new state. This method
	 * will be called by the render() method of CIvfGLBase.
	 */
	void remove();

	/** 
	 * Removes previously applied state
	 * 
	 * This method calls the virtual method doRestoreState()
	 * to remove the previously applied state. This method
	 * will be called by the render() method of CIvfGLBase.
	 */
	void apply();

	/** 
	 * Save the current state
	 *
	 * Should be overidden by derived classes.
	 */
	virtual void doSaveState();

	/** 
	 * Apply new state
	 *
	 * Should be overidden by derived classes.
	 */
	virtual void doState();

	/** 
	 * Restore state to previous state
	 *
	 * Should be overidden by derived classes.
	 */
	virtual void doRestoreState();
};

}