/* Given a square object with coordinate points A(0, 3), B(3, 3), C(3, 0), D(0, 0) and perform
1. Scaling with parameter 2 towards X axis and 3 towards Y axis
2. Translation with distance 1 towards X axis and 5 towards Y axis
3. Reflection on the X axis
4. Shear parameter 3 on X axis and 4 on Y axis
5. Rotation 90 degrees anticlockwise */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

float x[4] = {0, 3, 3, 0};
float y[4] = {3, 3, 0, 0};

int choice;

void drawSquare(float x[], float y[]) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
        glVertex2f(x[i], y[i]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Axes
    glColor3f(0,0,0);
    glBegin(GL_LINES);
        glVertex2i(-20,0); glVertex2i(20,0);
        glVertex2i(0,-20); glVertex2i(0,20);
    glEnd();

    // Original
    glColor3f(1,0,0);
    drawSquare(x, y);

    float xt[4], yt[4];

    switch(choice) {

        case 1: // Scaling
            for(int i=0;i<4;i++){
                xt[i] = x[i]*2;
                yt[i] = y[i]*3;
            }
            glColor3f(0,1,0);
            break;

        case 2: // Translation
            for(int i=0;i<4;i++){
                xt[i] = x[i]+1;
                yt[i] = y[i]+5;
            }
            glColor3f(0,0,1);
            break;

        case 3: // Reflection
            for(int i=0;i<4;i++){
                xt[i] = x[i];
                yt[i] = -y[i];
            }
            glColor3f(1,1,0);
            break;

        case 4: // Shearing
            for(int i=0;i<4;i++){
                xt[i] = x[i] + 3*y[i];
                yt[i] = y[i] + 4*x[i];
            }
            glColor3f(1,0,1);
            break;

        case 5: // Rotation
            for(int i=0;i<4;i++){
                xt[i] = -y[i];
                yt[i] = x[i];
            }
            glColor3f(0,1,1);
            break;

        default:
            cout << "Invalid choice";
            return;
    }

    drawSquare(xt, yt);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-20,20,-20,20);
}

int main(int argc, char** argv) {

    // 🔥 MENU IN TERMINAL
    cout << "MENU:\n";
    cout << "1. Scaling\n";
    cout << "2. Translation\n";
    cout << "3. Reflection\n";
    cout << "4. Shearing\n";
    cout << "5. Rotation\n";
    cout << "Enter choice: ";
    cin >> choice;

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

/*MENU:
1. Scaling
2. Translation
3. Reflection
4. Shearing
5. Rotation
Enter choice: 2*/