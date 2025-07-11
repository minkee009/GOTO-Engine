#pragma once
#include <type_traits>
#include <cmath>
#include <iostream>  
#include <stdexcept>   

//헤더 온리 라이브러리

namespace GOTOEngine
{
    //template<typename T, typename = typename std::enable_if<
    //    std::is_same<T, int>::value ||
    //    std::is_same<T, float>::value ||
    //    std::is_same<T, double>::value ||
    //    std::is_same<T, long long>::value
    //>::type>

    class Vector2
    {
    public:
        float x;
        float y;

        Vector2() : x(0), y(0)
        {

        }

        Vector2(float x, float y) : x(x), y(y)
        {

        }

        Vector2 operator+(const Vector2& rhs) const
        {
            return Vector2(x + rhs.x, y + rhs.y);
        }

        Vector2 operator-(const Vector2& rhs) const
        {
            return Vector2(x - rhs.x, y - rhs.y);
        }

        Vector2 operator*(const float& rhs) const
        {
            return Vector2(x * rhs, y * rhs);
        }

        Vector2 operator/(const float& rhs) const
        {
            return Vector2(x / rhs, y / rhs);
        }

        void operator*=(const float& rhs)
        {
            x *= rhs;
            y *= rhs;
        }

        void operator/=(const float& rhs)
        {
            x /= rhs;
            y /= rhs;
        }

        void operator+=(const Vector2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
        }

        void operator-=(const Vector2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
        }

        bool operator==(const Vector2& rhs) const
        {
            constexpr float eps = std::numeric_limits<float>::epsilon() * 100;
            return std::fabs(x - rhs.x) < eps && std::fabs(y - rhs.y) < eps;
            //return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const Vector2& rhs) const
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
            default:
                throw std::out_of_range("원소의 개수를 넘어 접근했습니다.");
                break;
            }
        }

        friend std::ostream& operator << (std::ostream& out, const Vector2& v)
        {
            out << "{ " << v.x << ", " << v.y << " }";

            return out;
        }

        float SqrMagnitude() const
        {
            return (x * x) + (y * y);
        }

        float Magnitude() const
        {
            return static_cast<float>(std::sqrt(static_cast<double>(this->SqrMagnitude())));
        }

        bool IsZero() const
        {
            return (std::abs(x) < static_cast<float>(1e-5) && std::abs(y) < static_cast<float>(1e-5));
            //return x == 0 && y == 0;
        }

        void Normalize()
        {
            if (!this->IsZero())
            {
                *this /= this->Magnitude();
            }
            else
            {
                *this = Vector2(0.0f, 0.0f);
            }
        }

        Vector2 Normalized() const
        {
            // 너무 작으면 0 벡터 리턴
            if (!this->IsZero())
            {
                return *this / this->Magnitude();
            }
            else
            {
                return Vector2(0.0f, 0.0f);
            }
        }

        static float Distance(const Vector2& from, const Vector2& to) 
        {
            Vector2 value = to - from;
            return  value.Magnitude();
        }

        static float SqrDistance(const Vector2& from, const Vector2& to)
        {
            Vector2 value = to - from;
            return value.SqrMagnitude();
        }

        static float Dot(const Vector2& lhs, const Vector2& rhs) 
        {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y);
        }

        static Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
        {
            return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
        }

        static Vector2 ClampMagnitude(const Vector2& v, float max)
        {
            if (v.Magnitude() > max)
                return v.Normalized() * max;

            return v;
        }

        static Vector2 Zero()
        {
            return Vector2{ 0, 0 };
        }
        static Vector2 One()
        {
            return Vector2{ 1, 1 };
        }
        static Vector2 Right()
        {
            return Vector2{ 1, 0 };
        }        
        static Vector2 Left()
        {
            return Vector2{ -1, 0 };
        }        
        static Vector2 Up()
        {
            return Vector2{ 0, 1 };
        }        
        static Vector2 Down()
        {
            return Vector2{ 0, -1 };
        }
    };
}