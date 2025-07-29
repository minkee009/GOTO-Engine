#pragma once
#include "Component.h"
#include "Renderer.h"
#include "Matrix3x3.h"

namespace GOTOEngine
{
	class IRenderAPI;
	class Renderer : public Component
	{
	private:
		friend class Camera;
		friend class RenderManager;
		bool m_enabled;
		int m_renderOrder; //그릴 순서
		size_t m_renderLayer; //렌더링 레이어 -> 카메라 별로 레이어 컬링 설정가능

	protected:
		virtual void Render(Matrix3x3& viewMatrix) = 0;
		Renderer();
		~Renderer();

		IRenderAPI* GetRenderAPIFromManager();
	public:
		bool GetEnabled();
		void SetEnabled(bool enabled);
		int GetRenderOrder();
		void SetRenderOrder(int value);
		size_t GetRenderLayer() { return m_renderLayer; }
		void SetRenderLayer(size_t layer);
	};
}