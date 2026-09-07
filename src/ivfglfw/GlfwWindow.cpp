#include <ivfglfw/GlfwWindow.h>

#include <ivf/GL.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <ivfglfw/GlfwApplication.h>

#include <cstdlib>
#include <iostream>

using namespace ivf;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

GlfwWindow::GlfwWindow(int X, int Y, int W, int H, bool fullScreen)
    : WidgetBase(), m_window(nullptr), m_monitor(nullptr), m_caption("Ivf++ Window"), m_modeRefresh(0),
      m_modeValid(false), m_fullScreen(fullScreen), m_blinnPhongEnabled(false), m_zeroTime(0.0), m_contextMajor(3),
      m_contextMinor(2), m_contextProfile(GLContextProfile::Compatibility), m_forwardCompatible(false),
      m_debugContext(false), m_samples(0), m_versionSetExplicitly(false), m_redrawPending(true),
      m_destroyDispatched(false)
{
    m_modeSize[0] = 0;
    m_modeSize[1] = 0;

    m_pos[0] = X;
    m_pos[1] = Y;
    m_size[0] = W;
    m_size[1] = H;
    m_fbSize[0] = W;
    m_fbSize[1] = H;

    m_globalAmbient[0] = 0.2f;
    m_globalAmbient[1] = 0.2f;
    m_globalAmbient[2] = 0.2f;
    m_globalAmbient[3] = 1.0f;

    for (int i = 0; i < 10; i++)
    {
        m_timerDeadline[i] = 0.0;
        m_timerEnabled[i] = false;
    }

    // Registering with the application also initialises GLFW, which has to
    // happen before glfwGetTime() means anything.

    GlfwApplication *application = GlfwApplication::getInstance();
    application->addWindow(this);

    m_zeroTime = glfwGetTime();
}

GlfwWindow::~GlfwWindow()
{
    this->destroy();

    GlfwApplication *application = GlfwApplication::getInstance();
    if (application != nullptr)
        application->removeWindow(this);
}

// ---------------------------------------------------------------------------
// Window creation
// ---------------------------------------------------------------------------

void GlfwWindow::applyWindowHints()
{
    glfwDefaultWindowHints();

    // A core profile has to be forward compatible on macOS and does no harm
    // anywhere else, so it is implied rather than left to the caller.

    if (m_contextProfile == GLContextProfile::Core)
    {
        if (!m_versionSetExplicitly)
        {
            m_contextMajor = 3;
            m_contextMinor = 3;
        }

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    }
    else
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, m_forwardCompatible ? GLFW_TRUE : GLFW_FALSE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_contextMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_contextMinor);

    if (m_debugContext)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    if (m_samples > 0)
        glfwWindowHint(GLFW_SAMPLES, m_samples);

    // The rest of the visual comes from the display mode the application was
    // given, which is applied on top of these.

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

void GlfwWindow::create()
{
    if (m_window != nullptr)
        return;

    GlfwApplication *application = GlfwApplication::getInstance();

    // A window that has not chosen a profile of its own follows the
    // application default, which is where a command line profile lands.

    if (application->hasDefaultRenderProfile())
    {
        RenderProfile profile = application->defaultRenderProfile();
        if (profile == RenderProfile::Core)
            m_contextProfile = GLContextProfile::Core;
        rcSetProfile(profile);
    }

    this->applyWindowHints();

    // The display mode hints are applied by the application, since that is
    // where the IVF_* flags were set.

    application->applyDisplayModeHints();

    if (m_fullScreen)
        m_monitor = glfwGetPrimaryMonitor();

    int width = m_size[0];
    int height = m_size[1];

    if (m_monitor != nullptr)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(m_monitor);
        if (mode != nullptr)
        {
            width = mode->width;
            height = mode->height;
        }
    }

    m_window = glfwCreateWindow(width, height, m_caption.c_str(), m_monitor, nullptr);

    if (m_window == nullptr)
    {
        std::cout << "GlfwWindow: failed to create a "
                  << ((m_contextProfile == GLContextProfile::Core) ? "core" : "compatibility") << " profile "
                  << m_contextMajor << "." << m_contextMinor << " context." << std::endl;
        return;
    }

    glfwSetWindowUserPointer(m_window, this);

    if (m_monitor == nullptr)
        glfwSetWindowPos(m_window, m_pos[0], m_pos[1]);

    glfwSetFramebufferSizeCallback(m_window, cbFramebufferSize);
    glfwSetWindowPosCallback(m_window, cbWindowPos);
    glfwSetMouseButtonCallback(m_window, cbMouseButton);
    glfwSetCursorPosCallback(m_window, cbCursorPos);
    glfwSetCursorEnterCallback(m_window, cbCursorEnter);
    glfwSetCharCallback(m_window, cbChar);
    glfwSetKeyCallback(m_window, cbKey);
    glfwSetWindowFocusCallback(m_window, cbWindowFocus);
    glfwSetWindowRefreshCallback(m_window, cbWindowRefresh);

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "GlfwWindow: failed to load OpenGL entry points." << std::endl;

    this->updateFramebufferSize();

    // GLUT delivered a reshape before the first display, and that is where
    // WidgetBase runs onInit(). GLFW only reports a size when it changes, so
    // the first one is delivered by hand.

    this->doResize(m_fbSize[0], m_fbSize[1]);

    m_redrawPending = true;
}

