#include <FL/Fl.H>

#include "RobotWindow.h"

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
    Fl::visual(FL_RGB|FL_DOUBLE);
	Fl::get_system_colors();

	CRobotWindow* window = new CRobotWindow();
	window->show();

	int result = Fl::run();

	delete window;

	return result;
}
