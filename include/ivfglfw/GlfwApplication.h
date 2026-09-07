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

#include <vector>

#include <ivfdef/SingletonDestroyer.h>
#include <ivfglfw/GlfwWindow.h>

//
// Visual flags accepted by GlfwApplication::setDisplayMode(). The values are
// the ones the GLUT layer used, so applications that pass them keep working.
// Several no longer mean anything: a modern context is always RGBA, and the
// accumulation buffer is gone. Those are accepted and ignored.
//

#ifndef IVF_RGB
#define IVF_RGB 0x0000
#define IVF_RGBA 0x0000
#define IVF_INDEX 0x0001
#define IVF_SINGLE 0x0000
#define IVF_DOUBLE 0x0002
#define IVF_ACCUM 0x0004
#define IVF_ALPHA 0x0008
#define IVF_DEPTH 0x0010
#define IVF_STENCIL 0x0020
#define IVF_MULTISAMPLE 0x0080
#define IVF_STEREO 0x0100
#define IVF_LUMINANCE 0x0200
#endif

namespace ivf {

/**
 * Application class for GLFW based Ivf++ applications.
 *
 * GlfwApplication owns the GLFW library initialisation and the main loop, and
 * is the GLFW replacement for GlutApplication. It keeps the same interface, so
 * an application ported from GLUT does not have to change its main().
 *
 * Unlike the GLUT version there is no limit on the number of windows, since
 * GLFW carries a user pointer on each window and needs no per window callback
 * trampolines.
 *
 * @author Jonas Lindemann
 */
class GlfwApplication {
private:
    static GlfwApplication *m_instance;
    static SingletonDestroyer<GlfwApplication> m_destroyer;

    std::vector<GlfwWindow *> m_windows;
    unsigned int m_displayMode;
    RenderProfile m_defaultRenderProfile;
    bool m_defaultRenderProfileSet;
    GlfwWindow *m_appLoopWindow;
    bool m_initialised;

    int mainLoop();

public:
    /**
     * Returns the application instance, creating it if needed.
     *
     * The argument form exists so that a main() written against
     * GlutApplication compiles unchanged. GLFW takes no command line
     * arguments, so they are only scanned for a render profile name -- see
     * parseProfileArgs().
     */
    static GlfwApplication *getInstance(int *argc, char **argv);

    /** Returns the application instance, creating it if needed. */
    static GlfwApplication *getInstance();

    /**
     * Sets the visual the windows are created with.
     *
     * Takes the IVF_* flags the GLUT layer used and translates them into GLFW
     * window hints.
     */
    void setDisplayMode(unsigned int mode);
    unsigned int getDisplayMode();

    /**
     * Turns the current display mode into GLFW window hints.
     *
     * Called by GlfwWindow::create() after it has applied its own context
     * hints, so that the visual and the context are chosen together.
     */
    void applyDisplayModeHints();

    /**
     * Sets the render profile new windows are created with.
     *
     * A window that has had setRenderProfile() called on it keeps its own
     * choice; this only supplies the default for the ones that have not.
     */
    void setDefaultRenderProfile(RenderProfile profile);
    RenderProfile defaultRenderProfile() const;
    bool hasDefaultRenderProfile() const;

    /**
     * Reads a render profile from the command line.
     *
     * Accepts "legacy", "mixed" and "core" as bare arguments, and
     * "--profile <name>". This gives every application the same profile switch
     * the profile_test example has, which is what makes an A/B between the
     * fixed function and shader paths a command line away.
     *
     * @return true if a profile name was found.
     */
    bool parseProfileArgs(int argc, char **argv);

    /** Registers a window with the application. */
    bool addWindow(GlfwWindow *window);

    /** Removes a window from the application. */
    void removeWindow(GlfwWindow *window);

    /** Returns the number of registered windows. */
    size_t windowCount() const;

    /** Runs the main loop until every window has closed. */
    int run();

    /**
     * Runs the main loop, calling onAppLoop() on the given window every frame.
     *
     * The loop ends when onAppLoop() returns false or the window closes. This
     * is the continuously animating form; run() only redraws when something
     * asks it to.
     */
    int runAppLoop(GlfwWindow *window);

protected:
    /** Protected constructor (do not use) */
    GlfwApplication();
    ~GlfwApplication();
    friend class SingletonDestroyer<GlfwApplication>;
};

typedef GlfwApplication *GlfwApplicationPtr;

} // namespace ivf
