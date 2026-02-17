/*
 * MODULE: segment_vbo.h
 *
 * FUNCTION:
 * Header for VBO-accelerated segment drawing functions
 *
 * PERFORMANCE:
 * These functions provide significantly improved rendering performance
 * by using Vertex Buffer Objects instead of immediate mode rendering.
 *
 * USAGE:
 * Call gleInitVBOCache() once during initialization with estimated max vertices.
 * Call gleDestroyVBOCache() during cleanup.
 * Use the _vbo variants of drawing functions for best performance.
 *
 * HISTORY:
 * Performance optimization for modern OpenGL
 */

#ifndef GLE_SEGMENT_VBO_H_
#define GLE_SEGMENT_VBO_H_

#include "gle.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize VBO cache - call once during initialization */
extern void gleInitVBOCache(int max_vertices);

/* Destroy VBO cache - call during cleanup */
extern void gleDestroyVBOCache(void);

/* VBO-accelerated drawing functions (internal use) */
extern void draw_segment_edge_n_vbo(int ncp,
                                    gleDouble front_contour[][3],
                                    gleDouble back_contour[][3],
                                    double norm_cont[][3],
                                    int inext, double len);

extern void draw_binorm_segment_edge_n_vbo(int ncp,
                                           double front_contour[][3],
                                           double back_contour[][3],
                                           double front_norm[][3],
                                           double back_norm[][3],
                                           int inext, double len);

#ifdef __cplusplus
}
#endif

#endif /* GLE_SEGMENT_VBO_H_ */
