#pragma once
#include <array>
#include <cmath>
#include <cstring>
#include "Vector4.h"
#include "Vector3.h"
#include "Mathf.h"

#ifdef _SIMD_OPTIMIZED
#include <xmmintrin.h>
#endif

namespace GOTOEngine
{
	class Quaternion; // ���� ����
    class Matrix4x4
    {
    public:
        // 4x4 float ���
        // �⺻������ column-major(OpenGL) ������� ����
        // m[column][row]
        alignas(16) float m[4][4];

        // �⺻ ������ (�������)
        Matrix4x4()
        {
            SetIdentity();
        }

		Matrix4x4(const float arr[16])
		{
			SetFromColumnMajorArray(arr);
		}

        // ������ķ� ����
        void SetIdentity();

        // ��ġ ��� ��ȯ
        Matrix4x4 Transpose() const;

        // ��� ��
        Matrix4x4 operator*(const Matrix4x4& rhs) const;

        // �迭 ������ ��ȯ (column-major)
        const float* Data() const
        {
            return &m[0][0];
        }

        // �� �켱 �迭�� ����
        void ToRowMajorArray(float out[16]) const;

        // �� �켱 �迭�� ���� -> ��ǻ� ��ȯ ����
        void ToColumnMajorArray(float out[16]) const;


        // ����� ���� ���� (��� ��� 16��)
        void SetFromColumnMajorArray(const float arr[16])
        {
            std::memcpy(m, arr, sizeof(float) * 16);
        }

        // ��� ����
        float& At(int column, int row)
        {
            return m[column][row];
        }

        const float& At(int column, int row) const
        {
            return m[column][row];
        }

		// ����� ��� - ũ�ƾ�
        Matrix4x4 Inverse() const;

        Vector3 GetPosition()
        {
			// ����� ������ ���� ��ġ ����
			return Vector3(m[3][0], m[3][1], m[3][2]);
        }

		// ȸ�� ���� (���ʹϾ����� ��ȯ)
        Quaternion GetRotation() const;

        Vector3 GetLossyScale() const;

        Vector3 MultiplyPoint(const Vector3& point) const;

        // ���� ���� ��� (OpenGL ���, z in [-1,1])
        static Matrix4x4 Perspective(float fovyRadians, float aspect, float zNear, float zFar);

        // ���� ���� ���
        static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

        static Matrix4x4 Translate(float x, float y, float z)
        {
            Matrix4x4 mat;
            mat.SetIdentity();
            mat.m[3][0] = x;
            mat.m[3][1] = y;
            mat.m[3][2] = z;
            return mat;
        }

        static Matrix4x4 Scale(float sx, float sy, float sz)
        {
            Matrix4x4 mat;
            mat.SetIdentity();
            mat.m[0][0] = sx;
            mat.m[1][1] = sy;
            mat.m[2][2] = sz;
            return mat;
        }

        static Matrix4x4 Rotate(Quaternion& quaternion);

        static Matrix4x4 RotateX(float angleRadians);

        static Matrix4x4 RotateY(float angleRadians);

        static Matrix4x4 RotateZ(float angleRadians);

        static Matrix4x4 TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
    };
}