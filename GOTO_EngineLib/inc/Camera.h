#pragma once
#include "Behaviour.h"
#include "Matrix4x4.h"

namespace GOTOEngine
{
	class Camera final : public Behaviour
	{
		//�ӽ÷� 2D ī�޶� ��길 ����
	private:
		static Camera* s_mainCam;
		float m_size; //������� ����, �ܾƿ�
		int m_depth; //ī�޶� ��������
		size_t m_renderLayer; //���� ���̾� -> �ø���
		//����Ʈ ��Ʈ ���� �߰��ϱ� -> 0.0~1.0 ��ֶ����� ������, Window Height�� Width�� ���ؼ� ���� ��Ʈ ũ��� ��ġ ���
	
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

		size_t GetRenderLayer() { return m_renderLayer; }

		Matrix4x4 GetMatrix();

		static Camera* GetMainCamera();

		static GameObject* CreateMainCamera();
	};
}