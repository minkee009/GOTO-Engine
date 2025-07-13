#include "Matrix3x3.h"

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::operator*(const Matrix3x3& rhs) const
{
    Matrix3x3 result;

    for (int c = 0; c < 3; ++c) // result�� ��
    {
        for (int r = 0; r < 3; ++r) // result�� ��
        {
            result.m[c][r] =
                m[0][r] * rhs.m[c][0] +
                m[1][r] * rhs.m[c][1] +
                m[2][r] * rhs.m[c][2];
        }
    }
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
    // �� �ڵ�� Column-Major ���� ��Ŀ� ���� �ε����� �����ؾ� �մϴ�.
    // mat[col * 3 + row] ���·� ��ȯ�Ͽ� �����ϰų� m[col][row] ���·� ���� �����մϴ�.
    // ���� �ڵ�� mat[idx] �������� �����ϹǷ�, �̸� m[c][r] ���·� �����մϴ�.
    const float* mat_ptr = &this->m[0][0]; // m[0][0], m[1][0], m[2][0], m[0][1] ...

    // ��� ��ҿ� ���� �����ϵ��� ���� (m[column][row])
    float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
        m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
        m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);

    if (std::fabs(det) < 1e-6f)
    {
        // Singular matrix
        return Matrix3x3(); // ������� ��ȯ
    }

    float invDet = 1.0f / det;

    // ���� ����� ��ġ (Adjugate)�� ����� �� ����� ���� ����
    // (column-major ����� ����)
    inv.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    inv.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    inv.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;

    inv.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    inv.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    inv.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;

    inv.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
    inv.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
    inv.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return inv;
}

float GOTOEngine::Matrix3x3::GetRotation() const
{
    // X�� ���� ������ ���� ���̸� ���� (sqrt ���� �ּ�ȭ)
    float scaleXSquared = m[0][0] * m[0][0] + m[0][1] * m[0][1];

    // ���� ���� ������ üũ (���������� ���Ͽ� sqrt ����)
    if (scaleXSquared < 1e-12f) {
        return 0.0f;
    }

    // ���� �������� �Ǵ� �Ϲ� sqrt ���
    float invScaleX = 1.0f / std::sqrt(scaleXSquared);

    // ����ȭ�� ���ͷκ��� ȸ�� ���� ����
    return std::atan2(m[0][1] * invScaleX, m[0][0] * invScaleX);
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::GetLossyScale() const
{
    // �� �켱 ��Ŀ��� ������ ����
    // m[0][0] = Sx * cos(theta), m[0][1] = Sx * sin(theta)
    // m[1][0] = -Sy * sin(theta), m[1][1] = Sy * cos(theta)

    // ù ��° �� (���� X��)�� ����: sqrt(m[0][0]^2 + m[0][1]^2) = Sx
    float scaleX = std::sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1]);
    // �� ��° �� (���� Y��)�� ����: sqrt(m[1][0]^2 + m[1][1]^2) = Sy
    float scaleY = std::sqrt(m[1][0] * m[1][0] + m[1][1] * m[1][1]);

    return Vector2(scaleX, scaleY);
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::MultiplyPoint(const Vector2& point) const
{
    Vector2 result;

#ifdef _SIMD_OPTIMIZED
    // SIMD ����ȭ: 2D �� ��ȯ�� �°� ���� (3x3 * 3x1 ����)
    // �Է� point = (x, y, 1.0f)
    // ����� �� ���͸� �ε��Ͽ� ���� �� ���� ����

    // point.x, point.y, 1.0f�� ���� ��Į��� ���
    __m128 p_vec = _mm_set_ps(0.0f, 1.0f, point.y, point.x); // (x, y, 1, 0)

    // ����� ������ �ε�
    // col0 = (m[0][0], m[0][1], m[0][2], 0)
    // col1 = (m[1][0], m[1][1], m[1][2], 0)
    // col2 = (m[2][0], m[2][1], m[2][2], 0)
    __m128 col0 = _mm_loadu_ps(&m[0][0]); // m[0][0], m[0][1], m[0][2], dummy
    __m128 col1 = _mm_loadu_ps(&m[1][0]); // m[1][0], m[1][1], m[1][2], dummy
    __m128 col2 = _mm_loadu_ps(&m[2][0]); // m[2][0], m[2][1], m[2][2], dummy

    // x * col0
    __m128 mul0 = _mm_mul_ps(col0, _mm_shuffle_ps(p_vec, p_vec, _MM_SHUFFLE(0, 0, 0, 0))); // point.x
    // y * col1
    __m128 mul1 = _mm_mul_ps(col1, _mm_shuffle_ps(p_vec, p_vec, _MM_SHUFFLE(1, 1, 1, 1))); // point.y
    // 1.0f * col2
    __m128 mul2 = _mm_mul_ps(col2, _mm_shuffle_ps(p_vec, p_vec, _MM_SHUFFLE(2, 2, 2, 2))); // 1.0f

    // (x * col0) + (y * col1) + (1.0f * col2)
    __m128 res = _mm_add_ps(_mm_add_ps(mul0, mul1), mul2);

    // ��� ����
    // ����� res�� ù �� ��ҿ� ���� (x', y')
    _mm_store_ss(&result.x, res); // res�� ù ��° ��� (x)
    _mm_store_ss(&result.y, _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 1, 1, 1))); // res�� �� ��° ��� (y)

