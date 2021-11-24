#include <FL/Fl.H>

#include "MainFrame.h"

int
main(int argc, char **argv) 
{
    Fl::gl_visual(FL_DOUBLE|FL_RGB|FL_ALPHA|FL_MULTISAMPLE|FL_DEPTH);
    Fl::get_system_colors();
	
    auto frame = new MainFrame();
    frame->workspaceView->mode(FL_DOUBLE|FL_RGB|FL_ALPHA|FL_MULTISAMPLE|FL_DEPTH);
    frame->show();

    return Fl::run();

    delete frame;
}
