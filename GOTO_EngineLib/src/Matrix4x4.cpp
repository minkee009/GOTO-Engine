#include "Matrix4x4.h"
#include "Quaternion.h"

void GOTOEngine::Matrix4x4::ToRowMajorArray(float out[16]) const
{
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            out[r * 4 + c] = m[c][r];
        }
    }
}

void GOTOEngine::Matrix4x4::ToColumnMajorArray(float out[16]) const
{
    std::memcpy(out, &m[0][0], sizeof(float) * 16);
}

GOTOEngine::Quaternion GOTOEngine::Matrix4x4::GetRotation() const
{
    Vector3 scale = GetLossyScale();

    // 스케일 제거 후 회전 행렬 생성
    float r00 = m[0][0] / scale.x;
    float r01 = m[0][1] / scale.y;
    float r02 = m[0][2] / scale.z;
    float r10 = m[1][0] / scale.x;
    float r11 = m[1][1] / scale.y;
    float r12 = m[1][2] / scale.z;
    float r20 = m[2][0] / scale.x;
    float r21 = m[2][1] / scale.y;
    float r22 = m[2][2] / scale.z;

    float trace = r00 + r11 + r22;
    float qw, qx, qy, qz;

    if (trace > 0.0f)
    {
        float s = 0.5f / std::sqrt(trace + 1.0f);
        qw = 0.25f / s;
        qx = (r21 - r12) * s;
        qy = (r02 - r20) * s;
        qz = (r10 - r01) * s;
    }
    else
    {
        if (r00 > r11 && r00 > r22)
        {
            float s = 2.0f * std::sqrt(1.0f + r00 - r11 - r22);
            qw = (r21 - r12) / s;
            qx = 0.25f * s;
            qy = (r01 + r10) / s;
            qz = (r02 + r20) / s;
        }
        else if (r11 > r22)
        {
            float s = 2.0f * std::sqrt(1.0f + r11 - r00 - r22);
            qw = (r02 - r20) / s;
            qx = (r01 + r10) / s;
            qy = 0.25f * s;
            qz = (r12 + r21) / s;
        }
        else
        {
            float s = 2.0f * std::sqrt(1.0f + r22 - r00 - r11);
            qw = (r10 - r01) / s;
            qx = (r02 + r20) / s;
            qy = (r12 + r21) / s;
            qz = 0.25f * s;
        }
    }

    return Quaternion(qx, qy, qz, qw);
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Rotate(Quaternion& quaternion)
{
    return quaternion.ToMatrix4x4();
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    Matrix4x4 m;

    // Quaternion → rotation matrix 요소 직접 계산
    float xx = rotation.x * rotation.x;
    float yy = rotation.y * rotation.y;
    float zz = rotation.z * rotation.z;
    float xy = rotation.x * rotation.y;
    float xz = rotation.x * rotation.z;
    float yz = rotation.y * rotation.z;
    float wx = rotation.w * rotation.x;
    float wy = rotation.w * rotation.y;
    float wz = rotation.w * rotation.z;

    // 열 0 (X axis) * scale.x
    m.m[0][0] = (1.0f - 2.0f * (yy + zz)) * scale.x;
    m.m[1][0] = (2.0f * (xy + wz)) * scale.x;
    m.m[2][0] = (2.0f * (xz - wy)) * scale.x;
    m.m[3][0] = 0.0f;

    // 열 1 (Y axis) * scale.y
    m.m[0][1] = (2.0f * (xy - wz)) * scale.y;
    m.m[1][1] = (1.0f - 2.0f * (xx + zz)) * scale.y;
    m.m[2][1] = (2.0f * (yz + wx)) * scale.y;
    m.m[3][1] = 0.0f;

    // 열 2 (Z axis) * scale.z
    m.m[0][2] = (2.0f * (xz + wy)) * scale.z;
    m.m[1][2] = (2.0f * (yz - wx)) * scale.z;
    m.m[2][2] = (1.0f - 2.0f * (xx + yy)) * scale.z;
    m.m[3][2] = 0.0f;

    // 열 3 (Translation)
    m.m[0][3] = position.x;
    m.m[1][3] = position.y;
    m.m[2][3] = position.z;
    m.m[3][3] = 1.0f;

    return m;
}