#else
    // �� �켱 ��Ŀ��� �� ��ȯ:
    // P_transformed.x = m[0][0] * P.x + m[1][0] * P.y + m[2][0] * 1.0
    // P_transformed.y = m[0][1] * P.x + m[1][1] * P.y + m[2][1] * 1.0
    // ������ 1.0�� 2D ���� ��ǥ���� w��
    result.x = m[0][0] * point.x + m[1][0] * point.y + m[2][0];
    result.y = m[0][1] * point.x + m[1][1] * point.y + m[2][1];
#endif

    return result;
}

GOTOEngine::Vector2 GOTOEngine::Matrix3x3::MultiplyVector(const Vector2& vector) const
{
    Vector2 result;

    // ���� ��ȯ�� ��ġ �̵� ���� ȸ���� �����ϸ� ����
    // P_transformed.x = m[0][0] * P.x + m[1][0] * P.y
    // P_transformed.y = m[0][1] * P.x + m[1][1] * P.y
    result.x = m[0][0] * vector.x + m[1][0] * vector.y;
    result.y = m[0][1] * vector.x + m[1][1] * vector.y;

    return result;
}

GOTOEngine::Matrix3x3 GOTOEngine::Matrix3x3::TRS(const Vector2& position, float rotationRadians, const Vector2& scale)
{
    Matrix3x3 mat;
    mat.SetIdentity(); // �⺻������ ���� ��ķ� �ʱ�ȭ

    float c = std::cos(rotationRadians);
    float s = std::sin(rotationRadians);

    // �� �켱 ����� TRS ����
    // �Ϲ������� OpenGL���� ����ϴ� 2D ��ȯ ��� ����:
    // [ Sx*cos(theta)   -Sy*sin(theta)   Tx ]
    // [ Sx*sin(theta)    Sy*cos(theta)   Ty ]
    // [ 0                0               1  ]

    // Column 0 (Scaled X-axis)
    mat.m[0][0] = c * scale.x;
    mat.m[0][1] = s * scale.x;
    mat.m[0][2] = 0.0f; // �׻� 0

    // Column 1 (Scaled Y-axis)
    mat.m[1][0] = -s * scale.y;
    mat.m[1][1] = c * scale.y;
    mat.m[1][2] = 0.0f; // �׻� 0

    // Column 2 (Translation)
    mat.m[2][0] = position.x;
    mat.m[2][1] = position.y;
    mat.m[2][2] = 1.0f; // �׻� 1

    return mat;
}