#include <ivfglut/IvfGlutApplication.h>
#include <ivfglut/IvfGlutBase.h>

#include "IvfFlyWidget.h"

int
main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CIvfGlutApplication* app = CIvfGlutApplication::getInstance(&argc, argv);
	app->setDisplayMode(IVF_DOUBLE|IVF_RGBA|IVF_DEPTH|IVF_MULTISAMPLE);

	// Create a window

	CIvfFlyWidgetPtr window = new CIvfFlyWidget(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Application loop example");
	window->show();

	// Enter main application loop

	app->runAppLoop(window);
	//app->run();

	return 0;
}
