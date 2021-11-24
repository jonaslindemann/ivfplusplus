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

#ifndef _CIvfWin32Application_h_
#define _CIvfWin32Application_h_

#include <windows.h>
#include <ivf/IvfBase.h>

#include <map>

class CIvfWin32Window;

IvfSmartPointer(CIvfWin32Application);

/**
 * Ivf++ Win32 application class
 *
 * This class implements a native Ivf++ Win32 application.
 * This application behaves in the same way as an application
 * created using the ivffltk library.
 */
class IVFWIN32_API CIvfWin32Application : public CIvfBase {
private:
	std::map<HWND,CIvfWin32Window*> m_windows;
	static CIvfWin32Application* m_instance;
	int m_idleProcessing;
public:
	/** Class destructor */
	~CIvfWin32Application();

	/** Disable idle processing */
	void disableIdleProcessing();

	/** Enables idle processing */
	void enableIdleProcessing();

	/** Returns instance of the CIvfWin32Application */
	static CIvfWin32Application* getInstance();

	/** Register CIvfWin32Window with the CIvfWin32Application */
	void registerWindow(CIvfWin32Window* window);

	/** Unregisters a CIvfWin32Window with CIvfWin32Application */
	void unregisterWindow(CIvfWin32Window* window);

	/** Returns an instance of an CIvfWin32Window from a windows handle */
	CIvfWin32Window* getWindow(HWND hWnd);

	/** Main Ivf++ application loop */
	int run();

	/**
	 * Special application loop for real-time apps
	 *
	 * Calls the onAppLoop events repeatedly in CIvfWidgetBase.
	 */
	void runAppLoop(CIvfWin32Window* window);
protected:
	/** Class constructor */
	CIvfWin32Application();
};

#endif
