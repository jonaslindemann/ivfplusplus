//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#pragma once

#include <string>

#include <ivfwidget/WidgetBase.h>
#include <ivf/rc.h>

// GLFW's own types, forward declared so that including this header does not
// drag GLFW/glfw3.h into every application. GLFW declares both as
// "typedef struct X X", so these declarations are compatible with it.

struct GLFWwindow;
struct GLFWmonitor;

namespace ivf {

/**
 * The OpenGL context profile a window asks for at creation time.
 *
 * This is a property of the context and cannot be changed once the window
 * exists -- unlike RenderProfile, which selects what the library does with the
 * context it was given and can be changed at any time.
 */
enum class GLContextProfile {
    /** Compatibility profile. Fixed function calls are legal. */
    Compatibility,

    /** Core profile. Fixed function calls are errors. */
    Core
};

IvfSmartPointer(GlfwWindow);

/**
 * GLFW backed window for Ivf++ applications.
 *
 * GlfwWindow is the GLFW replacement for the old GlutBase class and keeps the
 * same shape, so porting an application is a matter of changing the include
 * and the base class. Everything the window does with events, drawing and
 * timers is handed to WidgetBase, exactly as before.
 *
 * The window is not created by the constructor. It is created by show(), or by
 * GlfwApplication when the main loop starts, which leaves a window in which the
 * context can be configured:
 *
 * @code
 * auto window = ExampleWindow::create(0, 0, 512, 512);
 * window->setRenderProfile(RenderProfile::Core);  // picks a core context too
 * window->setWindowTitle("Example");
 * window->show();
 * @endcode
 *
 * Context settings applied after the window exists are ignored, and say so.
 *
 * @author Jonas Lindemann
 */
class GlfwWindow : public WidgetBase {
private:
    GLFWwindow *m_window;
    GLFWmonitor *m_monitor;

    std::string m_caption;

    // Fullscreen video mode, from setModeString(). GLUT took a game mode
    // string; GLFW takes a resolution and refresh rate, so the string is
    // parsed into one rather than dropped.

    std::string m_modeString;
    int m_modeSize[2];
    int m_modeRefresh;
    bool m_modeValid;
    int m_pos[2];
    int m_size[2];
    bool m_fullScreen;

    // Framebuffer size, which differs from the window size on a hi-dpi
    // display. Everything the widget layer sees -- the size passed to
    // doResize(), and mouse coordinates -- is in framebuffer pixels, so that
    // picking and glViewport agree with each other.

    int m_fbSize[2];

    bool m_blinnPhongEnabled;
    float m_globalAmbient[4];

    double m_zeroTime;

    // Context creation hints, applied by create().

    int m_contextMajor;
    int m_contextMinor;
    GLContextProfile m_contextProfile;
    bool m_forwardCompatible;
    bool m_debugContext;
    int m_samples;
    bool m_versionSetExplicitly;

    // Timers. GLUT re-armed a one shot callback; the same is done here against
    // the frame clock, so onTimeoutN() semantics are unchanged.

    double m_timerDeadline[10];
    bool m_timerEnabled[10];

    bool m_redrawPending;
    bool m_destroyDispatched;

    void applyWindowHints();
    void applyPosition();
    void parseModeString();
    void updateFramebufferSize();
    void dispatchTimeout(int nbr);

    // Callback trampolines. GLFW carries a user pointer, so unlike GLUT there
    // is one of each rather than one per window.

