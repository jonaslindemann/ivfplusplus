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

#include <ivf/GLDebug.h>

#include <cstdio>
#include <cstring>
#include <string>

#ifdef WIN32
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#else
#include <dlfcn.h>
#endif

using namespace ivf;

// The vendored glad loader was generated for OpenGL 3.2 compatibility with no
// extensions, so neither KHR_debug nor ARB_debug_output is declared anywhere in
// the headers. Rather than regenerate glad -- which would touch every
// translation unit in the library -- the handful of enums and entry points this
// file needs are declared locally and resolved from the GL library at runtime.

#ifndef GL_DEBUG_OUTPUT
#define GL_DEBUG_OUTPUT                  0x92E0
#endif
#ifndef GL_DEBUG_OUTPUT_SYNCHRONOUS
#define GL_DEBUG_OUTPUT_SYNCHRONOUS      0x8242
#endif
#ifndef GL_DEBUG_SEVERITY_HIGH
#define GL_DEBUG_SEVERITY_HIGH           0x9146
#define GL_DEBUG_SEVERITY_MEDIUM         0x9147
#define GL_DEBUG_SEVERITY_LOW            0x9148
#endif
#ifndef GL_DEBUG_SEVERITY_NOTIFICATION
#define GL_DEBUG_SEVERITY_NOTIFICATION   0x826B
#endif
#ifndef GL_DONT_CARE
#define GL_DONT_CARE                     0x1100
#endif

namespace {

typedef void (APIENTRY *IvfGLDebugProc)(GLenum source, GLenum type, GLuint id,
                                        GLenum severity, GLsizei length,
                                        const GLchar* message, const void* userParam);

typedef void (APIENTRY *PfnDebugMessageCallback)(IvfGLDebugProc callback, const void* userParam);
typedef void (APIENTRY *PfnDebugMessageControl)(GLenum source, GLenum type, GLenum severity,
                                                GLsizei count, const GLuint* ids, GLboolean enabled);

PfnDebugMessageCallback g_debugMessageCallback = nullptr;
PfnDebugMessageControl  g_debugMessageControl  = nullptr;

bool                g_enabled             = false;
bool                g_reportNotifications  = false;
DebugMessageHandler g_handler;

// Counted per severity, so a caller can gate on errors alone and still see how
// much the driver had to say. Index with DebugSeverity; slot 0 stays zero.

int g_messageCounts[4] = { 0, 0, 0, 0 };

// ------------------------------------------------------------
// Entry point resolution
//
// glad's own loader is not reusable here (0.1.x keeps it private), so this
// duplicates the small amount of platform glue needed for two functions.

void* resolveGLProc(const char* name)
{
#ifdef WIN32
	// wglGetProcAddress only returns extension entry points, and only for the
	// current context; core functions live in opengl32.dll. Debug output is an
	// extension, so the first call is the one that normally succeeds.

	void* p = (void*)wglGetProcAddress(name);

	// Some drivers signal "not found" with one of these rather than null.

	if ((p == nullptr) || (p == (void*)0x1) || (p == (void*)0x2) ||
	    (p == (void*)0x3) || (p == (void*)-1))
	{
		HMODULE module = GetModuleHandleA("opengl32.dll");
		p = (module != nullptr) ? (void*)GetProcAddress(module, name) : nullptr;
	}

	return p;
#elif defined(__APPLE__)
	// macOS caps out at a 4.1 core profile and ships neither debug extension.

	(void)name;
	return nullptr;
#else
	// glXGetProcAddressARB is resolved through the process image rather than
	// linked against, so this file adds no link-time dependency on GLX.

	typedef void* (*PfnGlxGetProcAddress)(const unsigned char*);

	static PfnGlxGetProcAddress glxGetProcAddress =
		(PfnGlxGetProcAddress)dlsym(RTLD_DEFAULT, "glXGetProcAddressARB");

	if (glxGetProcAddress == nullptr)
		glxGetProcAddress = (PfnGlxGetProcAddress)dlsym(RTLD_DEFAULT, "glXGetProcAddress");

	if (glxGetProcAddress != nullptr)
	{
		void* p = glxGetProcAddress((const unsigned char*)name);
		if (p != nullptr)
			return p;
	}

	return dlsym(RTLD_DEFAULT, name);
#endif
}

// ------------------------------------------------------------
bool hasExtension(const char* wanted)
{
	// glGetString(GL_EXTENSIONS) is not valid in a core profile, so walk the
	// indexed list instead. Both are available from OpenGL 3.0 onwards.

	GLint count = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);

	// A driver that refuses the query leaves count at zero and sets an error;
	// swallow it so the caller's error state is not disturbed by a probe.

	while (glGetError() != GL_NO_ERROR)
		;

	for (GLint i = 0; i < count; i++)
	{
		const GLubyte* name = glGetStringi(GL_EXTENSIONS, (GLuint)i);
		if ((name != nullptr) && (std::strcmp((const char*)name, wanted) == 0))
			return true;
	}

	return false;
}

// ------------------------------------------------------------
DebugSeverity toSeverity(GLenum glSeverity)
{
	switch (glSeverity)
	{
	case GL_DEBUG_SEVERITY_HIGH:   return DebugSeverity::High;
	case GL_DEBUG_SEVERITY_MEDIUM: return DebugSeverity::Medium;
	case GL_DEBUG_SEVERITY_LOW:    return DebugSeverity::Low;
	default:                       return DebugSeverity::Notification;
	}
}

