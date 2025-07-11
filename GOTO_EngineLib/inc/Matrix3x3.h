#pragma once
#include <cmath>
#include <cstring>
#include "Vector2.h"
#include "Mathf.h"

#ifdef _SIMD_OPTIMIZED
#include <xmmintrin.h>
#endif

namespace GOTOEngine
{
    class Matrix3x3
    {
    public:
        // 3x3 float ���
        // �⺻������ column-major ������� ����
        // m[column][row]
        alignas(16) float m[3][3];

        // �⺻ ������ (�������)
        Matrix3x3()
        {
            SetIdentity();
        }

        Matrix3x3(const float arr[9])
        {
            SetFromColumnMajorArray(arr);
        }

        // ������ķ� ����
        void SetIdentity();

        // ��ġ ��� ��ȯ
        Matrix3x3 Transpose() const;

        // ��� ��
        Matrix3x3 operator*(const Matrix3x3& rhs) const;

        // �迭 ������ ��ȯ (column-major)
        const float* Data() const
        {
            return &m[0][0];
        }

        // �� �켱 �迭�� ����
        void ToRowMajorArray(float out[9]) const;

        // �� �켱 �迭�� ����
        void ToColumnMajorArray(float out[9]) const;

        // ����� ���� ���� (��� ���� 9��)
        void SetFromColumnMajorArray(const float arr[9])
        {
            std::memcpy(m, arr, sizeof(float) * 9);
        }

        // ���� ����
        float& At(int column, int row)
        {
            return m[column][row];
        }

        const float& At(int column, int row) const
        {
            return m[column][row];
        }

        // ����� ���
        Matrix3x3 Inverse() const;

        // 2D ��ġ ����
        Vector2 GetPosition() const
        {
            return Vector2(m[2][0], m[2][1]);
        }

        // 2D ȸ�� ���� ���� (����)
        float GetRotation() const;

        // 2D ������ ����
        Vector2 GetScale() const;

        // 2D �� ��ȯ
        Vector2 MultiplyPoint(const Vector2& point) const;

        // 2D ���� ��ȯ (��ġ �̵� ����)
        Vector2 MultiplyVector(const Vector2& vector) const;

        // ���� ���� �Լ���
        static Matrix3x3 Translate(float x, float y)
        {
            Matrix3x3 mat;
            mat.SetIdentity();
            mat.m[2][0] = x;
            mat.m[2][1] = y;
            return mat;
        }

        static Matrix3x3 Scale(float sx, float sy)
        {
            Matrix3x3 mat;
            mat.SetIdentity();
            mat.m[0][0] = sx;
            mat.m[1][1] = sy;
            return mat;
        }

        static Matrix3x3 Rotate(float angleRadians)
        {
            Matrix3x3 mat;
            mat.SetIdentity();
            float c = std::cos(angleRadians);
            float s = std::sin(angleRadians);
            mat.m[0][0] = c;
            mat.m[0][1] = s;
            mat.m[1][0] = -s;
            mat.m[1][1] = c;
            return mat;
        }

        // TRS: Translation, Rotation, Scale
        static Matrix3x3 TRS(const Vector2& position, float rotationRadians, const Vector2& scale);
    };
}