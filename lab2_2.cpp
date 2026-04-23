// midpoint_ellipse_glfw_exact.c
// Modern OpenGL 3.3 + GLFW + GLAD
// Draws two filled ellipse-caps (red top-right, green bottom-left) and black outlines + axes.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WIN_W 600
#define WIN_H 600

// --- simple pass-through shader (positions are in pixel coords; we divide by 300 in shader) ---
const char* vertex_src =
"#version 330 core\n"
"layout(location = 0) in vec2 aPos;\n"
"uniform float scale; // convert pixels -> NDC\n"
"void main() {\n"
"  gl_Position = vec4(aPos / scale, 0.0, 1.0);\n"
"}\n";

const char* fragment_src =
"#version 330 core\n"
"uniform vec3 uColor;\n"
"out vec4 FragColor;\n"
"void main(){ FragColor = vec4(uColor, 1.0); }\n";

// create and compile a shader
static GLuint compile_shader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[1024]; glGetShaderInfoLog(s, 1024, NULL, buf);
        fprintf(stderr, "Shader compile error: %s\n", buf);
    }
    return s;
}

// build shader program
static GLuint build_program() {
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[1024]; glGetProgramInfoLog(p, 1024, NULL, buf);
        fprintf(stderr, "Program link error: %s\n", buf);
    }
    glDeleteShader(vs); glDeleteShader(fs);
    return p;
}

// Generate the filled red cap in top-right: pixels coordinates with center at (0,0)
// rx: horizontal radius, ry: vertical radius
// We will create a triangle fan that starts at (0,0) and runs along the ellipse from x=0..rx then to (rx,0)
void gen_red_cap(float rx, float ry, float** out_verts, int* out_count) {
    // We'll sample x with step 1.0 for pixel-accuracy and smoothness
    int steps = (int)ceilf(rx) + 1;
    // triangle fan: center + sample points + last baseline point
    int maxv = 2 + steps + 1;
    float* v = (float*)malloc(sizeof(float) * maxv * 2);
    int idx = 0;
    // center boundary at origin (0,0) - the vertex-fan center
    v[idx++] = 0.0f; v[idx++] = 0.0f;
    // sample along the ellipse curve from x=0 -> x=rx (top curve)
    for (int i = 0; i <= (int)rx; ++i) {
        float x = (float)i;
        float inside = 1.0f - (x * x) / (rx * rx);
        if (inside < 0.0f) inside = 0.0f;
        float y = ry * sqrtf(inside);
        v[idx++] = x;
        v[idx++] = y;
    }
    // ensure the baseline right endpoint at (rx,0) (already last sample if i==rx), but add explicitly for fan closure
    v[idx++] = rx; v[idx++] = 0.0f;
    *out_verts = v;
    *out_count = idx / 2;
}

// Generate the outline (perimeter) for the red cap as an ordered polyline
// This will be used to draw the black stroke
void gen_red_outline(float rx, float ry, float** out_verts, int* out_count) {
    int steps = (int)ceilf(rx) + 1;
    float* v = (float*)malloc(sizeof(float) * (steps + 3) * 2);
    int idx = 0;
    // start at (0,0)
    v[idx++] = 0.0f; v[idx++] = 0.0f;
    // along curve from x=0 .. rx
    for (int i = 0; i <= (int)rx; ++i) {
        float x = (float)i;
        float inside = 1.0f - (x * x) / (rx * rx);
        if (inside < 0.0f) inside = 0.0f;
        float y = ry * sqrtf(inside);
        v[idx++] = x; v[idx++] = y;
    }
    // baseline back to (0,0)
    v[idx++] = 0.0f; v[idx++] = 0.0f;
    *out_verts = v;
    *out_count = idx / 2;
}

// Green cap: bottom-left; major axis is vertical -> use rx2 (small horizontal) and ry2 (taller vertical)
// We'll generate triangle fan similarly, but sample x from 0 down to -rx2 (so the curve runs leftwards)
// The fan center is again (0,0)
void gen_green_cap(float rx2, float ry2, float** out_verts, int* out_count) {
    int steps = (int)ceilf(rx2) + 1;
    int maxv = 2 + steps + 1;
    float* v = (float*)malloc(sizeof(float) * maxv * 2);
    int idx = 0;
    v[idx++] = 0.0f; v[idx++] = 0.0f;
    // sample x from 0 down to -rx2
    for (int i = 0; i <= (int)rx2; ++i) {
        float x = 0.0f - (float)i;
        float inside = 1.0f - (x * x) / (rx2 * rx2);
        if (inside < 0.0f) inside = 0.0f;
        float y = -(ry2 * sqrtf(inside)); // negative y (downwards)
        v[idx++] = x; v[idx++] = y;
    }
    // add leftmost baseline point (-rx2, 0) for closure
    v[idx++] = -rx2; v[idx++] = 0.0f;
    *out_verts = v;
    *out_count = idx / 2;
}

