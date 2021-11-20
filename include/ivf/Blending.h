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

#include <ivf/Base.h>
#include <ivfdef/SingletonDestroyer.h>

namespace ivf {

IvfStdPointer(CBlending);

/**
 * Blending singleton 
 * 
 * The CIvfBlending singleton encapsulates OpenGL blending. 
 * An instance of the CIvfBlending class is retrieved using the
 * getInstance() method. See the following code:
 *
 * \code
 * CIvfBlendingPtr blending = CIvfBlending::getInstance();
 * blending->enable();
 * blending->setFunction(CIvfBlending::BF_SRC_ALPHA, CIvfBlending::BF_ONE_MINUS_SRC_ALPHA);
 * ...
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CBlending : public CBase {
public:
	enum TBlendFactor {
		BF_ZERO,
		BF_ONE,
		BF_DST_COLOR,
		BF_SRC_COLOR,
		BF_ONE_MINUS_DST_COLOR,
		BF_ONE_MINUS_SRC_COLOR,
		BF_SRC_ALPHA,
		BF_ONE_MINUS_SRC_ALPHA,
		BF_DST_ALPHA,
		BF_ONE_MINUS_DST_ALPHA,
		BF_SRC_ALPHA_SATURATE
	};
private:
	static CBlending* m_instance;
	static CSingletonDestroyer<CBlending> m_destroyer;
public:
	/** Returns the blending singleton */
	static CBlending* getInstance();

	IvfClassInfo("CIvfBlending",CBase);

	/** Enable blending */
	void enable();

	/** Disable blending */
	void disable();

	/** Returns true if blending is enabled */
	bool isEnabled();

	/** Sets the blend function */
	void setFunction(TBlendFactor src, TBlendFactor dst);

	/** Saves the current blending state */
	void saveState();

	/** Restores the saved blend state */
	void restoreState();

	void defineAlphaBlendFunction();
	void defineAddFunction();

protected:

	/** Protected constructor (do not use) */
	CBlending();
	friend class CSingletonDestroyer<CBlending>;
};

}

