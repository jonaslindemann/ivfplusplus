/*
 * MODULE NAME: segment_vbo.c
 *
 * FUNCTION:
 * VBO-accelerated version of segment drawing for improved rendering performance.
 * Uses Vertex Buffer Objects instead of immediate mode rendering.
 *
 * PERFORMANCE IMPROVEMENTS:
 * - Uses VBOs to batch vertex data
 * - Eliminates glBegin/glEnd overhead
 * - Reduces function call overhead
 * - Better GPU cache utilization
 *
 * HISTORY:
 * Performance optimization for modern OpenGL
 */

#include "config.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <glad/glad.h>

#include <GL/gle.h>
#include <GL/segment_vbo.h>
#include "port.h"
#include "extrude.h"
#include "tube_gc.h"
#include "segment.h"

#ifdef OPENGL_10

/* VBO cache structure */
typedef struct {
    GLuint vbo;
    GLuint normal_vbo;
    GLuint color_vbo;
    int allocated_size;
    int enabled;
} gleVBOCache;

static gleVBOCache g_vbo_cache = {0, 0, 0, 0, 0};

/* Initialize VBO cache */
void gleInitVBOCache(int max_vertices) {
    if (g_vbo_cache.enabled) return;
    
    glGenBuffers(1, &g_vbo_cache.vbo);
    glGenBuffers(1, &g_vbo_cache.normal_vbo);
    glGenBuffers(1, &g_vbo_cache.color_vbo);
    
    g_vbo_cache.allocated_size = max_vertices;
    g_vbo_cache.enabled = 1;
}

/* Clean up VBO cache */
void gleDestroyVBOCache(void) {
    if (!g_vbo_cache.enabled) return;
    
    if (g_vbo_cache.vbo) glDeleteBuffers(1, &g_vbo_cache.vbo);
    if (g_vbo_cache.normal_vbo) glDeleteBuffers(1, &g_vbo_cache.normal_vbo);
    if (g_vbo_cache.color_vbo) glDeleteBuffers(1, &g_vbo_cache.color_vbo);
    
    g_vbo_cache.vbo = 0;
    g_vbo_cache.normal_vbo = 0;
    g_vbo_cache.color_vbo = 0;
    g_vbo_cache.enabled = 0;
}

/* VBO-accelerated segment drawing with normals */
void draw_segment_edge_n_vbo(int ncp,
                              gleDouble front_contour[][3],
                              gleDouble back_contour[][3],
                              double norm_cont[][3],
                              int inext, double len)
{
    if (!g_vbo_cache.enabled) {
        /* Fall back to immediate mode */
        draw_segment_edge_n(ncp, front_contour, back_contour, norm_cont, inext, len);
        return;
    }
    
    int num_vertices = ncp * 2;
    if (__TUBE_CLOSE_CONTOUR) num_vertices += 2;
    
    /* Allocate temporary arrays */
    GLfloat *vertices = (GLfloat*)malloc(num_vertices * 3 * sizeof(GLfloat));
    GLfloat *normals = (GLfloat*)malloc(num_vertices * 3 * sizeof(GLfloat));
    
    if (!vertices || !normals) {
        free(vertices);
        free(normals);
        draw_segment_edge_n(ncp, front_contour, back_contour, norm_cont, inext, len);
        return;
    }
    
    /* Pack data into arrays */
    int idx = 0;
    for (int j = 0; j < ncp; j++) {
        /* Front vertex */
        vertices[idx*3+0] = (GLfloat)front_contour[j][0];
        vertices[idx*3+1] = (GLfloat)front_contour[j][1];
        vertices[idx*3+2] = (GLfloat)front_contour[j][2];
        normals[idx*3+0] = (GLfloat)norm_cont[j][0];
        normals[idx*3+1] = (GLfloat)norm_cont[j][1];
        normals[idx*3+2] = (GLfloat)norm_cont[j][2];
        idx++;
        
        /* Back vertex */
        vertices[idx*3+0] = (GLfloat)back_contour[j][0];
        vertices[idx*3+1] = (GLfloat)back_contour[j][1];
        vertices[idx*3+2] = (GLfloat)back_contour[j][2];
        normals[idx*3+0] = (GLfloat)norm_cont[j][0];
        normals[idx*3+1] = (GLfloat)norm_cont[j][1];
        normals[idx*3+2] = (GLfloat)norm_cont[j][2];
        idx++;
    }
    
    if (__TUBE_CLOSE_CONTOUR) {
        /* Front vertex */
        vertices[idx*3+0] = (GLfloat)front_contour[0][0];
        vertices[idx*3+1] = (GLfloat)front_contour[0][1];
        vertices[idx*3+2] = (GLfloat)front_contour[0][2];
        normals[idx*3+0] = (GLfloat)norm_cont[0][0];
        normals[idx*3+1] = (GLfloat)norm_cont[0][1];
        normals[idx*3+2] = (GLfloat)norm_cont[0][2];
        idx++;
        
        /* Back vertex */
        vertices[idx*3+0] = (GLfloat)back_contour[0][0];
        vertices[idx*3+1] = (GLfloat)back_contour[0][1];
        vertices[idx*3+2] = (GLfloat)back_contour[0][2];
        normals[idx*3+0] = (GLfloat)norm_cont[0][0];
        normals[idx*3+1] = (GLfloat)norm_cont[0][1];
        normals[idx*3+2] = (GLfloat)norm_cont[0][2];
        idx++;
    }
    
    /* Upload to VBO */
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), vertices, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), normals, GL_STREAM_DRAW);
    
    /* Enable client state */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Bind and draw */
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.normal_vbo);
    glNormalPointer(GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
    
    /* Cleanup */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    free(vertices);
    free(normals);
}

