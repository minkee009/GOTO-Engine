#pragma once

#include "Vector3.h"
#include "Mathf.h"
#include <cmath>

namespace GOTOEngine
{
    class Matrix4x4;
    class Quaternion
    {
    public:
        float x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        float Magnitude() const;

        void Normalize();

        Quaternion Normalized() const;

        // Quaternion 곱: 회전 결합
        Quaternion operator*(const Quaternion& rhs) const;

        // Quaternion * Vector3 → 회전 적용
        Vector3 operator*(const Vector3& v) const;

        //켤레 복소수
        Quaternion Inverse() const;

        Vector3 ToEulerAngles() const;
        Matrix4x4 ToMatrix4x4() const;

        static Quaternion Identity();

        static Quaternion AngleAxis(float angleDeg, const Vector3& axis);
        static Quaternion Euler(float xDeg, float yDeg, float zDeg);
        static Quaternion Euler(const Vector3 angles);

        static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
        static Quaternion FromRotationMatrix(const Matrix4x4& m);
    };
}
