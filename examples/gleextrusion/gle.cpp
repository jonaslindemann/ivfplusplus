// ------------------------------------------------------------
// Include files
// ------------------------------------------------------------

#include <ivfglfw/GlfwApplication.h>
#include <ivfglfw/GlfwWindow.h>

#include <ivfwidget/MouseViewHandler.h>
#include <ivfwidget/SceneHandler.h>

#include <ivf/Axis.h>
#include <ivf/Cube.h>
#include <ivf/Lighting.h>
#include <ivf/Material.h>
#include <ivf/Scene.h>
#include <ivf/Switch.h>

#include <ivfgle/Gle.h>
#include <ivfgle/GleContour.h>
#include <ivfgle/GleSpiral.h>
#include <ivfgle/GleSpiralCylinder.h>

using namespace std;
using namespace ivf;

// ------------------------------------------------------------
// Window class definition
// ------------------------------------------------------------

IvfSmartPointer(ExampleWindow);

class ExampleWindow : public GlfwWindow
    , InitEvent
    , InitContextEvent
    , ResizeEvent
    , RenderEvent
    , ClearEvent
    , KeyboardEvent
{
private:
    ScenePtr m_scene;

    SwitchPtr m_gleShapes;

    LightingPtr m_lighting;

    MouseViewHandlerPtr m_mouseViewHandler;
    SceneHandlerPtr m_sceneHandler;

public:
    ExampleWindow(int X, int Y, int W, int H);

    static ExampleWindowPtr create(int X, int Y, int W, int H);

    virtual void onInit(int width, int height);
    virtual void onInitContext(int width, int height);
    virtual void onClear();
    virtual void onKeyboard(int key, int x, int y);
};

// ------------------------------------------------------------
// Window class implementation
// ------------------------------------------------------------

ExampleWindowPtr ExampleWindow::create(int X, int Y, int W, int H)
{
    return ExampleWindowPtr(new ExampleWindow(X, Y, W, H));
}

ExampleWindow::ExampleWindow(int X, int Y, int W, int H)
    : GlfwWindow(X, Y, W, H)
{
    addInitEvent(this);
    addInitContextEvent(this);
    addResizeEvent(this);
    addRenderEvent(this);
    addClearEvent(this);
}

void ExampleWindow::onInit(int width, int height)
{
    enableBlinnPhongShader(0.2f, 0.2f, 0.2f);

    // Setup a simple scene

    gleInitTessCache();

    m_scene = Scene::create();
    m_scene->getCamera()->setPosition(3.0, 3.0, 3.0);

    m_gleShapes = Switch::create();

    auto material = Material::create();
    material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
    material->setColorMaterial(false);

    ////////////////////////////////////////////////////////////////////
    // Testing GLE classes

    auto gle = Gle::getInstance();
    gle->setNumSides(20);
    gle->setJoinStyle(TUBE_JN_ANGLE | TUBE_NORM_EDGE);

    ////////////////////////////////////////////////////////////////////
    // Contour swept by the spiral shapes below

    auto contourArray = GleContour::create(5);
    contourArray->setCoord(0, -0.2, -0.2);
    contourArray->setCoord(1, 0.2, -0.2);
    contourArray->setCoord(2, 0.2, 0.2);
    contourArray->setCoord(3, -0.2, 0.2);
    contourArray->setCoord(4, -0.2, -0.2);
    contourArray->calcNormals();

    ////////////////////////////////////////////////////////////////////
    // Test spiral

    auto spiral = GleSpiral::create();
    spiral->setContour(contourArray);
    spiral->setContourUp(0.0, 1.0, 0.0);
    spiral->setMaterial(material);

    m_gleShapes->addChild(spiral);

    m_scene->addChild(m_gleShapes);

    ////////////////////////////////////////////////////////////////////
    // Test spiral 2

    auto spiralCyl = GleSpiralCylinder::create();
    spiralCyl->setContourUp(0.0, 1.0, 0.0);
    spiralCyl->setStartRadius(2.0);
    spiralCyl->setRadiusChangePerRev(0.0);
    spiralCyl->setStartZ(0.0);
    spiralCyl->setZChangePerRev(0.0);
    spiralCyl->setStartAngle(0.0);
    spiralCyl->setTotalSpiralAngle(270.0);
    spiralCyl->setMaterial(material);

    m_gleShapes->addChild(spiralCyl);
    m_gleShapes->setCurrentChild(m_gleShapes->getSize() - 1);

    auto axis = Axis::create();
    m_scene->addChild(axis);

    // Setup OpenGL

    m_lighting = Lighting::getInstance();
    m_lighting->enable();
    m_lighting->getLight(0)->enable();

    // Create handlers

    m_mouseViewHandler = MouseViewHandler::create(this, m_scene->getCamera());
    m_sceneHandler = SceneHandler::create(this, m_scene);
}

void ExampleWindow::onKeyboard(int key, int x, int y)
{
    m_gleShapes->cycleForward();
    redraw();
}

void ExampleWindow::onInitContext(int width, int height)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void ExampleWindow::onClear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------

int main(int argc, char** argv)
{
    // Create Ivf++ application object.

    auto app = GlfwApplication::getInstance(&argc, argv);
    app->setDisplayMode(IVF_DOUBLE | IVF_RGB | IVF_DEPTH | IVF_MULTISAMPLE);




    // Create a window

    auto window = ExampleWindow::create(0, 0, 512, 512);

    // Set window title and show window

    window->setWindowTitle("Ivf++ event handler examples");
    window->show();

    // Enter main application loop

    app->run();

    return 0;
}
