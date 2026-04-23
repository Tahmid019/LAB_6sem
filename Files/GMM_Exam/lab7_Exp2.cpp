/*Experiment 2: Liang–Barsky Line Clipping
Q. Given a rectangular clipping window xmin=10, ymin=10, xmax=100, ymax=100 and a line segment from (0,50) to (120,80):

• Calculate the parameters p and q.

• Determine the entering and leaving values of the parameter t.

• Compute the clipped line coordinates using the Liang–Barsky algorithm.

• Display the original and clipped line segments.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Clipping window
float xmin=10, ymin=10, xmax=100, ymax=100;

// Generic Liang–Barsky function
bool liangBarsky(float x1, float y1, float x2, float y2,
                 float &cx1, float &cy1, float &cx2, float &cy2) {

    float dx = x2 - x1;
    float dy = y2 - y1;

    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {x1 - xmin, xmax - x1, y1 - ymin, ymax - y1};

    float t_enter = 0.0, t_leave = 1.0;

    for (int i = 0; i < 4; i++) {

        if (p[i] == 0) {
            if (q[i] < 0) return false; // parallel outside
        } else {
            float t = q[i] / p[i];

            if (p[i] < 0)
                t_enter = max(t_enter, t);
            else
                t_leave = min(t_leave, t);
        }
    }

    if (t_enter > t_leave) return false;

    cx1 = x1 + t_enter * dx;
    cy1 = y1 + t_enter * dy;
    cx2 = x1 + t_leave * dx;
    cy2 = y1 + t_leave * dy;

    cout << "t_enter: " << t_enter << endl;
    cout << "t_leave: " << t_leave << endl;

    return true;
}

// Input line
float x_start = 0, y_start = 50;
float x_end   = 120, y_end = 80;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔵 Window
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin,ymin);
        glVertex2f(xmax,ymin);
        glVertex2f(xmax,ymax);
        glVertex2f(xmin,ymax);
    glEnd();

    // 🔴 Original line
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2f(x_start,y_start);
        glVertex2f(x_end,y_end);
    glEnd();

    float cx1, cy1, cx2, cy2;

    // 🟢 Clipped line
    if (liangBarsky(x_start,y_start,x_end,y_end,cx1,cy1,cx2,cy2)) {
        glColor3f(0,1,0);
        glBegin(GL_LINES);
            glVertex2f(cx1,cy1);
            glVertex2f(cx2,cy2);
        glEnd();
    }

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,150,0,150);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Liang-Barsky Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}