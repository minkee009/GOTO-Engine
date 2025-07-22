#pragma once
#include "Behaviour.h"
#include "Rect.h"
#include "Color.h"
#include "Matrix3x3.h"

namespace GOTOEngine
{
	class Camera final : public Behaviour
	{
		//임시로 2D 카메라 계산만 적용
	private:
		static Camera* s_mainCam;
		float m_size; //직교행렬 줌인, 줌아웃
		int m_depth; //카메라 렌더오더
		Rect m_rect;
		Color m_backGroundColor;
		size_t m_renderLayer; //렌더 레이어 -> 컬링용

		mutable Matrix3x3 m_cachedMatrix;
		mutable bool m_isMatrixDirty = true;
		mutable Vector2 m_lastPosition;
		mutable float m_lastRotation;

	public:
		Camera();
		~Camera();

		void OnEnable();
		void OnDisable();
		void OnDestroy();

		int GetDepth() { return m_depth; }
		void SetDepth(int value);

		float GetSize() { return m_size; }
		void SetSize(float value) { m_isMatrixDirty = true; m_size = value; }

		Rect GetRect() { return m_rect; }
		void SetRect(Rect rect) { m_isMatrixDirty = true; m_rect = rect; }

		Color GetBackGroundColor() { return m_backGroundColor; }
		void SetBackGroundColor(const Color& value) { m_backGroundColor = value; }

		size_t GetRenderLayer() { return m_renderLayer; }

		Matrix3x3 GetMatrix();

		Vector2 ScreenToViewportPoint(const Vector2& screenPoint);
		Vector2 ScreenToWorldPoint(const Vector2& screenPoint);

		Vector2 ViewportToScreenPoint(const Vector2& viewportPoint);
		Vector2 ViewportToWorldPoint(const Vector2& viewportPoint);

		Vector2 WorldToScreenPoint(const Vector2& worldPoint);
		Vector2 WorldToViewportPoint(const Vector2& worldPoint);


		static Camera* GetMainCamera();
		static GameObject* CreateMainCamera();
	};
}