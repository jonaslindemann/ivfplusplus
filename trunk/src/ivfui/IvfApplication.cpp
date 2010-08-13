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

#include <ivfui/IvfApplication.h>

CIvfApplication* CIvfApplication::m_instance = 0;
CIvfSingletonDestroyer<CIvfApplication> CIvfApplication::m_destroyer;

CIvfApplication::CIvfApplication(int* argc, char** argv)
:CIvfGlutApplication(argc, argv)
{

}

CIvfApplication* CIvfApplication::getInstance(int* argc, char** argv) 
{
    if (m_instance == 0)  
    {  
		m_instance = new CIvfApplication(argc, argv); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CIvfApplication* CIvfApplication::getInstance() 
{
    if (m_instance != 0)  
    	return m_instance;
    else 
    	return 0;
}

/*
CIvfApplication::CIvfApplication(int visual)
:CIvfGlutApplicationBase(visual)
{
    Fl::visual(visual);
	Fl::get_system_colors();
}

CIvfApplication::~CIvfApplication()
{

}

int CIvfApplication::run()
{
	return Fl::run();
}

void CIvfApplication::runAppLoop(CIvfWindow *window)
{
	bool finished = false;

	while (!finished)
	{
		Fl::check();
		finished = (!window->doAppLoop())||(Fl::first_window()==NULL);
	}
}
*/