#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;


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
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
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


void drawCenterRectangle()
{
    vector<float> rectVertices = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    setupBuffers(rectVertices);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glLineWidth(2.5f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}


void drawDDALine()
{
    float x0, y0, x1, y1;
    cout << "Enter x0 y0 x1 y1 (range -1 to 1): ";
    cin >> x0 >> y0 >> x1 >> y1;

    vector<float> points;

    float dx = x1 - x0;
    float dy = y1 - y0;

    int steps = max(abs(dx * 100), abs(dy * 100));
    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++)
    {
        points.push_back(x);
        points.push_back(y);
        x += xInc;
        y += yInc;
    }

    setupBuffers(points);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glPointSize(3.0f);
    glDrawArrays(GL_POINTS, 0, points.size() / 2);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "DDA & Rectangle", NULL, NULL);
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



    bool useRectangle = false;   

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (useRectangle)
            drawCenterRectangle();
        else
            drawDDALine();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
