/*. Implement a triangle and perform the following 2D transformation over the triangle
1. Translation
2. Scaling
3. Shearing
4. Reflection and
5. Rotation.
*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
using namespace std;

// 🔺 Triangle points
float x[3] = {0, 4, 2};
float y[3] = {0, 0, 4};

int choice;

void drawTriangle(float x[], float y[]) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 3; i++)
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

    // 🔴 Original triangle
    glColor3f(1,0,0);
    drawTriangle(x, y);

    float xt[3], yt[3];

    switch(choice) {

        case 1: // Translation
            for(int i=0;i<3;i++){
                xt[i] = x[i] + 3;
                yt[i] = y[i] + 2;
            }
            glColor3f(0,1,0);
            break;

        case 2: // Scaling
            for(int i=0;i<3;i++){
                xt[i] = x[i] * 2;
                yt[i] = y[i] * 2;
            }
            glColor3f(0,0,1);
            break;

        case 3: // Shearing
            for(int i=0;i<3;i++){
                xt[i] = x[i] + 1*y[i];
                yt[i] = y[i] + 1*x[i];
            }
            glColor3f(1,1,0);
            break;

        case 4: // Reflection (X-axis)
            for(int i=0;i<3;i++){
                xt[i] = x[i];
                yt[i] = -y[i];
            }
            glColor3f(1,0,1);
            break;

        case 5: // Rotation 90° ACW
            for(int i=0;i<3;i++){
                xt[i] = -y[i];
                yt[i] = x[i];
            }
            glColor3f(0,1,1);
            break;

        default:
            cout << "Invalid choice";
            return;
    }

    drawTriangle(xt, yt);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-20,20,-20,20);
}

int main(int argc, char** argv) {

    // 🔥 MENU (terminal-based)
    cout << "MENU:\n";
    cout << "1. Translation\n";
    cout << "2. Scaling\n";
    cout << "3. Shearing\n";
    cout << "4. Reflection\n";
    cout << "5. Rotation\n";
    cout << "Enter choice: ";
    cin >> choice;

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Triangle Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

/*MENU:
1. Translation
2. Scaling
3. Shearing
4. Reflection
5. Rotation
Enter choice: 1*/