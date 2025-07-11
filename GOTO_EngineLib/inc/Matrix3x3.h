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
        // 3x3 float 행렬
        // 기본적으로 column-major 방식으로 저장
        // m[column][row]
        alignas(16) float m[3][3];

        // 기본 생성자 (단위행렬)
        Matrix3x3()
        {
            SetIdentity();
        }

        Matrix3x3(const float arr[9])
        {
            SetFromColumnMajorArray(arr);
        }

        // 단위행렬로 설정
        void SetIdentity();

        // 전치 행렬 반환
        Matrix3x3 Transpose() const;

        // 행렬 곱
        Matrix3x3 operator*(const Matrix3x3& rhs) const;

        // 배열 포인터 반환 (column-major)
        const float* Data() const
        {
            return &m[0][0];
        }

        // 행 우선 배열로 복사
        void ToRowMajorArray(float out[9]) const;

        // 열 우선 배열로 복사
        void ToColumnMajorArray(float out[9]) const;

        // 행렬을 직접 설정 (행렬 원소 9개)
        void SetFromColumnMajorArray(const float arr[9])
        {
            std::memcpy(m, arr, sizeof(float) * 9);
        }

        // 원소 접근
        float& At(int column, int row)
        {
            return m[column][row];
        }

        const float& At(int column, int row) const
        {
            return m[column][row];
        }

        // 역행렬 계산
        Matrix3x3 Inverse() const;

        // 2D 위치 추출
        Vector2 GetPosition() const
        {
            return Vector2(m[2][0], m[2][1]);
        }

        // 2D 회전 각도 추출 (라디안)
        float GetRotation() const;

        // 2D 스케일 추출
        Vector2 GetScale() const;

        // 2D 점 변환
        Vector2 MultiplyPoint(const Vector2& point) const;

        // 2D 벡터 변환 (위치 이동 없이)
        Vector2 MultiplyVector(const Vector2& vector) const;

        // 정적 생성 함수들
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