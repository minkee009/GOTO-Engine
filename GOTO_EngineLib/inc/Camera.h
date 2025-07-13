#pragma once
#include "Behaviour.h"
#include "Rect.h"
#include "Matrix3x3.h"

namespace GOTOEngine
{
	class Camera final : public Behaviour
	{
		//�ӽ÷� 2D ī�޶� ��길 ����
	private:
		static Camera* s_mainCam;
		float m_size; //������� ����, �ܾƿ�
		int m_depth; //ī�޶� ��������
		Rect m_rect;
		size_t m_renderLayer; //���� ���̾� -> �ø���
		//����Ʈ ��Ʈ ���� �߰��ϱ� -> 0.0~1.0 ��ֶ����� ������, Window Height�� Width�� ���ؼ� ���� ��Ʈ ũ��� ��ġ ���
		
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