// ------------------------------------------------------------
void report(const std::string& message, DebugSeverity severity, unsigned int id)
{
	if (g_handler)
		g_handler(message, severity, id);
	else
		std::fprintf(stderr, "ivf GL debug [%s] %u: %s\n",
		             debugSeverityString(severity), id, message.c_str());
}

// ------------------------------------------------------------
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar* message, const void* userParam)
{
	(void)source;
	(void)type;
	(void)userParam;

	DebugSeverity mapped = toSeverity(severity);

	if (mapped != DebugSeverity::Notification)
		g_messageCounts[(int)mapped]++;
	else if (!g_reportNotifications)
		return;

	// length is negative when the driver passes a null-terminated string.

	std::string text = (length < 0) ? std::string(message ? message : "")
	                                : std::string(message ? message : "", (size_t)length);

	while (!text.empty() && ((text.back() == '\n') || (text.back() == '\r') || (text.back() == ' ')))
		text.pop_back();

	report(text, mapped, (unsigned int)id);
}

} // namespace

// ------------------------------------------------------------
const char* ivf::debugSeverityString(DebugSeverity severity)
{
	switch (severity)
	{
	case DebugSeverity::High:   return "high";
	case DebugSeverity::Medium: return "medium";
	case DebugSeverity::Low:    return "low";
	default:                    return "notification";
	}
}

// ------------------------------------------------------------
bool ivf::enableDebugOutput(bool synchronous)
{
	if (g_enabled)
		return true;

	// KHR_debug is the modern spelling; ARB_debug_output is the older one with
	// the same entry points under an ARB suffix. Try both before giving up.

	if (hasExtension("GL_KHR_debug"))
	{
		g_debugMessageCallback = (PfnDebugMessageCallback)resolveGLProc("glDebugMessageCallback");
		g_debugMessageControl  = (PfnDebugMessageControl)resolveGLProc("glDebugMessageControl");
	}

	if (g_debugMessageCallback == nullptr)
	{
		if (hasExtension("GL_ARB_debug_output"))
		{
			g_debugMessageCallback = (PfnDebugMessageCallback)resolveGLProc("glDebugMessageCallbackARB");
			g_debugMessageControl  = (PfnDebugMessageControl)resolveGLProc("glDebugMessageControlARB");
		}
	}

	if (g_debugMessageCallback == nullptr)
	{
		g_debugMessageControl = nullptr;
		return false;
	}

	glEnable(GL_DEBUG_OUTPUT);

	if (synchronous)
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	g_debugMessageCallback(debugCallback, nullptr);

	// Ask for everything and filter in the callback, so setDebugReportNotifications()
	// takes effect without another round trip to the driver.

	if (g_debugMessageControl != nullptr)
		g_debugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	// Enabling debug output on a context that had it off can itself queue a
	// notification. Do not let that land in the caller's error state.

	while (glGetError() != GL_NO_ERROR)
		;

	g_enabled = true;
	return true;
}

// ------------------------------------------------------------
void ivf::disableDebugOutput()
{
	if (!g_enabled)
		return;

	if (g_debugMessageCallback != nullptr)
		g_debugMessageCallback(nullptr, nullptr);

	glDisable(GL_DEBUG_OUTPUT);
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	while (glGetError() != GL_NO_ERROR)
		;

	g_enabled = false;
}

// ------------------------------------------------------------
bool ivf::isDebugOutputEnabled()
{
	return g_enabled;
}

// ------------------------------------------------------------
void ivf::setDebugMessageHandler(DebugMessageHandler handler)
{
	g_handler = handler;
}

// ------------------------------------------------------------
void ivf::setDebugReportNotifications(bool flag)
{
	g_reportNotifications = flag;
}

// ------------------------------------------------------------
bool ivf::debugReportNotifications()
{
	return g_reportNotifications;
}

// ------------------------------------------------------------
int ivf::debugMessageCount(DebugSeverity minimumSeverity)
{
	int total = 0;

	for (int i = (int)minimumSeverity; i <= (int)DebugSeverity::High; i++)
		total += g_messageCounts[i];

	return total;
}

// ------------------------------------------------------------
void ivf::resetDebugMessageCount()
{
	for (int i = 0; i < 4; i++)
		g_messageCounts[i] = 0;
}

// ------------------------------------------------------------
const char* ivf::glErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:                      return "GL_NO_ERROR";
	case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
	case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
	default:                               return "GL_UNKNOWN_ERROR";
	}
}

// ------------------------------------------------------------
int ivf::checkGLError(const char* where)
{
	int found = 0;

	// The error flag latches: one glGetError() clears only the first error and
	// leaves any others queued behind it. Loop until the queue is empty.
	//
	// Bound the loop as well. A context lost mid-frame can return the same
	// error forever, and a diagnostic that hangs is worse than one that lies.

	const int maxErrors = 32;

	GLenum error = glGetError();

	while ((error != GL_NO_ERROR) && (found < maxErrors))
	{
		found++;

		std::string message = std::string(glErrorString(error));

		if ((where != nullptr) && (where[0] != '\0'))
			message += std::string(" in ") + where;

		report(message, DebugSeverity::High, (unsigned int)error);

		error = glGetError();
	}

	return found;
}

// ------------------------------------------------------------
void ivf::clearGLErrors()
{
	const int maxErrors = 32;
	int drained = 0;

	while ((glGetError() != GL_NO_ERROR) && (drained < maxErrors))
		drained++;
}
