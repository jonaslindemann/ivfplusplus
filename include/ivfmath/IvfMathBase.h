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

#ifndef _CIvfMathBase_h_
#define _CIvfMathBase_h_

#include <ivfdef/IvfDef.h>
#include <ivfdef/IvfPointer.h>

IvfSmartPointer(CIvfMathBase);

/**
 * Root class for the ivf library
 *
 * CIvfMathBase handles class type information and reference counting.
 *
 * @author Jonas Lindemann
 */
class IVFMATH_API CIvfMathBase {
private:
	int m_ref;
public:
	/** CIvfMathBase constructor */
	CIvfMathBase();

	/** CIvfMathBase destrcutor */
	virtual ~CIvfMathBase();

	/** Returns current reference count */
	int getRefCount();

	/** Returns TRUE if object is referenced. */
	bool referenced();

	/** Increases the reference count of the object */
	void addReference();

	/**
	 * Decreases the reference count of the object, if
	 * the current value is greater than 1.
	 */
	void deleteReference();

	/**
	 * Class name query.
	 *
	 * Returns true if name is equal to the name of the class.
	 * @param name Class name to be queried.
	 */
	virtual bool isClass(const std::string& name);

	/**
	 * Returns the name of the class.
	 *
	 * The name buffer must be large enough to hold the
	 * name of the class.
	 * @param name Reference to a string buffer.
	 */
	const std::string getClassNameThis();
	virtual const std::string getClassName();
};

#endif
