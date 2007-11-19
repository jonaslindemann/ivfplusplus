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

#ifndef _CIvfLightComposite_h_
#define _CIvfLightComposite_h_

#include <ivf/IvfComposite.h>
#include <ivf/IvfOldLight.h>

IvfSmartPointer(CIvfLightComposite);

/**
 * Composite class with attached light
 *
 * Implements a composite that has a light
 * attached to it. OBSOLETE see CIvfLight
 */
class IVF_API CIvfLightComposite : public CIvfComposite {
private:
	CIvfOldLight* m_light;
public:
	CIvfLightComposite();

	virtual ~CIvfLightComposite();

	IvfClassInfo("CIvfLightComposite",CIvfComposite);

	void setLight(CIvfOldLight* light);

protected:
	virtual void createGeometry();
};

#endif 