/* VBO-accelerated binormal segment drawing */
void draw_binorm_segment_edge_n_vbo(int ncp,
                                     double front_contour[][3],
                                     double back_contour[][3],
                                     double front_norm[][3],
                                     double back_norm[][3],
                                     int inext, double len)
{
    if (!g_vbo_cache.enabled) {
        draw_binorm_segment_edge_n(ncp, front_contour, back_contour, 
                                   front_norm, back_norm, inext, len);
        return;
    }
    
    int num_vertices = ncp * 2;
    if (__TUBE_CLOSE_CONTOUR) num_vertices += 2;
    
    /* Allocate temporary arrays */
    GLfloat *vertices = (GLfloat*)malloc(num_vertices * 3 * sizeof(GLfloat));
    GLfloat *normals = (GLfloat*)malloc(num_vertices * 3 * sizeof(GLfloat));
    
    if (!vertices || !normals) {
        free(vertices);
        free(normals);
        draw_binorm_segment_edge_n(ncp, front_contour, back_contour,
                                   front_norm, back_norm, inext, len);
        return;
    }
    
    /* Pack data into arrays */
    int idx = 0;
    for (int j = 0; j < ncp; j++) {
        /* Front vertex */
        vertices[idx*3+0] = (GLfloat)front_contour[j][0];
        vertices[idx*3+1] = (GLfloat)front_contour[j][1];
        vertices[idx*3+2] = (GLfloat)front_contour[j][2];
        normals[idx*3+0] = (GLfloat)front_norm[j][0];
        normals[idx*3+1] = (GLfloat)front_norm[j][1];
        normals[idx*3+2] = (GLfloat)front_norm[j][2];
        idx++;
        
        /* Back vertex */
        vertices[idx*3+0] = (GLfloat)back_contour[j][0];
        vertices[idx*3+1] = (GLfloat)back_contour[j][1];
        vertices[idx*3+2] = (GLfloat)back_contour[j][2];
        normals[idx*3+0] = (GLfloat)back_norm[j][0];
        normals[idx*3+1] = (GLfloat)back_norm[j][1];
        normals[idx*3+2] = (GLfloat)back_norm[j][2];
        idx++;
    }
    
    if (__TUBE_CLOSE_CONTOUR) {
        vertices[idx*3+0] = (GLfloat)front_contour[0][0];
        vertices[idx*3+1] = (GLfloat)front_contour[0][1];
        vertices[idx*3+2] = (GLfloat)front_contour[0][2];
        normals[idx*3+0] = (GLfloat)front_norm[0][0];
        normals[idx*3+1] = (GLfloat)front_norm[0][1];
        normals[idx*3+2] = (GLfloat)front_norm[0][2];
        idx++;
        
        vertices[idx*3+0] = (GLfloat)back_contour[0][0];
        vertices[idx*3+1] = (GLfloat)back_contour[0][1];
        vertices[idx*3+2] = (GLfloat)back_contour[0][2];
        normals[idx*3+0] = (GLfloat)back_norm[0][0];
        normals[idx*3+1] = (GLfloat)back_norm[0][1];
        normals[idx*3+2] = (GLfloat)back_norm[0][2];
        idx++;
    }
    
    /* Upload to VBO */
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), vertices, GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 3 * sizeof(GLfloat), normals, GL_STREAM_DRAW);
    
    /* Enable client state */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    /* Bind and draw */
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo_cache.normal_vbo);
    glNormalPointer(GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
    
    /* Cleanup */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    free(vertices);
    free(normals);
}

#endif /* OPENGL_10 */
