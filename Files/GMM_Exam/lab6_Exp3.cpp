/*
Experiment 3: Point Clipping
Given a clipping window defined by xmin=10, ymin=10, xmax=80, ymax=80:

• Determine whether the points (20,30), (90,40), (50,50), (5,60) lie inside or outside the window.

• Implement point clipping using a program.

• Display the points that remain after clipping in a different color.
*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Clipping window
int xmin=10, ymin=10, xmax=80, ymax=80;

// Points
int points[4][2] = {{20,30},{90,40},{50,50},{5,60}};

// Check function
bool isInside(int x, int y) {
    return (x>=xmin && x<=xmax && y>=ymin && y<=ymax);
}

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

    // 🔴 All points (unclipped)
    glColor3f(1,0,0);
    glPointSize(6);
    glBegin(GL_POINTS);
    for(int i=0;i<4;i++)
        glVertex2i(points[i][0], points[i][1]);
    glEnd();

    // 🟢 Clipped points (only visible)
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    for(int i=0;i<4;i++){
        if(isInside(points[i][0], points[i][1]))
            glVertex2i(points[i][0], points[i][1]);
    }
    glEnd();

    // Print result
    for(int i=0;i<4;i++){
        if(isInside(points[i][0], points[i][1]))
            cout << "(" << points[i][0] << "," << points[i][1] << ") Visible\n";
        else
            cout << "(" << points[i][0] << "," << points[i][1] << ") Invisible\n";
    }

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,100,0,100);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Point Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}