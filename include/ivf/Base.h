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

#include <ivfdef/Def.h>
#include <ivfdef/Pointer.h>

#include <ivf/RefCountBase.h>
#include <ivf/RTTIBase.h>

namespace ivf {

IvfSmartPointer(CBase);

/**
 * Root class for the ivf library
 * 
 * CIvfBase handles class type information and reference counting.
 *
 * @author Jonas Lindemann
 */
class IVF_API CBase : public CRefCountBase, public CRTTIBase {
public:
	/** CIvfBase constructor */
	CBase();

	/** CIvfBase destrcutor */
	virtual ~CBase();

	IvfClassInfo("CIvfBase",CRTTIBase);
};

}

