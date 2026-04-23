#include "help.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>


using namespace std;

#define M_PI 3.14159265358979323846


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

void drawLine(float x1, float y1, float x2, float y2, int col = 0)
{
    vector<float> line = {
        x1, y1,
        x2, y2
    };

    setupBuffers(line);

    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    if (col == 0) glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);          // red
    else if (col == 1) glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);     // green
    else if (col == 2) glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);     // blue
    else if (col == 3) glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f);     // yellow
    else if (col == 4) glUniform3f(colorLoc, 1.0f, 0.0f, 1.0f);     // magenta
    else glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);                   // black

    glLineWidth(3.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void drawAxes()
{
    glUseProgram(shaderProgram);

    GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f); // black

    vector<float> axes = {
        // X axis
        -1.0f,  0.0f,
         1.0f,  0.0f,

         // Y axis
          0.0f, -1.0f,
          0.0f,  1.0f
    };

    setupBuffers(axes);
    glBindVertexArray(VAO);

    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 4);
}

void draw3DAxes(float d, float m, float b)
{
    auto project = [&](float x, float y, float z) {
        float xp = x + 0.5 * z;
        float yp = y + 0.5 * z;
        return normalize(xp, yp, d, m, b);
        };

    auto O = project(0, 0, 0);

    auto X = project(200, 0, 0);
    drawLine(O.first, O.second, X.first, X.second, 2);

    auto Y = project(0, 200, 0);
    drawLine(O.first, O.second, Y.first, Y.second, 3);

    auto Z = project(0, 0, 200);
    drawLine(O.first, O.second, Z.first, Z.second, 4);
}

pair<int, int> project(float x, float y, float z)
{
    float xp = x - z * 0.5f;
    float yp = y - z * 0.5f;

    xp += 200;   
    yp += 200;   

    return { (int)xp, (int)yp };
}

void drawCube(vector<vector<float>>& cube, int col)
{
    vector<pair<int, int>> edges = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (auto& e : edges)
    {
        auto p1 = project(cube[e.first][0], cube[e.first][1], cube[e.first][2]);
        auto p2 = project(cube[e.second][0], cube[e.second][1], cube[e.second][2]);

        float x1 = (p1.first - 300.0f) / 300.0f;
        float y1 = (p1.second - 300.0f) / 300.0f;

        float x2 = (p2.first - 300.0f) / 300.0f;
        float y2 = (p2.second - 300.0f) / 300.0f;

        drawLine(x1, y1, x2, y2, col);
    }
}

void q1()
{
    float angle = 30.0f * M_PI / 180.0f;
    float c = cos(angle);
    float s = sin(angle);

    vector<vector<float>> Rz = {
        {c, -s, 0},
        {s,  c, 0},
        {0,  0, 1}
    };

    auto multiply = [&](vector<vector<float>> R, vector<float> P) {
        vector<float> res(3, 0);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                res[i] += R[i][j] * P[j];
        return res;
        };

    auto scale = [&](vector<float> p) {
        return vector<float>{p[0] * 2.0f, p[1] * 1.5f, p[2] * 1.0f};
        };

    auto translate = [&](vector<float> p) {
        return vector<float>{p[0] + 20.0f, p[1] + 30.0f, p[2] + 10.0f};
        };

    vector<vector<float>> cube = {
        {-25,-25,-25}, {25,-25,-25}, {25,25,-25}, {-25,25,-25},
        {-25,-25,25},  {25,-25,25},  {25,25,25},  {-25,25,25}
    };

    // Order 1
    vector<vector<float>> final1;
    for (auto& p : cube)
    {
        auto p1 = scale(p);
        auto p2 = multiply(Rz, p1);
        auto p3 = translate(p2);
        final1.push_back(p3);
    }

    // Order 2
    vector<vector<float>> final2;
    for (auto& p : cube)
    {
        auto p1 = translate(p);
        auto p2 = multiply(Rz, p1);
        auto p3 = scale(p2);
        final2.push_back(p3);
    }

    // ===================================
    drawAxes();

    drawCube(cube, 2);     // blue 
    drawCube(final1, 1);   // green
    drawCube(final2, 0);   // red

    cout << "\n--- Order 1: Scale -> Rotate -> Translate ---\n";
    for (auto& p : final1)
        cout << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";

    cout << "\n--- Order 2: Translate -> Rotate -> Scale ---\n";
    for (auto& p : final2)
        cout << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";

    cout << "\nTransformation order matters\n";
}

