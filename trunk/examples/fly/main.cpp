#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWindow.h>

#include "IvfFlyWidget.h"

int
main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

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
