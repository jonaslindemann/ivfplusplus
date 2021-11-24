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

#ifndef _CIvfMfcApplication_h_
#define _CIvfMfcApplication_h_

#include <afxwin.h>

#include <ivfdef/IvfPointer.h>

/**
 * Ivf++ MFC appliction base class
 *
 * This class is used to merge in functionality in
 * the CIvfMfcApplication class.
 */
class CIvfMfcApplicationBase {
private:
	int m_idleProcessing;
public:
	/** Ininitalise variables */
	virtual BOOL InitInstance()
	{
		m_idleProcessing = 0;
		return true;
	}

	/** disable idlde processing */
	void disableIdleProcessing()
	{
		if (m_idleProcessing>0)
			m_idleProcessing--;
	}

	/** Enable idle processing */
	void enableIdleProcessing()
	{
		m_idleProcessing++;
	}

	/** Return idle processing state */
	bool isIdleProcessing()
	{
		return m_idleProcessing>0;
	}
};

/**
 * Ivf++ MFC application class
 *
 * This class implements a complete MFC based Ivf++ application,
 * that can be used as a normal Ivf++ application.
 */
template<class T>
class CIvfMfcApplication: public CIvfMfcApplicationBase, public CWinApp {
private:
	T* m_mfcWindow;
public:
	/**
	 * WIN32 InitInstance
	 *
	 * Setup up application instance and create application main window.
	 */
	virtual BOOL InitInstance()
	{
		CWinApp::InitInstance();
		m_mfcWindow = new T(0,0,640,480);
		m_mfcWindow->setApplication(this);
		m_pMainWnd = m_mfcWindow;
		m_pMainWnd -> ShowWindow(m_nCmdShow);
		m_pMainWnd -> UpdateWindow();
		return true;
	}

	/** Applicatio loop */
	virtual int Run()
	{
		MSG		msg;
		BOOL	bRet;
		bool running = true;
		while (running)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if ((bRet = GetMessage(&msg, NULL, 0, 0)) == 0)
					return 0;
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				if (isIdleProcessing())
				{
					if (m_mfcWindow->isIdleProcessing())
						m_mfcWindow->doIdle();
				}
				else
				{
					WaitMessage();
				}
			}
		}
		return 0;
	}
};
/** \example mfcsimple.cpp
 * This example illustrates the use of the
 * CIvfMfcApplication class and the ivfmfc library
 * for creating MFC application using Ivf++
 */

#endif
