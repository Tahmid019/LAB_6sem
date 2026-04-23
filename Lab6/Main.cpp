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

    float wxmin = 0, wymin = 0;
    float wxmax = 100, wymax = 100;

    float vxmin = 200, vymin = 200;
    float vxmax = 400, vymax = 400;

    vector<pair<float, float>> points = {
        {20, 30},
        {60, 70}
    };

    float sx = (vxmax - vxmin) / (wxmax - wxmin);
    float sy = (vymax - vymin) / (wymax - wymin);

    cout << "\n--- Window to Viewport Mapping ---\n";
    cout << "Scale factors: sx = " << sx << ", sy = " << sy << endl;

    auto normalize = [](float x, float y) {
        float nx = (x / 400.0f) * 2.0f - 1.0f;
        float ny = (y / 400.0f) * 2.0f - 1.0f;
        return pair<float, float>(nx, ny);
        };

    vector<float> windowRect = {
        normalize(wxmin, wymin).first, normalize(wxmin, wymin).second,
        normalize(wxmax, wymin).first, normalize(wxmax, wymin).second,
        normalize(wxmax, wymax).first, normalize(wxmax, wymax).second,
        normalize(wxmin, wymax).first, normalize(wxmin, wymax).second
    };
    drawShape(windowRect, 4, 0); // red

    vector<float> viewportRect = {
        normalize(vxmin, vymin).first, normalize(vxmin, vymin).second,
        normalize(vxmax, vymin).first, normalize(vxmax, vymin).second,
        normalize(vxmax, vymax).first, normalize(vxmax, vymax).second,
        normalize(vxmin, vymax).first, normalize(vxmin, vymax).second
    };
    drawShape(viewportRect, 4, 1); // green

    for (auto& p : points)
    {
        float xw = p.first;
        float yw = p.second;

        float xv = vxmin + (xw - wxmin) * sx;
        float yv = vymin + (yw - wymin) * sy;

        cout << "Window Point: (" << xw << ", " << yw << ")";
        cout << " -> Viewport Point: (" << xv << ", " << yv << ")\n";

        //auto wn = pair<float, float>(xw, yw);
        auto wn = normalize(xw, yw);
        drawPoint(wn.first, wn.second, 2);


        //auto vn = pair<float, float>(xv, yv);
        auto vn = normalize(xv, yv);
        drawPoint(vn.first, vn.second, 0);
    }
}

