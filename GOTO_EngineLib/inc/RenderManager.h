#pragma once
#include "Singleton.h"
#include "Color.h"
#include "Vector2.h"
#include "vector"

namespace GOTOEngine
{
	class IRenderTarget;
	class IRenderImage;
	class IRenderFont;
	class IRenderAPI;
	class IWindow;
	class RenderManager : public Singleton<RenderManager>
	{
	public:
		void DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image);
		void DrawImage(int x, int y, float scale, const IRenderImage* image);
		void DrawImage(int x, int y, const IRenderImage* image);
		void DrawImage(Vector2 pos, float scale, bool flipX, const IRenderImage* image);
		void DrawImage(Vector2 pos, float scale, const IRenderImage* image);
		void DrawImage(Vector2 pos, const IRenderImage* image);
		void DrawRect(int x, int y, int width, int height, bool fill, Color color);
		void DrawRect(int x, int y, int width, int height, Color color);

		void DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color);
		void DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, Color color);
		void DrawString(int x, int y, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color);
		void DrawString(int x, int y, const wchar_t* string, const IRenderFont* font, Color color);

		void SetVSyncInterval(int interval);

		void Render();
		
		const IWindow* GetWindow();
	private:
		friend class Engine;
		friend class Camera;
		friend class Renderer;
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

		bool m_needCamDepthSort = false;
		bool m_needRenderOrderSort = false;
	};
}

#define RENDER_DRAW_IMAGE GOTOEngine::RenderManager::Get()->DrawImage
#define RENDER_DRAW_RECT GOTOEngine::RenderManager::Get()->DrawRect
#define RENDER_DRAW_STRING GOTOEngine::RenderManager::Get()->DrawString