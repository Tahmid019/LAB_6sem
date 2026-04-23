#include "help.h"

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
uniform vec3 uColor;
void main()
{
    FragColor = vec4(uColor, 1.0); 
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








vector<float> createRectangle(float w, float h) {
    return {
        -w / 2, -h / 2,
         w / 2, -h / 2,
         w / 2,  h / 2,
        -w / 2,  h / 2
    };
}

vector<float> createSquare(float size) {
    return createRectangle(size, size);
}

vector<float> createTriangle(float side) {
    /**
        equil;ateral triangle
    **/

    float h = (sqrt(3.0f) / 2.0f) * side;

    return {
        0.0f,        h / 2.0f,
       -side / 2.0f, -h / 2.0f,
        side / 2.0f, -h / 2.0f
    };

}


void drawShape(const vector<float>& vertices, int count, int col = 0)
{
    setupBuffers(vertices);
    glUseProgram(shaderProgram);
    glLineWidth(3.0f);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    if (col == 0) glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);          // red   
    else if (col == 1) glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);     // green
    else if (col == 2) glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);     // blue
    else if (col == 3) glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f);     // yellow
    else if (col == 4) glUniform3f(colorLoc, 1.0f, 0.0f, 1.0f);     // magenta
    else glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);                   // black

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, count);
}

void drawPoint(float x, float y, int col = 2)
{
    vector<float> p = { x, y };
    setupBuffers(p);

    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    if (col == 0) glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
    else if (col == 1) glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
    else glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);

    glPointSize(10.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

void drawAxes()
{
    vector<float> xAxis = {
        -1.0f, 0.0f,
         1.0f, 0.0f
    };

    vector<float> yAxis = {
        0.0f, -1.0f,
        0.0f,  1.0f
    };

    setupBuffers(xAxis);
    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);

    setupBuffers(yAxis);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);

}


void q1()
{

    drawAxes();
    /*
    vector<float> poly = {
        0.2f, 0.2f,
        0.5f, 0.4f,
        0.4f, 0.7f,
        0.1f, 0.5f
    };
    */
    vector<float> poly = {
        0.08f, 0.08f,
        0.20f, 0.16f,
        0.16f, 0.28f,
        0.04f, 0.20f
    };
    drawShape(poly, poly.size() / 2, 0);

    Mat3 S = scaling(2.0f, 2.0f);
    vector<float> scaled = applyTransform(poly, S);
    //drawShape(scaled, scaled.size() / 2, 0);

    Mat3 T = translation(0.3f, 0.5f);
    vector<float> translated = applyTransform(scaled, T);
    //drawShape(translated, translated.size() / 2, 1);

    Mat3 Rf = reflectionY();
    vector<float> reflected = applyTransform(translated, Rf);
    //drawShape(reflected, reflected.size() / 2, 2);

    Mat3 Sh = shear(1.0f, 2.0f);
    vector<float> sheared = applyTransform(reflected, Sh);
    //drawShape(sheared, sheared.size() / 2, 3);

    Mat3 R = rotation(-90.0f);
    vector<float> rotated = applyTransform(sheared, R);
    drawShape(rotated, rotated.size() / 2, 4);
}

void q2()
{
    drawAxes();

    int r = 5;
    int x = 0;
    int y = r;

    int p = 1 - r;

    float scale = 0.08f;

    while (x <= y)
    {
        drawPoint(x * scale, y * scale, 2);
        drawPoint(y * scale, x * scale, 2);
        drawPoint(-x * scale, y * scale, 2);
        drawPoint(-y * scale, x * scale, 2);
        drawPoint(-x * scale, -y * scale, 2);
        drawPoint(-y * scale, -x * scale, 2);
        drawPoint(x * scale, -y * scale, 2);
        drawPoint(y * scale, -x * scale, 2);

        if (p < 0)
        {
            p = p + 2 * x + 3;
        }
        else
        {
            p = p + 2 * (x - y) + 5;
            y--;
        }

        x++;
    }
}

void q3()
{
    drawAxes();

    vector<float> sq = createSquare(0.15f);

    drawShape(sq, sq.size() / 2, 0);

    Mat3 T = translation(0.4f, 0.3f);
    vector<float> translated = applyTransform(sq, T);
    //drawShape(translated, translated.size() / 2, 1);

    Mat3 S = scaling(2.0f, 2.0f);
    vector<float> scaled = applyTransform(translated, S);
    //drawShape(scaled, scaled.size() / 2, 2);

    Mat3 Rx = reflectionX();
    vector<float> reflected = applyTransform(scaled, Rx);
    //drawShape(reflected, reflected.size() / 2, 3);

    Mat3 Sh = shear(2.0f, 1.0f);
    vector<float> sheared = applyTransform(reflected, Sh);
    //drawShape(sheared, sheared.size() / 2, 4);

    Mat3 R = rotation(90.0f);
    vector<float> rotated = applyTransform(sheared, R);
    drawShape(rotated, rotated.size() / 2, 4);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Transformation", NULL, NULL);
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        cout << "Enter q: ";
        int q = 1;
        cin >> q;

        switch (q) {
        case 1: {
            q1();
            break;
        }
        case 2: {
            q2();
            break;
        }
        case 3: {
            q3();
            break;
        }
        default:
            q1();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}