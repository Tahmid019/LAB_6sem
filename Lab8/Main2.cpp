#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define PI 3.1415926535

GLuint VAO, VBO, shaderProgram;

// -------- SHADERS --------
const char* vs = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
}
)";

const char* fs = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}
)";

// -------- SHADER SETUP --------
GLuint compile(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    return s;
}

void setupShader()
{
    GLuint v = compile(GL_VERTEX_SHADER, vs);
    GLuint f = compile(GL_FRAGMENT_SHADER, fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, v);
    glAttachShader(shaderProgram, f);
    glLinkProgram(shaderProgram);

    glDeleteShader(v);
    glDeleteShader(f);
}

// -------- BUFFERS --------
void setupBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// -------- DRAW LINE --------
void drawLine3D(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float r, float g, float b)
{
    float line[] = {
        x1,y1,z1,
        x2,y2,z2
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_DYNAMIC_DRAW);

    glUniform3f(glGetUniformLocation(shaderProgram, "color"), r, g, b);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

// -------- AXES --------
void drawAxes()
{
    drawLine3D(0, 0, 0, 100, 0, 0, 1, 0, 0); // X
    drawLine3D(0, 0, 0, 0, 100, 0, 0, 1, 0); // Y
    drawLine3D(0, 0, 0, 0, 0, 100, 0, 0, 1); // Z
}

// -------- CUBE --------
void drawCube(vector<vector<float>>& c, float r, float g, float b)
{
    int e[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (auto& ed : e)
    {
        auto& p1 = c[ed[0]];
        auto& p2 = c[ed[1]];

        drawLine3D(p1[0], p1[1], p1[2],
            p2[0], p2[1], p2[2],
            r, g, b);
    }
}

// -------- MVP (FIXED) --------
vector<float> getMVP()
{
    float aspect = 800.0f / 600.0f;
    float fov = 45.0f * PI / 180.0f;
    float near = 0.1f, far = 1000.0f;

    float f = 1.0f / tan(fov / 2);

    // COLUMN-MAJOR projection matrix
    float proj[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0
    };

    // camera back
    float view[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,-150,1
    };

    vector<float> mvp(16, 0);

    // multiply proj * view
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            for (int k = 0; k < 4; k++)
                mvp[col * 4 + row] += proj[k * 4 + row] * view[col * 4 + k];

    return mvp;
}

// -------- MAIN --------
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(800, 600, "3D Reflection", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGL();

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    setupShader();
    setupBuffers();

    // cube
    vector<vector<float>> cube = {
        {-15,-15,-15}, {15,-15,-15}, {15,15,-15}, {-15,15,-15},
        {-15,-15,15},  {15,-15,15},  {15,15,15},  {-15,15,15}
    };

    vector<vector<float>> xy, yz, zx;

    for (auto& p : cube) {
        xy.push_back({ p[0],p[1],-p[2] });
        yz.push_back({ -p[0],p[1],p[2] });
        zx.push_back({ p[0],-p[1],p[2] });
    }


    for (auto& p : xy) p[0] += 50;
    for (auto& p : yz) p[0] -= 50;
    for (auto& p : zx) p[1] += 50;

    while (!glfwWindowShouldClose(w))
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        auto mvp = getMVP();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"),
            1, GL_FALSE, mvp.data());

        drawAxes();

        drawCube(cube, 0, 0, 1);   // blue
        drawCube(xy, 0, 1, 0);   // green
        drawCube(yz, 1, 0, 0);   // red
        drawCube(zx, 1, 1, 0);   // yellow

        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
}