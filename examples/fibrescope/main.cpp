//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include <FL/Fl.H>
#include "MainFrame.h"
#include "SplashFrame.h"

int
main(int argc, char **argv) 
{
	Fl::gl_visual(FL_DOUBLE|FL_RGB|FL_ALPHA);
	Fl::get_system_colors();
	
	MainFrame* frame = new MainFrame();
	frame->center();
    frame->show();

	SplashFrame* splash = new SplashFrame();
	splash->setTimer(true);
	splash->center();
	splash->show();

    return Fl::run();

	delete splash;
	delete frame;
}
