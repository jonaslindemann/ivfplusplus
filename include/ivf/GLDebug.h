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

#include <ivf/GL.h>
#include <ivfdef/Def.h>

#include <functional>
#include <string>

namespace ivf {

/**
 * Severity of a message reported by the driver's debug output.
 *
 * Mirrors the KHR_debug severity enums. Notification is ordinary driver
 * chatter (buffer allocation hints and the like); Low and above indicate
 * something the caller did that the driver objects to.
 */
enum class DebugSeverity {
	Notification,
	Low,
	Medium,
	High
};

/** Returns a short name for a DebugSeverity, for logging. */
IVF_API const char* debugSeverityString(DebugSeverity severity);

/**
 * Signature of a custom handler for driver debug messages.
 *
 * @param message  the driver's text, already trimmed of trailing whitespace
 * @param severity how seriously the driver takes it
 * @param id       the driver's message id, useful for suppressing a known one
 */
using DebugMessageHandler = std::function<void(const std::string& message,
                                               DebugSeverity severity,
                                               unsigned int id)>;

/**
 * Installs a driver debug-output callback on the current context.
 *
 * This is the fastest way to find out why the modern path misbehaves: instead
 * of a bare GL_INVALID_ENUM surfacing several draw calls later, the driver
 * names the offending call. Worth turning on unconditionally in the test
 * examples and in debug builds of an application.
 *
 * Requires KHR_debug or ARB_debug_output. Neither is exposed by the vendored
 * glad loader, so the entry points are resolved directly from the GL library
 * at call time. Returns false when the context offers neither extension --
 * on a plain OpenGL 3.2 context, or on macOS, that is the normal outcome and
 * is not an error.
 *
 * @param synchronous when true the driver reports each message from the thread
 *                    and call that caused it, so a breakpoint in the handler
 *                    lands on the offending call. Costs performance; leave it
 *                    on for diagnosis, off for timing runs.
 * @return true if a callback was installed.
 */
IVF_API bool enableDebugOutput(bool synchronous = true);

/** Removes the debug callback installed by enableDebugOutput(). */
IVF_API void disableDebugOutput();

/** Returns true when a debug callback is currently installed. */
IVF_API bool isDebugOutputEnabled();

/**
 * Routes debug messages somewhere other than stderr.
 *
 * Pass an empty handler to restore the default, which prints
 * "ivf GL debug [severity] id: text" to stderr.
 */
IVF_API void setDebugMessageHandler(DebugMessageHandler handler);

/**
 * Controls whether Notification-severity messages reach the handler.
 *
 * Default is false -- drivers are chatty at that level and the noise buries
 * everything else. Turn it on when chasing a problem the driver only hints at.
 */
IVF_API void setDebugReportNotifications(bool flag);

/** Returns whether Notification-severity messages are being reported. */
IVF_API bool debugReportNotifications();

/**
 * Number of messages at or above a severity seen since the last reset.
 *
 * A test harness can reset this, exercise one object, and read it back to
 * attribute driver complaints to that object. Separating the severities matters
 * because drivers report a great deal at Low and Medium that is advisory rather
 * than wrong -- an unbound sampler, a shader recompiled on a state change. Gate
 * on High and report the rest.
 *
 * Notifications are never counted, whether or not they are being reported.
 */
IVF_API int debugMessageCount(DebugSeverity minimumSeverity = DebugSeverity::Low);

/** Sets debugMessageCount() back to zero. */
IVF_API void resetDebugMessageCount();

/** Returns a readable name for a GL error enum, e.g. "GL_INVALID_ENUM". */
IVF_API const char* glErrorString(GLenum error);

/**
 * Drains glGetError() and reports every error found.
 *
 * The GL error flag latches, so a single glGetError() can hide further errors
 * behind the first; this loops until the queue is empty. Errors go to the
 * handler set by setDebugMessageHandler() at High severity, so a harness sees
 * them through the same channel as driver messages, but they do NOT count
 * towards debugMessageCount() -- the return value is the count.
 *
 * @param where a label identifying the call site, included in the message.
 * @return the number of errors drained; zero means the context is clean.
 */
IVF_API int checkGLError(const char* where);

/**
 * Drains glGetError() and discards whatever it finds.
 *
 * Use before a measurement to make sure errors already pending are not
 * attributed to the code about to run.
 */
IVF_API void clearGLErrors();

} // namespace ivf
