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

#ifndef _CIvfWin32Window_h_
#define _CIvfWin32Window_h_

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>

#include <ivfwidget/IvfWidgetBase.h>

IvfSmartPointer(CIvfWin32Window);

/**
 * Ivf++ native Win32 window class
 *
 * Implements a native Win32 window for use with Ivf++
 */
class IVFWIN32_API CIvfWin32Window : public CIvfWidgetBase {
private:
	HDC			m_hDC;		
	HGLRC		m_hRC;		
	HWND		m_hWnd;		
	HINSTANCE	m_hInstance;		

	int			m_size[2];
	int			m_pos[2];
	int			m_colorBits;

	LARGE_INTEGER m_countsPerSec;
	double		m_elapsedTime;
	double		m_zeroTime;

	bool		m_fullscreen;
	bool		m_multisample;
	bool		m_active;

	bool		m_created;

	static int	m_classRegistered;

	static CIvfWin32Window* m_self;

	std::string m_caption;

	bool createWindow();
	void destroyWindow();

	LRESULT CALLBACK WndProc(	HWND	hWnd,			
										UINT	uMsg,			
										WPARAM	wParam,			
										LPARAM	lParam);

	double largeToDouble(LARGE_INTEGER &value);
public:
	/** Class constructor */
	CIvfWin32Window(int X, int Y, int W, int H);

	/** Class destructor */
	~CIvfWin32Window();

	/** Shows window */
	void show();

	/** Set window caption */
	void setCaption(const char* caption);

	/** Enable/Disable fullscreen */
	void setFullscreen(bool flag);

	/** Enable/Disable multisample support */
	void setMultisample(bool flag);

	/** Set color depth */
	void setColorBits(int bits);

	/** Return window handle */
	HWND getHandle();

	/** Return handle to display context (DC) */
	HDC getDC();

	/** Return handle to current rendering context (RC) */
	HGLRC getRC();

	/** Activate window */
	void setActive(bool flag);

	virtual void doDestroy();
	virtual void doRedraw();
	virtual void doEnableTimeout(float time, int nbr);
	virtual void doDisableTimeout(int nbr);
	virtual void doDisableIdleProcessing();
	virtual void doEnableIdleProcessing();
	virtual double doElapsedTime();
};

#endif
