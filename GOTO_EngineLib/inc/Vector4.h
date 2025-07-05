#pragma once
#include <type_traits>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <limits> // Required for std::numeric_limits

// Header-only library

namespace GOTOEngine
{
    class Vector4
    {
    public:
        float x;
        float y;
        float z;
        float w;

        Vector4() : x(0), y(0), z(0), w(0)
        {

        }

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
        {

        }

        Vector4 operator+(const Vector4& rhs) const
        {
            return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        Vector4 operator-(const Vector4& rhs) const
        {
            return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        Vector4 operator*(const float& rhs) const
        {
            return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        Vector4 operator/(const float& rhs) const
        {
            return Vector4(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        void operator*=(const float& rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
        }

        void operator/=(const float& rhs)
        {
            // Consider adding a check for division by zero.
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
        }

        void operator+=(const Vector4& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
        }

        void operator-=(const Vector4& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
        }

        bool operator==(const Vector4& rhs) const
        {
            float eps = std::numeric_limits<float>::epsilon() * 100;
            return std::fabs(x - rhs.x) < eps && std::fabs(y - rhs.y) < eps && std::fabs(z - rhs.z) < eps && std::fabs(w - rhs.w) < eps;
        }

        bool operator!=(const Vector4& rhs) const
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
            case 3:
                return w;
            default:
                throw std::out_of_range("원소의 개수를 넘어 접근했습니다.");
            }
        }

        friend std::ostream& operator << (std::ostream& out, const Vector4& v)
        {
            out << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
            return out;
        }

        float SqrMagnitude() const
        {
            return (x * x) + (y * y) + (z * z) + (w * w);
        }

        float Magnitude() const
        {
            return static_cast<float>(std::sqrt(static_cast<double>(this->SqrMagnitude())));
        }

        bool IsZero() const
        {
            return (std::abs(x) < static_cast<float>(1e-5) && std::abs(y) < static_cast<float>(1e-5) && std::abs(z) < static_cast<float>(1e-5) && std::abs(w) < static_cast<float>(1e-5));
        }

        void Normalize()
        {
            if (!this->IsZero())
            {
                *this /= this->Magnitude();
            }
            else
            {
                *this = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }

        Vector4 Normalized() const
        {
            if (!this->IsZero())
            {
                return *this / this->Magnitude();
            }
            else
            {
                return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }

        static float Distance(const Vector4& from, const Vector4& to)
        {
            Vector4 value = to - from;
            return value.Magnitude();
        }

        static float SqrDistance(const Vector4& from, const Vector4& to)
        {
            Vector4 value = to - from;
            return value.SqrMagnitude();
        }

        static float Dot(const Vector4& lhs, const Vector4& rhs)
        {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
        }

        static Vector4 Lerp(const Vector4& a, const Vector4& b, float t)
        {
            return Vector4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
        }

        static Vector4 ClampMagnitude(const Vector4& v, float max)
        {
            if (v.Magnitude() > max)
                return v.Normalized() * max;

            return v;
        }
    };
}