    static void cbFramebufferSize(GLFWwindow *window, int width, int height);
    static void cbWindowPos(GLFWwindow *window, int x, int y);
    static void cbMouseButton(GLFWwindow *window, int button, int action, int mods);
    static void cbCursorPos(GLFWwindow *window, double x, double y);
    static void cbCursorEnter(GLFWwindow *window, int entered);
    static void cbChar(GLFWwindow *window, unsigned int codepoint);
    static void cbKey(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void cbWindowFocus(GLFWwindow *window, int focused);
    static void cbWindowRefresh(GLFWwindow *window);

    static GlfwWindow *self(GLFWwindow *window);

public:
    /**
     * GlfwWindow constructor.
     *
     * Registers the window with the application. The GLFW window itself is not
     * created until show() or the main loop runs.
     *
     * @param X window position, x
     * @param Y window position, y
     * @param W window width
     * @param H window height
     * @param fullScreen create the window fullscreen on the primary monitor
     */
    GlfwWindow(int X, int Y, int W, int H, bool fullScreen = false);

    virtual ~GlfwWindow();

    IvfClassInfo("GlfwWindow", WidgetBase);

    /** Creates the GLFW window and its OpenGL context. */
    void create();

    /** Returns true when the window and context exist. */
    bool isCreated() const;

    /** Creates the window if needed and makes it visible. */
    void show();

    /** Hides the window without destroying it. */
    void hide();

    /** Destroys the window and its context. */
    void destroy();

    /** Requests that the window close, ending the main loop. */
    void close();

    /** Returns true once the window has been asked to close. */
    bool isClosing() const;

    /** Makes this window's OpenGL context current. */
    void makeCurrent();

    /** Swaps the front and back buffers. */
    void swapBuffers();

    /** Returns the underlying GLFW window, or nullptr before create(). */
    GLFWwindow *handle() const;

    void setCaption(const std::string &caption);
    const std::string getCaption();

    void setWindowTitle(const std::string &title);
    const std::string getWindowTitle();

    /**
     * Sets the OpenGL version the context is created with.
     *
     * Has no effect after the window has been created. When left alone the
     * version follows the context profile: 3.2 for compatibility, 3.3 for core.
     */
    void setContextVersion(int major, int minor);

    /** Returns the requested context version. */
    void getContextVersion(int &major, int &minor) const;

    /**
     * Sets the context profile.
     *
     * Has no effect after the window has been created. Note that this only
     * selects the context; use setRenderProfile() to move the library with it.
     */
    void setContextProfile(GLContextProfile profile);

    /** Returns the requested context profile. */
    GLContextProfile contextProfile() const;

    /** Requests a forward compatible context. Implied by a core profile. */
    void setForwardCompatible(bool flag);
    bool forwardCompatible() const;

    /** Requests a debug context, which makes GLDebug output usable. */
    void setDebugContext(bool flag);
    bool debugContext() const;

    /** Requests multisampling with the given number of samples. */
    void setSamples(int samples);
    int samples() const;

    /**
     * Chooses the pipeline the library renders with.
     *
     * Called before the window is created this picks the matching context as
     * well: RenderProfile::Core asks for a core, forward compatible context,
     * and the other two for a compatibility context. That is the whole point of
     * the call -- a core RenderProfile on a compatibility context runs, but it
     * cannot prove anything, since the fixed function calls it is meant to
     * suppress would have worked anyway.
     *
     * Called after creation it moves the library only, and refuses to select
     * Legacy or Mixed on a core context, where the fixed function pipeline
     * those profiles need does not exist.
     *
     * @return true if the profile was applied.
     */
    bool setRenderProfile(RenderProfile profile);

    /** Returns the profile the library is rendering with. */
    RenderProfile renderProfile() const;

    /** Returns true if the live context is a core profile context. */
    bool isCoreContext() const;

    void enableBlinnPhongShader(float ambR = 0.2f, float ambG = 0.2f, float ambB = 0.2f, float ambA = 1.0f);
    void disableBlinnPhongShader();
    bool isBlinnPhongShaderEnabled() const;

    /**
     * Sets the video mode used when the window goes fullscreen.
     *
     * Accepts the GLUT game mode syntax, "WIDTHxHEIGHT", optionally followed
     * by ":BITS" and "@REFRESH". The bit depth is ignored -- it has not been
     * selectable for a long time -- and the closest mode the monitor actually
     * reports is used. An empty or unparseable string means the monitor's
     * current mode, which is what a fullscreen window normally wants.
     */
    void setModeString(const std::string &modeString);
    const std::string getModeString();

    /** Enters fullscreen on the primary monitor. */
    void enterFullscreen();

    /** Leaves fullscreen, restoring the previous position and size. */
    void leaveFullscreen();

    /** Returns true if the window is fullscreen. */
    bool isFullscreen() const;

    //
    // Frame driving. Called by GlfwApplication.
    //

    /** Fires any timeouts that have come due. */
    void processTimers();

    /** Returns the time until the next timeout, or -1.0 if none is armed. */
    double nextTimerDelay() const;

    /** Draws the window if a redraw is pending, or if force is true. */
    void drawFrame(bool force = false);

    /** Dispatches the destroy event once, on the way out of the main loop. */
    void notifyDestroy();

    //
    // WidgetBase implementation
    //

    virtual void doRedraw() override;
    virtual double doElapsedTime() override;
    virtual void doEnableTimeout(float time, int nbr) override;
    virtual void doDisableTimeout(int nbr) override;
    virtual void doFullscreen() override;
    virtual void doSetPosition(int x, int y) override;
    virtual void doSetSize(int w, int h) override;

protected:
    virtual void doRender() override;
};

} // namespace ivf
