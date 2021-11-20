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

#ifndef IVFDEF_H
#define IVFDEF_H

#include <ivf/ivfconfig.h>

#define IVF_VERSION_MAJOR	1
#define IVF_VERSION_MINOR	0
#define IVF_VERSION_RELEASE 0

#ifdef IVF_DEBUG_PRINT
	#define IvfDbg(txt) \
		std::cout << txt << std::endl;

	#ifdef IVF_DEBUG_LEVEL_1
		#define IvfDbg1(txt) \
			std::cout << txt << std::endl;
		#define IvfDbg2(txt)
		#define IvfDbg3(txt)
	#endif

	#ifdef IVF_DEBUG_LEVEL_2
		#define IvfDbg1(txt) \
			std::cout << txt << std::endl;
		#define IvfDbg2(txt) \
			std::cout << txt << std::endl;
		#define IvfDbg3(txt)
	#endif

	#ifdef IVF_DEBUG_LEVEL_3
		#define IvfDbg1(txt) \
			std::cout << txt << std::endl;
		#define IvfDbg2(txt) \
			std::cout << txt << std::endl;
		#define IvfDbg3(txt) \
			std::cout << txt << std::endl;
	#endif
#else
	#define IvfDbg(txt)
	#define IvfDbg1(txt)
	#define IvfDbg2(txt)
	#define IvfDbg3(txt)
#endif

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
	#ifdef IVF_DLL
		#ifdef IVFMATH_LIB
			#define IVFMATH_API __declspec(dllexport)
		#else
			#define IVFMATH_API __declspec(dllimport)
		#endif

		#ifdef IVF_LIB
			#define IVF_API __declspec(dllexport)
		#else
			#define IVF_API __declspec(dllimport)
		#endif

		#ifdef IVFIMAGE_LIB
			#define IVFIMAGE_API __declspec(dllexport)
		#else
			#define IVFIMAGE_API __declspec(dllimport)
		#endif

		#ifdef IVFFILE_LIB
			#define IVFFILE_API __declspec(dllexport)
		#else
			#define IVFFILE_API __declspec(dllimport)
		#endif

		#ifdef IVFFLTK_LIB
			#define IVFFLTK_API __declspec(dllexport)
		#else
			#define IVFFLTK_API __declspec(dllimport)
		#endif

		#ifdef IVFUI_LIB
			#define IVFUI_API __declspec(dllexport)
		#else
			#define IVFUI_API __declspec(dllimport)
		#endif

		#ifdef IVFWIDGET_LIB
			#define IVFWIDGET_API __declspec(dllexport)
		#else
			#define IVFWIDGET_API __declspec(dllimport)
		#endif

		#ifdef IVFCTL_LIB
			#define IVFCTL_API __declspec(dllexport)
		#else
			#define IVFCTL_API __declspec(dllimport)
		#endif

		#ifdef IVF3DUI_LIB
			#define IVF3DUI_API __declspec(dllexport)
		#else
			#define IVF3DUI_API __declspec(dllimport)
		#endif

		#ifdef IVFEXT_LIB
			#define IVFEXT_API __declspec(dllexport)
		#else
			#define IVFEXT_API __declspec(dllimport)
		#endif

		#ifdef IVFWIN32_LIB
			#define IVFWIN32_API __declspec(dllexport)
		#else
			#define IVFWIN32_API __declspec(dllimport)
		#endif

		#ifdef IVFFONT_LIB
			#define IVFFONT_API __declspec(dllexport)
		#else
			#define IVFFONT_API __declspec(dllimport)
		#endif

		#ifdef IVFGLE_LIB
			#define IVFGLE_API __declspec(dllexport)
		#else
			#define IVFGLE_API __declspec(dllimport)
		#endif

		#pragma warning(disable:4251)
	#else
		#define IVFMATH_API
		#define IVF_API
		#define IVFIMAGE_API
		#define IVFFILE_API
		#define IVFUI_API
		#define IVFFLTK_API
		#define IVFWIDGET_API
		#define IVF3DUI_API
		#define IVFCTL_API
		#define IVFEXT_API
		#define IVFWIN32_API
		#define IVFFONT_API
		#define IVFGLE_API
	#endif
#else
		#define IVFMATH_API
		#define IVF_API
		#define IVFIMAGE_API
		#define IVFFILE_API
		#define IVFUI_API
		#define IVFFLTK_API
		#define IVFWIDGET_API
		#define IVF3DUI_API
		#define IVFCTL_API
		#define IVFEXT_API
		#define IVFWIN32_API
		#define IVFFONT_API
		#define IVFGLE_API
#endif

#define IvfStdPointer(classname) \
	class classname; \
	typedef classname* classname##Ptr

#define IvfSmartPointer(classname) \
	class classname; \
	typedef CPointer<classname> classname##Ptr; \
	typedef classname* classname##StdPtr;

#define IvfSmartPointerRefBase(classname,refbase) \
	class classname; \
	typedef CPointerRefBase<classname,refbase> classname##Ptr; \
	typedef classname* classname##StdPtr;

#define IvfClassInfo(ivfclassname,ivfparent) \
	const std::string getClassNameThis() { return ivfclassname; } \
	virtual const std::string getClassName() { return ivfclassname; } \
	virtual bool isClass(const std::string& name) { \
		std::string className; \
		className = getClassNameThis(); \
		if (!className.empty()) { \
			if (className == name) \
				return true; \
			else \
				return ivfparent::isClass(name); \
		}\
		else \
			return false; \
	}

#define IvfClassInfoTop(ivfclassname) \
	void getClassNameThis(std::string& name) { name = ivfclassname; } \
	const std::string getClassNameThis() { return ivfclassname; } \
	virtual void getClassName(std::string& name) { name = ivfclassname; } \
	virtual const std::string getClassName() { return ivfclassname; } \
	virtual bool isClass(const std::string& name) { \
	    std::string className = ""; \
		className = getClassNameThis(); \
		if (!className.empty()) { \
			if (className == name) \
				return true; \
			else \
				return false; \
		}\
		else \
			return false; \
	}

#endif
