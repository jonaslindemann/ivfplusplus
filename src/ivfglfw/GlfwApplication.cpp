#include <ivfglfw/GlfwApplication.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>

using namespace ivf;

GlfwApplication *GlfwApplication::m_instance = nullptr;
SingletonDestroyer<GlfwApplication> GlfwApplication::m_destroyer;

static void ivfGlfwErrorCallback(int error, const char *description)
{
    std::cout << "GLFW error " << error << ": " << description << std::endl;
}

GlfwApplication::GlfwApplication()
    : m_displayMode(IVF_DOUBLE | IVF_RGBA | IVF_DEPTH), m_defaultRenderProfile(RenderProfile::Mixed),
      m_defaultRenderProfileSet(false), m_appLoopWindow(nullptr), m_initialised(false)
{
    glfwSetErrorCallback(ivfGlfwErrorCallback);

    if (!glfwInit())
    {
        std::cout << "GlfwApplication: glfwInit() failed." << std::endl;
        return;
    }

    m_initialised = true;
}

GlfwApplication::~GlfwApplication()
{
    if (m_initialised)
        glfwTerminate();
}

GlfwApplication *GlfwApplication::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new GlfwApplication();
        m_destroyer.setSingleton(m_instance);
    }

    return m_instance;
}

GlfwApplication *GlfwApplication::getInstance(int *argc, char **argv)
{
    GlfwApplication *application = getInstance();

    if ((argc != nullptr) && (argv != nullptr))
        application->parseProfileArgs(*argc, argv);

    return application;
}

// ---------------------------------------------------------------------------
// Display mode
// ---------------------------------------------------------------------------

void GlfwApplication::setDisplayMode(unsigned int mode)
{
    m_displayMode = mode;
}

unsigned int GlfwApplication::getDisplayMode()
{
    return m_displayMode;
}

void GlfwApplication::applyDisplayModeHints()
{
    glfwWindowHint(GLFW_DOUBLEBUFFER, (m_displayMode & IVF_DOUBLE) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, (m_displayMode & IVF_DEPTH) ? 24 : 0);
    glfwWindowHint(GLFW_STENCIL_BITS, (m_displayMode & IVF_STENCIL) ? 8 : 0);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STEREO, (m_displayMode & IVF_STEREO) ? GLFW_TRUE : GLFW_FALSE);

    // IVF_MULTISAMPLE never said how many samples it wanted, and GLUT's answer
    // was whatever the driver felt like. Four is the conventional default and
    // a window can ask for something else with setSamples().

    if (m_displayMode & IVF_MULTISAMPLE)
        glfwWindowHint(GLFW_SAMPLES, 4);

    // IVF_INDEX, IVF_LUMINANCE and IVF_ACCUM describe visuals that no longer
    // exist. They are accepted and ignored rather than made into an error, so
    // that a ported main() does not have to be edited to drop them.
}

// ---------------------------------------------------------------------------
// Render profile
// ---------------------------------------------------------------------------

void GlfwApplication::setDefaultRenderProfile(RenderProfile profile)
{
    m_defaultRenderProfile = profile;
    m_defaultRenderProfileSet = true;
}

RenderProfile GlfwApplication::defaultRenderProfile() const
{
    return m_defaultRenderProfile;
}

bool GlfwApplication::hasDefaultRenderProfile() const
{
    return m_defaultRenderProfileSet;
}

bool GlfwApplication::parseProfileArgs(int argc, char **argv)
{
    if (argv == nullptr)
        return false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i] == nullptr)
            continue;

        std::string arg = argv[i];

        if ((arg == "--profile") && (i + 1 < argc) && (argv[i + 1] != nullptr))
            arg = argv[++i];

        if (arg == "legacy")
        {
            this->setDefaultRenderProfile(RenderProfile::Legacy);
            return true;
        }
        else if (arg == "mixed")
        {
            this->setDefaultRenderProfile(RenderProfile::Mixed);
            return true;
        }
        else if (arg == "core")
        {
            this->setDefaultRenderProfile(RenderProfile::Core);
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
// Windows
// ---------------------------------------------------------------------------

bool GlfwApplication::addWindow(GlfwWindow *window)
{
    if (window == nullptr)
        return false;

    if (std::find(m_windows.begin(), m_windows.end(), window) != m_windows.end())
        return false;

    m_windows.push_back(window);
    return true;
}

void GlfwApplication::removeWindow(GlfwWindow *window)
{
    auto it = std::find(m_windows.begin(), m_windows.end(), window);

    if (it != m_windows.end())
        m_windows.erase(it);

    if (m_appLoopWindow == window)
        m_appLoopWindow = nullptr;
}

size_t GlfwApplication::windowCount() const
{
    return m_windows.size();
}

// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

int GlfwApplication::mainLoop()
{
    if (!m_initialised)
        return -1;

    // A window that was never shown still has to be created, which is what
    // GLUT did from the constructor.

    for (auto window : m_windows)
    {
        if (!window->isCreated())
            window->create();
    }

    bool running = true;

    while (running && !m_windows.empty())
    {
        // The app loop form animates continuously; the plain form only redraws
        // what asked to be redrawn, and sleeps in between.

        bool continuous = (m_appLoopWindow != nullptr);

        std::vector<GlfwWindow *> open;

        for (auto window : m_windows)
        {
            if (window->isClosing())
            {
                window->notifyDestroy();
                continue;
            }

            open.push_back(window);
        }

        if (open.empty())
            break;

        for (auto window : open)
        {
            window->makeCurrent();
            window->processTimers();

            if (window->isIdleProcessing())
            {
                window->doIdle();
                continuous = true;
            }

            if (window == m_appLoopWindow)
            {
                if (!window->doAppLoop())
                {
                    running = false;
                    break;
                }
            }

            window->drawFrame(window == m_appLoopWindow);
        }

        if (!running)
            break;

        if (continuous)
        {
            glfwPollEvents();
        }
        else
        {
            // Wait for something to happen, but no longer than the next timer
            // is due. Without the timeout an armed onTimeoutN() would only
            // fire when the mouse happened to move.

            double delay = -1.0;

            for (auto window : open)
            {
                double windowDelay = window->nextTimerDelay();

                if (windowDelay < 0.0)
                    continue;

                if ((delay < 0.0) || (windowDelay < delay))
                    delay = windowDelay;
            }

            if (delay < 0.0)
                glfwWaitEvents();
            else
                glfwWaitEventsTimeout(delay);
        }
    }

    for (auto window : m_windows)
        window->notifyDestroy();

    m_appLoopWindow = nullptr;

    return 0;
}

int GlfwApplication::run()
{
    m_appLoopWindow = nullptr;
    return this->mainLoop();
}

int GlfwApplication::runAppLoop(GlfwWindow *window)
{
    m_appLoopWindow = window;
    return this->mainLoop();
}
