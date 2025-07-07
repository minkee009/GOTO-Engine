#pragma once
#include <array>
#include <cmath>
#include <cstring>
#include "Vector4.h"
#include "Vector3.h"
#include "Mathf.h"

namespace GOTOEngine
{
	class Quaternion; // ���� ����
    class Matrix4x4
    {
    public:
        // 4x4 float ���
        // �⺻������ column-major(OpenGL) ������� ����
        // m[column][row]
        float m[4][4];

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
        void SetIdentity()
        {
            std::memset(m, 0, sizeof(m));
            for (int i = 0; i < 4; ++i)
                m[i][i] = 1.0f;
        }

        // ��ġ ��� ��ȯ
        Matrix4x4 Transpose() const
        {
            Matrix4x4 result;
            for (int c = 0; c < 4; ++c)
            {
                for (int r = 0; r < 4; ++r)
                {
                    result.m[r][c] = m[c][r];
                }
            }
            return result;
        }

        // ��� ��
        Matrix4x4 operator*(const Matrix4x4& rhs) const
        {
            Matrix4x4 result;
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
            return result;
        }

        // �迭 ������ ��ȯ (column-major)
        const float* Data() const
        {
            return &m[0][0];
        }

        // �� �켱 �迭�� ����
        void ToRowMajorArray(float out[16]) const
        {
            for (int r = 0; r < 4; ++r)
            {
                for (int c = 0; c < 4; ++c)
                {
                    out[r * 4 + c] = m[c][r];
                }
            }
        }

        // �� �켱 �迭�� ���� -> ��ǻ� ��ȯ ����
        void ToColumnMajorArray(float out[16]) const
        {
            std::memcpy(out, &m[0][0], sizeof(float) * 16);
        }

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
        Matrix4x4 Inverse() const
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

			// ������� ��Ľ� ���
            float det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];
            if (std::fabs(det) < 1e-6f)
            {
                // Singular matrix
                return Matrix4x4(); // ������� Ȥ�� ���� ó��
            }

            float invDet = 1.0f / det;
            for (int i = 0; i < 16; i++)
                invOut[i] *= invDet;

            return Matrix4x4(invOut); // �� �����ڴ� column-major �迭�� �޴� ����
        }

        Vector3 GetPosition()
        {
			// ����� ������ ���� ��ġ ����
			return Vector3(m[3][0], m[3][1], m[3][2]);
        }

		// ȸ�� ���� (���ʹϾ����� ��ȯ)
        Quaternion GetRotation() const;

        Vector3 GetLossyScale() const
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

        Vector3 MultiplyPoint(const Vector3& point) const
        {
            Vector3 result;
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

            return result;
        }

        // ���� ���� ��� (OpenGL ���, z in [-1,1])
        static Matrix4x4 Perspective(float fovyRadians, float aspect, float zNear, float zFar)
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

        // ���� ���� ���
        static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
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

        static Matrix4x4 RotateX(float angleRadians)
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

        static Matrix4x4 RotateY(float angleRadians)
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

        static Matrix4x4 RotateZ(float angleRadians)
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

        static Matrix4x4 TRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
    };
}