void q2()
{
    float px = 10.0f, py = 10.0f, pz = 10.0f;

    // Scaling factor
    float sx = 2.0f, sy = 2.0f, sz = 2.0f;

    vector<vector<float>> cube = {
        {-25,-25,-25}, {25,-25,-25}, {25,25,-25}, {-25,25,-25},
        {-25,-25,25},  {25,-25,25},  {25,25,25},  {-25,25,25}
    };

    // Step functions
    auto translateToOrigin = [&](vector<float> p) {
        return vector<float>{p[0] - px, p[1] - py, p[2] - pz};
        };

    auto scale = [&](vector<float> p) {
        return vector<float>{p[0] * sx, p[1] * sy, p[2] * sz};
        };

    auto translateBack = [&](vector<float> p) {
        return vector<float>{p[0] + px, p[1] + py, p[2] + pz};
        };

    vector<vector<float>> finalCube;

    cout << "\n--- Scaling About Point (10,10,10) ---\n";

    for (auto& p : cube)
    {
        auto p1 = translateToOrigin(p);
        auto p2 = scale(p1);
        auto p3 = translateBack(p2);

        finalCube.push_back(p3);

        cout << "Original: (" << p[0] << "," << p[1] << "," << p[2] << ")";
        cout << " -> Scaled: ("
            << round(p3[0] * 100) / 100 << ", "
            << round(p3[1] * 100) / 100 << ", "
            << round(p3[2] * 100) / 100 << ")\n";
    }

    // Draw
    drawAxes();

    drawCube(cube, 2);       // original (blue)
    drawCube(finalCube, 1);  // scaled (green)
}

void q3()
{
    vector<vector<float>> cube = {
        {-25,-25,-25}, {25,-25,-25}, {25,25,-25}, {-25,25,-25},
        {-25,-25,25},  {25,-25,25},  {25,25,25},  {-25,25,25}
    };

    vector<vector<float>> xy_ref, yz_ref, zx_ref;

    // Reflection transformations
    for (auto& p : cube)
    {
        xy_ref.push_back({ p[0],  p[1], -p[2] }); // XY
        yz_ref.push_back({ -p[0],  p[1],  p[2] }); // YZ
        zx_ref.push_back({ p[0], -p[1],  p[2] }); // ZX
    }

    // Spread cubes so they are visible
    for (auto& p : xy_ref) p[0] += 80;   // right
    for (auto& p : yz_ref) p[0] -= 80;   // left
    for (auto& p : zx_ref) p[1] += 80;   // up

    cout << "\n--- Reflection Outputs ---\n";

    // Print
    for (int i = 0; i < cube.size(); i++)
    {
        cout << "Original: (" << cube[i][0] << "," << cube[i][1] << "," << cube[i][2] << ")";

        cout << " | XY: (" << xy_ref[i][0] << "," << xy_ref[i][1] << "," << xy_ref[i][2] << ")";
        cout << " | YZ: (" << yz_ref[i][0] << "," << yz_ref[i][1] << "," << yz_ref[i][2] << ")";
        cout << " | ZX: (" << zx_ref[i][0] << "," << zx_ref[i][1] << "," << zx_ref[i][2] << ")\n";
    }

    // Draw axes
    drawAxes();

    // Draw cubes
    drawCube(cube, 2);     // Blue - Original
    drawCube(xy_ref, 1);   // Green - XY reflection
    drawCube(yz_ref, 0);   // Red - YZ reflection
    drawCube(zx_ref, 3);   // Yellow - ZX reflection

    // -------- DRAW REFLECTION PLANES (DASHED) --------

    auto drawDashedLine = [&](float x1, float y1, float x2, float y2, int col)
        {
            int segments = 20;
            for (int i = 0; i < segments; i += 2)
            {
                float t1 = i / (float)segments;
                float t2 = (i + 1) / (float)segments;

                float sx = x1 + (x2 - x1) * t1;
                float sy = y1 + (y2 - y1) * t1;

                float ex = x1 + (x2 - x1) * t2;
                float ey = y1 + (y2 - y1) * t2;

                drawLine(sx, sy, ex, ey, col);
            }
        };

    drawDashedLine(0.0f, -1.0f, 0.0f, 1.0f, 4);

    drawDashedLine(-1.0f, 0.0f, 1.0f, 0.0f, 4);

    drawDashedLine(-1.0f, -1.0f, 1.0f, 1.0f, 4);
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

    glViewport(0, 0, 1000, 1000);

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