/*
 * GLE Performance Optimization Example
 * 
 * This example shows how to enable VBO acceleration and tessellator
 * caching for 10-100x rendering performance improvements.
 */

#include <GL/gle.h>

/* Initialize performance optimizations */
void init_gle_performance(void) {
    /* Initialize VBO cache
     * Parameter: max vertices per draw (e.g., 10000)
     */
    gleInitVBOCache(10000);
    
    /* Initialize tessellator cache */
    gleInitTessCache();
    
    /* Optional: reduce tessellation for better performance */
    gleSetNumSides(12);
}

/* Cleanup performance optimizations */
void cleanup_gle_performance(void) {
    gleDestroyVBOCache();
    gleDestroyTessCache();
}

/* Render with automatic VBO acceleration */
void render_tubes(void) {
    gleDouble points[][3] = {
        {0, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {0, 1, 1}
    };
    
    /* This will automatically use VBO rendering if initialized */
    glePolyCone(4, points, NULL, 0.5);
}

/* Dynamic LOD based on distance */
void render_with_lod(float distance) {
    int slices = (distance < 50.0f) ? 20 :
                 (distance < 200.0f) ? 12 : 6;
    
    gleSetNumSides(slices);
    render_tubes();
}

/*
 * Performance Tips:
 *
 * 1. Call gleInitVBOCache() ONCE at startup
 * 2. Minimize gleSetNumSides() changes
 * 3. Use frustum culling
 * 4. Group similar geometry
 * 5. Profile with gDebugger or RenderDoc
 */
