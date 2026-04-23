/*All Homogeneous Transformations*/

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

void drawAxes() {
    glColor3f(0,0,0); // black
    glBegin(GL_LINES);

    // X-axis
    glVertex2f(-200,0);
    glVertex2f(200,0);

    // Y-axis
    glVertex2f(0,-200);
    glVertex2f(0,200);

    glEnd();
}

// Square
float square[4][3] = {
    {0,0,1}, {50,0,1}, {50,50,1}, {0,50,1}
};

float result[4][3];
int choice;

// Matrix multiplication
void multiply(float a[3][3], float b[3][3], float res[3][3]) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            res[i][j] = 0;
            for(int k=0;k<3;k++)
                res[i][j] += a[i][k]*b[k][j];
        }
}

// Apply transformation
void apply(float mat[3][3]) {
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            result[i][j] = 0;
            for(int k=0;k<3;k++)
                result[i][j] += mat[j][k]*square[i][k];
        }
    }
}

// Draw square
void draw(float obj[4][3]) {
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<4;i++)
        glVertex2f(obj[i][0], obj[i][1]);
    glEnd();
}

// Transformations
void translation() {
    float tx, ty;
    cout << "Enter tx, ty: ";
    cin >> tx >> ty;

    float T[3][3] = {
        {1,0,tx},
        {0,1,ty},
        {0,0,1}
    };

    apply(T);
}

void scaling() {
    float sx, sy;
    cout << "Enter sx, sy: ";
    cin >> sx >> sy;

    float S[3][3] = {
        {sx,0,0},
        {0,sy,0},
        {0,0,1}
    };

    apply(S);
}

void rotation() {
    float angle;
    cout << "Enter angle: ";
    cin >> angle;
    angle = angle * 3.1416 / 180;

    float R[3][3] = {
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0,0,1}
    };

    apply(R);
}

// Rotation about arbitrary point
void rotationArbitrary() {
    float angle, px, py;
    cout << "Enter angle: ";
    cin >> angle;
    cout << "Enter pivot (px py): ";
    cin >> px >> py;

    angle = angle * 3.1416 / 180;

    float T1[3][3] = {
        {1,0,-px},{0,1,-py},{0,0,1}
    };

    float R[3][3] = {
        {cos(angle), -sin(angle),0},
        {sin(angle), cos(angle),0},
        {0,0,1}
    };

    float T2[3][3] = {
        {1,0,px},{0,1,py},{0,0,1}
    };

    float temp[3][3], final[3][3];

    multiply(R, T1, temp);
    multiply(T2, temp, final);

    apply(final);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    // 🔴 Original
    glColor3f(1,0,0);
    draw(square);

    // 🟢 Transformed
    glColor3f(0,1,0);
    draw(result);

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(-100,200,-100,200);
}

int main(int argc, char** argv) {

    cout << "1. Translation\n2. Scaling\n3. Rotation\n4. Rotation about arbitrary point\n";
    cout << "Enter choice: ";
    cin >> choice;

    if(choice==1) translation();
    else if(choice==2) scaling();
    else if(choice==3) rotation();
    else if(choice==4) rotationArbitrary();
    else {
        cout << "Invalid\n";
        return 0;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("2D Homogeneous Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}