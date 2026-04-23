#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

struct Point {
    float x, y, z;
};

Point cube[8] = {
    {0,0,0}, {50,0,0}, {50,50,0}, {0,50,0},
    {0,0,50}, {50,0,50}, {50,50,50}, {0,50,50}
};

Point reflectXY(Point p) { return { p.x, p.y, -p.z }; }
Point reflectYZ(Point p) { return { -p.x, p.y, p.z }; }
Point reflectZX(Point p) { return { p.x, -p.y, p.z }; }

void drawCube(Point c[8]) {
    glBegin(GL_LINES);
    int edges[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (int i = 0;i < 12;i++) {
        glVertex3f(c[edges[i][0]].x, c[edges[i][0]].y, c[edges[i][0]].z);
        glVertex3f(c[edges[i][1]].x, c[edges[i][1]].y, c[edges[i][1]].z);
    }
    glEnd();
}

void drawAxes() {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);

    glEnd();
}

void perspective(float fov, float aspect, float near, float far) {
    float top = tan(fov * 3.14159 / 360.0f) * near;
    float right = top * aspect;
    glFrustum(-right, right, -top, top, near, far);
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Reflection", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    Point xy[8], yz[8], zx[8];

    for (int i = 0;i < 8;i++) {
        xy[i] = reflectXY(cube[i]);
        yz[i] = reflectYZ(cube[i]);
        zx[i] = reflectZX(cube[i]);

        cout << "XY: (" << xy[i].x << "," << xy[i].y << "," << xy[i].z << ")\n";
        cout << "YZ: (" << yz[i].x << "," << yz[i].y << "," << yz[i].z << ")\n";
        cout << "ZX: (" << zx[i].x << "," << zx[i].y << "," << zx[i].z << ")\n";
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(45, 800.0 / 600.0, 0.1, 500);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0, 0, -300);
        glRotatef(30, 1, 0, 0);
        glRotatef(30, 0, 1, 0);

        drawAxes();

        glPushMatrix();
        glTranslatef(-120, 0, 0);
        glColor3f(1, 1, 1);
        drawCube(cube);
        glColor3f(1, 0, 0);
        drawCube(xy);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0, 0);
        glColor3f(1, 1, 1);
        drawCube(cube);
        glColor3f(0, 1, 0);
        drawCube(yz);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(120, 0, 0);
        glColor3f(1, 1, 1);
        drawCube(cube);
        glColor3f(0, 0, 1);
        drawCube(zx);
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}