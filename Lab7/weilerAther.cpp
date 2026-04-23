#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    float x, y;
};

float xmin = 30, ymin = 30, xmax = 90, ymax = 90;

vector<Point> poly = {
    {20,20}, {80,20}, {100,60}, {60,100}, {20,80}
};

vector<Point> clipped;

bool inside(Point p) {
    return (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax);
}

Point intersect(Point p1, Point p2, int edge) {
    Point p;
    float m;

    if (p2.x != p1.x)
        m = (p2.y - p1.y) / (p2.x - p1.x);

    switch (edge) {
    case 0:
        p.x = xmin;
        p.y = p1.y + m * (xmin - p1.x);
        break;
    case 1:
        p.x = xmax;
        p.y = p1.y + m * (xmax - p1.x);
        break;
    case 2:
        p.y = ymin;
        p.x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
        break;
    case 3:
        p.y = ymax;
        p.x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
        break;
    }

    cout << "Intersection: (" << p.x << "," << p.y << ")\n";
    return p;
}

vector<Point> clipEdge(vector<Point> input, int edge) {
    vector<Point> output;

    for (int i = 0; i < input.size(); i++) {
        Point curr = input[i];
        Point prev = input[(i + input.size() - 1) % input.size()];

        bool curr_in, prev_in;

        switch (edge) {
        case 0: curr_in = curr.x >= xmin; prev_in = prev.x >= xmin; break;
        case 1: curr_in = curr.x <= xmax; prev_in = prev.x <= xmax; break;
        case 2: curr_in = curr.y >= ymin; prev_in = prev.y >= ymin; break;
        case 3: curr_in = curr.y <= ymax; prev_in = prev.y <= ymax; break;
        }

        if (curr_in && prev_in) {
            output.push_back(curr);
        }
        else if (prev_in && !curr_in) {
            Point inter = intersect(prev, curr, edge);
            cout << "EXIT point\n";
            output.push_back(inter);
        }
        else if (!prev_in && curr_in) {
            Point inter = intersect(prev, curr, edge);
            cout << "ENTRY point\n";
            output.push_back(inter);
            output.push_back(curr);
        }
    }

    return output;
}

void performClipping() {
    vector<Point> temp = poly;

    cout << "\n--- Clipping Process ---\n";

    temp = clipEdge(temp, 0);
    temp = clipEdge(temp, 1);
    temp = clipEdge(temp, 2);
    temp = clipEdge(temp, 3);

    clipped = temp;

    cout << "\nFinal Clipped Polygon:\n";
    for (auto p : clipped)
        cout << "(" << p.x << "," << p.y << ")\n";
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (auto p : poly)
        glVertex2f(p.x, p.y);
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
    for (auto p : clipped)
        glVertex2f(p.x, p.y);
    glEnd();
}

int main() {
    performClipping();

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(500, 500, "Clipping", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glClearColor(1, 1, 1, 1);

    while (!glfwWindowShouldClose(window)) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 120, 0, 120, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}