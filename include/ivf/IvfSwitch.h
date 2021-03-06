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

#ifndef _CIvfSwitch_h_
#define _CIvfSwitch_h_

#include <ivf/IvfComposite.h>

IvfSmartPointer(CIvfSwitch);

/**
 * Switch node
 *
 * The IvfSwitch class displays one of its 
 * child shapes determined by the 
 * currentChild property.
 * @author Jonas Lindemann
 */
class IVF_API CIvfSwitch : public CIvfComposite {
public:
	/** Constructor */
	CIvfSwitch ();

	/** Destructor */
	virtual ~CIvfSwitch ();

	IvfClassInfo("CIvfSwitch",CIvfComposite);

	/** Set current visible child */
	void setCurrentChild(int child);

	/** Return index of visible child */
	int getCurrentChild();

	/** Cycle to next child shape */
	void cycleForward();

	/** Cycle to previous child shape */
	void cycleBackwards();
private:
	int m_currentChild;
protected:
	virtual void doCreateGeometry();
};

/** \example lod.cpp */

#endif
