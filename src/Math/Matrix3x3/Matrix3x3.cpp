#include "Math/Matrix3x3/Matrix3x3.hpp"

namespace Math {

Matrix3x3::Matrix3x3() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = (i == j) ? 1.0 : 0.0;
}

Matrix3x3::Matrix3x3(const Vector3D& row1, const Vector3D& row2, const Vector3D& row3) {
    m[0][0] = row1.X; m[0][1] = row1.Y; m[0][2] = row1.Z;
    m[1][0] = row2.X; m[1][1] = row2.Y; m[1][2] = row2.Z;
    m[2][0] = row3.X; m[2][1] = row3.Y; m[2][2] = row3.Z;
}

Vector3D Matrix3x3::operator*(const Vector3D& v) const {
    return Vector3D({
        m[0][0] * v.X + m[0][1] * v.Y + m[0][2] * v.Z,
        m[1][0] * v.X + m[1][1] * v.Y + m[1][2] * v.Z,
        m[2][0] * v.X + m[2][1] * v.Y + m[2][2] * v.Z
    });
}

Matrix3x3 Matrix3x3::transpose() const {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[j][i] = m[i][j];
    return result;
}

}  // namespace Math
