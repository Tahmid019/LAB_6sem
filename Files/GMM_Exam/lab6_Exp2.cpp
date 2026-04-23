/*Experiment 2: Clipping
Consider a rectangular clipping window with coordinates xmin=10, ymin=10, xmax=60, ymax=60.

• Plot the following points and identify whether they are visible or invisible after clipping: (5,5), (20,30), (70,40), (50,70).

• Draw different geometric shapes and show how clipping removes the portions outside the clipping window.

• Display the clipped and unclipped objects in different colors.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Clipping window
int xmin=10, ymin=10, xmax=60, ymax=60;

// Check visibility
bool isInside(int x, int y) {
    return (x>=xmin && x<=xmax && y>=ymin && y<=ymax);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔵 Clipping Window
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex2i(xmin,ymin);
        glVertex2i(xmax,ymin);
        glVertex2i(xmax,ymax);
        glVertex2i(xmin,ymax);
    glEnd();

    // ======================
    // 🔴 POINTS (UNCLIPPED)
    // ======================
    int points[4][2] = {{5,5},{20,30},{70,40},{50,70}};
    glColor3f(1,0,0);
    glPointSize(6);

    glBegin(GL_POINTS);
    for(int i=0;i<4;i++)
        glVertex2i(points[i][0], points[i][1]);
    glEnd();

    // 🟢 CLIPPED POINTS
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    for(int i=0;i<4;i++)
        if(isInside(points[i][0], points[i][1]))
            glVertex2i(points[i][0], points[i][1]);
    glEnd();

    // ======================
    // 🔴 LINE (UNCLIPPED)
    // ======================
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2i(0,0);
        glVertex2i(80,80);
    glEnd();

    // 🟢 CLIPPED LINE (manually inside)
    glColor3f(0,1,0);
    glBegin(GL_LINES);
        glVertex2i(10,10);
        glVertex2i(60,60);
    glEnd();

    // ======================
    // 🔴 RECTANGLE (UNCLIPPED)
    // ======================
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(5,5);
        glVertex2i(70,5);
        glVertex2i(70,70);
        glVertex2i(5,70);
    glEnd();

    // 🟢 CLIPPED RECTANGLE (only visible part)
    glColor3f(0,1,0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(10,10);
        glVertex2i(60,10);
        glVertex2i(60,60);
        glVertex2i(10,60);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,100,0,100);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Clipping with Shapes");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}