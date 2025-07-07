#pragma once
#include "Singleton.h"
#include "Color.h"
#include "Vector2.h"

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
		void Clear();
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
		void SwapBuffer();
		void SetVSyncInterval(int interval);
	private:
		friend class Engine;
		void StartUp(IWindow* window);
		void ShutDown();
		IRenderAPI* m_pRenderer = nullptr;
	};
}

#define RENDER_DRAW_IMAGE GOTOEngine::RenderManager::Get()->DrawImage
#define RENDER_DRAW_RECT GOTOEngine::RenderManager::Get()->DrawRect
#define RENDER_DRAW_STRING GOTOEngine::RenderManager::Get()->DrawString