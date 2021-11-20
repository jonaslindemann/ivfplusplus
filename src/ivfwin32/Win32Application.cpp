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

#include <ivfwin32/IvfWin32Application.h>

#include <ivfwin32/IvfWin32Window.h>

using namespace std;

CIvfWin32Application* CIvfWin32Application::m_instance = 0;


CIvfWin32Application::CIvfWin32Application()
{
	m_idleProcessing = 0;
}

CIvfWin32Application::~CIvfWin32Application()
{

}

CIvfWin32Application* CIvfWin32Application::getInstance()
{
    if (m_instance == 0)  
    {  
		m_instance = new CIvfWin32Application(); 
    }
    return m_instance; 
}

int CIvfWin32Application::run()
{
	MSG		msg;									
	BOOL	bRet;
	
	bool running = true; // WaitMessage

	while (running) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if ((bRet = GetMessage(&msg, NULL, 0, 0)) == 0)
			{
				return 0;
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
		{
			if (m_idleProcessing>0)
			{
				map<HWND,CIvfWin32Window*>::iterator it;

				for (it=m_windows.begin(); it != m_windows.end(); it++ )
				{
					CIvfWin32Window* window = it->second;
					if (window->isIdleProcessing())
						window->doIdle();
				};
			}
			else
			{
				WaitMessage();	
			}
		}
	}

	/* Standard Windows loop 
	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (bRet == -1)
		{
			return -1;
		}
		else
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
	*/

	return 0;
}

void CIvfWin32Application::runAppLoop(CIvfWin32Window *window)
{
	MSG		msg;									
	BOOL	bRet;
	
	bool running = true; // WaitMessage

	while (running) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if ((bRet = GetMessage(&msg, NULL, 0, 0)) == 0)
			{
				return;
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		running = window->doAppLoop();
	}
}


CIvfWin32Window* CIvfWin32Application::getWindow(HWND hWnd)
{
	return m_windows[hWnd];	
}

void CIvfWin32Application::registerWindow(CIvfWin32Window *window)
{
	m_windows[window->getHandle()] = window;
}

void CIvfWin32Application::unregisterWindow(CIvfWin32Window *window)
{
	map<HWND,CIvfWin32Window*>::iterator it;
	it = m_windows.find(window->getHandle());
	m_windows.erase(it);
}

void CIvfWin32Application::enableIdleProcessing()
{
	m_idleProcessing++;
}

void CIvfWin32Application::disableIdleProcessing()
{
	if (m_idleProcessing>0)
		m_idleProcessing--;
}
