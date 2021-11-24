//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#ifndef _IVFWIN32STATIC_H_
#define _IVFWIN32STATIC_H_

#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib, "ivfd.lib")
		#pragma comment(lib, "ivfmathd.lib")	
		#pragma comment(lib, "ivfuid.lib")
		#pragma comment(lib, "ivfwidgetd.lib")
		#pragma comment(lib, "ivffltkd.lib")
		#pragma comment(lib, "ivffontd.lib")
		#pragma comment(lib, "ivfimaged.lib")
		#pragma comment(lib, "ivffiled.lib")
		#pragma comment(lib, "ivfextd.lib")
		#pragma comment(lib, "ivfgled.lib")
		#pragma comment(lib, "ivfctld.lib")
		#pragma comment(lib, "ivf3duid.lib")
		#pragma comment(lib, "ivfmfcd.lib")
		#pragma comment(lib, "ivfwin32d.lib")
		#pragma comment(lib, "gled.lib")
		#pragma comment(lib, "libpngd.lib")
		#pragma comment(lib, "libjpegd.lib")
		#pragma comment(lib, "zlibd.lib")
		#ifndef _AFX
			#pragma comment(lib, "fltkd.lib")
			#pragma comment(lib, "fltkgld.lib")
		#endif
		#pragma comment(lib, "glu32.lib")
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "wsock32.lib")
		#pragma comment(lib, "comctl32.lib")
		#pragma comment(lib, "freetype218_D_imp.lib")
		#pragma comment(lib, "ftgl_dynamic_MTD_d.lib")
	#else
		#pragma comment(lib, "ivf.lib")
		#pragma comment(lib, "ivfmath.lib")	
		#pragma comment(lib, "ivfui.lib")
		#pragma comment(lib, "ivfwidget.lib")
		#pragma comment(lib, "ivffltk.lib")
		#pragma comment(lib, "ivffont.lib")
		#pragma comment(lib, "ivfimage.lib")
		#pragma comment(lib, "ivffile.lib")
		#pragma comment(lib, "ivfext.lib")
		#pragma comment(lib, "ivfgle.lib")
		#pragma comment(lib, "ivfctl.lib")
		#pragma comment(lib, "ivf3dui.lib")
		#pragma comment(lib, "ivfmfc.lib")
		#pragma comment(lib, "ivfwin32.lib")
		#pragma comment(lib, "gle.lib")
		#pragma comment(lib, "libpng.lib")
		#pragma comment(lib, "libjpeg.lib")
		#pragma comment(lib, "zlib.lib")
		#ifndef _AFX
			#pragma comment(lib, "fltk.lib")
			#pragma comment(lib, "fltkgl.lib")
		#endif
		#pragma comment(lib, "glu32.lib")
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "wsock32.lib")
		#pragma comment(lib, "comctl32.lib")
		#pragma comment(lib, "freetype218_imp.lib")
		#pragma comment(lib, "ftgl_dynamic_MTD.lib")
	#endif
#endif

#endif
