#include "Matrix4x4.h"
#include "Quaternion.h"

#

GOTOEngine::Quaternion GOTOEngine::Quaternion::AngleAxis(float angleDeg, const Vector3& axis)
{
    float angleRad = angleDeg * Mathf::Deg2Rad;
    float halfAngle = angleRad * 0.5f;
    float s = std::sin(halfAngle);
    Vector3 normAxis = axis.Normalized();
    return Quaternion(
        normAxis.x * s,
        normAxis.y * s,
        normAxis.z * s,
        std::cos(halfAngle)
    );
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Euler(float xDeg, float yDeg, float zDeg)
{
    float xRad = xDeg * Mathf::Deg2Rad;
    float yRad = yDeg * Mathf::Deg2Rad;
    float zRad = zDeg * Mathf::Deg2Rad;

    Quaternion qx = AngleAxis(xDeg, Vector3(1, 0, 0));
    Quaternion qy = AngleAxis(yDeg, Vector3(0, 1, 0));
    Quaternion qz = AngleAxis(zDeg, Vector3(0, 0, 1));

    return qy * qx * qz; // Unity는 Y-X-Z 순서 (주의)
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Euler(const Vector3 angles)
{
    return Euler(angles.x,angles.y,angles.z);
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Identity()
{
    return Quaternion(0, 0, 0, 1);
}

float GOTOEngine::Quaternion::Magnitude() const
{
#ifdef _SIMD_OPTIMIZED
    __m128 q = _mm_set_ps(w, z, y, x);           // 쿼터니언 4원소 로드
    __m128 mul = _mm_mul_ps(q, q);               // 제곱 (x², y², z², w²)
    __m128 shuf1 = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1)); // (y², z², w², x²)
    __m128 sum1 = _mm_add_ps(mul, shuf1);        // (x²+y², y²+z², z²+w², w²+x²)
    __m128 shuf2 = _mm_movehl_ps(sum1, sum1);    // (z²+w², w²+x²)
    __m128 sum2 = _mm_add_ss(sum1, shuf2);       // 합산 최종 (x²+y²+z²+w²)
    float result = _mm_cvtss_f32(sum2);
    return std::sqrt(result);
#else
    return std::sqrt(x * x + y * y + z * z + w * w);
#endif
}

void GOTOEngine::Quaternion::Normalize()
{
#ifdef _SIMD_OPTIMIZED
    __m128 q = _mm_set_ps(w, z, y, x);
    __m128 mul = _mm_mul_ps(q, q);
    __m128 shuf1 = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 sum1 = _mm_add_ps(mul, shuf1);
    __m128 shuf2 = _mm_movehl_ps(sum1, sum1);
    __m128 sum2 = _mm_add_ss(sum1, shuf2);

    float magSq = _mm_cvtss_f32(sum2);

    if (magSq > 1e-6f)
    {
        float invMag = 1.0f / std::sqrt(magSq);
        __m128 invMagVec = _mm_set1_ps(invMag);
        __m128 normalized = _mm_mul_ps(q, invMagVec);
        _mm_storeu_ps(&x, normalized);
    }
    else
    {
        x = 0; y = 0; z = 0; w = 1;
    }
#else
    float mag = Magnitude();
    if (mag > 1e-6f)
    {
        x /= mag; y /= mag; z /= mag; w /= mag;
    }
    else
    {
        *this = Identity();
    }
#endif
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Normalized() const
{
    Quaternion q = *this;
    q.Normalize();
    return q;
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::operator*(const Quaternion& rhs) const
{
    return Quaternion(
        w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
        w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
        w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
        w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
    );
}

GOTOEngine::Vector3 GOTOEngine::Quaternion::operator*(const Vector3& v) const
{
    Quaternion qv(v.x, v.y, v.z, 0.0f);
    Quaternion result = (*this) * qv * Inverse();
    return Vector3(result.x, result.y, result.z);
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Inverse() const
{
    // Norm squared = x^2 + y^2 + z^2 + w^2
    float normSq = x * x + y * y + z * z + w * w;

    // 0 quaternion 방지
    if (normSq == 0.0f)
    {
        return Quaternion(0, 0, 0, 1);
    }

    // Conjugate를 norm squared로 나눔
    return Quaternion(-x / normSq, -y / normSq, -z / normSq, w / normSq);
}


GOTOEngine::Vector3 GOTOEngine::Quaternion::ToEulerAngles() const
{
    Vector3 angles;

    // X (pitch)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // Y (yaw)
    float sinp = 2.0f * (w * y - z * x);
    if (std::abs(sinp) >= 1.0f)
        angles.y = std::copysign(3.1415926535f / 2.0f, sinp); // 짐벌락
    else
        angles.y = std::asin(sinp);

    // Z (roll)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    angles *= Mathf::Rad2Deg;

    angles.x = Mathf::NormalizeAngle(angles.x);
    angles.y = Mathf::NormalizeAngle(angles.y);
    angles.z = Mathf::NormalizeAngle(angles.z);

    return angles;
}

GOTOEngine::Matrix4x4 GOTOEngine::Quaternion::ToMatrix4x4() const
{
    Matrix4x4 m;
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    m.SetIdentity();

    // Column-major 설정
    m.m[0][0] = 1.0f - 2.0f * (yy + zz);
    m.m[0][1] = 2.0f * (xy + wz);
    m.m[0][2] = 2.0f * (xz - wy);
    m.m[0][3] = 0.0f;

    m.m[1][0] = 2.0f * (xy - wz);
    m.m[1][1] = 1.0f - 2.0f * (xx + zz);
    m.m[1][2] = 2.0f * (yz + wx);
    m.m[1][3] = 0.0f;

    m.m[2][0] = 2.0f * (xz + wy);
    m.m[2][1] = 2.0f * (yz - wx);
    m.m[2][2] = 1.0f - 2.0f * (xx + yy);
    m.m[2][3] = 0.0f;

    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;

    return m;
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
    // Compute the cosine of the angle between the two vectors.
    float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    const float threshold = 0.9995f;
    if (dot > threshold)
    {
        // If the inputs are too close for comfort, linearly interpolate and normalize the result.
        Quaternion result(
            a.x + t * (b.x - a.x),
            a.y + t * (b.y - a.y),
            a.z + t * (b.z - a.z),
            a.w + t * (b.w - a.w)
        );
        result.Normalize();
        return result;
    }

    // If the dot product is negative, the quaternions have opposite handed-ness
    // and slerp won't take the shorter path. Fix by reversing one quaternion.
    if (dot < 0.0f)
    {
        dot = -dot;
        return Slerp(a, Quaternion(-b.x, -b.y, -b.z, -b.w), t);
    }

    float theta_0 = std::acos(dot);        // theta_0 = angle between input vectors
    float theta = theta_0 * t;             // theta = angle between a and result
    float sin_theta = std::sin(theta);     // compute this value only once
    float sin_theta_0 = std::sin(theta_0); // compute this value only once

    float s0 = std::cos(theta) - dot * sin_theta / sin_theta_0; // == sin(theta_0 - theta) / sin(theta_0)
    float s1 = sin_theta / sin_theta_0;

    return Quaternion(
        (a.x * s0) + (b.x * s1),
        (a.y * s0) + (b.y * s1),
        (a.z * s0) + (b.z * s1),
        (a.w * s0) + (b.w * s1)
    );
}

GOTOEngine::Quaternion GOTOEngine::Quaternion::FromRotationMatrix(const Matrix4x4& m)
{
    Quaternion q;
    float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];

    if (trace > 0.0f)
    {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        q.w = 0.25f / s;
        q.x = (m.m[2][1] - m.m[1][2]) * s;
        q.y = (m.m[0][2] - m.m[2][0]) * s;
        q.z = (m.m[1][0] - m.m[0][1]) * s;
    }
    else
    {
        if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2])
        {
            float s = 2.0f * std::sqrt(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
            q.w = (m.m[2][1] - m.m[1][2]) / s;
            q.x = 0.25f * s;
            q.y = (m.m[0][1] + m.m[1][0]) / s;
            q.z = (m.m[0][2] + m.m[2][0]) / s;
        }
        else if (m.m[1][1] > m.m[2][2])
        {
            float s = 2.0f * std::sqrt(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
            q.w = (m.m[0][2] - m.m[2][0]) / s;
            q.x = (m.m[0][1] + m.m[1][0]) / s;
            q.y = 0.25f * s;
            q.z = (m.m[1][2] + m.m[2][1]) / s;
        }
        else
        {
            float s = 2.0f * std::sqrt(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
            q.w = (m.m[1][0] - m.m[0][1]) / s;
            q.x = (m.m[0][2] + m.m[2][0]) / s;
            q.y = (m.m[1][2] + m.m[2][1]) / s;
            q.z = 0.25f * s;
        }
    }

    return q.Normalized();
}