bool GlfwWindow::isCreated() const
{
    return m_window != nullptr;
}

void GlfwWindow::show()
{
    if (m_window == nullptr)
        this->create();

    if (m_window != nullptr)
        glfwShowWindow(m_window);
}

void GlfwWindow::hide()
{
    if (m_window != nullptr)
        glfwHideWindow(m_window);
}

void GlfwWindow::destroy()
{
    if (m_window == nullptr)
        return;

    glfwDestroyWindow(m_window);
    m_window = nullptr;
}

void GlfwWindow::close()
{
    if (m_window != nullptr)
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

bool GlfwWindow::isClosing() const
{
    if (m_window == nullptr)
        return false;

    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}

void GlfwWindow::makeCurrent()
{
    if (m_window != nullptr)
        glfwMakeContextCurrent(m_window);
}

void GlfwWindow::swapBuffers()
{
    if (m_window != nullptr)
        glfwSwapBuffers(m_window);
}

GLFWwindow *GlfwWindow::handle() const
{
    return m_window;
}

void GlfwWindow::updateFramebufferSize()
{
    if (m_window == nullptr)
        return;

    glfwGetFramebufferSize(m_window, &m_fbSize[0], &m_fbSize[1]);
    glfwGetWindowSize(m_window, &m_size[0], &m_size[1]);
}

// ---------------------------------------------------------------------------
// Caption
// ---------------------------------------------------------------------------

void GlfwWindow::setCaption(const std::string &caption)
{
    m_caption = caption;

    if (m_window != nullptr)
        glfwSetWindowTitle(m_window, m_caption.c_str());
}

const std::string GlfwWindow::getCaption()
{
    return m_caption;
}

void GlfwWindow::setWindowTitle(const std::string &title)
{
    this->setCaption(title);
}

const std::string GlfwWindow::getWindowTitle()
{
    return this->getCaption();
}

// ---------------------------------------------------------------------------
// Context configuration
// ---------------------------------------------------------------------------

void GlfwWindow::setContextVersion(int major, int minor)
{
    if (m_window != nullptr)
    {
        std::cout << "GlfwWindow: setContextVersion() ignored, the context already exists." << std::endl;
        return;
    }

    m_contextMajor = major;
    m_contextMinor = minor;
    m_versionSetExplicitly = true;
}

void GlfwWindow::getContextVersion(int &major, int &minor) const
{
    major = m_contextMajor;
    minor = m_contextMinor;
}

void GlfwWindow::setContextProfile(GLContextProfile profile)
{
    if (m_window != nullptr)
    {
        std::cout << "GlfwWindow: setContextProfile() ignored, the context already exists." << std::endl;
        return;
    }

    m_contextProfile = profile;
}

GLContextProfile GlfwWindow::contextProfile() const
{
    return m_contextProfile;
}

void GlfwWindow::setForwardCompatible(bool flag)
{
    m_forwardCompatible = flag;
}

bool GlfwWindow::forwardCompatible() const
{
    return m_forwardCompatible;
}

void GlfwWindow::setDebugContext(bool flag)
{
    m_debugContext = flag;
}

bool GlfwWindow::debugContext() const
{
    return m_debugContext;
}

void GlfwWindow::setSamples(int samples)
{
    m_samples = samples;
}

int GlfwWindow::samples() const
{
    return m_samples;
}

bool GlfwWindow::isCoreContext() const
{
    if (m_window == nullptr)
        return m_contextProfile == GLContextProfile::Core;

    GLint mask = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);

    return (mask & GL_CONTEXT_CORE_PROFILE_BIT) != 0;
}

