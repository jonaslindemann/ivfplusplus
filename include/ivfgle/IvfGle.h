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

#ifndef _CIvfGle_h_
#define _CIvfGle_h_

#include <ivf/IvfBase.h>
#include <ivfdef/IvfSingletonDestroyer.h>

IvfStdPointer(CIvfGle);

class IVFGLE_API CIvfGle: public CIvfBase {
private:
	static CIvfGle* m_instance;
	static CIvfSingletonDestroyer<CIvfGle> m_destroyer;
public:
	/** Returns the IvfGle singleton */
	static CIvfGle* getInstance();

	IvfClassInfo("CIvfGle", CIvfBase);

	void setNumSides(int sides);
	int getNumSides();

	void setJoinStyle(int style);
	int getJoinStyle();
protected:
	/** Protected constructor (do not use) */
	CIvfGle();
	friend class CIvfSingletonDestroyer<CIvfGle>;
};
#endif
