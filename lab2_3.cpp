#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const int SCALE = 200;   // Larger scale for big circle visibility

GLuint VAO, VBO;

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
uniform vec3 uColor;
void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

GLuint createShader()
{
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void setupBuffers(const vector<float>& pts)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        pts.size() * sizeof(float),
        pts.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// ================= MIDPOINT CIRCLE =================
void midpointCircle(int xc, int yc, int r, vector<float>& pts)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    auto add = [&](int px, int py)
        {
            pts.push_back(px / (float)SCALE);
            pts.push_back(py / (float)SCALE);
        };

    while (x <= y)
    {
        add(xc + x, yc + y);
        add(xc + y, yc + x);
        add(xc - x, yc + y);
        add(xc - y, yc + x);
        add(xc - x, yc - y);
        add(xc - y, yc - x);
        add(xc + x, yc - y);
        add(xc + y, yc - x);

        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

int main()
{
    glfwInit();
    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Midpoint Circle Algorithm", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    GLuint shaderProgram = createShader();
    glUseProgram(shaderProgram);

    vector<float> circle1Pts;
    vector<float> circle2Pts;

    // ================= QUESTION VALUES =================
    midpointCircle(10, 10, 8, circle1Pts);        // Small circle
    midpointCircle(100, 100, 50, circle2Pts);    // Large circle

    // Merge both circles into one buffer
    vector<float> allPts = circle1Pts;
    allPts.insert(allPts.end(), circle2Pts.begin(), circle2Pts.end());

    setupBuffers(allPts);

    glPointSize(4.0f);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        // Draw first circle (RED)
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_POINTS, 0, circle1Pts.size() / 2);

        // Draw second circle (GREEN)
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_POINTS,
            circle1Pts.size() / 2,
            circle2Pts.size() / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
