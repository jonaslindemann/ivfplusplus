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

#ifndef _CIvfSelectOrtho_h_
#define _CivfSelectOrtho_h_

#include <ivf/IvfSelectComposite.h>

IvfSmartPointer(CIvfSelectOrtho);

/**
 * Ortho selection
 *
 * ....
 */
class IVF_API CIvfSelectOrtho : public CIvfSelectComposite {
private:
	int m_width;
	int m_height;
public:
	CIvfSelectOrtho();
	virtual ~CIvfSelectOrtho();

	IvfClassInfo("CIvfSelectOrtho",CIvfSelectComposite);

	void initializeSelect(int x, int y, int w, int h);
	void customView();

	void setViewport(int width, int height);
	void getViewport(int &width, int &height);
};

#endif 
