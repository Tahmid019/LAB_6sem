/* Implement a square and perform the following transformations:
• Translation with distance 4 towards X axis and 3 towards Y axis
• Scaling with parameter 2 on X axis and 2 on Y axis
• Reflection on the X axis
• Shear parameter 2 on X axis and 1 on Y axis
• Rotation 90 degrees anticlockwise
Divide the coordinate axes and display the original square and transformed square in different colors.*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// 🔷 Original square
float x[4] = {0, 3, 3, 0};
float y[4] = {3, 3, 0, 0};

void drawSquare(float x[], float y[]) {
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

    // 🔴 Original square
    glColor3f(1,0,0);
    drawSquare(x, y);

    float xt[4], yt[4];

    // 🟢 Translation (+4,+3)
    for(int i=0;i<4;i++){
        xt[i] = x[i] + 4;
        yt[i] = y[i] + 3;
    }
    glColor3f(0,1,0);
    drawSquare(xt, yt);

    // 🔵 Scaling (2,2)
    for(int i=0;i<4;i++){
        xt[i] = x[i] * 2;
        yt[i] = y[i] * 2;
    }
    glColor3f(0,0,1);
    drawSquare(xt, yt);

    // 🟡 Reflection (X-axis)
    for(int i=0;i<4;i++){
        xt[i] = x[i];
        yt[i] = -y[i];
    }
    glColor3f(0,0,0);
    drawSquare(xt, yt);

    // 🟣 Shearing (2,1)
    for(int i=0;i<4;i++){
        xt[i] = x[i] + 2*y[i];
        yt[i] = y[i] + 1*x[i];
    }
    glColor3f(1,0,1);
    drawSquare(xt, yt);

    // 🔷 Rotation (90° anticlockwise)
    for(int i=0;i<4;i++){
        xt[i] = -y[i];
        yt[i] = x[i];
    }
    glColor3f(0,1,1);
    drawSquare(xt, yt);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-20,20,-20,20);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Square Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}