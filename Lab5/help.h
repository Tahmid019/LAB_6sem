#pragma once
#include <vector>

using namespace std;

typedef vector<vector<float>> Mat3;
typedef vector<float> Vec3;

Mat3 identity();
Mat3 multiply(const Mat3& A, const Mat3& B);
Vec3 multiply(const Mat3& M, const Vec3& v);

Mat3 translation(float tx, float ty);
Mat3 scaling(float sx, float sy);
Mat3 rotation(float angleDeg);

Mat3 reflectionX();
Mat3 reflectionY();
Mat3 reflectionXY();

Mat3 shear(float shx, float shy);

vector<float> applyTransform(
    const vector<float>& vertices,
    const Mat3& transform
);

vector<float> applyTransform(const vector<float>&, const Mat3&);
