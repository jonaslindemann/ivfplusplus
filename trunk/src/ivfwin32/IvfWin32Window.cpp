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

#include <ivfwin32/IvfWin32Window.h>

#include <ivfwin32/IvfWin32Application.h>

#include <ivf/IvfLighting.h>
#include <ivf/IvfRasterization.h>
#include <ivf/IvfPixelOps.h>

#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp)) 
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))

LRESULT CALLBACK WndProc(	HWND	hWnd,			
						 UINT	uMsg,			
						 WPARAM	wParam,			
						 LPARAM	lParam)			
{
	CIvfWin32Application* app = CIvfWin32Application::getInstance();
	CIvfWin32Window* window = app->getWindow(hWnd);

	int menuId;
	
	if (window!=NULL)
	{
		switch (uMsg)									
		{
		case WM_TIMER:
			switch (wParam) {
			case 1:
				window->doTimeout0();
				return 0;
			case 2:
				window->doTimeout1();
				return 0;
			case 3:
				window->doTimeout2();
				return 0;
			case 4:
				window->doTimeout3();
				return 0;
			case 5:
				window->doTimeout4();
				return 0;
			case 6:
				window->doTimeout5();
				return 0;
			case 7:
				window->doTimeout6();
				return 0;
			case 8:
				window->doTimeout7();
				return 0;
			case 9:
				window->doTimeout8();
				return 0;
			case 10:
				window->doTimeout9();
				return 0;
			default:
				break;				
			}
			break;
		case WM_ACTIVATE:							
			if (!HIWORD(wParam))					
			{
				window->setActive(true);
			}
			else
			{
				window->setActive(false);
			}
			
			return 0;
			
		case WM_COMMAND:
			menuId = LOWORD(wParam);

			window->doMenuItem(menuId);
			
			return 0;
			
		case WM_SYSCOMMAND:							
			switch (wParam)							
			{
			case SC_SCREENSAVE:					
			case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
			
			case WM_PAINT:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->doDraw();
				SwapBuffers(window->getDC());
				wglMakeCurrent( NULL, NULL );
				break;
				
			case WM_CLOSE:
				window->doDestroy();
				PostQuitMessage(0);						
				return 0;								
				
			case WM_CHAR:
				if (wParam==27)
					PostQuitMessage(0);
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->doKeyboard(wParam, 0, 0);
				wglMakeCurrent( NULL, NULL );
				//keys[wParam] = TRUE;					
				return 0;
				
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
				switch (wParam) 
				{ 
                case VK_LEFT: 
                    break; 
                case VK_SHIFT: 
                    window->setModifierKey(CIvfWidgetBase::MT_SHIFT);
					wglMakeCurrent( window->getDC(), window->getRC() );
					window->doModifierDown();
					wglMakeCurrent( NULL, NULL );
                    break; 
                case VK_CONTROL: 
                    window->setModifierKey(CIvfWidgetBase::MT_CTRL);
					wglMakeCurrent( window->getDC(), window->getRC() );
					window->doModifierDown();
					wglMakeCurrent( NULL, NULL );
                    break; 
				case VK_MENU:
                    window->setModifierKey(CIvfWidgetBase::MT_ALT);
					wglMakeCurrent( window->getDC(), window->getRC() );
					window->doModifierDown();
					wglMakeCurrent( NULL, NULL );
					break;
                case VK_RIGHT: 
                    break; 
                case VK_UP: 
                    break; 
                case VK_DOWN: 
                    break; 
                case VK_HOME: 
                    break; 
                case VK_END: 
                    break; 
                case VK_INSERT: 
                    break; 
                case VK_DELETE: 
					wglMakeCurrent( window->getDC(), window->getRC() );
					window->doFunctionKey(CIvfWidgetBase::FK_DELETE, 0, 0);
					wglMakeCurrent( NULL, NULL );
                    break; 
                case VK_F2: 
                    break; 
                default: 
                    break; 
				} 
				break;
				
			case WM_SYSKEYUP:
			case WM_KEYUP:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->doModifierUp();
				wglMakeCurrent( NULL, NULL );
				window->setModifierKey(CIvfWidgetBase::MT_NONE);
				break;
				
			case WM_SIZE:								
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->doResize(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				return 0;								
				
			case WM_LBUTTONDOWN:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->clearMouseStatus();
				window->setLeftButtonStatus(true);
				window->doMouseDown(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				break;

			case WM_MBUTTONDOWN:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->clearMouseStatus();
				window->setMiddleButtonStatus(true);
				window->doMouseDown(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				break;
				
			case WM_RBUTTONUP:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->clearMouseStatus();
				window->doMouseUp(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				break;
				
			case WM_RBUTTONDOWN:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->clearMouseStatus();
				window->setRightButtonStatus(true);
				window->doMouseDown(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				break;
			case WM_MOUSEMOVE:
				wglMakeCurrent( window->getDC(), window->getRC() );
				window->doMouseMove(LOWORD(lParam),HIWORD(lParam));
				wglMakeCurrent( NULL, NULL );
				break;
		}
	}
	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int CIvfWin32Window::m_classRegistered = 0;

CIvfWin32Window::CIvfWin32Window(int X, int Y, int W, int H)
{
	m_hDC = NULL;		
	m_hRC = NULL;		
	m_hWnd = NULL;		
	m_hInstance = NULL;		
	
	m_pos[0] = X;
	m_pos[1] = Y;
	m_size[0] = W;
	m_size[1] = H;
	m_fullscreen = false;
	m_colorBits = 32;
	
	m_created = false;

	m_hInstance			= GetModuleHandle(NULL);				
	
	WNDCLASS	wc;						

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) ::WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= m_hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "Ivf";

	if (m_classRegistered==0)
	{
		if (!RegisterClass(&wc))									
		{
			MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		}
		else
			m_classRegistered++;
	}
	else
		m_classRegistered++;

	LARGE_INTEGER count;
	QueryPerformanceFrequency(&m_countsPerSec);
	QueryPerformanceCounter(&count);

	m_zeroTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	m_elapsedTime = 0.0;
}

CIvfWin32Window::~CIvfWin32Window()
{
	CIvfWin32Application* app = CIvfWin32Application::getInstance();
	app->unregisterWindow(this);
}

bool CIvfWin32Window::createWindow()
{
	GLuint		PixelFormat;			
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	
	WindowRect.left=(long)m_pos[0];			
	WindowRect.right=(long)m_pos[0]+(long)m_size[0];
	WindowRect.top=(long)m_pos[1];				
	WindowRect.bottom=(long)m_pos[1]+(long)m_size[1];		
	
	if (m_fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= m_size[0];				
		dmScreenSettings.dmPelsHeight	= m_size[1];				
		dmScreenSettings.dmBitsPerPel	= m_colorBits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				m_fullscreen = false;		
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}
	
	if (m_fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(TRUE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		
	
	if (!(m_hWnd=CreateWindowEx(dwExStyle,							
								"Ivf",							
								m_caption.c_str(),								
								dwStyle |							
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								m_hInstance,							
								NULL)))								
	{
		destroyWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								
	}
	
	
	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
			1,											
			PFD_DRAW_TO_WINDOW |						
			PFD_SUPPORT_OPENGL |						
			PFD_DOUBLEBUFFER | PFD_STEREO,							
			PFD_TYPE_RGBA,								
			m_colorBits,										
			0, 0, 0, 0, 0, 0,							
			0, 0,				// Alpha
			0, 0, 0, 0, 0,		// Accum
			16,	
			0,											
			0,											
			PFD_MAIN_PLANE,								
			0,											
			0, 0, 0										
	};
	
	if (!(m_hDC=GetDC(m_hWnd)))							
	{
		destroyWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	
	{
		destroyWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))		
	{
		destroyWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	if (!(m_hRC=wglCreateContext(m_hDC)))				
	{
		destroyWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	if(!wglMakeCurrent(m_hDC,m_hRC))					
	{
		destroyWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	ShowWindow(m_hWnd,SW_SHOW);						
	SetForegroundWindow(m_hWnd);						
	SetFocus(m_hWnd);									
	
	CIvfWin32Application* app = CIvfWin32Application::getInstance();
	app->registerWindow(this);

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

	wglMakeCurrent( getDC(), getRC() );
	doResize(m_size[0], m_size[1]);					
	wglMakeCurrent( NULL, NULL );
	
	return true;									
}


void CIvfWin32Window::destroyWindow()
{
	if (m_fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}
	
	if (m_hRC)											
	{
		if (!wglDeleteContext(m_hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_hRC=NULL;										
	}
	
	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hDC=NULL;										
	}
	
	if (m_hWnd && !DestroyWindow(m_hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hWnd=NULL;										
	}

	if (m_classRegistered>0)
		m_classRegistered--;

	if (m_classRegistered==0)
	{
		if (!UnregisterClass("Ivf",m_hInstance))			
		{
			MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			m_hInstance=NULL;									
		}
	}
}

//void CIvfWin32Window::doResize(int width, int height)
//{
//	onResize(width, height);
//}

//void CIvfWin32Window::doInit(int width, int height)
//{
//	onInit(width, height);
//}

void CIvfWin32Window::doDestroy()
{
	int i;
	
	for (i=0; i<10; i++)
		doDisableTimeout(i);
	
	destroyWindow();

	CIvfWidgetBase::doDestroy();
}

void CIvfWin32Window::setActive(bool flag)
{
	m_active = flag;
}

HDC CIvfWin32Window::getDC()
{
	return m_hDC;
}

HWND CIvfWin32Window::getHandle()
{
	return m_hWnd;
}

void CIvfWin32Window::setFullscreen(bool flag)
{
	m_fullscreen = flag;
}

void CIvfWin32Window::show()
{
	if (!m_created)
		createWindow();
	else
	{
		ShowWindow(m_hWnd,SW_SHOW);						
		SetFocus(m_hWnd);									
	}
	
}

void CIvfWin32Window::doRedraw()
{
	InvalidateRect(m_hWnd, NULL, FALSE);
	UpdateWindow(m_hWnd);	
}

HGLRC CIvfWin32Window::getRC()
{
	return m_hRC;
}

void CIvfWin32Window::doEnableTimeout(float time, int nbr)
{
	if ((nbr>=0)&&(nbr<10))
	{
		int elapse = (int)(time*1000);
		SetTimer(m_hWnd, nbr+1, elapse, NULL);
	}
}

void CIvfWin32Window::doDisableTimeout(int nbr)
{
	if ((nbr>=0)&&(nbr<10))
		KillTimer(m_hWnd, nbr+1);
}

void CIvfWin32Window::doEnableIdleProcessing()
{
	CIvfWin32Application* app = CIvfWin32Application::getInstance();
	app->enableIdleProcessing();
}

void CIvfWin32Window::doDisableIdleProcessing()
{
	CIvfWin32Application* app = CIvfWin32Application::getInstance();
	app->disableIdleProcessing();
}

double CIvfWin32Window::doElapsedTime()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	double currentTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	return currentTime - m_zeroTime;
}

void CIvfWin32Window::setColorBits(int bits)
{
	m_colorBits = bits;
}

double CIvfWin32Window::largeToDouble(LARGE_INTEGER &value)
{
	return (double)value.LowPart + pow(64.0,(double)value.HighPart);
}

void CIvfWin32Window::setCaption(const char *caption)
{
	m_caption = caption;
}
