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

#ifndef _IvfImageDef_h_
#define _IvfImageDef_h_

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*) 0)
	#endif
#endif

// DLL-support on Win32

#ifdef WIN32
	#ifdef IVFIMAGE_DLL
		#ifdef IVFIMAGE_EXPORTS
			#define IVFIMAGE_API __declspec(dllexport)
		#else
			#define IVFIMAGE_API __declspec(dllimport)
		#endif
		#pragma warning(disable:4251)
	#else
		#define IVFIMAGE_API
	#endif
#else
	#define IVFIMAGE_API
#endif
#endif
