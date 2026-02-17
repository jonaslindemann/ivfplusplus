/*
 * MODULE: tess_cache.h
 *
 * FUNCTION:
 * Header for GLU tessellator caching functions
 *
 * PERFORMANCE:
 * Provides cached tessellator to avoid expensive creation/deletion
 *
 * USAGE:
 * Call gleInitTessCache() once during initialization
 * Use gleGetCachedTess() instead of gluNewTess()
 * Call gleDestroyTessCache() during cleanup
 *
 * HISTORY:
 * Performance optimization
 */

#ifndef GLE_TESS_CACHE_H_
#define GLE_TESS_CACHE_H_

#ifdef OPENGL_10

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize tessellator cache - call once during initialization */
extern void gleInitTessCache(void);

/* Get cached tessellator instance (internal use) */
extern GLUtriangulatorObj* gleGetCachedTess(void);

/* Destroy tessellator cache - call during cleanup */
extern void gleDestroyTessCache(void);

#ifdef __cplusplus
}
#endif

#endif /* OPENGL_10 */

#endif /* GLE_TESS_CACHE_H_ */
