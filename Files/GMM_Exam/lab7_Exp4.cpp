/*Experiment 4: 3D Rotation About Axes

Q. Given a cube with vertices (0,0,0), (50,0,0), (50,50,0), (0,50,0), (0,0,50), (50,0,50), (50,50,50), (0,50,50):

• Rotate the cube about X-axis by 45°

• Rotate the cube about Y-axis by 45°

• Rotate the cube about Z-axis by 45°

• Compute the transformed coordinates using rotation matrices

• Display original and rotated cube*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
using namespace std;


float angle = 45;

// Draw cube
void drawCube()
{
    glutWireCube(50);
}

// Draw axes
void drawAxes()
{
    glBegin(GL_LINES);

    // X-axis (Red)
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(100,0,0);

    // Y-axis (Green)
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,100,0);

    // Z-axis (Blue)
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,100);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera
    gluLookAt(150,150,150, 0,0,0, 0,1,0);

    drawAxes();

    // Original Cube (White)
    glPushMatrix();
    glColor3f(1,1,1);
    drawCube();
    glPopMatrix();

    // Rotate about X-axis (Red)
    glPushMatrix();
    glTranslatef(80,0,0);
    glRotatef(angle,1,0,0);
    glColor3f(1,0,0);
    drawCube();
    glPopMatrix();

    // Rotate about Y-axis (Green)
    glPushMatrix();
    glTranslatef(0,80,0);
    glRotatef(angle,0,1,0);
    glColor3f(0,1,0);
    drawCube();
    glPopMatrix();

    // Rotate about Z-axis (Blue)
    glPushMatrix();
    glTranslatef(0,0,80);
    glRotatef(angle,0,0,1);
    glColor3f(0,0,1);
    drawCube();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60,1,1,500);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);

    glutCreateWindow("3D Rotation of Cube");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}