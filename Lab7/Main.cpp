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

void draw3DAxes(float d, float m, float b)
{
    auto project = [&](float x, float y, float z) {
        float xp = x + 0.5 * z;
        float yp = y + 0.5 * z;
        return normalize(xp, yp, d, m, b);
        };

    // Origin
    auto O = project(0, 0, 0);

    // X-axis (red)
    auto X = project(200, 0, 0);
    drawLine(O.first, O.second, X.first, X.second, 2);

    // Y-axis (green)
    auto Y = project(0, 200, 0);
    drawLine(O.first, O.second, Y.first, Y.second, 3);

    // Z-axis (blue)
    auto Z = project(0, 0, 200);
    drawLine(O.first, O.second, Z.first, Z.second, 4);
}

void q1()
{
    drawAxes();

    float xmin = 10, ymin = 10;
    float xmax = 100, ymax = 100;

    float x1 = 5, y1 = 20;
    float x2 = 120, y2 = 80;
    
    float d = 150.0f, m = 2.0f, b = 1.0f;

    vector<float> clipRect = {
        normalize(xmin, ymin, d, m, b).first, normalize(xmin, ymin, d, m, b).second,
        normalize(xmax, ymin, d, m, b).first, normalize(xmax, ymin, d, m, b).second,
        normalize(xmax, ymax, d, m, b).first, normalize(xmax, ymax, d, m, b).second,
        normalize(xmin, ymax, d, m, b).first, normalize(xmin, ymax, d, m, b).second
    };                      
    drawShape(clipRect, 4, 1); // green

    auto p1 = normalize(x1, y1, d, m, b);
    auto p2 = normalize(x2, y2, d, m, b);

    vector<float> orig = {
        p1.first, p1.second,
        p2.first, p2.second
    };

    drawLine(p1.first, p1.second, p2.first, p2.second, 0); // red

    auto computeCode = [&](float x, float y) {
        int code = 0;
        if (x < xmin) code |= 1;       // left
        if (x > xmax) code |= 2;       // right
        if (y < ymin) code |= 4;       // bottom
        if (y > ymax) code |= 8;       // top
        return code;
        };


    int c1 = computeCode(x1, y1);
    int c2 = computeCode(x2, y2);

    std::cout << "P1 Code = " << bitset<4>(c1) << endl;
    std::cout << "P2 Code = " << bitset<4>(c2) << endl;

    bool accept = false;

    while (true)
    {
        if ((c1 | c2) == 0)
        {
            accept = true;
            break;
        }
        else if (c1 & c2)
        {
            break;
        }
        else
        {
            int out = c1 ? c1 : c2;
            float x, y;

            if (out & 8) { // top
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (out & 4) { // bottom
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (out & 2) { // right
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else { // left
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (out == c1)
            {
                x1 = x; y1 = y;
                c1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x; y2 = y;
                c2 = computeCode(x2, y2);
            }
        }
    }

    if (accept)
    {
        cout << "Line is Partially Clipped\n";
        cout << "Clipped Line: (" << x1 << ", " << y1 << ") to ("
            << x2 << ", " << y2 << ")\n";

        // Draw clipped line (green)
        auto cp1 = normalize(x1, y1, d, m, b);
        auto cp2 = normalize(x2, y2, d, m, b);

        vector<float> clipped = {
            cp1.first, cp1.second,
            cp2.first, cp2.second
        };

        drawLine(cp1.first, cp1.second, cp2.first, cp2.second, 1); // green
    }
    else
    {
        cout << "Line Rejected (Completely Outside)\n";
    }
}

void q2()
{
    drawAxes();

    float xmin = 10, ymin = 10;
    float xmax = 100, ymax = 100;

    float x1 = 0, y1 = 50;
    float x2 = 120, y2 = 80;

    float d = 150.0f, m = 2.0f, b = 1.0f;

    float dx = x2 - x1;
    float dy = y2 - y1;

    vector<float> clipRect = {
        normalize(xmin, ymin, d, m, b).first, normalize(xmin, ymin, d, m, b).second,
        normalize(xmax, ymin, d, m, b).first, normalize(xmax, ymin, d, m, b).second,
        normalize(xmax, ymax, d, m, b).first, normalize(xmax, ymax, d, m, b).second,
        normalize(xmin, ymax, d, m, b).first, normalize(xmin, ymax, d, m, b).second
    };
    drawShape(clipRect, 4, 1); // green


    float p[4] = { -dx, dx, -dy, dy };
    float q[4] = { x1 - xmin, xmax - x1, y1 - ymin, ymax - y1 };

    float t1 = 0.0f, t2 = 1.0f;

    for (int i = 0; i < 4; i++)
    {
        if (p[i] == 0)
        {
            if (q[i] < 0)
            {
                std::cout << "Line Rejected\n";
                return;
            }
        }
        else
        {
            float t = q[i] / p[i];

            if (p[i] < 0)
                t1 = max(t1, t);
            else
                t2 = min(t2, t);
        }

        std::cout << "p[" << i << "] = " << p[i]
            << ", q[" << i << "] = " << q[i] << endl;
    }

    if (t1 > t2)
    {
        std::cout << "Line Rejected\n";
        return;
    }

    float cx1 = x1 + t1 * dx;
    float cy1 = y1 + t1 * dy;

    float cx2 = x1 + t2 * dx;
    float cy2 = y1 + t2 * dy;

    std::cout << "t_enter = " << t1 << ", t_leave = " << t2 << endl;

    std::cout << "Clipped Line: (" << cx1 << ", " << cy1
        << ") to (" << cx2 << ", " << cy2 << ")\n";

    auto p1 = normalize(x1, y1, d, m, b);
    auto p2 = normalize(x2, y2, d, m, b);
    drawLine(p1.first, p1.second, p2.first, p2.second, 0);

    auto cp1 = normalize(cx1, cy1, d, m, b);
    auto cp2 = normalize(cx2, cy2, d, m, b);
    drawLine(cp1.first, cp1.second, cp2.first, cp2.second, 1);
}


void q3()
{
    drawAxes();

    float xmin = 30, ymin = 30;
    float xmax = 90, ymax = 90;

    float d = 150.0f, m = 2.0f, b = 1.0f;

    vector<pair<float, float>> poly = {
        {20,20}, {80,20}, {100,60}, {60,100}, {20,80}
    };

    // clipping window
    vector<float> clipRect = {
        normalize(xmin, ymin, d, m, b).first, normalize(xmin, ymin, d, m, b).second,
        normalize(xmax, ymin, d, m, b).first, normalize(xmax, ymin, d, m, b).second,
        normalize(xmax, ymax, d, m, b).first, normalize(xmax, ymax, d, m, b).second,
        normalize(xmin, ymax, d, m, b).first, normalize(xmin, ymax, d, m, b).second
    };
    drawShape(clipRect, 4, 1); // green

    cout << "\n--- Weiler-Atherton Polygon Clipping ---\n";

    vector<float> orig; // red
    for (auto& p : poly)
    {
        auto np = normalize(p.first, p.second, d, m, b);
        orig.push_back(np.first);
        orig.push_back(np.second);
    }
    drawShape(orig, orig.size() / 2, 0);

    auto inside = [&](pair<float, float> p) {
        return (p.first >= xmin && p.first <= xmax &&
            p.second >= ymin && p.second <= ymax);
        };

    auto intersect = [&](pair<float, float> a, pair<float, float> b, int edge) {
        float x, y;

        if (edge == 0) { // left
            x = xmin;
            y = a.second + (b.second - a.second) * (xmin - a.first) / (b.first - a.first);
        }
        else if (edge == 1) { // right
            x = xmax;
            y = a.second + (b.second - a.second) * (xmax - a.first) / (b.first - a.first);
        }
        else if (edge == 2) { // bottom
            y = ymin;
            x = a.first + (b.first - a.first) * (ymin - a.second) / (b.second - a.second);
        }
        else { // top
            y = ymax;
            x = a.first + (b.first - a.first) * (ymax - a.second) / (b.second - a.second);
        }

        return make_pair(x, y);
        };

    vector<pair<float, float>> output = poly;

    for (int edge = 0; edge < 4; edge++)
    {
        vector<pair<float, float>> input = output;
        output.clear();

        for (int i = 0; i < input.size(); i++)
        {
            auto curr = input[i];
            auto prev = input[(i + input.size() - 1) % input.size()];

            bool currIn, prevIn;

            if (edge == 0) { currIn = curr.first >= xmin; prevIn = prev.first >= xmin; }
            else if (edge == 1) { currIn = curr.first <= xmax; prevIn = prev.first <= xmax; }
            else if (edge == 2) { currIn = curr.second >= ymin; prevIn = prev.second >= ymin; }
            else { currIn = curr.second <= ymax; prevIn = prev.second <= ymax; }

            if (currIn)
            {
                if (!prevIn)
                {
                    auto ip = intersect(prev, curr, edge);
                    cout << "Intersection: (" << ip.first << "," << ip.second << ") [ENTER]\n";
                    output.push_back(ip);
                }
                output.push_back(curr);
            }
            else if (prevIn)
            {
                auto ip = intersect(prev, curr, edge);
                cout << "Intersection: (" << ip.first << "," << ip.second << ") [EXIT]\n";
                output.push_back(ip);
            }
        }
    }

    cout << "\nClipped Polygon Vertices:\n";
    for (auto& p : output)
        cout << "(" << p.first << "," << p.second << ")\n";

    vector<float> clipped;
    for (auto& p : output)
    {
        auto np = normalize(p.first, p.second, d, m, b);
        clipped.push_back(np.first);
        clipped.push_back(np.second);
    }

    if (!clipped.empty())
        drawShape(clipped, clipped.size() / 2, 1);
}


void q4()
{
    //drawAxes();

    float d = 150.0f, m = 2.0f, b = 1.0f;
    //draw3DAxes(d, m, b);

    float angle = 15.0f * M_PI / 180.0f;

    float c = cos(angle);
    float s = sin(angle);

    vector<vector<float>> Rx = {
        {1, 0, 0},
        {0, c, -s},
        {0, s, c}
    };

    vector<vector<float>> Ry = {
        {c, 0, s},
        {0, 1, 0},
        {-s, 0, c}
    };

    vector<vector<float>> Rz = {
        {c, -s, 0},
        {s, c, 0},
        {0, 0, 1}
    };

    auto multiply = [&](vector<vector<float>> R, vector<float> P) {
        vector<float> res(3, 0);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                res[i] += R[i][j] * P[j];
        return res;
        };

    vector<vector<float>> cube = {
        {-25,-25,-25}, {25,-25,-25}, {25,25,-25}, {-25,25,-25},
        {-25,-25,25},  {25,-25,25},  {25,25,25},  {-25,25,25}
    };

    auto project = [&](float x, float y, float z) {
        float xp = x - z * 0.5;
        float yp = y - z * 0.5;

        xp += 50;   
        yp += 50;

        return normalize(xp, yp, d, m, b);
        };

    vector<pair<int, int>> edges = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    cout << "\n--- 3D Rotation using Matrices ---\n";

    for (auto& e : edges)
    {
        auto p1 = project(cube[e.first][0], cube[e.first][1], cube[e.first][2]);
        auto p2 = project(cube[e.second][0], cube[e.second][1], cube[e.second][2]);
        drawLine(p1.first, p1.second, p2.first, p2.second, 0);
    }

    vector<vector<float>> rotated;

    for (auto& p : cube)
    {
        vector<float> p1 = multiply(Rx, p);
        vector<float> p2 = multiply(Ry, p1);
        vector<float> p3 = multiply(Rz, p2);

        rotated.push_back(p3);

        cout << "Original: (" << p[0] << "," << p[1] << "," << p[2] << ")";
        cout << " -> Rotated: ("
            << round(p3[0] * 100) / 100 << ", "
            << round(p3[1] * 100) / 100 << ", "
            << round(p3[2] * 100) / 100 << ")\n";
    }

    for (auto& e : edges)
    {
        auto p1 = project(rotated[e.first][0], rotated[e.first][1], rotated[e.first][2]);
        auto p2 = project(rotated[e.second][0], rotated[e.second][1], rotated[e.second][2]);
        drawLine(p1.first, p1.second, p2.first, p2.second, 1);
    }
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
        case 4: {
            q4();
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