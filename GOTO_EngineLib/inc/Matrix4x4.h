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
	class Quaternion; // 전방 선언
    class Matrix4x4
    {
    public:
        // 4x4 float 행렬
        // 기본적으로 column-major(OpenGL) 방식으로 저장
        // m[column][row]
        alignas(16) float m[4][4];

        // 기본 생성자 (단위행렬)
        Matrix4x4()
        {
            SetIdentity();
        }

		Matrix4x4(const float arr[16])
		{
			SetFromColumnMajorArray(arr);
		}

        // 단위행렬로 설정
        void SetIdentity();

        // 전치 행렬 반환
        Matrix4x4 Transpose() const;

        // 행렬 곱
        Matrix4x4 operator*(const Matrix4x4& rhs) const;

        // 배열 포인터 반환 (column-major)
        const float* Data() const
        {
            return &m[0][0];
        }

        // 행 우선 배열로 복사
        void ToRowMajorArray(float out[16]) const;

        // 열 우선 배열로 복사 -> 사실상 변환 없음
        void ToColumnMajorArray(float out[16]) const;


        // 행렬을 직접 설정 (행렬 요소 16개)
        void SetFromColumnMajorArray(const float arr[16])
        {
            std::memcpy(m, arr, sizeof(float) * 16);
        }

        // 요소 접근
        float& At(int column, int row)
        {
            return m[column][row];
        }

        const float& At(int column, int row) const
        {
            return m[column][row];
        }

		// 역행렬 계산 - 크아악
        Matrix4x4 Inverse() const;

        Vector3 GetPosition()
        {
			// 행렬의 마지막 열이 위치 벡터
			return Vector3(m[3][0], m[3][1], m[3][2]);
        }

		// 회전 추출 (쿼터니언으로 반환)
        Quaternion GetRotation() const;

        Vector3 GetLossyScale() const;

        Vector3 MultiplyPoint(const Vector3& point) const;

        // 원근 투영 행렬 (OpenGL 방식, z in [-1,1])
        static Matrix4x4 Perspective(float fovyRadians, float aspect, float zNear, float zFar);

        // 직교 투영 행렬
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