bool GlfwWindow::setRenderProfile(RenderProfile profile)
{
    if (m_window == nullptr)
    {
        // Before creation the context follows the render profile, which is the
        // only way a Core profile gets a context that can prove anything.

        m_contextProfile = (profile == RenderProfile::Core) ? GLContextProfile::Core : GLContextProfile::Compatibility;
        rcSetProfile(profile);
        return true;
    }

    if ((profile != RenderProfile::Core) && this->isCoreContext())
    {
        std::cout << "GlfwWindow: cannot select a fixed function render profile on a core context." << std::endl;
        return false;
    }

    rcSetProfile(profile);
    return true;
}

RenderProfile GlfwWindow::renderProfile() const
{
    return rcProfile();
}

// ---------------------------------------------------------------------------
// Shading
// ---------------------------------------------------------------------------

void GlfwWindow::enableBlinnPhongShader(float ambR, float ambG, float ambB, float ambA)
{
    m_globalAmbient[0] = ambR;
    m_globalAmbient[1] = ambG;
    m_globalAmbient[2] = ambB;
    m_globalAmbient[3] = ambA;
    rcUseBlinnPhong();
    m_blinnPhongEnabled = true;
}

void GlfwWindow::disableBlinnPhongShader()
{
    m_blinnPhongEnabled = false;
    rcSetShader(nullptr);

    // Turning the shader off means the fixed-function pipeline is the only one
    // left, so say so rather than leaving the profile claiming otherwise.

    rcSetProfile(RenderProfile::Legacy);
}

bool GlfwWindow::isBlinnPhongShaderEnabled() const
{
    return m_blinnPhongEnabled;
}

// ---------------------------------------------------------------------------
// Fullscreen
// ---------------------------------------------------------------------------

void GlfwWindow::setModeString(const std::string &modeString)
{
    m_modeString = modeString;
    this->parseModeString();
}

const std::string GlfwWindow::getModeString()
{
    return m_modeString;
}

void GlfwWindow::parseModeString()
{
    m_modeValid = false;
    m_modeSize[0] = 0;
    m_modeSize[1] = 0;
    m_modeRefresh = 0;

    // "WIDTHxHEIGHT", optionally ":BITS" and "@REFRESH". The bit depth is
    // parsed only so that it can be skipped.

    int width = 0;
    int height = 0;
    int refresh = 0;

    size_t x = m_modeString.find('x');
    if (x == std::string::npos)
        return;

    std::string widthPart = m_modeString.substr(0, x);
    std::string rest = m_modeString.substr(x + 1);

    size_t at = rest.find('@');
    if (at != std::string::npos)
    {
        std::string refreshPart = rest.substr(at + 1);
        rest = rest.substr(0, at);
        refresh = std::atoi(refreshPart.c_str());
    }

    size_t colon = rest.find(':');
    if (colon != std::string::npos)
        rest = rest.substr(0, colon);

    width = std::atoi(widthPart.c_str());
    height = std::atoi(rest.c_str());

    if ((width <= 0) || (height <= 0))
        return;

    m_modeSize[0] = width;
    m_modeSize[1] = height;
    m_modeRefresh = refresh;
    m_modeValid = true;
}

void GlfwWindow::enterFullscreen()
{
    if (m_window == nullptr)
    {
        m_fullScreen = true;
        return;
    }

    if (m_monitor != nullptr)
        return;

    glfwGetWindowPos(m_window, &m_pos[0], &m_pos[1]);
    glfwGetWindowSize(m_window, &m_size[0], &m_size[1]);

    m_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(m_monitor);

    if (mode == nullptr)
    {
        m_monitor = nullptr;
        return;
    }

    int width = mode->width;
    int height = mode->height;
    int refresh = mode->refreshRate;

    if (m_modeValid)
    {
        // Pick the reported mode closest to what was asked for, rather than
        // handing GLFW a resolution the monitor does not have.

        int count = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(m_monitor, &count);
        long bestScore = -1;

        for (int i = 0; i < count; i++)
        {
            long dw = modes[i].width - m_modeSize[0];
            long dh = modes[i].height - m_modeSize[1];
            long score = dw * dw + dh * dh;

            if (m_modeRefresh > 0)
            {
                long dr = modes[i].refreshRate - m_modeRefresh;
                score += dr * dr;
            }

            if ((bestScore < 0) || (score < bestScore))
            {
                bestScore = score;
                width = modes[i].width;
                height = modes[i].height;
                refresh = modes[i].refreshRate;
            }
        }
    }

    glfwSetWindowMonitor(m_window, m_monitor, 0, 0, width, height, refresh);
    m_fullScreen = true;

    this->updateFramebufferSize();
    this->doResize(m_fbSize[0], m_fbSize[1]);
}

