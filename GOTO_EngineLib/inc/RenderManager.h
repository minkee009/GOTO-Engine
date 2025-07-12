#pragma once
#include "Singleton.h"
#include "Color.h"
#include "Vector2.h"
#include "vector"

namespace GOTOEngine
{
	class IRenderTarget;
	class IRenderImage;
	class IRenderBitmap;
	class IRenderFont;
	class IRenderAPI;
	class IWindow;
	class RenderManager : public Singleton<RenderManager>
	{
	public:
		void SetVSyncInterval(int interval);

		void Render();
		
		const IWindow* GetWindow();
	private:
		friend class Engine;
		friend class Camera;
		friend class Renderer;
		friend class Texture2D;
		void StartUp(IWindow* window);
		void ShutDown();
		IRenderAPI* m_pRenderAPI = nullptr;

		std::vector<Camera*> m_cameras;
		std::vector<Renderer*> m_renderers;
		void RegisterCamera(Camera* cam);
		void UnRegisterCamera(Camera* cam);
		void RegisterRenderer(Renderer* renderer);
		void UnRegisterRenderer(Renderer* renderer);
		void SortCamera();
		void SortRenderer();
		void SetCamSortDirty() { m_needCamDepthSort = true; }
		void SetRendererSortDirty() { m_needRenderOrderSort = true; }

		IRenderBitmap* CreateBitmap(std::wstring filePath);

		bool m_needCamDepthSort = false;
		bool m_needRenderOrderSort = false;
	};
}

#define RENDER_DRAW_IMAGE GOTOEngine::RenderManager::Get()->DrawImage
#define RENDER_DRAW_RECT GOTOEngine::RenderManager::Get()->DrawRect
#define RENDER_DRAW_STRING GOTOEngine::RenderManager::Get()->DrawString