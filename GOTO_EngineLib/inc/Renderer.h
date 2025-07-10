#pragma once
#include "Component.h"
#include "Renderer.h"
#include "Matrix4x4.h"

namespace GOTOEngine
{
	class Renderer : public Component
	{
	private:
		friend class Camera;
		friend class RenderManager;
		bool m_enabled;
		int m_renderOrder; //�׸� ����
		size_t m_renderLayer; //������ ���̾� -> ī�޶� ���� ���̾� �ø� ��������

	protected:
		virtual void Render(Matrix4x4& matrix) = 0;
		Renderer();
		~Renderer();

	public:
		bool GetEnabled();
		void SetEnabled(bool enabled);
		int GetRenderOrder();
		void SetRenderOrder(int value);
		size_t GetRenderLayer() { return m_renderLayer; }
	};
}