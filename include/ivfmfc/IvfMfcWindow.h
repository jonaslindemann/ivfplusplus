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
// Based on code from from the smotri.h example
//
// smotri - Simple MFC, C++, OpenGL Tutorial Program
// by:		Joel Parris
// date:	10/9/2000

#include <afxwin.h>
#include <GL/gl.h>		// OpenGL include file
#include <GL/glu.h>		// OpenGL Utilities include file
#include <cmath>

#include <ivfwidget/IvfWidgetBase.h>

#include <ivfmfc/IvfMfcApplication.h>

IvfSmartPointer(CIvfMfcWindow);

/**
 * Ivf++ MFC Window class
 *
 * This class implements a MFC window using the CFrameWnd
 * MFC class and the Ivf++ CIvfWidgetBase class. This
 * class should behave as the CIvfFltkBase class in the
 * ivffltk library.
 */
class CIvfMfcWindow: public CFrameWnd, public CIvfWidgetBase {
private:
    HDC		m_hgldc;		// GDI Device Context
    HGLRC	m_hglRC;		// Rendering Context

	LARGE_INTEGER m_countsPerSec;
	double		m_elapsedTime;
	double		m_zeroTime;

	int		m_size[2];
	int		m_pos[2];
	int		m_colorBits;

	int		m_mousePos[2];

	CIvfMfcApplicationBase* m_application;

	double largeToDouble(LARGE_INTEGER &value);
public:
	/**
	 * Class constructor
	 *
	 * @param X window x position
	 * @param Y window y position
	 * @param W window width
	 * @param H window height
	 */
	CIvfMfcWindow(int X, int Y, int W, int H);

	/** Class destructor */
	virtual ~CIvfMfcWindow();

	/** Set the application handling the window */
	void setApplication(CIvfMfcApplicationBase* app);

	/** Setup pixel format */
    BOOL SetPixelformat(HDC hdc);

	/** PreCreate window */
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	virtual void doInit(int width, int height);
	virtual void doDestroy();
	virtual void doRedraw();
	virtual void doEnableTimeout(float time, int nbr);
	virtual void doDisableTimeout(int nbr);
	virtual void doDisableIdleProcessing();
	virtual void doEnableIdleProcessing();
	virtual double doElapsedTime();
	virtual void doSetPosition(int x, int y);
	virtual void doSetSize(int w, int h);

protected:

    afx_msg void OnPaint();
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy );
    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};
/** \example mfcsimple.cpp
 * This example illustrates the use of CIvfMfcWindow and the ivfmfc library
 * for creating MFC application using Ivf++
 */