void GlfwWindow::leaveFullscreen()
{
    if ((m_window == nullptr) || (m_monitor == nullptr))
        return;

    glfwSetWindowMonitor(m_window, nullptr, m_pos[0], m_pos[1], m_size[0], m_size[1], 0);
    m_monitor = nullptr;
    m_fullScreen = false;

    this->updateFramebufferSize();
    this->doResize(m_fbSize[0], m_fbSize[1]);
}

bool GlfwWindow::isFullscreen() const
{
    return m_monitor != nullptr;
}

// ---------------------------------------------------------------------------
// Timers
// ---------------------------------------------------------------------------

void GlfwWindow::dispatchTimeout(int nbr)
{
    bool repeat = false;

    switch (nbr) {
    case 0: repeat = this->doTimeout0(); break;
    case 1: repeat = this->doTimeout1(); break;
    case 2: repeat = this->doTimeout2(); break;
    case 3: repeat = this->doTimeout3(); break;
    case 4: repeat = this->doTimeout4(); break;
    case 5: repeat = this->doTimeout5(); break;
    case 6: repeat = this->doTimeout6(); break;
    case 7: repeat = this->doTimeout7(); break;
    case 8: repeat = this->doTimeout8(); break;
    case 9: repeat = this->doTimeout9(); break;
    default: return;
    }

    if (repeat)
        m_timerDeadline[nbr] = glfwGetTime() + (double)this->getTimeout(nbr);
    else
        m_timerEnabled[nbr] = false;
}

void GlfwWindow::processTimers()
{
    double now = glfwGetTime();

    for (int i = 0; i < 10; i++)
    {
        if (m_timerEnabled[i] && (now >= m_timerDeadline[i]))
            this->dispatchTimeout(i);
    }
}

double GlfwWindow::nextTimerDelay() const
{
    double now = glfwGetTime();
    double delay = -1.0;

    for (int i = 0; i < 10; i++)
    {
        if (!m_timerEnabled[i])
            continue;

        double remaining = m_timerDeadline[i] - now;
        if (remaining < 0.0)
            remaining = 0.0;

        if ((delay < 0.0) || (remaining < delay))
            delay = remaining;
    }

    return delay;
}

void GlfwWindow::doEnableTimeout(float time, int nbr)
{
    if ((nbr < 0) || (nbr > 9))
        return;

    m_timerEnabled[nbr] = true;
    m_timerDeadline[nbr] = glfwGetTime() + (double)time;
}

void GlfwWindow::doDisableTimeout(int nbr)
{
    if ((nbr < 0) || (nbr > 9))
        return;

    m_timerEnabled[nbr] = false;
}

// ---------------------------------------------------------------------------
// Frame driving
// ---------------------------------------------------------------------------

void GlfwWindow::drawFrame(bool force)
{
    if (m_window == nullptr)
        return;

    if (!force && !m_redrawPending)
        return;

    m_redrawPending = false;

    glfwMakeContextCurrent(m_window);
    this->doDraw();
    glfwSwapBuffers(m_window);
}

void GlfwWindow::notifyDestroy()
{
    if (m_destroyDispatched)
        return;

    m_destroyDispatched = true;
    this->doDestroy();
}

void GlfwWindow::doRedraw()
{
    m_redrawPending = true;
}

double GlfwWindow::doElapsedTime()
{
    return glfwGetTime() - m_zeroTime;
}

void GlfwWindow::doFullscreen()
{
    this->enterFullscreen();
}

void GlfwWindow::doSetPosition(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;

    if ((m_window != nullptr) && (m_monitor == nullptr))
        glfwSetWindowPos(m_window, x, y);
}

