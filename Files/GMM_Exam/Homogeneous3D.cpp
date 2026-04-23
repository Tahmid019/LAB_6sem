#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

float cube[8][3] = {
    {0,0,0}, {50,0,0}, {50,50,0}, {0,50,0},
    {0,0,50}, {50,0,50}, {50,50,50}, {0,50,50}
};

float result[8][3];
int choice;

// Draw cube
void drawCube(float v[8][3]) {
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<4;i++) glVertex3fv(v[i]);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for(int i=4;i<8;i++) glVertex3fv(v[i]);
    glEnd();

    glBegin(GL_LINES);
    for(int i=0;i<4;i++){
        glVertex3fv(v[i]);
        glVertex3fv(v[i+4]);
    }
    glEnd();
}

// Draw axes
void drawAxes() {
    glLineWidth(2);
    glBegin(GL_LINES);

    glColor3f(1,0,0); // X
    glVertex3f(-100,0,0);
    glVertex3f(100,0,0);

    glColor3f(0,1,0); // Y
    glVertex3f(0,-100,0);
    glVertex3f(0,100,0);

    glColor3f(0,0,1); // Z
    glVertex3f(0,0,-100);
    glVertex3f(0,0,100);

    glEnd();
}

// Translation
void translate() {
    float tx, ty, tz;
    cin >> tx >> ty >> tz;

    for(int i=0;i<8;i++){
        result[i][0] = cube[i][0] + tx;
        result[i][1] = cube[i][1] + ty;
        result[i][2] = cube[i][2] + tz;
    }
}

// Scaling
void scale() {
    float sx, sy, sz;
    cin >> sx >> sy >> sz;

    for(int i=0;i<8;i++){
        result[i][0] = cube[i][0] * sx;
        result[i][1] = cube[i][1] * sy;
        result[i][2] = cube[i][2] * sz;
    }
}

// Rotations
void rotateX() {
    float a; cin >> a;
    a = a * 3.1416 / 180;
    for(int i=0;i<8;i++){
        result[i][0] = cube[i][0];
        result[i][1] = cube[i][1]*cos(a) - cube[i][2]*sin(a);
        result[i][2] = cube[i][1]*sin(a) + cube[i][2]*cos(a);
    }
}

void rotateY() {
    float a; cin >> a;
    a = a * 3.1416 / 180;
    for(int i=0;i<8;i++){
        result[i][0] = cube[i][0]*cos(a) + cube[i][2]*sin(a);
        result[i][1] = cube[i][1];
        result[i][2] = -cube[i][0]*sin(a) + cube[i][2]*cos(a);
    }
}

void rotateZ() {
    float a; cin >> a;
    a = a * 3.1416 / 180;
    for(int i=0;i<8;i++){
        result[i][0] = cube[i][0]*cos(a) - cube[i][1]*sin(a);
        result[i][1] = cube[i][0]*sin(a) + cube[i][1]*cos(a);
        result[i][2] = cube[i][2];
    }
}

// 🔥 REFLECTIONS
void reflectXY() { // z = -z
    for(int i=0;i<8;i++){
        result[i][0]=cube[i][0];
        result[i][1]=cube[i][1];
        result[i][2]=-cube[i][2];
    }
}

void reflectYZ() { // x = -x
    for(int i=0;i<8;i++){
        result[i][0]=-cube[i][0];
        result[i][1]=cube[i][1];
        result[i][2]=cube[i][2];
    }
}

void reflectZX() { // y = -y
    for(int i=0;i<8;i++){
        result[i][0]=cube[i][0];
        result[i][1]=-cube[i][1];
        result[i][2]=cube[i][2];
    }
}

void reflectXaxis() {
    for(int i=0;i<8;i++){
        result[i][0]=cube[i][0];
        result[i][1]=-cube[i][1];
        result[i][2]=-cube[i][2];
    }
}

void reflectYaxis() {
    for(int i=0;i<8;i++){
        result[i][0]=-cube[i][0];
        result[i][1]=cube[i][1];
        result[i][2]=-cube[i][2];
    }
}

void reflectZaxis() {
    for(int i=0;i<8;i++){
        result[i][0]=-cube[i][0];
        result[i][1]=-cube[i][1];
        result[i][2]=cube[i][2];
    }
}

void reflectOrigin() {
    for(int i=0;i<8;i++){
        result[i][0]=-cube[i][0];
        result[i][1]=-cube[i][1];
        result[i][2]=-cube[i][2];
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(150,150,200, 0,0,0, 0,1,0);

    drawAxes();

    // Original
    glColor3f(1,0,0);
    drawCube(cube);

    // Transformed
    glPushMatrix();
    glTranslatef(80,0,0);
    glColor3f(0,1,0);
    drawCube(result);
    glPopMatrix();

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,500);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    cout << "1.Translate 2.Scale 3.RotX 4.RotY 5.RotZ\n";
    cout << "6.XY-plane 7.YZ-plane 8.ZX-plane\n";
    cout << "9.X-axis 10.Y-axis 11.Z-axis 12.Origin\n";
    cin >> choice;

    if(choice==1) translate();
    else if(choice==2) scale();
    else if(choice==3) rotateX();
    else if(choice==4) rotateY();
    else if(choice==5) rotateZ();
    else if(choice==6) reflectXY();
    else if(choice==7) reflectYZ();
    else if(choice==8) reflectZX();
    else if(choice==9) reflectXaxis();
    else if(choice==10) reflectYaxis();
    else if(choice==11) reflectZaxis();
    else if(choice==12) reflectOrigin();
    else return 0;

    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutCreateWindow("3D Transformations with Reflection");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}