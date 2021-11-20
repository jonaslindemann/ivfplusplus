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

#ifndef _CIvfCube_h_
#define _CIvfCube_h_

#include <ivf/IvfBrick.h>

IvfSmartPointer(CCube);

/**
 * Cube shape class
 * 
 * CIvfCube implements i CIvfBrick with equal 
 * dimensions.
 * @author Jonas Lindemann
 */
class IVF_API CCube : public CBrick {
private:

public:
	/** CIvfCube constructor. */
	CCube ();
	
	/** CIvfCube destructor. */
	virtual ~CCube ();
	
	IvfClassInfo("CIvfCube",CBrick);

	/** Set size of cube size */
	void	setSize (const double size);
	/** Return size of cube size */
	void	getSize (double &size);
protected:
};
#endif
