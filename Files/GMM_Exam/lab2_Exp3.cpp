/*3. Given the centre point coordinates (10, 10) and radius as 8, generate all the points to form a circle. */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

int xc = 10, yc = 10, r = 8;

void plotCirclePoints(int x, int y) {
    glBegin(GL_POINTS);

    // 8-way symmetry
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);

    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Axes
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(-100, 0);
        glVertex2i(100, 0);
        glVertex2i(0, -100);
        glVertex2i(0, 100);
    glEnd();

    int x = 0;
    int y = r;

    int p = 1 - r;

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(4.0);

    while (x <= y) {
        plotCirclePoints(x, y);

        x++;

        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-100, 100, -100, 100);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Midpoint Circle");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}