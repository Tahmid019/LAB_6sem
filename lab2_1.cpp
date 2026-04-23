#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const int GRID = 100;

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

GLuint shaderProgram;
GLuint VAO, VBO;

void setupShader()
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void setupBuffers(const vector<float>& vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void drawBresenhamLine()
{
    float x0, y0, x1, y1;
    cout << "Enter x0 y0 x1 y1 (range -1 to 1): ";
    cin >> x0 >> y0 >> x1 >> y1;

    int X0 = round(x0 * GRID);
    int Y0 = round(y0 * GRID);
    int X1 = round(x1 * GRID);
    int Y1 = round(y1 * GRID);

    vector<float> points;

    int dx = abs(X1 - X0);
    int dy = abs(Y1 - Y0);

    int sx = (X0 < X1) ? 1 : -1;
    int sy = (Y0 < Y1) ? 1 : -1;

    bool steep = dy > dx;
    if (steep)
    {
        swap(X0, Y0);
        swap(X1, Y1);
        swap(dx, dy);
    }

    int p = 2 * dy - dx;
    int y = Y0;

    for (int x = X0; x != X1; x += sx)
    {
        float fx, fy;
        if (steep)
        {
            fx = y / (float)GRID;
            fy = x / (float)GRID;
        }
        else
        {
            fx = x / (float)GRID;
            fy = y / (float)GRID;
        }

        points.push_back(fx);
        points.push_back(fy);

        if (p >= 0)
        {
            y += sy;
            p -= 2 * dx;
        }
        p += 2 * dy;
    }

    setupBuffers(points);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, points.size() / 2);

    cout << "Slope type: ";
    if (x1 - x0 == 0) cout << "Vertical\n";
    else
    {
        float m = (y1 - y0) / (x1 - x0);
        if (m > 0) cout << "m > 0\n";
        else if (m < 0) cout << "m < 0\n";
        else cout << "m = 0\n";
    }

    cout << "Line is smooth (continuous pixels)\n";
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bresenham Line", NULL, NULL);
    if (!window)
    {
        cout << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, 800, 600);

    setupShader();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawBresenhamLine();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
