/*Using the Midpoint Circle Algorithm, calculate the pixel positions for a circle with center (0,0) and radius = 5. Show the decision parameter calculations for the first few iterations.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

int xc = 0, yc = 0, r = 5;

void plotPoints(int x, int y) {
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

    // 🔷 Axes
    glColor3f(0,0,0);
    glBegin(GL_LINES);
        glVertex2i(-10,0); glVertex2i(10,0);
        glVertex2i(0,-10); glVertex2i(0,10);
    glEnd();

    int x = 0;
    int y = r;

    int p = 1 - r;

    glColor3f(1,0,0);
    glPointSize(5.0);

    cout << "x  y  p\n";
    cout << x << "  " << y << "  " << p << endl;

    while (x <= y) {
        plotPoints(x, y);

        x++;

        if (p < 0) {
            p = p + 2*x + 1;
        } else {
            y--;
            p = p + 2*x + 1 - 2*y;
        }

        cout << x << "  " << y << "  " << p << endl;
    }

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-10,10,-10,10);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Midpoint Circle r=5");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}