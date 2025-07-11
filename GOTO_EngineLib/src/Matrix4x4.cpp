#include "Matrix4x4.h"
#include "Quaternion.h"

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::operator*(const Matrix4x4& rhs) const
{
    Matrix4x4 result;
    
#ifdef _SIMD_OPTIMIZED
    for (int row = 0; row < 4; ++row)
    {
        // lhs의 row 번째 행의 각 열 원소를 SSE 레지스터에 4번 복제
        __m128 lhs0 = _mm_set1_ps(m[0][row]);
        __m128 lhs1 = _mm_set1_ps(m[1][row]);
        __m128 lhs2 = _mm_set1_ps(m[2][row]);
        __m128 lhs3 = _mm_set1_ps(m[3][row]);

        // rhs의 각 열(4개 원소)을 로드
        __m128 rhs_col0 = _mm_load_ps(&rhs.m[0][0]); // 첫번째 열
        __m128 rhs_col1 = _mm_load_ps(&rhs.m[1][0]);
        __m128 rhs_col2 = _mm_load_ps(&rhs.m[2][0]);
        __m128 rhs_col3 = _mm_load_ps(&rhs.m[3][0]);

        // 각각 곱하고 더함
        __m128 res = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(lhs0, rhs_col0), _mm_mul_ps(lhs1, rhs_col1)),
            _mm_add_ps(_mm_mul_ps(lhs2, rhs_col2), _mm_mul_ps(lhs3, rhs_col3))
        );

        // 결과를 result의 해당 row에 저장 (column-major라 m[col][row])
        _mm_store_ps(&result.m[0][row], res);
    }
#else
    for (int c = 0; c < 4; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            result.m[c][r] =
                m[0][r] * rhs.m[c][0] +
                m[1][r] * rhs.m[c][1] +
                m[2][r] * rhs.m[c][2] +
                m[3][r] * rhs.m[c][3];
        }
    }
#endif
    return result;
}

void GOTOEngine::Matrix4x4::SetIdentity()
{
    std::memset(m, 0, sizeof(m));
    for (int i = 0; i < 4; ++i)
        m[i][i] = 1.0f;
}


GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Transpose() const
{
    Matrix4x4 result;
#ifdef _SIMD_OPTIMIZED
    __m128 row0 = _mm_load_ps(&m[0][0]); // col0 전체, 즉 m[0][0..3]
    __m128 row1 = _mm_load_ps(&m[1][0]); // col1 전체
    __m128 row2 = _mm_load_ps(&m[2][0]);
    __m128 row3 = _mm_load_ps(&m[3][0]);

    _MM_TRANSPOSE4_PS(row0, row1, row2, row3);

    _mm_store_ps(&result.m[0][0], row0);
    _mm_store_ps(&result.m[1][0], row1);
    _mm_store_ps(&result.m[2][0], row2);
    _mm_store_ps(&result.m[3][0], row3);
#else
    for (int c = 0; c < 4; ++c)
    {
        for (int r = 0; r < 4; ++r)
        {
            result.m[r][c] = m[c][r];
        }
    }
#endif
    return result;
}


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

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Inverse() const
{
    Matrix4x4 inv;
    const float* m = &this->m[0][0];

    float invOut[16];

    invOut[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    invOut[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    invOut[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    invOut[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    invOut[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    invOut[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    invOut[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    invOut[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    invOut[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    invOut[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    invOut[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    invOut[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    invOut[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    invOut[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    invOut[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    invOut[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    // 역행렬의 행렬식 계산
    float det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];
    if (std::fabs(det) < 1e-6f)
    {
        // Singular matrix
        return Matrix4x4(); // 단위행렬 혹은 예외 처리
    }

    float invDet = 1.0f / det;
    for (int i = 0; i < 16; i++)
        invOut[i] *= invDet;

    return Matrix4x4(invOut); // 이 생성자는 column-major 배열을 받는 버전
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

GOTOEngine::Vector3 GOTOEngine::Matrix4x4::GetLossyScale() const
{

    float scaleX = std::sqrt(
        m[0][0] * m[0][0] +
        m[1][0] * m[1][0] +
        m[2][0] * m[2][0]);

    float scaleY = std::sqrt(
        m[0][1] * m[0][1] +
        m[1][1] * m[1][1] +
        m[2][1] * m[2][1]);

    float scaleZ = std::sqrt(
        m[0][2] * m[0][2] +
        m[1][2] * m[1][2] +
        m[2][2] * m[2][2]);

    return Vector3(scaleX, scaleY, scaleZ);
}

GOTOEngine::Vector3 GOTOEngine::Matrix4x4::MultiplyPoint(const Vector3& point) const
{
    Vector3 result;
 
#ifdef _SIMD_OPTIMIZED
    // point.x, point.y, point.z을 각각 4개 복제해서 SIMD 연산 수행 후 합산
    __m128 px = _mm_set1_ps(point.x);
    __m128 py = _mm_set1_ps(point.y);
    __m128 pz = _mm_set1_ps(point.z);

    // 각 열 벡터(4원소)
    __m128 col0 = _mm_load_ps(&m[0][0]);
    __m128 col1 = _mm_load_ps(&m[1][0]);
    __m128 col2 = _mm_load_ps(&m[2][0]);
    __m128 col3 = _mm_load_ps(&m[3][0]);

    // point * matrix 계산 (x, y, z, w)
    __m128 res = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(col0, px), _mm_mul_ps(col1, py)),
        _mm_add_ps(_mm_mul_ps(col2, pz), col3));

    // res에 담긴 4개 원소 중 x,y,z 추출
    alignas(16) float vals[4];
    _mm_store_ps(vals, res);

    result.x = vals[0];
    result.y = vals[1];
    result.z = vals[2];
#else
    result.x =
        m[0][0] * point.x +
        m[0][1] * point.y +
        m[0][2] * point.z +
        m[0][3];

    result.y =
        m[1][0] * point.x +
        m[1][1] * point.y +
        m[1][2] * point.z +
        m[1][3];

    result.z =
        m[2][0] * point.x +
        m[2][1] * point.y +
        m[2][2] * point.z +
        m[2][3];
#endif

    return result;
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Perspective(float fovyRadians, float aspect, float zNear, float zFar)
{
    Matrix4x4 mat;
    float f = 1.0f / std::tan(fovyRadians / 2.0f);
    mat.SetIdentity();
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (zFar + zNear) / (zNear - zFar);
    mat.m[2][3] = -1.0f;
    mat.m[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
    mat.m[3][3] = 0.0f;
    return mat;
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
    Matrix4x4 mat;
    mat.SetIdentity();
    mat.m[0][0] = 2.0f / (right - left);
    mat.m[1][1] = 2.0f / (top - bottom);
    mat.m[2][2] = -2.0f / (zFar - zNear);
    mat.m[3][0] = -(right + left) / (right - left);
    mat.m[3][1] = -(top + bottom) / (top - bottom);
    mat.m[3][2] = -(zFar + zNear) / (zFar - zNear);
    return mat;
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::Rotate(Quaternion& quaternion)
{
    return quaternion.ToMatrix4x4();
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::RotateX(float angleRadians)
{
    Matrix4x4 mat;
    mat.SetIdentity();
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    mat.m[1][1] = c;
    mat.m[1][2] = s;
    mat.m[2][1] = -s;
    mat.m[2][2] = c;
    return mat;
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::RotateY(float angleRadians)
{
    Matrix4x4 mat;
    mat.SetIdentity();
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    mat.m[0][0] = c;
    mat.m[0][2] = -s;
    mat.m[2][0] = s;
    mat.m[2][2] = c;
    return mat;
}

GOTOEngine::Matrix4x4 GOTOEngine::Matrix4x4::RotateZ(float angleRadians)
{
    Matrix4x4 mat;
    mat.SetIdentity();
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);
    mat.m[0][0] = c;
    mat.m[0][1] = s;
    mat.m[1][0] = -s;
    mat.m[1][1] = c;
    return mat;
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