void GlfwWindow::doSetSize(int w, int h)
{
    m_size[0] = w;
    m_size[1] = h;

    if ((m_window != nullptr) && (m_monitor == nullptr))
        glfwSetWindowSize(m_window, w, h);
}

void GlfwWindow::doRender()
{
    rcBeginFrame();
    if (m_blinnPhongEnabled)
        rcSetGlobalAmbient(m_globalAmbient[0], m_globalAmbient[1], m_globalAmbient[2], m_globalAmbient[3]);
    WidgetBase::doRender();
}

// ---------------------------------------------------------------------------
// Callbacks
//
// GLFW carries a user pointer on the window, so a single trampoline per event
// replaces the ten-way macro expansion the GLUT layer needed.
// ---------------------------------------------------------------------------

GlfwWindow *GlfwWindow::self(GLFWwindow *window)
{
    return static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));
}

void GlfwWindow::cbFramebufferSize(GLFWwindow *window, int width, int height)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    // A minimised window reports a zero sized framebuffer. Passing that on
    // would set a zero viewport and divide by zero in the camera aspect.

    if ((width <= 0) || (height <= 0))
        return;

    self->updateFramebufferSize();

    glfwMakeContextCurrent(window);
    self->doResize(width, height);
    self->m_redrawPending = true;
}

void GlfwWindow::cbWindowPos(GLFWwindow *window, int x, int y)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    if (self->m_monitor == nullptr)
    {
        self->m_pos[0] = x;
        self->m_pos[1] = y;
    }
}

void GlfwWindow::cbMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    // Mouse coordinates are reported in window coordinates and the widget
    // layer works in framebuffer pixels, which differ on a hi-dpi display.
    // Scaling here keeps picking lined up with the viewport.

    int windowWidth = (self->m_size[0] > 0) ? self->m_size[0] : 1;
    int windowHeight = (self->m_size[1] > 0) ? self->m_size[1] : 1;

    int x = (int)(cursorX * (double)self->m_fbSize[0] / (double)windowWidth);
    int y = (int)(cursorY * (double)self->m_fbSize[1] / (double)windowHeight);

    self->clearMouseStatus();

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        self->setLeftButtonStatus(true);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        self->setRightButtonStatus(true);
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        self->setMiddleButtonStatus(true);
        break;
    }

    if (action == GLFW_PRESS)
    {
        if (mods & GLFW_MOD_SHIFT)
            self->setModifierKey(WidgetBase::MT_SHIFT);
        else if (mods & GLFW_MOD_CONTROL)
            self->setModifierKey(WidgetBase::MT_CTRL);
        else if (mods & GLFW_MOD_ALT)
            self->setModifierKey(WidgetBase::MT_ALT);
        else
            self->setModifierKey(WidgetBase::MT_NONE);

        self->doMouseDown(x, y);
    }
    else if (action == GLFW_RELEASE)
    {
        self->setModifierKey(WidgetBase::MT_NONE);
        self->doMouseUp(x, y);
        self->clearMouseStatus();
    }
}

void GlfwWindow::cbCursorPos(GLFWwindow *window, double x, double y)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    int windowWidth = (self->m_size[0] > 0) ? self->m_size[0] : 1;
    int windowHeight = (self->m_size[1] > 0) ? self->m_size[1] : 1;

    int scaledX = (int)(x * (double)self->m_fbSize[0] / (double)windowWidth);
    int scaledY = (int)(y * (double)self->m_fbSize[1] / (double)windowHeight);

    self->doMouseMove(scaledX, scaledY);
}

void GlfwWindow::cbCursorEnter(GLFWwindow *window, int entered)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    if (entered == GLFW_TRUE)
        self->doMouseEnter((int)cursorX, (int)cursorY);
    else
        self->doMouseLeave((int)cursorX, (int)cursorY);
}

void GlfwWindow::cbChar(GLFWwindow *window, unsigned int codepoint)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    self->doKeyboard((int)codepoint, (int)cursorX, (int)cursorY);
}

