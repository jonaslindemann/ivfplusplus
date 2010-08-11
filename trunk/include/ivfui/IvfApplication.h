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

#ifndef _CIvfApplication_h_
#define _CIvfApplication_h_

#include <ivfglut/IvfGlutApplication.h>

#include <ivfui/IvfWindow.h>

enum EIvfVisual { 
	IVF_RGB	= 0,
	IVF_INDEX	= 1,
	IVF_SINGLE	= 0,
	IVF_DOUBLE	= 2,
	IVF_ACCUM	= 4,
	IVF_ALPHA	= 8,
	IVF_DEPTH	= 16,
	IVF_STENCIL	= 32,
	IVF_RGB8	= 64,
	IVF_MULTISAMPLE= 128,
	IVF_STEREO = 256
};

IvfSmartPointer(CIvfApplication);

/**
 * Application class for Ivf++ applications
 *
 * The application class encapsulates the event loop
 * and graphic initialisation of a FLTK based 
 * application. 
 *
 * @author Jonas Lindemann
 */
class IVFUI_API CIvfApplication : public CIvfGlutApplication {
private:

public:
	IvfClassInfo("CIvfApplication",CIvfBase);
	int run();
	void runAppLoop(CIvfWindow* window);
};

#endif 
