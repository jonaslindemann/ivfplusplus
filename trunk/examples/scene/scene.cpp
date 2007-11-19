// ------------------------------------------------------------
//
// Ivf++ Scene example
//
// ------------------------------------------------------------
//
// Author: Jonas Lindemann
//

// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfui/IvfApplication.h>
#include <ivfui/IvfWorkspaceWindow.h>

#include <ivf/IvfMaterial.h>
#include <ivf/IvfSphere.h>

using namespace std;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(CExampleWindow);

class CExampleWindow: public CIvfWorkspaceWindow {
private:

public:
	CExampleWindow(int X, int Y, int W, int H)
		:CIvfWorkspaceWindow(X, Y, W, H) {};

	virtual void onInit(int width, int height);
	virtual void onCursor(double x, double y, double z);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

void CExampleWindow::onInit(int width, int height)
{
}

void CExampleWindow::onCursor(double x, double y, double z)
{
	cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char **argv) 
{
	// Create Ivf++ application object.

	CIvfApplicationPtr app = new CIvfApplication(IVF_DOUBLE|IVF_RGB);

	// Create a window

	CExampleWindowPtr window = new CExampleWindow(0, 0, 512, 512);

	// Set window title and show window

	window->setWindowTitle("Ivf++ Workspace example");
	window->show();

	// Enter main application loop

    app->run();

	return 0;
}
