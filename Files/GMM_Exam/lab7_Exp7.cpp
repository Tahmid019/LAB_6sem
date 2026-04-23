/*Experiment 7: Reflection in 3D

Q. Given a 3D cube:

• Reflect the object about XY-plane

• Reflect the object about YZ-plane

• Reflect the object about ZX-plane

• Compute transformed coordinates for each case

• Display original and reflected objects*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>


// Draw cube
void drawCube()
{
    glutWireCube(40);
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
    gluLookAt(200,200,200, 0,0,0, 0,1,0);

    drawAxes();

    // ---------- ORIGINAL ----------
    glPushMatrix();
    glColor3f(1,1,1); // White
    drawCube();
    glPopMatrix();

    // ---------- REFLECTION ABOUT XY (Z → -Z) ----------
    glPushMatrix();
    glTranslatef(80,0,0);
    glScalef(1,1,-1);
    glColor3f(1,0,0); // Red
    drawCube();
    glPopMatrix();

    // ---------- REFLECTION ABOUT YZ (X → -X) ----------
    glPushMatrix();
    glTranslatef(0,80,0);
    glScalef(-1,1,1);
    glColor3f(0,1,0); // Green
    drawCube();
    glPopMatrix();

    // ---------- REFLECTION ABOUT ZX (Y → -Y) ----------
    glPushMatrix();
    glTranslatef(0,0,80);
    glScalef(1,-1,1);
    glColor3f(0,0,1); // Blue
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

    glutCreateWindow("3D Reflection");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}