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

#ifndef _CIvfRefCountBase_h_
#define _CIvfRefCountBase_h_

#include <ivfdef/IvfDef.h>

/**
 * Reference counting base class
 *
 * This class implements the reference counting
 * scheme used in Ivf++. See template CPointer
 * for a smart pointer class.
 */
class IVF_API CRefCountBase {
private:
	int m_ref;
public:
	/** Class constructor */
	CRefCountBase();

	/** Class destructor */
	virtual ~CRefCountBase();

	/** Returns current reference count */
	int getRefCount();
	
	/** Returns true if object is referenced. */
	bool referenced();
	
	/** Increases the reference count of the object */
	void addReference();
	
	/** 
	 * Decreases the reference count of the object, if 
	 * the current value is greater than 1.
	 */
	void deleteReference();
};

#endif 
