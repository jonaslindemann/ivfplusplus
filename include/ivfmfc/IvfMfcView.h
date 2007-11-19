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

#ifndef _CIvfMfcView_h_
#define _CIvfMfcView_h_

#include <afxwin.h>

#include <ivfwidget/IvfWidgetBase.h>

/**
 * Ivf++ MFC view
 *
 * This class implements a Ivf++ compatible MFC view
 * for use in MFC single/multiple document interface
 * applications. Functionality is the same as in
 * all CIvfWidgetBase derived classes.
 */
class CIvfMfcView : public CWnd, public CIvfWidgetBase {
private:
    HDC				m_hDC;		
    HGLRC			m_hRC;		

	LARGE_INTEGER	m_countsPerSec;
	double			m_elapsedTime;
	double			m_zeroTime;

	int				m_size[2];
	int				m_pos[2];
	int				m_colorBits;

	int				m_mousePos[2];

	bool			m_initDone;

	double largeToDouble(LARGE_INTEGER &value);
public:
	/** Class constructor */
	CIvfMfcView();

	BOOL SetPixelformat(HDC hdc);
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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};
/** \example mfcsdi.cpp
 * This example illustrates the use of the CIvfMfcView class
 */

#endif
