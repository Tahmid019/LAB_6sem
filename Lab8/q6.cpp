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


Point scaleAboutPoint(Point p) {
    float px = 10, py = 10, pz = 10;

    
    float x = p.x - px;
    float y = p.y - py;
    float z = p.z - pz;

    
    x *= 2;
    y *= 2;
    z *= 2;

    
    x += px;
    y += py;
    z += pz;

    return { x, y, z };
}


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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Scaling About Arbitrary Point", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    
    Point scaled[8];
    for (int i = 0;i < 8;i++) {
        scaled[i] = scaleAboutPoint(cube[i]);

        cout << "Scaled: ("
            << scaled[i].x << ", "
            << scaled[i].y << ", "
            << scaled[i].z << ")\n";
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(45, 800.0 / 600.0, 0.1, 500);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        
        glTranslatef(0, 0, -200);
        glRotatef(30, 1, 0, 0);
        glRotatef(30, 0, 1, 0);

        
        glTranslatef(-25, -25, -25);

        
        drawAxes();

        
        glColor3f(1, 1, 1);
        drawCube(cube);

        
        glColor3f(0, 1, 1);
        drawCube(scaled);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}