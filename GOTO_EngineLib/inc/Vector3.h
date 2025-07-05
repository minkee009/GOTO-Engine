#pragma once
#include <type_traits>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <limits> // Required for std::numeric_limits

// Header-only library

namespace GOTOEngine
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3() : x(0), y(0), z(0)
        {

        }

        Vector3(float x, float y, float z) : x(x), y(y), z(z)
        {

        }

        Vector3 operator+(const Vector3& rhs) const
        {
            return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vector3 operator-(const Vector3& rhs) const
        {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Vector3 operator*(const float& rhs) const
        {
            return Vector3(x * rhs, y * rhs, z * rhs);
        }

        Vector3 operator/(const float& rhs) const
        {
            return Vector3(x / rhs, y / rhs, z / rhs);
        }

        void operator*=(const float& rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
        }

        void operator/=(const float& rhs)
        {
            // Consider adding a check for division by zero.
            x /= rhs;
            y /= rhs;
            z /= rhs;
        }

        void operator+=(const Vector3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
        }

        void operator-=(const Vector3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
        }

        bool operator==(const Vector3& rhs) const
        {
            float eps = std::numeric_limits<float>::epsilon() * 100;
            return std::fabs(x - rhs.x) < eps && std::fabs(y - rhs.y) < eps && std::fabs(z - rhs.z) < eps;
        }

        bool operator!=(const Vector3& rhs) const
        {
            return !(*this == rhs);
        }

        float operator[](size_t idx) const
        {
            switch (idx)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                throw std::out_of_range("원소의 개수를 넘어 접근했습니다.");
            }
        }

        friend std::ostream& operator << (std::ostream& out, const Vector3& v)
        {
            out << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
            return out;
        }

        float SqrMagnitude() const
        {
            return (x * x) + (y * y) + (z * z);
        }

        float Magnitude() const
        {
            return static_cast<float>(std::sqrt(static_cast<double>(this->SqrMagnitude())));
        }

        bool IsZero() const
        {
            return (std::abs(x) < static_cast<float>(1e-5) && std::abs(y) < static_cast<float>(1e-5) && std::abs(z) < static_cast<float>(1e-5));
        }

        void Normalize()
        {
            if (!this->IsZero())
            {
                *this /= this->Magnitude();
            }
            else
            {
                *this = Vector3(0.0f, 0.0f, 0.0f);
            }
        }

        Vector3 Normalized() const
        {
            if (!this->IsZero())
            {
                return *this / this->Magnitude();
            }
            else
            {
                return Vector3(0.0f, 0.0f, 0.0f);
            }
        }

        static float Distance(const Vector3& from, const Vector3& to)
        {
            Vector3 value = to - from;
            return value.Magnitude();
        }

        static float SqrDistance(const Vector3& from, const Vector3& to)
        {
            Vector3 value = to - from;
            return value.SqrMagnitude();
        }

        static float Dot(const Vector3& lhs, const Vector3& rhs)
        {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
        }

        static Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
        {
            return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
        }

        static Vector3 ClampMagnitude(const Vector3& v, float max)
        {
            if (v.Magnitude() > max)
                return v.Normalized() * max;

            return v;
        }
    };
}