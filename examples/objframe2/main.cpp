#include <FL/Fl.H>

#include "MainFrame.h"

int
main(int argc, char **argv) 
{
	Fl::gl_visual(FL_DOUBLE|FL_RGB|FL_ALPHA);
	Fl::get_system_colors();
	
	CMainFrame *frame = new CMainFrame();
    frame->show();

    return Fl::run();

	delete frame;
}
