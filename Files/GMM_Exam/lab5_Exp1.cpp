/*Given a polygon with vertices A(2,2), B(5,4), C(4,7), D(1,5) perform the following transformations:
• Scaling with parameter 2 towards X axis and 2 towards Y axis
• Translation with distance 3 towards X axis and 5 towards Y axis
• Reflection on the Y axis
• Shear parameter 1 on X axis and 2 on Y axis
• Rotation 90 degrees clockwise
Divide the coordinate axes and display the original square and transformed square in different colors.*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// 🔷 Original polygon
float x[4] = {2, 5, 4, 1};
float y[4] = {2, 4, 7, 5};

void drawPolygon(float x[], float y[]) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
        glVertex2f(x[i], y[i]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔷 Axes
    glColor3f(0,0,0);
    glBegin(GL_LINES);
        glVertex2i(-20,0); glVertex2i(20,0);
        glVertex2i(0,-20); glVertex2i(0,20);
    glEnd();

    // 🔴 Original
    glColor3f(1,0,0);
    drawPolygon(x, y);

    float xt[4], yt[4];

    // 🟢 Scaling (2,2)
    for(int i=0;i<4;i++){
        xt[i] = x[i] * 2;
        yt[i] = y[i] * 2;
    }
    glColor3f(0,1,0);
    drawPolygon(xt, yt);

    // 🔵 Translation (+3,+5)
    for(int i=0;i<4;i++){
        xt[i] = x[i] + 3;
        yt[i] = y[i] + 5;
    }
    glColor3f(0,0,1);
    drawPolygon(xt, yt);

    // 🟡 Reflection (Y-axis)
    for(int i=0;i<4;i++){
        xt[i] = -x[i];
        yt[i] = y[i];
    }
    glColor3f(1,1,0);
    drawPolygon(xt, yt);

    // 🟣 Shearing (1,2)
    for(int i=0;i<4;i++){
        xt[i] = x[i] + y[i];
        yt[i] = y[i] + 2*x[i];
    }
    glColor3f(1,0,1);
    drawPolygon(xt, yt);

    // 🔵 Rotation (90° clockwise)
    for(int i=0;i<4;i++){
        xt[i] = y[i];
        yt[i] = -x[i];
    }
    glColor3f(0,1,1);
    drawPolygon(xt, yt);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-20,20,-20,20);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Polygon Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
