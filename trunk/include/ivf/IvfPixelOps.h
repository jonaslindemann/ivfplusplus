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

#ifndef _CIvfPixelOps_h_
#define _CIvfPixelOps_h_

#include <ivf/IvfBase.h>

#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(CIvfPixelOps);

/**
 * PixelOps singleton 
 * 
 * The CIvfPixelOps singleton encapsulates OpenGL pixel operations. 
 * An instance of the CIvfPixelOps class is retrieved using the
 * getInstance() method. See the following code:
 *
 * \code
 * CIvfPixelOpsPtr pixelOps = CIvfPixelOps::getInstance();
 * pixelOps->enableDepthTest();
 * ...
 * \endcode
 *
 * @author Jonas Lindemann
 */
class IVF_API CIvfPixelOps : public CIvfBase {
private:
	static CIvfPixelOps* m_instance;
	static CIvfSingletonDestroyer<CIvfPixelOps> m_destroyer;
public:
	/** Get instance of CIvfPixelOps */
	static CIvfPixelOps* getInstance();

	IvfClassInfo("CIvfPixelOps",CIvfBase);

	/** Enable depth testing */
	void enableDepthTest();

	/** Disable depth testing */
	void disableDepthTest();

	/** Return true if depth testing is enabled */
	bool isDepthTestEnabled();
protected:
	CIvfPixelOps();
	friend class CIvfSingletonDestroyer<CIvfPixelOps>;
};

#endif 
