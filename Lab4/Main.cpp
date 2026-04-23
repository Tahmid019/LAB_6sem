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





vector<float> customRect(vector<pair<float, float>> pts) {
    return {
		pts[0].first, pts[0].second,
		pts[1].first, pts[1].second,
		pts[2].first, pts[2].second,
		pts[3].first, pts[3].second
    };
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

    if (col == 0) glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);       
    else if (col == 1) glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);  
    else if (col == 2) glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);  
    else glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);               

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




void q1() {
    
    vector<pair<float, float>> points = {
		{0.0f, 0.3f},
		{0.3f, 0.3f},
		{0.3f, 0.0f},
		{0.0f, 0.0f}
	};

    vector<float> rect = customRect(points);
    drawShape(rect, rect.size() / 2, 0);

    
    Mat3 S = scaling(0.2f, 0.3f);
    Mat3 T = translation(0.1f, 0.5f);
    Mat3 Rx = reflectX();
    Mat3 Sh = shear(0.3f, 0.4f);
    Mat3 R = rotation(90.0f);

    Mat3 M = multiply(S, multiply(T, multiply(Rx, multiply(Sh, R))));
    vector<float> trans = applyTransform(rect, M);
    drawShape(trans, trans.size()  /2, 2);
    
}


void q2() {
    vector<float> tri = createTriangle(0.5f);
	drawShape(tri, tri.size() / 2, 0);

    Mat3 T = translation(0.5f, 0.0f);
    Mat3 S = scaling(1.5f, 1.5f);
    Mat3 Sh = shear(0.5f, 0.5f);
	Mat3 Rx = reflectX();   
    Mat3 R = rotation(30.0f);

    Mat3 M = multiply(T, multiply(S, multiply(Sh, multiply(Rx, R))));
    vector<float> trans = applyTransform(tri, M);
    drawShape(trans, tri.size() / 2, 1);
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

        
        q2();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
