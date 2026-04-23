/*Experiment 5: Composite 3D Transformations

Q. Given a 3D cube of side length 50 units:

• Apply scaling with factors (2, 1.5, 1)

• Apply rotation about Z-axis by 30°

• Apply translation by (20, 30, 10)

• Perform transformations in order: Scale → Rotate → Translate

• Repeat with order: Translate → Rotate → Scale

• Compare the final outputs*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>


float angle = 30;

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
    gluLookAt(200,200,200, 0,0,0, 0,1,0);

    drawAxes();

    // ---------- ORIGINAL ----------
    glPushMatrix();
    glColor3f(1,1,1); // White
    drawCube();
    glPopMatrix();

    // ---------- SCALE → ROTATE → TRANSLATE ----------
    glPushMatrix();

    glTranslatef(20,30,10);     // last
    glRotatef(angle,0,0,1);     // middle
    glScalef(2,1.5,1);          // first

    glColor3f(1,0,0); // Red
    drawCube();

    glPopMatrix();

    // ---------- TRANSLATE → ROTATE → SCALE ----------
    glPushMatrix();

    glScalef(2,1.5,1);          // last
    glRotatef(angle,0,0,1);     // middle
    glTranslatef(20,30,10);     // first

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

    glutCreateWindow("Composite 3D Transformations");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}