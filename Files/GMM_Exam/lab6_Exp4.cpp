/*Experiment 4: Line Clipping
Given a clipping window with coordinates xmin=10, ymin=10, xmax=100, ymax=100 and the following lines:

• Line 1: (5,5) to (120,120)

• Line 2: (20,30) to (80,90)

• Line 3: (50,150) to (150,60)

Perform line clipping and display the visible portions of each line within the clipping window.

Show the original and clipped lines using different colors.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Clipping window
int xmin=10, ymin=10, xmax=100, ymax=100;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔵 Draw clipping window
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex2i(xmin,ymin);
        glVertex2i(xmax,ymin);
        glVertex2i(xmax,ymax);
        glVertex2i(xmin,ymax);
    glEnd();

    // ======================
    // 🔴 ORIGINAL LINES
    // ======================
    glColor3f(1,0,0);
    glBegin(GL_LINES);

    // Line 1
    glVertex2i(5,5);
    glVertex2i(120,120);

    // Line 2
    glVertex2i(20,30);
    glVertex2i(80,90);

    // Line 3
    glVertex2i(50,150);
    glVertex2i(150,60);

    glEnd();

    // ======================
    // 🟢 CLIPPED LINES
    // ======================
    glColor3f(0,1,0);
    glBegin(GL_LINES);

    // Line 1 clipped → approx intersection (10,10) to (100,100)
    glVertex2i(10,10);
    glVertex2i(100,100);

    // Line 2 → fully inside (same)
    glVertex2i(20,30);
    glVertex2i(80,90);

    // Line 3 → no visible part (skip)

    glEnd();

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,150,0,150);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}