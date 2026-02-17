/*
 * MODULE NAME: tess_cache.c
 *
 * FUNCTION:
 * GLU Tessellator caching for improved performance.
 * Avoids expensive tessellator creation/deletion on every cap.
 *
 * PERFORMANCE IMPROVEMENT:
 * Creating and destroying GLU tessellators is expensive. By caching
 * a single tessellator and reusing it, we can significantly improve
 * performance when drawing many end caps.
 *
 * HISTORY:
 * Performance optimization
 */

#include "config.h"
#include <stdlib.h>
#include <GL/gle.h>
#include <GL/tess_cache.h>

#ifdef OPENGL_10

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

static GLUtriangulatorObj *g_cached_tess = NULL;
static int g_tess_initialized = 0;

/* Initialize tessellator cache */
void gleInitTessCache(void) {
    if (g_tess_initialized) return;
    
    g_cached_tess = gluNewTess();
    if (g_cached_tess) {
        gluTessCallback(g_cached_tess, GLU_BEGIN, (GLvoid (*)())glBegin);
        gluTessCallback(g_cached_tess, GLU_VERTEX, (GLvoid (*)())glVertex3dv);
        gluTessCallback(g_cached_tess, GLU_END, (GLvoid (*)())glEnd);
        g_tess_initialized = 1;
    }
}

/* Get cached tessellator */
GLUtriangulatorObj* gleGetCachedTess(void) {
    if (!g_tess_initialized) {
        gleInitTessCache();
    }
    return g_cached_tess;
}

/* Destroy tessellator cache */
void gleDestroyTessCache(void) {
    if (g_cached_tess) {
        gluDeleteTess(g_cached_tess);
        g_cached_tess = NULL;
    }
    g_tess_initialized = 0;
}

#endif /* OPENGL_10 */
