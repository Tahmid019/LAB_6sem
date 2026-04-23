/*Experiment 3: Weiler–Atherton Polygon Clipping

Q. Given a concave polygon with vertices (20,20), (80,20), (100,60), (60,100), (20,80) and a rectangular clipping window xmin=30, ymin=30, xmax=90, ymax=90:

• Identify all intersection points between polygon edges and clipping window

• Determine entry and exit points

• Apply the Weiler–Atherton polygon clipping algorithm

• Generate the clipped polygon vertices

• Display the original and clipped polygon using different colors*/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    float x, y;
};

// Clipping window
float xmin=30, ymin=30, xmax=90, ymax=90;

// Polygon
vector<Point> poly = {
    {20,20}, {80,20}, {100,60}, {60,100}, {20,80}
};

vector<Point> clipped;

// Check inside
bool inside(Point p) {
    return (p.x>=xmin && p.x<=xmax && p.y>=ymin && p.y<=ymax);
}

// Intersection with boundaries
Point intersect(Point p1, Point p2, int edge) {
    Point p;
    float m;

    if (p2.x != p1.x)
        m = (p2.y - p1.y) / (p2.x - p1.x);

    switch(edge) {
        case 0: // LEFT
            p.x = xmin;
            p.y = p1.y + m*(xmin - p1.x);
            break;

        case 1: // RIGHT
            p.x = xmax;
            p.y = p1.y + m*(xmax - p1.x);
            break;

        case 2: // BOTTOM
            p.y = ymin;
            p.x = p1.x + (p2.x - p1.x)*(ymin - p1.y)/(p2.y - p1.y);
            break;

        case 3: // TOP
            p.y = ymax;
            p.x = p1.x + (p2.x - p1.x)*(ymax - p1.y)/(p2.y - p1.y);
            break;
    }

    cout << "Intersection: (" << p.x << "," << p.y << ")\n";
    return p;
}

// Clip against one boundary (Sutherland-Hodgman style)
vector<Point> clipEdge(vector<Point> input, int edge) {

    vector<Point> output;

    for (int i = 0; i < input.size(); i++) {
        Point curr = input[i];
        Point prev = input[(i + input.size() - 1) % input.size()];

        bool curr_in, prev_in;

        switch(edge) {
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

    temp = clipEdge(temp, 0); // LEFT
    temp = clipEdge(temp, 1); // RIGHT
    temp = clipEdge(temp, 2); // BOTTOM
    temp = clipEdge(temp, 3); // TOP

    clipped = temp;

    cout << "\nFinal Clipped Polygon:\n";
    for (auto p : clipped)
        cout << "(" << p.x << "," << p.y << ")\n";
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔵 Window
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin,ymin);
        glVertex2f(xmax,ymin);
        glVertex2f(xmax,ymax);
        glVertex2f(xmin,ymax);
    glEnd();

    // 🔴 Original polygon
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    for(auto p: poly)
        glVertex2f(p.x,p.y);
    glEnd();

    // 🟢 Clipped polygon
    glColor3f(0,1,0);
    glBegin(GL_LINE_LOOP);
    for(auto p: clipped)
        glVertex2f(p.x,p.y);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1,1,1,1);
    gluOrtho2D(0,120,0,120);
}

int main(int argc, char** argv) {

    performClipping(); // 🔥 all calculations printed

    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("Polygon Clipping (Weiler-Atherton style)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
}