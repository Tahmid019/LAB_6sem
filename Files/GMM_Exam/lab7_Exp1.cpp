/*Q. Given a rectangular clipping window xmin=10, ymin=10, xmax=100, ymax=100 and line endpoints (5,20) and (120,80):

• Compute the region codes for the endpoints.

• Apply the Cohen–Sutherland line clipping algorithm.

• Determine whether the line is accepted, rejected, or partially clipped.

• Display the clipped line segment within the clipping window.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// Clipping window
int xmin=10, ymin=10, xmax=100, ymax=100;

// Region codes
const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;

// Compute region code
int computeCode(float x, float y) {
    int code = INSIDE;

    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;

    return code;
}

// Cohen–Sutherland function
bool cohenClip(float &x_start, float &y_start, float &x_end, float &y_end) {

    int code1 = computeCode(x_start, y_start);
    int code2 = computeCode(x_end, y_end);

    cout << "Region Code 1: " << code1 << endl;
    cout << "Region Code 2: " << code2 << endl;

    while (true) {

        // Case 1: Both inside
        if (code1 == 0 && code2 == 0) {
            cout << "Line Accepted\n";
            return true;
        }

        // Case 2: Both outside same region
        else if (code1 & code2) {
            cout << "Line Rejected\n";
            return false;
        }

        // Case 3: Partial clipping
        else {
            float x, y;
            int code_out = (code1 != 0) ? code1 : code2;

            if (code_out & TOP) {
                x = x_start + (x_end - x_start) * (ymax - y_start) / (y_end - y_start);
                y = ymax;
            }
            else if (code_out & BOTTOM) {
                x = x_start + (x_end - x_start) * (ymin - y_start) / (y_end - y_start);
                y = ymin;
            }
            else if (code_out & RIGHT) {
                y = y_start + (y_end - y_start) * (xmax - x_start) / (x_end - x_start);
                x = xmax;
            }
            else if (code_out & LEFT) {
                y = y_start + (y_end - y_start) * (xmin - x_start) / (x_end - x_start);
                x = xmin;
            }

            if (code_out == code1) {
                x_start = x;
                y_start = y;
                code1 = computeCode(x_start, y_start);
            } else {
                x_end = x;
                y_end = y;
                code2 = computeCode(x_end, y_end);
            }
        }
    }
}

// Original line (RENAMED VARIABLES)
float x_start = 5, y_start = 20;
float x_end   = 120, y_end = 80;

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

    // 🔴 Draw original line
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2f(x_start,y_start);
        glVertex2f(x_end,y_end);
    glEnd();

    // Copy values for clipping
    float cx1 = x_start, cy1 = y_start;
    float cx2 = x_end,   cy2 = y_end;

    // 🟢 Draw clipped line
    if (cohenClip(cx1, cy1, cx2, cy2)) {
        glColor3f(0,1,0);
        glBegin(GL_LINES);
            glVertex2f(cx1, cy1);
            glVertex2f(cx2, cy2);
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
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}