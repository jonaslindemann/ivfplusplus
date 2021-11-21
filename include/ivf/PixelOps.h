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

#include <ivf/Base.h>

#include <ivfdef/SingletonDestroyer.h>

namespace ivf {

IvfStdPointer(CPixelOps);

/**
 * PixelOps singleton 
 * 
 * The CPixelOps singleton encapsulates OpenGL pixel operations. 
 * An instance of the CPixelOps class is retrieved using the
 * getInstance() method. See the following code:
 *
 * \code
 * CIvfPixelOpsPtr pixelOps = CPixelOps::getInstance();
 * pixelOps->enableDepthTest();
 * ...
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CPixelOps : public CBase {
private:
	static CPixelOps* m_instance;
	static CSingletonDestroyer<CPixelOps> m_destroyer;
public:
	/** Get instance of CPixelOps */
	static CPixelOps* getInstance();

	IvfClassInfo("CPixelOps",CBase);
	IvfStdFactory(CPixelOps);

	/** Enable depth testing */
	void enableDepthTest();

	/** Disable depth testing */
	void disableDepthTest();

	/** Return true if depth testing is enabled */
	bool isDepthTestEnabled();
protected:
	CPixelOps();
	friend class CSingletonDestroyer<CPixelOps>;
};

}