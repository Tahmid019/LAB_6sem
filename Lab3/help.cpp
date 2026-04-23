#include "help.h"
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

typedef vector<vector<float>> Mat3;
typedef vector<float> Vec3;

Mat3 identity() {
    return {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };
}

Mat3 multiply(const Mat3& A, const Mat3& B) {
    Mat3 R(3, vector<float>(3, 0));
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            for (int k = 0;k < 3;k++)
                R[i][j] += A[i][k] * B[k][j];
    return R;
}

Vec3 multiply(const Mat3& M, const Vec3& v) {
    Vec3 r(3);
    for (int i = 0;i < 3;i++)
        r[i] = M[i][0] * v[0] + M[i][1] * v[1] + M[i][2] * v[2];
    return r;
}


Mat3 translation(float tx, float ty) {
    return {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}
    };
}

Mat3 scaling(float sx, float sy) {
    return {
        {sx, 0,  0},
        {0, sy,  0},
        {0,  0,  1}
    };
}

Mat3 rotation(float angleDeg) {
    float rad = angleDeg * M_PI / 180.0f;
    float c = cos(rad), s = sin(rad);

    return {
        { c, -s, 0},
        { s,  c, 0},
        { 0,  0, 1}
    };
}


vector<float> applyTransform(
    const vector<float>& vertices,
    const Mat3& transform)
{
    vector<float> out;

    for (int i = 0; i < vertices.size(); i += 2) {
        Vec3 p = { vertices[i], vertices[i + 1], 1 };
        Vec3 r = multiply(transform, p);
        out.push_back(r[0]);
        out.push_back(r[1]);
    }
    return out;
}
