#pragma once
#include "Behaviour.h"
#include "Rect.h"
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
		size_t m_renderLayer; //렌더 레이어 -> 컬링용
		//뷰포트 렉트 개념 추가하기 -> 0.0~1.0 노멀라이즈 사이즈, Window Height및 Width에 곱해서 최종 렉트 크기와 위치 계산
		
		mutable Matrix3x3 m_cachedMatrix;
		mutable bool m_isMatrixDirty = true;
		mutable Vector2 m_lastPosition;
		mutable float m_lastRotation;
		mutable float m_lastSize;

	public:
		Camera();
		~Camera();

		void OnEnable();
		void OnDisable();
		void OnDestroy();

		int GetDepth() { return m_depth; }
		void SetDepth(int value);

		float GetSize() { return m_size; }
		void SetSize(float value) { m_size = value; }

		Rect GetRect() { return m_rect; }
		void SetRect(Rect rect) { m_rect = rect; }

		size_t GetRenderLayer() { return m_renderLayer; }

		Matrix3x3 GetMatrix();

		static Camera* GetMainCamera();

		static GameObject* CreateMainCamera();
	};
}