void q2()
{
    drawAxes();

    float xmin = 10, ymin = 10;
    float xmax = 60, ymax = 60;

    auto normalize = [](float x, float y) {
        float nx = (x / 100.0f) * 2.0f - 1.0f;
        float ny = (y / 100.0f) * 2.0f - 1.0f;
        return pair<float, float>(nx, ny);
        };

    vector<float> clipRect = {
        normalize(xmin, ymin).first, normalize(xmin, ymin).second,
        normalize(xmax, ymin).first, normalize(xmax, ymin).second,
        normalize(xmax, ymax).first, normalize(xmax, ymax).second,
        normalize(xmin, ymax).first, normalize(xmin, ymax).second
    };
    drawShape(clipRect, 4, 1); // green

    vector<pair<float, float>> pts = {
        {5,5}, {20,30}, {70,40}, {50,70}
    };

    cout << "\n--- Point Clipping ---\n";

    for (auto& p : pts)
    {
        float x = p.first, y = p.second;

        bool visible = (x >= xmin && x <= xmax && y >= ymin && y <= ymax);

        cout << "Point (" << x << "," << y << ") -> "
            << (visible ? "Visible" : "Invisible") << endl;

        auto np = normalize(x, y);

        if (visible)
            drawPoint(np.first, np.second, 1); // green
        else
            drawPoint(np.first, np.second, 0); // red
    }

    vector<float> poly = {
        normalize(0, 20).first, normalize(0, 20).second,
        normalize(80, 20).first, normalize(80, 20).second,
        normalize(80, 80).first, normalize(80, 80).second,
        normalize(0, 80).first, normalize(0, 80).second
    };

    drawShape(poly, poly.size() / 2, 0);

    auto clipEdge = [&](vector<pair<float, float>> input, int edge) {
        vector<pair<float, float>> output;

        for (int i = 0; i < input.size(); i++)
        {
            auto curr = input[i];
            auto prev = input[(i + input.size() - 1) % input.size()];

            auto inside = [&](pair<float, float> p) {
                if (edge == 0) return p.first >= xmin; // left
                if (edge == 1) return p.first <= xmax; // right
                if (edge == 2) return p.second >= ymin; // bottom
                return p.second <= ymax; // top
                };

            auto intersect = [&](pair<float, float> a, pair<float, float> b) {
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

            bool currIn = inside(curr);
            bool prevIn = inside(prev);

            if (currIn)
            {
                if (!prevIn)
                    output.push_back(intersect(prev, curr));
                output.push_back(curr);
            }
            else if (prevIn)
            {
                output.push_back(intersect(prev, curr));
            }
        }

        return output;
        };

    vector<pair<float, float>> polyPts = {
        {0,20}, {80,20}, {80,80}, {0,80}
    };

    for (int i = 0; i < 4; i++)
        polyPts = clipEdge(polyPts, i);

    vector<float> clipped;
    for (auto& p : polyPts)
    {
        auto np = normalize(p.first, p.second);
        clipped.push_back(np.first);
        clipped.push_back(np.second);
    }

        if (!clipped.empty())
        drawShape(clipped, clipped.size() / 2, 1);
}

void q3()
{
    drawAxes();

    float xmin = 10, ymin = 10;
    float xmax = 80, ymax = 80;

    auto normalize = [](float x, float y) {
        float nx = (x / 100.0f) * 2.0f - 1.0f;
        float ny = (y / 100.0f) * 2.0f - 1.0f;
        return pair<float, float>(nx, ny);
        };

    vector<float> clipRect = {
        normalize(xmin, ymin).first, normalize(xmin, ymin).second,
        normalize(xmax, ymin).first, normalize(xmax, ymin).second,
        normalize(xmax, ymax).first, normalize(xmax, ymax).second,
        normalize(xmin, ymax).first, normalize(xmin, ymax).second
    };
    drawShape(clipRect, 4, 1); // green

    vector<pair<float, float>> pts = {
        {20,30}, {90,40}, {50,50}, {5,60}
    };

    cout << "\n--- Point Clipping Result ---\n";

    for (auto& p : pts)
    {
        float x = p.first;
        float y = p.second;

        bool inside = (x >= xmin && x <= xmax &&
            y >= ymin && y <= ymax);

        cout << "Point (" << x << "," << y << ") -> "
            << (inside ? "Inside" : "Outside") << endl;

        auto np = normalize(x, y);

        if (inside)
            drawPoint(np.first, np.second, 1); // green 
        else
            drawPoint(np.first, np.second, 0); // red 
    }
}

void q4()
{
    drawAxes();

    float xmin = 10, ymin = 10;
    float xmax = 100, ymax = 100;

    auto normalize = [](float x, float y) {
        float nx = (x / 150.0f) * 2.0f - 1.0f;
        float ny = (y / 150.0f) * 2.0f - 1.0f;
        return pair<float, float>(nx, ny);
        };

    vector<float> clipRect = {
        normalize(xmin, ymin).first, normalize(xmin, ymin).second,
        normalize(xmax, ymin).first, normalize(xmax, ymin).second,
        normalize(xmax, ymax).first, normalize(xmax, ymax).second,
        normalize(xmin, ymax).first, normalize(xmin, ymax).second
    };
    drawShape(clipRect, 4, 1); // green

    auto computeCode = [&](float x, float y) {
        int code = 0;
        if (x < xmin) code |= 1;       // left
        if (x > xmax) code |= 2;       // right
        if (y < ymin) code |= 4;       // bottom
        if (y > ymax) code |= 8;       // top
        return code;
        };

    auto clipLine = [&](float& x1, float& y1, float& x2, float& y2) {
        int c1 = computeCode(x1, y1);
        int c2 = computeCode(x2, y2);

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

        return accept;
        };

    vector<vector<float>> lines = {
        {5,5, 120,120},
        {20,30, 80,90},
        {50,150, 150,60}
    };

    cout << "\n--- Line Clipping ---\n";

    for (int i = 0; i < lines.size(); i++)
    {
        float x1 = lines[i][0], y1 = lines[i][1];
        float x2 = lines[i][2], y2 = lines[i][3];

        auto p1 = normalize(x1, y1);
        auto p2 = normalize(x2, y2);

        vector<float> orig = { p1.first, p1.second, p2.first, p2.second };
        setupBuffers(orig);
        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1, 0, 0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);

        float cx1 = x1, cy1 = y1, cx2 = x2, cy2 = y2;

        if (clipLine(cx1, cy1, cx2, cy2))
        {
            cout << "Line " << i + 1 << " Visible: ("
                << cx1 << "," << cy1 << ") to ("
                << cx2 << "," << cy2 << ")\n";

            auto cp1 = normalize(cx1, cy1);
            auto cp2 = normalize(cx2, cy2);

            vector<float> clipped = {
                cp1.first, cp1.second,
                cp2.first, cp2.second
            };

            setupBuffers(clipped);
            glUseProgram(shaderProgram);
            glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 0, 1, 0);
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
        }
        else
        {
            cout << "Line " << i + 1 << " Completely Outside\n";
        }
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