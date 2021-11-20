#include <ivfglut/GlutApplication.h>
#include <ivfglut/GlutBase.h>

#include "IvfFlyWidget.h"

using namespace ivf;

int
main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CGlutApplication* app = CGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGBA|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CFlyWidgetPtr window = new CFlyWidget(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Application loop example");
	window->show();

	// Enter main application loop

	app->runAppLoop(window);
	//app->run();

	return 0;
}
