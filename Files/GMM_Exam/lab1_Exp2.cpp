/*
Implementation of DDA line drawing algorithm. “The program
should take input of the initial end-point and the final end-point. Divide the coordinate axes
into four quadrants and draw the line. Show that your program works for [m>0, m=0 and
m<0]”.
*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

float x_start, y_start, x_end, y_end;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Axes
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(-500, 0);
        glVertex2i(500, 0);
        glVertex2i(0, -500);
        glVertex2i(0, 500);
    glEnd();

    // DDA
    float dx = x_end - x_start;
    float dy = y_end - y_start;

    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x_start;
    float y = y_start;

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(3.0); // better visibility

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char** argv) {

    cout << "Enter x1 y1: ";
    cin >> x_start >> y_start;

    cout << "Enter x2 y2: ";
    cin >> x_end >> y_end;

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("DDA Line Drawing");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}

/*
m > 0
0 0
200 200

m = 0
0 100
200 100

m < 0
0 200
200 0
*/