void GlfwWindow::cbKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    if ((action != GLFW_PRESS) && (action != GLFW_REPEAT))
        return;

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    int x = (int)cursorX;
    int y = (int)cursorY;

    // The character callback does not report the control keys, and GLUT's
    // keyboard callback did. Applications that quit on 27 keep working.

    switch (key) {
    case GLFW_KEY_ESCAPE:
        self->doKeyboard(27, x, y);
        break;
    case GLFW_KEY_ENTER:
        self->doKeyboard(13, x, y);
        break;
    case GLFW_KEY_TAB:
        self->doKeyboard(9, x, y);
        break;
    case GLFW_KEY_BACKSPACE:
        self->doKeyboard(8, x, y);
        break;
    case GLFW_KEY_DELETE:
        self->doKeyboard(127, x, y);
        break;
    default:
        break;
    }

    // Function keys, which the GLUT layer never wired up at all.

    TFunctionKey functionKey;
    bool haveFunctionKey = true;

    if ((key >= GLFW_KEY_F1) && (key <= GLFW_KEY_F12))
        functionKey = (TFunctionKey)(FK_F1 + (key - GLFW_KEY_F1));
    else if ((key >= GLFW_KEY_KP_0) && (key <= GLFW_KEY_KP_9))
        functionKey = (TFunctionKey)(FK_KP0 + (key - GLFW_KEY_KP_0));
    else
    {
        switch (key) {
        case GLFW_KEY_BACKSPACE:    functionKey = FK_BACKSPACE; break;
        case GLFW_KEY_TAB:          functionKey = FK_TAB; break;
        case GLFW_KEY_ENTER:        functionKey = FK_ENTER; break;
        case GLFW_KEY_PAUSE:        functionKey = FK_PAUSE; break;
        case GLFW_KEY_SCROLL_LOCK:  functionKey = FK_SCROLL_LOCK; break;
        case GLFW_KEY_ESCAPE:       functionKey = FK_ESCAPE; break;
        case GLFW_KEY_HOME:         functionKey = FK_HOME; break;
        case GLFW_KEY_LEFT:         functionKey = FK_LEFT; break;
        case GLFW_KEY_UP:           functionKey = FK_UP; break;
        case GLFW_KEY_RIGHT:        functionKey = FK_RIGHT; break;
        case GLFW_KEY_DOWN:         functionKey = FK_DOWN; break;
        case GLFW_KEY_PAGE_UP:      functionKey = FK_PAGE_UP; break;
        case GLFW_KEY_PAGE_DOWN:    functionKey = FK_PAGE_DOWN; break;
        case GLFW_KEY_END:          functionKey = FK_END; break;
        case GLFW_KEY_PRINT_SCREEN: functionKey = FK_PRINT; break;
        case GLFW_KEY_INSERT:       functionKey = FK_INSERT; break;
        case GLFW_KEY_MENU:         functionKey = FK_MENU; break;
        case GLFW_KEY_NUM_LOCK:     functionKey = FK_NUM_LOCK; break;
        case GLFW_KEY_KP_ENTER:     functionKey = FK_KP_ENTER; break;
        case GLFW_KEY_LEFT_SHIFT:   functionKey = FK_SHIFT_L; break;
        case GLFW_KEY_RIGHT_SHIFT:  functionKey = FK_SHIFT_R; break;
        case GLFW_KEY_LEFT_CONTROL: functionKey = FK_CONTROL_L; break;
        case GLFW_KEY_RIGHT_CONTROL:functionKey = FK_CONTROL_R; break;
        case GLFW_KEY_CAPS_LOCK:    functionKey = FK_CAPS_LOCK; break;
        case GLFW_KEY_LEFT_SUPER:   functionKey = FK_META_L; break;
        case GLFW_KEY_RIGHT_SUPER:  functionKey = FK_META_R; break;
        case GLFW_KEY_LEFT_ALT:     functionKey = FK_ALT_L; break;
        case GLFW_KEY_RIGHT_ALT:    functionKey = FK_ALT_R; break;
        case GLFW_KEY_DELETE:       functionKey = FK_DELETE; break;
        default:
            haveFunctionKey = false;
            functionKey = FK_BUTTON;
            break;
        }
    }

    if (haveFunctionKey)
        self->doFunctionKey(functionKey, x, y);
}

void GlfwWindow::cbWindowFocus(GLFWwindow *window, int focused)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    if (focused == GLFW_TRUE)
        self->doFocus();
    else
        self->doUnFocus();
}

void GlfwWindow::cbWindowRefresh(GLFWwindow *window)
{
    GlfwWindow *self = GlfwWindow::self(window);
    if (self == nullptr)
        return;

    self->m_redrawPending = true;
}
