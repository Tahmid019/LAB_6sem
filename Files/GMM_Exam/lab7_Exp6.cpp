/*Experiment 6: Scaling About an Arbitrary Point

Q. Given a 3D object and a fixed point P(10,10,10):

• Translate the object so that point P becomes origin

• Apply scaling with factors (2,2,2)

• Translate the object back to original position

• Compute the final transformed coordinates

• Display original and scaled object*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>

// Fixed point P
float px = 10, py = 10, pz = 10;

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
    gluLookAt(150,150,150, 0,0,0, 0,1,0);

    drawAxes();

    // ---------- ORIGINAL CUBE ----------
    glPushMatrix();
    glColor3f(1,1,1); // White
    drawCube();
    glPopMatrix();

    // ---------- SCALING ABOUT POINT P ----------
    glPushMatrix();

    // Step 3: Translate back
    glTranslatef(px, py, pz);

    // Step 2: Scale
    glScalef(2,2,2);

    // Step 1: Move P to origin
    glTranslatef(-px, -py, -pz);

    glColor3f(0,1,0); // Green
    drawCube();

    glPopMatrix();

    // Draw point P
    glPointSize(6);
    glBegin(GL_POINTS);
    glColor3f(1,1,0); // Yellow
    glVertex3f(px,py,pz);
    glEnd();

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

    glutCreateWindow("Scaling about Arbitrary Point");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}