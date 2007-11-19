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

#ifndef _IvfGL_h_
#define _IvfGL_h_

//////////////////////////////////////////////////
// This is ripped from the GLUT 3.7 header file.
//////////////////////////////////////////////////
#if defined(_WIN32)

/* GLUT 3.7 now tries to avoid including <windows.h>
   to avoid name space pollution, but Win32's <GL/gl.h> 
   needs APIENTRY and WINGDIAPI defined properly. */
# if 0
#  define  WIN32_LEAN_AND_MEAN
#  include <windows.h>
# else
   /* XXX This is from Win32's <windef.h> */
#  ifndef APIENTRY
#   define GLUT_APIENTRY_DEFINED
#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#    define APIENTRY    __stdcall
#   else
#    define APIENTRY
#   endif
#  endif
   /* XXX This is from Win32's <winnt.h> */
#  ifndef CALLBACK
#   if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#   else
#    define CALLBACK
#   endif
#  endif
   /* XXX This is from Win32's <wingdi.h> and <winnt.h> */
#  ifndef WINGDIAPI
#   define GLUT_WINGDIAPI_DEFINED
#   define WINGDIAPI __declspec(dllimport)
#  endif
   /* XXX This is from Win32's <ctype.h> */
#  ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#   define _WCHAR_T_DEFINED
#  endif
# endif
#pragma warning (disable:4244)  /* Disable bogus conversion warnings. */
#pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */
#pragma warning (disable:4800)  
#pragma warning (disable:4005)  
#endif
//////////////////////////////////////////////////
// End rip...
//////////////////////////////////////////////////

#include <GL/gl.h>
#include <GL/glu.h>

#endif

