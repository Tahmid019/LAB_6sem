/*Experiment 1: Window to Viewport Mapping
Given a window with coordinates W(xmin=0, ymin=0, xmax=100, ymax=100) and a viewport with coordinates V(xmin=200, ymin=200, xmax=400, ymax=400), perform the following:

• Map the point (20,30) from the window to the viewport.

• Map the point (60,70) from the window to the viewport.

• Derive the window-to-viewport transformation equations.

• Display the original window coordinates and transformed viewport coordinates.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Window
float wxmin=0, wymin=0, wxmax=100, wymax=100;

// Viewport
float vxmin=200, vymin=200, vxmax=400, vymax=400;

// Mapping function
void mapPoint(float xw, float yw, float &xv, float &yv) {
    xv = vxmin + (xw - wxmin) * (vxmax - vxmin) / (wxmax - wxmin);
    yv = vymin + (yw - wymin) * (vymax - vymin) / (wymax - wymin);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔷 Draw Window
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(wxmin, wymin);
        glVertex2f(wxmax, wymin);
        glVertex2f(wxmax, wymax);
        glVertex2f(wxmin, wymax);
    glEnd();

    // 🔷 Draw Viewport
    glColor3f(0,1,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(vxmin, vymin);
        glVertex2f(vxmax, vymin);
        glVertex2f(vxmax, vymax);
        glVertex2f(vxmin, vymax);
    glEnd();

    float xv, yv;

    // 🔴 Point 1
    glColor3f(0,0,0);
    glPointSize(6);
     glBegin(GL_POINTS);
        glVertex2f(20,30);
    glEnd();
    mapPoint(20,30,xv,yv);
    glColor3f(1,0,0);
    glPointSize(6);
    glBegin(GL_POINTS);
        glVertex2f(xv,yv);
    glEnd();

    cout << "(20,30) -> (" << xv << "," << yv << ")" << endl;

    // 🔴 Point 2
    glColor3f(0,0,0);
    glPointSize(6);
     glBegin(GL_POINTS);
        glVertex2f(60,70);
    glEnd();
    glColor3f(1,0,0);
    mapPoint(60,70,xv,yv);
    glBegin(GL_POINTS);
        glVertex2f(xv,yv);
    glEnd();

    cout << "(60,70) -> (" << xv << "," << yv << ")" << endl;

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,500,0,500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Window to Viewport Mapping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}