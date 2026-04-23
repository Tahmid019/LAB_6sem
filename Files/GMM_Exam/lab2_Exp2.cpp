/* Implementation of Midpoint ellipse drawing algorithm
A). Divide the coordinate axes into four quadrants and then draw only a portion of an ellipse with the major axis as X-axis.

B). Draw another portion of the ellipse with the major axis as Y-axis to the quadrant diagonally opposite to each other. Assign different colours to them respectively. Please consider the below image as a reference:*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;


int rx = 150;  
int ry = 80; 

void plotPoint(int x, int y, int quadrant) {
    glBegin(GL_POINTS);

    if (quadrant == 1) {
        glVertex2i(x, y);
    }
    else if (quadrant == 3) { 
        glVertex2i(-y, -x); 
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔷 Axes
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(-500, 0);
        glVertex2i(500, 0);
        glVertex2i(0, -500);
        glVertex2i(0, 500);
    glEnd();

    int x = 0;
    int y = ry;

    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;

   
    float p1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);

    glColor3f(1.0, 0.0, 0.0); 

    while (dx < dy) {
        plotPoint(x, y, 1);

        if (p1 < 0) {
            x++;
            dx = dx + (2 * ry * ry);
            p1 = p1 + dx + (ry * ry);
        } else {
            x++; y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            p1 = p1 + dx - dy + (ry * ry);
        }
    }

  
    float p2 = (ry * ry) * (x + 0.5) * (x + 0.5) +
               (rx * rx) * (y - 1) * (y - 1) -
               (rx * rx * ry * ry);

    while (y >= 0) {
        plotPoint(x, y, 1);

        if (p2 > 0) {
            y--;
            dy = dy - (2 * rx * rx);
            p2 = p2 + (rx * rx) - dy;
        } else {
            y--; x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            p2 = p2 + dx - dy + (rx * rx);
        }
    }

    
    x = 0;
    y = rx;

    dx = 2 * rx * rx * x;
    dy = 2 * ry * ry * y;

    float p3 = (rx * rx) - (ry * ry * rx) + (0.25 * ry * ry);

    glColor3f(0.0, 1.0, 0.0); 

    while (dx < dy) {
        plotPoint(x, y, 3);

        if (p3 < 0) {
            x++;
            dx = dx + (2 * rx * rx);
            p3 = p3 + dx + (rx * rx);
        } else {
            x++; y--;
            dx = dx + (2 * rx * rx);
            dy = dy - (2 * ry * ry);
            p3 = p3 + dx - dy + (rx * rx);
        }
    }

    float p4 = (rx * rx) * (x + 0.5) * (x + 0.5) +
               (ry * ry) * (y - 1) * (y - 1) -
               (rx * rx * ry * ry);

    while (y >= 0) {
        plotPoint(x, y, 3);

        if (p4 > 0) {
            y--;
            dy = dy - (2 * ry * ry);
            p4 = p4 + (ry * ry) - dy;
        } else {
            y--; x++;
            dx = dx + (2 * rx * rx);
            dy = dy - (2 * ry * ry);
            p4 = p4 + dx - dy + (ry * ry);
        }
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Midpoint Ellipse");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}