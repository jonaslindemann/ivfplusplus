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

#ifndef _IVFWIN32SHARED_H_
#define _IVFWIN32SHARED_H_

#ifdef WIN32
	#ifndef NDEBUG
		#pragma comment(lib, "ivfd_dll.lib")
		#pragma comment(lib, "ivfmathd_dll.lib")	
		#pragma comment(lib, "ivfuid.lib")
		#pragma comment(lib, "ivfwidgetd.lib")
		#pragma comment(lib, "ivffontd_dll.lib")
		#pragma comment(lib, "ivfimaged_dll.lib")
		#pragma comment(lib, "ivffiled_dll.lib")
		#pragma comment(lib, "ivfextd_dll.lib")
		#pragma comment(lib, "ivfgled_dll.lib")
		#pragma comment(lib, "ivfctld_dll.lib")
		#pragma comment(lib, "ivf3duid_dll.lib")
		#pragma comment(lib, "ivfwin32d.lib")
		#ifndef _AFX
			#pragma comment(lib, "fltkd.lib")
			#pragma comment(lib, "fltkgld.lib")
			#pragma comment(lib, "ivffltkd.lib")
		#endif
		#ifdef _AFX
			#pragma comment(lib, "ivfmfcd.lib")
		#endif
		#pragma comment(lib, "glu32.lib")
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "wsock32.lib")
		#pragma comment(lib, "comctl32.lib")
		#pragma comment(lib, "freetype218_D_imp.lib")
		#pragma comment(lib, "ftgl_dynamic_MTD_d.lib")
	#else
		#pragma comment(lib, "ivf_dll.lib")
		#pragma comment(lib, "ivfmath_dll.lib")	
		#pragma comment(lib, "ivfui.lib")
		#pragma comment(lib, "ivfwidget.lib")
		#pragma comment(lib, "ivffltk.lib")
		#pragma comment(lib, "ivffont_dll.lib")
		#pragma comment(lib, "ivfimage_dll.lib")
		#pragma comment(lib, "ivffile_dll.lib")
		#pragma comment(lib, "ivfext_dll.lib")
		#pragma comment(lib, "ivfgle_dll.lib")
		#pragma comment(lib, "ivfctl_dll.lib")
		#pragma comment(lib, "ivf3dui_dll.lib")
		#pragma comment(lib, "ivfwin32.lib")
		#ifndef _AFX
			#pragma comment(lib, "fltk.lib")
			#pragma comment(lib, "fltkgl.lib")
		#endif
		#ifdef _AFX
			#pragma comment(lib, "ivfmfc.lib")
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
