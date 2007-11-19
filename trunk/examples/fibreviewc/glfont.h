#ifndef _GLFONT_H_
#define _GLFONT_H_

#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output

#include <GL\gl.h>			// Header File For The OpenGL32 Library
#include <GL\glu.h>			// Header File For The GLu32 Library
#include <stdarg.h>			// Header File For Variable Argument Routines

/*
 *		This Code Was Created By Jeff Molofee 2000
 *		Modified by Shawn T. to handle (%3.2f, num) parameters.
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 *
 *		Adapted by Jonas Lindemann
 */

static GLuint	g_fontBase;				// Base Display List For The Font Set

GLvoid glPrint(const char *fmt, ...);					// Custom GL "Print" Routine
GLvoid glBuildFont(HDC handle);

#endif