void gen_green_outline(float rx2, float ry2, float** out_verts, int* out_count) {
    int steps = (int)ceilf(rx2) + 1;
    float* v = (float*)malloc(sizeof(float) * (steps + 3) * 2);
    int idx = 0;
    // start at (0,0)
    v[idx++] = 0.0f; v[idx++] = 0.0f;
    // curve leftwards
    for (int i = 0; i <= (int)rx2; ++i) {
        float x = 0.0f - (float)i;
        float inside = 1.0f - (x * x) / (rx2 * rx2);
        if (inside < 0.0f) inside = 0.0f;
        float y = -(ry2 * sqrtf(inside));
        v[idx++] = x; v[idx++] = y;
    }
    v[idx++] = 0.0f; v[idx++] = 0.0f; // close back
    *out_verts = v;
    *out_count = idx / 2;
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* w = glfwCreateWindow(WIN_W, WIN_H, "Midpoint Ellipse - exact structure", NULL, NULL);
    if (!w) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(w);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to init GLAD\n");
        return -1;
    }

    GLuint program = build_program();
    glUseProgram(program);
    GLint scaleLoc = glGetUniformLocation(program, "scale");
    GLint colorLoc = glGetUniformLocation(program, "uColor");
    // set scale (we use 300 so that pixel coords around +/-300 map to NDC -1..1)
    glUniform1f(scaleLoc, 300.0f);

    // Choose radii that visually match your reference image
    // RED cap: wider horizontal radius (major = X)
    float rx_red = 200.0f;
    float ry_red = 85.0f;
    // GREEN cap: narrow horizontal radius, tall vertical radius (major = Y)
    float rx_green = 58.0f;
    float ry_green = 220.0f;

    // Generate vertices
    float* redFan = NULL, * redOutline = NULL;
    float* greenFan = NULL, * greenOutline = NULL;
    int redFanCount = 0, redOutCount = 0, greenFanCount = 0, greenOutCount = 0;

    gen_red_cap(rx_red, ry_red, &redFan, &redFanCount);
    gen_red_outline(rx_red, ry_red, &redOutline, &redOutCount);

    gen_green_cap(rx_green, ry_green, &greenFan, &greenFanCount);
    gen_green_outline(rx_green, ry_green, &greenOutline, &greenOutCount);

    // Create VAOs/VBOs
    GLuint VAO[4], VBO[4];
    glGenVertexArrays(4, VAO);
    glGenBuffers(4, VBO);

    // red fan
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * redFanCount, redFan, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // red outline
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * redOutCount, redOutline, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // green fan
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * greenFanCount, greenFan, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // green outline
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * greenOutCount, greenOutline, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // free CPU arrays (GPU has them now)
    free(redFan); free(redOutline); free(greenFan); free(greenOutline);

    // Setup GL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // loop
    while (!glfwWindowShouldClose(w)) {
        glViewport(0, 0, WIN_W, WIN_H);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform1f(scaleLoc, 300.0f);

        // Draw axes (black thick lines)
        //float axes[] = {
        //    -300.0f, 0.0f,  300.0f, 0.0f,   // horizontal
        //    0.0f, -300.0f,  0.0f, 300.0f    // vertical
        //};
        //GLuint axesVAO, axesVBO;
        //glGenVertexArrays(1, &axesVAO);
        //glGenBuffers(1, &axesVBO);
        //glBindVertexArray(axesVAO);
        //glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);

        //glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
        //glLineWidth(6.0f);
        //glBindVertexArray(axesVAO);
        //glDrawArrays(GL_LINES, 0, 4);
        //glDeleteBuffers(1, &axesVBO);
        //glDeleteVertexArrays(1, &axesVAO);

        // Draw red filled fan
        glBindVertexArray(VAO[0]);
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // vivid red
        glDrawArrays(GL_TRIANGLE_FAN, 0, redFanCount);

        // Draw green filled fan
        glBindVertexArray(VAO[2]);
        glUniform3f(colorLoc, 0.0f, 0.7f, 0.0f); // green matching reference
        glDrawArrays(GL_TRIANGLE_FAN, 0, greenFanCount);

        // Draw black outlines (thick)
        //glLineWidth(4.0f);
        //glBindVertexArray(VAO[1]); // red outline
        //glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
        //glDrawArrays(GL_LINE_STRIP, 0, redOutCount);

        //glBindVertexArray(VAO[3]); // green outline
        //glDrawArrays(GL_LINE_STRIP, 0, greenOutCount);

        // small black borders for baseline & vertical divider near origin to match the picture's short axis segments
        // draw small horizontal/vertical ticks near origin (thin)
        /*float ticks[] = {
            -12.0f, 0.0f,  12.0f, 0.0f,
            0.0f, -12.0f,  0.0f, 12.0f
        };
        GLuint ticksVAO, ticksVBO;
        glGenVertexArrays(1, &ticksVAO);
        glGenBuffers(1, &ticksVBO);
        glBindVertexArray(ticksVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ticksVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ticks), ticks, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, 4);
        glDeleteBuffers(1, &ticksVBO);
        glDeleteVertexArrays(1, &ticksVAO);*/

        // swap
        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(4, VAO);
    glDeleteBuffers(4, VBO);
    glDeleteProgram(program);

    glfwDestroyWindow(w);
    glfwTerminate();
    return 0;
}
