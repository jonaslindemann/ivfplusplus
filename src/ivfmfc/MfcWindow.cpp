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

//  This class is based on MFC code by Joel Parris

//  smotri - Simple MFC, C++, OpenGL Tutorial Program
//  by:		 Joel Parris
//  date:	 1/21/2001 

#include <ivfmfc/IvfMfcWindow.h>

#include <ivf/IvfLighting.h>
#include <ivf/IvfRasterization.h>
#include <ivf/IvfPixelOps.h>

BEGIN_MESSAGE_MAP(CIvfMfcWindow, CFrameWnd)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYUP()
	ON_WM_CHAR()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CIvfMfcWindow::CIvfMfcWindow(int X, int Y, int W, int H)
{
	m_pos[0] = X;
	m_pos[1] = Y;
	m_size[0] = W;
	m_size[1] = H;
	m_application = NULL;

	LARGE_INTEGER count;
	QueryPerformanceFrequency(&m_countsPerSec);
	QueryPerformanceCounter(&count);

	m_zeroTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	m_elapsedTime = 0.0;

	Create(	NULL, _T("Ivf++ MFC Window"), 
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC);
}

CIvfMfcWindow::~CIvfMfcWindow()
{
    wglMakeCurrent(NULL,NULL);
    wglDeleteContext(m_hglRC);
}

BOOL CIvfMfcWindow::SetPixelformat(HDC hdc)
{

    PIXELFORMATDESCRIPTOR *ppfd; 
    int pixelformat; 
 
    PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_GENERIC_FORMAT |
    PFD_DOUBLEBUFFER | PFD_STEREO,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 32-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // no alpha buffer 
    0,                     // shift bit ignored 
    8,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    64,                    // 64-bit z-buffer	 
    8,                     // stencil buffer 
    8,                     // auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 

   
    ppfd = &pfd;

 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        ::MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        ::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 

}

BOOL CIvfMfcWindow::PreCreateWindow( CREATESTRUCT& cs )
{
    if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	//	Set the initial size of the window
	
	cs.x = m_pos[0];
	cs.y = m_pos[1];
	cs.cx = m_size[0];
	cs.cy = m_size[1];

    return TRUE;
}

void CIvfMfcWindow::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		doInit(m_size[0], m_size[1]);
	}
}

BOOL CIvfMfcWindow::OnEraseBkgnd( CDC* pDC )
{
/*	This overrides the MFC Message Loop's OnEraseBkgnd(), which
	keeps the OpenGL window from flashing.  I shouldn't tell you
	this but contrary to popular opinion it doesn't matter what this
	override returns TRUE or FALSE.  If you don't believe me, try it
    for yourself.  
*/   
    return TRUE;
}

void CIvfMfcWindow::OnSize(UINT nType, int cx, int cy )
{
	doResize(cx, cy);
}

void CIvfMfcWindow::OnPaint()
{
    CPaintDC dc(this); // Needed 
	this->doDraw();
    SwapBuffers(m_hgldc);
}

int CIvfMfcWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Get a device context for OpenGL

    m_hgldc = ::GetDC(m_hWnd);		

	// set pixel format

    if(!SetPixelformat(m_hgldc))	
    {
		::MessageBox(::GetFocus(),"SetPixelformat Failed!","Error",MB_OK);
		return -1;
    }

	// Create an OpenGL rendering context 

    m_hglRC = wglCreateContext(m_hgldc);	

	// Make rendering context current
    
	wglMakeCurrent(m_hgldc,m_hglRC);	

    return 0;
}

void CIvfMfcWindow::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	this->doKeyboard(nChar, m_mousePos[0], m_mousePos[1]);
}

void CIvfMfcWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) 
	{ 
    case VK_LEFT: 
		doFunctionKey(CIvfWidgetBase::FK_LEFT, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_SHIFT: 
        this->setModifierKey(CIvfWidgetBase::MT_SHIFT);
        break; 
    case VK_CONTROL: 
        this->setModifierKey(CIvfWidgetBase::MT_CTRL);
        break; 
    case VK_RIGHT: 
		doFunctionKey(CIvfWidgetBase::FK_RIGHT, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_UP: 
		doFunctionKey(CIvfWidgetBase::FK_UP, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_DOWN: 
		doFunctionKey(CIvfWidgetBase::FK_DOWN, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_HOME: 
		doFunctionKey(CIvfWidgetBase::FK_HOME, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_END: 
		doFunctionKey(CIvfWidgetBase::FK_END, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_INSERT: 
		doFunctionKey(CIvfWidgetBase::FK_INSERT, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_DELETE: 
		doFunctionKey(CIvfWidgetBase::FK_DELETE, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F1: 
		doFunctionKey(CIvfWidgetBase::FK_F1, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F3: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F4: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F5: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F6: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F7: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F8: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F9: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F10: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F11: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    case VK_F12: 
		doFunctionKey(CIvfWidgetBase::FK_F2, m_mousePos[0], m_mousePos[1]);
        break; 
    default: 
        break; 
	} 
}

void CIvfMfcWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	this->setModifierKey(CIvfWidgetBase::MT_NONE);
}

void CIvfMfcWindow::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	this->setModifierKey(CIvfWidgetBase::MT_NONE);
}

void CIvfMfcWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	this->clearMouseStatus();
	this->setLeftButtonStatus(true);
	this->doMouseDown(point.x, point.y);
}

void CIvfMfcWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	this->clearMouseStatus();
	this->doMouseUp(point.x, point.y);
}

void CIvfMfcWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
	this->clearMouseStatus();
	this->setRightButtonStatus(true);
	this->doMouseDown(point.x, point.y);
}

void CIvfMfcWindow::OnRButtonUp(UINT nFlags, CPoint point)
{
	this->clearMouseStatus();
	this->doMouseUp(point.x, point.y);
}

void CIvfMfcWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mousePos[0] = point.x;
	m_mousePos[1] = point.y;
	this->doMouseMove(point.x, point.y);
}

void CIvfMfcWindow::OnClose()
{
	this->doDestroy();
	DestroyWindow();
}

void CIvfMfcWindow::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case 1:
		this->doTimeout0();
		break;
	case 2:
		this->doTimeout1();
		break;
	case 3:
		this->doTimeout2();
		break;
	case 4:
		this->doTimeout3();
		break;
	case 5:
		this->doTimeout4();
		break;
	case 6:
		this->doTimeout5();
		break;
	case 7:
		this->doTimeout6();
		break;
	case 8:
		this->doTimeout7();
		break;
	case 9:
		this->doTimeout8();
		break;
	case 10:
		this->doTimeout9();
		break;
	default:
		break;				
	}
}

void CIvfMfcWindow::doInit(int width, int height)
{
	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();
	lighting->setTwoSide(false);
	
	CIvfRasterizationPtr rasterOps = CIvfRasterization::getInstance();
	rasterOps->enableCullFace();
	rasterOps->setCullFace(CIvfRasterization::CF_BACK);
	
	CIvfPixelOpsPtr pixelOps = CIvfPixelOps::getInstance();
	pixelOps->enableDepthTest();
	
	glClearDepth(1.0f);							
	glDepthFunc(GL_LESS);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	onInit(width, height);
}

void CIvfMfcWindow::doDestroy()
{
	int i;

	for (i=0; i<10; i++)
		doDisableTimeout(i);

	onDestroy();
}

void CIvfMfcWindow::doRedraw()
{
	InvalidateRect(FALSE);
	UpdateWindow();	
}

void CIvfMfcWindow::doSetSize(int w, int h)
{
	m_size[0] = w;
	m_size[1] = h;

	SetWindowPos(NULL, 
		m_pos[0],
		m_pos[1],
		m_size[0],
		m_size[1],
		SWP_SHOWWINDOW
	);
}

void CIvfMfcWindow::doSetPosition(int x, int y)
{
	m_pos[0] = x;
	m_pos[1] = y;

	SetWindowPos(NULL, 
		m_pos[0],
		m_pos[1],
		m_size[0],
		m_size[1],
		SWP_SHOWWINDOW
	);
}

void CIvfMfcWindow::doEnableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		int elapse = (int)(time*1000);
		SetTimer(nbr+1, elapse, NULL);
	}
}

void CIvfMfcWindow::doDisableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		KillTimer(nbr+1);
}

void CIvfMfcWindow::doEnableIdleProcessing()
{
	if (m_application!=NULL)
		m_application->enableIdleProcessing();
}

void CIvfMfcWindow::doDisableIdleProcessing()
{
	if (m_application!=NULL)
		m_application->disableIdleProcessing();
}

double CIvfMfcWindow::doElapsedTime()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	double currentTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	return currentTime - m_zeroTime;
}

double CIvfMfcWindow::largeToDouble(LARGE_INTEGER &value)
{
	return (double)value.LowPart + pow(64.0,(double)value.HighPart);
}

void CIvfMfcWindow::setApplication(CIvfMfcApplicationBase *app)
{
	m_application = app;
}
