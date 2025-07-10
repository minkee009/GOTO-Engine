#pragma once
#include "Component.h"
#include "Renderer.h"

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
		virtual void Render(Camera* cam) = 0;
		Renderer();
		~Renderer();

	public:
		bool GetEnabled();
		void SetEnabled(bool enabled);
		int GetRenderOrder();
		void SetRenderOrder(int value);
	};
}