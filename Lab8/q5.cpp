#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
using namespace std;

#define M_PI 3.14159265358979323846

float degToRad(float deg) {
    return deg * M_PI / 180.0f;
}

struct Point {
    float x, y, z;
};

// Rotation functions
Point rotateX(Point p, float angle) {
    float rad = degToRad(angle);
    return {
        p.x,
        p.y * cos(rad) - p.z * sin(rad),
        p.y * sin(rad) + p.z * cos(rad)
    };
}

Point rotateY(Point p, float angle) {
    float rad = degToRad(angle);
    return {
        p.x * cos(rad) + p.z * sin(rad),
        p.y,
        -p.x * sin(rad) + p.z * cos(rad)
    };
}

Point rotateZ(Point p, float angle) {
    float rad = degToRad(angle);
    return {
        p.x * cos(rad) - p.y * sin(rad),
        p.x * sin(rad) + p.y * cos(rad),
        p.z
    };
}

// Apply all rotations
Point transform(Point p) {
    p = rotateX(p, 45);
    p = rotateY(p, 45);
    p = rotateZ(p, 45);
    return p;
}

// Cube vertices
Point cube[8] = {
    {0,0,0}, {50,0,0}, {50,50,0}, {0,50,0},
    {0,0,50}, {50,0,50}, {50,50,50}, {0,50,50}
};

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

void perspective(float fov, float aspect, float near, float far) {
    float top = tan(fov * M_PI / 360.0f) * near;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glFrustum(left, right, bottom, top, near, far);
}

void drawAxes() {
    glBegin(GL_LINES);

    // X-axis (Red)
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);

    // Y-axis (Green)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);

    // Z-axis (Blue)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);

    glEnd();
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Cube Rotation", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    Point rotated[8];
    for (int i = 0;i < 8;i++) {
        rotated[i] = transform(cube[i]);

        cout << "Rotated: ("
            << rotated[i].x << ", "
            << rotated[i].y << ", "
            << rotated[i].z << ")\n";
    }


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    drawAxes();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(45, 800.0 / 600.0, 0.1, 500);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Move camera back
        glTranslatef(0, 0, -200);

        glRotatef(30, 1, 0, 0);   //
        glRotatef(30, 0, 1, 0);   //

        // Center cube
        glTranslatef(-25, -25, -25);

        glColor3f(1, 1, 1);
        drawCube(cube);

        glColor3f(0, 1, 0);
        drawCube(rotated);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}