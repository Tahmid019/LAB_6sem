// Using OpenGL Primitives display a rectangular window placed it
// in suitable position and draw a rectangle inside the window having red edges.

#include <GLUT/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_LINE_LOOP);
        glVertex2i(100, 100);
        glVertex2i(400, 100);
        glVertex2i(400, 400);
        glVertex2i(100, 400);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 150);
        glVertex2i(350, 150);
        glVertex2i(350, 350);
        glVertex2i(150, 350);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); 
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Rectangle Window");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}