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
#include <ivf/rc.h>

// ---------------------------------------------------------------------------
// Fixed-function calls, suppressed in a core profile.
//
// Every one of these is a call that OpenGL removed in the core profile. Routing
// them through this header rather than calling them directly does two things:
// it makes them no-ops under RenderProfile::Core, so the library can run on a
// core context without emitting GL_INVALID_OPERATION for every vertex; and it
// gives one auditable list of what still depends on compatibility. Grep for lg
// to see how much of the migration is left.
//
// Calls that remain legal in core -- glEnable(GL_DEPTH_TEST), glLineWidth,
// glPointSize, glBlendFunc, glPolygonMode, glViewport, glClear -- deliberately
// have no wrapper here. Leaving them as plain gl calls is what marks them as
// safe, so this header doubles as the whitelist's complement.
//
// Nothing calls these yet. Phase 2 converts the library file by file; adding
// the header first keeps that change mechanical.
// ---------------------------------------------------------------------------

namespace ivf {

// ---- Immediate mode ----

inline void lgBegin(GLenum mode)                  { if (rcLegacyAllowed()) glBegin(mode); }
inline void lgEnd()                               { if (rcLegacyAllowed()) glEnd(); }

inline void lgVertex3d(GLdouble x, GLdouble y, GLdouble z)
                                                  { if (rcLegacyAllowed()) glVertex3d(x, y, z); }
inline void lgVertex3f(GLfloat x, GLfloat y, GLfloat z)
                                                  { if (rcLegacyAllowed()) glVertex3f(x, y, z); }
inline void lgVertex3dv(const GLdouble* v)        { if (rcLegacyAllowed()) glVertex3dv(v); }
inline void lgVertex3fv(const GLfloat* v)         { if (rcLegacyAllowed()) glVertex3fv(v); }

inline void lgNormal3d(GLdouble x, GLdouble y, GLdouble z)
                                                  { if (rcLegacyAllowed()) glNormal3d(x, y, z); }
inline void lgNormal3dv(const GLdouble* v)        { if (rcLegacyAllowed()) glNormal3dv(v); }
inline void lgNormal3fv(const GLfloat* v)         { if (rcLegacyAllowed()) glNormal3fv(v); }

inline void lgColor3f(GLfloat r, GLfloat g, GLfloat b)
                                                  { if (rcLegacyAllowed()) glColor3f(r, g, b); }
inline void lgColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
                                                  { if (rcLegacyAllowed()) glColor4f(r, g, b, a); }
inline void lgColor3d(GLdouble r, GLdouble g, GLdouble b)
                                                  { if (rcLegacyAllowed()) glColor3d(r, g, b); }
inline void lgColor3dv(const GLdouble* v)         { if (rcLegacyAllowed()) glColor3dv(v); }
inline void lgColor3fv(const GLfloat* v)          { if (rcLegacyAllowed()) glColor3fv(v); }
inline void lgColor4fv(const GLfloat* v)          { if (rcLegacyAllowed()) glColor4fv(v); }

inline void lgTexCoord2d(GLdouble s, GLdouble t)  { if (rcLegacyAllowed()) glTexCoord2d(s, t); }
inline void lgTexCoord2dv(const GLdouble* v)      { if (rcLegacyAllowed()) glTexCoord2dv(v); }
inline void lgTexCoord2fv(const GLfloat* v)       { if (rcLegacyAllowed()) glTexCoord2fv(v); }

// ---- Matrix stack ----

inline void lgMatrixMode(GLenum mode)             { if (rcLegacyAllowed()) glMatrixMode(mode); }
inline void lgLoadIdentity()                      { if (rcLegacyAllowed()) glLoadIdentity(); }
inline void lgPushMatrix()                        { if (rcLegacyAllowed()) glPushMatrix(); }
inline void lgPopMatrix()                         { if (rcLegacyAllowed()) glPopMatrix(); }
inline void lgTranslated(GLdouble x, GLdouble y, GLdouble z)
                                                  { if (rcLegacyAllowed()) glTranslated(x, y, z); }
inline void lgRotated(GLdouble a, GLdouble x, GLdouble y, GLdouble z)
                                                  { if (rcLegacyAllowed()) glRotated(a, x, y, z); }
inline void lgScaled(GLdouble x, GLdouble y, GLdouble z)
                                                  { if (rcLegacyAllowed()) glScaled(x, y, z); }
inline void lgMultMatrixd(const GLdouble* m)      { if (rcLegacyAllowed()) glMultMatrixd(m); }
inline void lgMultMatrixf(const GLfloat* m)       { if (rcLegacyAllowed()) glMultMatrixf(m); }
inline void lgLoadMatrixd(const GLdouble* m)      { if (rcLegacyAllowed()) glLoadMatrixd(m); }

// ---- Projection helpers ----
//
// glFrustum and glOrtho load the fixed-function projection matrix, and the GLU
// helpers are thin wrappers that end in glMultMatrix. Camera mirrors all of them
// into RenderContext::projection() as glm matrices, which is what the shader
// path reads.

inline void lgFrustum(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
                                                  { if (rcLegacyAllowed()) glFrustum(l, r, b, t, n, f); }
inline void lgOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
                                                  { if (rcLegacyAllowed()) glOrtho(l, r, b, t, n, f); }
inline void lgPerspective(GLdouble fovy, GLdouble aspect, GLdouble n, GLdouble f)
                                                  { if (rcLegacyAllowed()) gluPerspective(fovy, aspect, n, f); }
inline void lgLookAt(GLdouble ex, GLdouble ey, GLdouble ez,
                     GLdouble cx, GLdouble cy, GLdouble cz,
                     GLdouble ux, GLdouble uy, GLdouble uz)
                                                  { if (rcLegacyAllowed()) gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz); }
// gluPickMatrix takes its viewport as a non-const GLint[4], so take one too
// rather than casting the constness away at every call site.

inline void lgPickMatrix(GLdouble x, GLdouble y, GLdouble w, GLdouble h, GLint* viewport)
                                                  { if (rcLegacyAllowed()) gluPickMatrix(x, y, w, h, viewport); }

// ---- Attribute stack ----

inline void lgPushAttrib(GLbitfield mask)         { if (rcLegacyAllowed()) glPushAttrib(mask); }
inline void lgPopAttrib()                         { if (rcLegacyAllowed()) glPopAttrib(); }

// ---- Lighting and material ----
//
// glEnable and glDisable are only wrapped for the caps that core removed --
// GL_LIGHTING, GL_LIGHTn, GL_COLOR_MATERIAL, GL_NORMALIZE, GL_FOG, GL_ALPHA_TEST
// and friends. Pass a cap that survives in core and you want plain glEnable.

inline void lgEnableLegacy(GLenum cap)            { if (rcLegacyAllowed()) glEnable(cap); }
inline void lgDisableLegacy(GLenum cap)           { if (rcLegacyAllowed()) glDisable(cap); }

inline void lgLightfv(GLenum light, GLenum pname, const GLfloat* params)
                                                  { if (rcLegacyAllowed()) glLightfv(light, pname, params); }
inline void lgLightf(GLenum light, GLenum pname, GLfloat param)
                                                  { if (rcLegacyAllowed()) glLightf(light, pname, param); }
inline void lgLighti(GLenum light, GLenum pname, GLint param)
                                                  { if (rcLegacyAllowed()) glLighti(light, pname, param); }
inline void lgLightModelfv(GLenum pname, const GLfloat* params)
                                                  { if (rcLegacyAllowed()) glLightModelfv(pname, params); }
inline void lgLightModeliv(GLenum pname, const GLint* params)
                                                  { if (rcLegacyAllowed()) glLightModeliv(pname, params); }

inline void lgMaterialfv(GLenum face, GLenum pname, const GLfloat* params)
                                                  { if (rcLegacyAllowed()) glMaterialfv(face, pname, params); }
inline void lgMaterialf(GLenum face, GLenum pname, GLfloat param)
                                                  { if (rcLegacyAllowed()) glMaterialf(face, pname, param); }
inline void lgColorMaterial(GLenum face, GLenum mode)
                                                  { if (rcLegacyAllowed()) glColorMaterial(face, mode); }

// ---- Line width ----
//
// glLineWidth itself survives in core, but its useful range does not: a
// forward-compatible core context accepts only 1.0 and raises GL_INVALID_VALUE
// for anything wider. This clamps rather than suppresses, so a wide line still
// draws -- just thin. Real wide lines in core need to be built from triangles;
// until then this keeps the geometry visible instead of erroring it away.

inline void lgLineWidth(GLfloat width)
{
    glLineWidth(rcLegacyAllowed() ? width : 1.0f);
}

// ---- Fog ----
//
// Fog mirrors these into RenderContext as well, where the fragment shader
// reimplements the same three fog equations.

inline void lgFogi(GLenum pname, GLint param)     { if (rcLegacyAllowed()) glFogi(pname, param); }
inline void lgFogf(GLenum pname, GLfloat param)   { if (rcLegacyAllowed()) glFogf(pname, param); }
inline void lgFogfv(GLenum pname, const GLfloat* params)
                                                  { if (rcLegacyAllowed()) glFogfv(pname, params); }

// ---- Texture environment ----

inline void lgTexEnvi(GLenum target, GLenum pname, GLint param)
                                                  { if (rcLegacyAllowed()) glTexEnvi(target, pname, param); }
inline void lgTexEnvfv(GLenum target, GLenum pname, const GLfloat* params)
                                                  { if (rcLegacyAllowed()) glTexEnvfv(target, pname, params); }

// ---- Alpha test ----

inline void lgAlphaFunc(GLenum func, GLclampf ref)
                                                  { if (rcLegacyAllowed()) glAlphaFunc(func, ref); }

// ---- Selection ----
//
// Replaced by colour-id picking against an offscreen target in Phase 5. Until
// then these keep working in Legacy and Mixed and vanish in Core.

inline void lgInitNames()                         { if (rcLegacyAllowed()) glInitNames(); }
inline void lgPushName(GLuint name)               { if (rcLegacyAllowed()) glPushName(name); }
inline void lgPopName()                           { if (rcLegacyAllowed()) glPopName(); }
inline void lgLoadName(GLuint name)               { if (rcLegacyAllowed()) glLoadName(name); }

} // namespace ivf
