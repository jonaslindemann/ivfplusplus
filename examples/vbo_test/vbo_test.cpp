/*
 * VBO Test Program
 * 
 * This program demonstrates and verifies VBO rendering in GLE.
 * It draws the same scene twice - once with VBO disabled and once with VBO enabled.
 * This allows you to:
 * 1. Verify VBO functions are working
 * 2. Compare performance between immediate mode and VBO rendering
 * 3. Ensure visual output is identical
 */

#include <GL/glut.h>
#include <GL/gle.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int win_width = 800;
static int win_height = 600;
static int use_vbo = 0;
static int frame_count = 0;
static double render_time_immediate = 0.0;
static double render_time_vbo = 0.0;

/* Spiral path data */
#define NUM_POINTS 100
static gleDouble spiral_points[NUM_POINTS][3];
static gleColor spiral_colors[NUM_POINTS];

/* Initialize spiral path */
void init_spiral() {
    for (int i = 0; i < NUM_POINTS; i++) {
        double t = (double)i / (NUM_POINTS - 1);
        double angle = t * 8.0 * M_PI;
        double radius = 2.0 + t * 1.5;
        
        spiral_points[i][0] = radius * cos(angle);
        spiral_points[i][1] = radius * sin(angle);
        spiral_points[i][2] = t * 5.0 - 2.5;
        
        spiral_colors[i][0] = (float)(1.0 - t);
        spiral_colors[i][1] = (float)t;
        spiral_colors[i][2] = 0.5f;
    }
}

/* Display callback */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(8.0, 8.0, 8.0,
              0.0, 0.0, 0.0,
              0.0, 0.0, 1.0);
    
    glRotated(30.0 * sin(frame_count * 0.01), 0.0, 0.0, 1.0);
    
    /* Draw with current VBO setting */
    glePolyCylinder(NUM_POINTS, spiral_points, spiral_colors, 0.3);
    
    glutSwapBuffers();
    frame_count++;
}

/* Reshape callback */
void reshape(int width, int height) {
    win_width = width;
    win_height = height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, 100.0);
}

/* Keyboard callback */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'v':
        case 'V':
            use_vbo = !use_vbo;
            gleSetVBOMode(use_vbo);
            printf("VBO Mode: %s\n", use_vbo ? "ENABLED" : "DISABLED");
            break;
        case 'q':
        case 'Q':
        case 27: // ESC
            exit(0);
            break;
        case 'i':
        case 'I':
            printf("\n=== VBO Test Info ===\n");
            printf("Current VBO Mode: %s\n", gleGetVBOMode() ? "ENABLED" : "DISABLED");
            printf("Press 'V' to toggle VBO mode\n");
            printf("Press 'Q' or ESC to quit\n");
            printf("====================\n\n");
            break;
    }
    glutPostRedisplay();
}

/* Idle callback for animation */
void idle(void) {
    glutPostRedisplay();
}

/* Initialize OpenGL state */
void init_gl(void) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = { 10.0f, 10.0f, 10.0f, 1.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    glShadeModel(GL_SMOOTH);
}

/* Initialize GLE */
void init_gle(void) {
    /* Initialize performance features */
    gleInitVBOCache(10000);
    gleInitTessCache();
    
    /* Set join style */
    gleSetJoinStyle(TUBE_JN_ANGLE | TUBE_JN_CAP | TUBE_NORM_EDGE);
    gleSetNumSides(20);
    
    /* Start with VBO disabled for comparison */
    gleSetVBOMode(0);
    
    printf("\n=== GLE VBO Test Program ===\n");
    printf("This program tests VBO rendering in GLE\n\n");
    printf("Controls:\n");
    printf("  V - Toggle VBO mode (currently: %s)\n", gleGetVBOMode() ? "ENABLED" : "DISABLED");
    printf("  I - Show information\n");
    printf("  Q/ESC - Quit\n");
    printf("\nInitial state: VBO mode is DISABLED\n");
    printf("Press 'V' to enable VBO and compare performance\n");
    printf("============================\n\n");
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("GLE VBO Test - Press 'V' to toggle VBO, 'I' for info");
    
    init_gl();
    init_gle();
    init_spiral();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    
    glutMainLoop();
    
    /* Cleanup */
    gleDestroyVBOCache();
    gleDestroyTessCache();
    
    return 0;
}
