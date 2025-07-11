#include "Matrix3x3.h"

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::operator*(const Matrix3x3& rhs) const
{
    Matrix3x3 result;

#ifdef _SIMD_OPTIMIZED
    for (int row = 0; row < 3; ++row)
    {
        // lhs의 row 번째 행의 각 원소를 SSE 레지스터에 3번 복제
        __m128 lhs0 = _mm_set1_ps(m[0][row]);
        __m128 lhs1 = _mm_set1_ps(m[1][row]);
        __m128 lhs2 = _mm_set1_ps(m[2][row]);

        // rhs의 각 열을 로드 (3개 원소만 사용)
        __m128 rhs_col0 = _mm_set_ps(0.0f, rhs.m[0][2], rhs.m[0][1], rhs.m[0][0]);
        __m128 rhs_col1 = _mm_set_ps(0.0f, rhs.m[1][2], rhs.m[1][1], rhs.m[1][0]);
        __m128 rhs_col2 = _mm_set_ps(0.0f, rhs.m[2][2], rhs.m[2][1], rhs.m[2][0]);

        // 각각 곱하고 더함
        __m128 res = _mm_add_ps(
            _mm_add_ps(_mm_mul_ps(lhs0, rhs_col0), _mm_mul_ps(lhs1, rhs_col1)),
            _mm_mul_ps(lhs2, rhs_col2)
        );

        // 결과를 저장 (3개 원소만)
        float temp[4];
        _mm_storeu_ps(temp, res);
        result.m[0][row] = temp[0];
        result.m[1][row] = temp[1];
        result.m[2][row] = temp[2];
    }
#else
    for (int c = 0; c < 3; ++c)
    {
        for (int r = 0; r < 3; ++r)
        {
            result.m[c][r] =
                m[0][r] * rhs.m[c][0] +
                m[1][r] * rhs.m[c][1] +
                m[2][r] * rhs.m[c][2];
        }
    }
#endif
    return result;
}

void GOTOEngine::Matrix3x3::SetIdentity()
{
    std::memset(m, 0, sizeof(m));
    for (int i = 0; i < 3; ++i)
        m[i][i] = 1.0f;
}

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::Transpose() const
{
    Matrix3x3 result;
    for (int c = 0; c < 3; ++c)
    {
        for (int r = 0; r < 3; ++r)
        {
            result.m[r][c] = m[c][r];
        }
    }
    return result;
}

void GOTOEngine::Matrix3x3::ToRowMajorArray(float out[9]) const
{
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            out[r * 3 + c] = m[c][r];
        }
    }
}

void GOTOEngine::Matrix3x3::ToColumnMajorArray(float out[9]) const
{
    std::memcpy(out, &m[0][0], sizeof(float) * 9);
}

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::Inverse() const
{
    Matrix3x3 inv;
    const float* mat = &this->m[0][0];

    // 3x3 행렬의 역행렬 계산
    float det = mat[0] * (mat[4] * mat[8] - mat[7] * mat[5]) -
        mat[3] * (mat[1] * mat[8] - mat[7] * mat[2]) +
        mat[6] * (mat[1] * mat[5] - mat[4] * mat[2]);

    if (std::fabs(det) < 1e-6f)
    {
        // Singular matrix
        return Matrix3x3(); // 단위행렬 반환
    }

    float invDet = 1.0f / det;

    // 수반 행렬 계산
    inv.m[0][0] = (mat[4] * mat[8] - mat[7] * mat[5]) * invDet;
    inv.m[0][1] = -(mat[1] * mat[8] - mat[7] * mat[2]) * invDet;
    inv.m[0][2] = (mat[1] * mat[5] - mat[4] * mat[2]) * invDet;

    inv.m[1][0] = -(mat[3] * mat[8] - mat[6] * mat[5]) * invDet;
    inv.m[1][1] = (mat[0] * mat[8] - mat[6] * mat[2]) * invDet;
    inv.m[1][2] = -(mat[0] * mat[5] - mat[3] * mat[2]) * invDet;

    inv.m[2][0] = (mat[3] * mat[7] - mat[6] * mat[4]) * invDet;
    inv.m[2][1] = -(mat[0] * mat[7] - mat[6] * mat[1]) * invDet;
    inv.m[2][2] = (mat[0] * mat[4] - mat[3] * mat[1]) * invDet;

    return inv;
}

float GOTOEngine::Matrix3x3::GetRotation() const
{
    // 스케일 제거 후 회전 각도 추출
    Vector2 scale = GetScale();
    float r00 = m[0][0] / scale.x;
    float r01 = m[0][1] / scale.y;

    return std::atan2(r01, r00);
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::GetScale() const
{
    float scaleX = std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
    float scaleY = std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1]);

    return Vector2(scaleX, scaleY);
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::MultiplyPoint(const Vector2& point) const
{
    Vector2 result;

#ifdef _SIMD_OPTIMIZED
    // point.x, point.y, 1.0f를 각각 복제해서 SIMD 연산 수행
    __m128 px = _mm_set1_ps(point.x);
    __m128 py = _mm_set1_ps(point.y);
    __m128 pz = _mm_set1_ps(1.0f);

    // 각 열 벡터 로드
    __m128 col0 = _mm_set_ps(0.0f, m[0][2], m[0][1], m[0][0]);
    __m128 col1 = _mm_set_ps(0.0f, m[1][2], m[1][1], m[1][0]);
    __m128 col2 = _mm_set_ps(0.0f, m[2][2], m[2][1], m[2][0]);

    // point * matrix 계산
    __m128 res = _mm_add_ps(
        _mm_add_ps(_mm_mul_ps(col0, px), _mm_mul_ps(col1, py)),
        _mm_mul_ps(col2, pz));

    // 결과 추출
    float vals[4];
    _mm_storeu_ps(vals, res);
    result.x = vals[0];
    result.y = vals[1];
#else
    result.x = m[0][0] * point.x + m[1][0] * point.y + m[2][0];
    result.y = m[0][1] * point.x + m[1][1] * point.y + m[2][1];
#endif

    return result;
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::MultiplyVector(const Vector2& vector) const
{
    Vector2 result;

    // 벡터 변환은 위치 이동 없이 회전과 스케일만 적용
    result.x = m[0][0] * vector.x + m[1][0] * vector.y;
    result.y = m[0][1] * vector.x + m[1][1] * vector.y;

    return result;
}

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::TRS(const Vector2& position, float rotationRadians, const Vector2& scale)
{
    Matrix3x3 mat;

    float c = std::cos(rotationRadians);
    float s = std::sin(rotationRadians);

    // 열 0 (X axis) * scale.x
    mat.m[0][0] = c * scale.x;
    mat.m[0][1] = s * scale.x;
    mat.m[0][2] = 0.0f;

    // 열 1 (Y axis) * scale.y
    mat.m[1][0] = -s * scale.y;
    mat.m[1][1] = c * scale.y;
    mat.m[1][2] = 0.0f;

    // 열 2 (Translation)
    mat.m[2][0] = position.x;
    mat.m[2][1] = position.y;
    mat.m[2][2] = 1.0f;

    return mat;
}