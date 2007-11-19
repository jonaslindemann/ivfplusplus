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

#ifndef _CIvfBlending_h_
#define _CIvfBlending_h_

#include <ivf/IvfBase.h>
#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(CIvfBlending);

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
class IVF_API CIvfBlending : public CIvfBase {
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
	static CIvfBlending* m_instance;
	static CIvfSingletonDestroyer<CIvfBlending> m_destroyer;
public:
	/** Returns the blending singleton */
	static CIvfBlending* getInstance();

	IvfClassInfo("CIvfBlending",CIvfBase);

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
	CIvfBlending();
	friend class CIvfSingletonDestroyer<CIvfBlending>;
};

#endif 
