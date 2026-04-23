/*Implementation of Bresenham’s Line drawing algorithm. “The program should take input of the initial end-point and the final end-point, and verify whether the drawn line is smooth one or not. The program should be generic so that it works for integer values. Check for the slop of the line (i.e. m>0, m=0 and m<0)*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

int x_start, y_start, x_end, y_end;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔷 Axes (same as before)
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(-500, 0);
        glVertex2i(500, 0);
        glVertex2i(0, -500);
        glVertex2i(0, 500);
    glEnd();

    // 🔴 Bresenham Algorithm
    int dx = abs(x_end - x_start);
    int dy = abs(y_end - y_start);

    int sx = (x_end > x_start) ? 1 : -1;
    int sy = (y_end > y_start) ? 1 : -1;

    int err = dx - dy;

    int x = x_start;
    int y = y_start;

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(3.0);

    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x, y);

        if (x == x_end && y == y_end)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }

        if (e2 < dx) {
            err += dx;
            y += sy;
        }
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
    glutCreateWindow("Bresenham Line");

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