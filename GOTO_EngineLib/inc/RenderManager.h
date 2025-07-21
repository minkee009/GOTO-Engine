#pragma once
#include "Singleton.h"
#include "Color.h"
#include "Vector2.h"
#include "vector"
#include "TextHelper.h"
#include "Rect.h"

namespace GOTOEngine
{
	class IRenderBitmap;
	class IRenderFont;
	class IRenderAPI;
	class IWindow;
	struct RenderAPIMemoryStatus;
	class RenderManager : public Singleton<RenderManager>
	{
	public:
		void SetVSyncInterval(int interval);
		const IWindow* GetWindow() const;

		//OnGUI용 그리기 함수
		void DrawString(
			const wchar_t* text,
			Rect rect = Rect{ 0,0,0,0 },
			const IRenderFont* font = nullptr,
			size_t size = 24,
			IRenderFontStyle fontStyle = IRenderFontStyle::Bold,
			Color color = Color{ 255,255,255,255 },
			TextHoriAlign hAlign = TextHoriAlign::Left,
			TextVertAlign vAlign = TextVertAlign::Up);

		void DrawString(
			const wchar_t* text,
			Rect rect,
			Color color);


		RenderAPIMemoryStatus CollectMemoryUsage();
	private:
		friend class Engine;
		friend class Camera;
		friend class Renderer;
		friend class Texture2D;
		friend class Font;
		void StartUp(IWindow* window);
		void ShutDown();
		IRenderAPI* m_pRenderAPI = nullptr;

		std::vector<Camera*> m_cameras;
		std::vector<Renderer*> m_renderers;

		IRenderAPI* GetRenderAPI() { return m_pRenderAPI; }

		void RegisterCamera(Camera* cam);
		void UnRegisterCamera(Camera* cam);
		void RegisterRenderer(Renderer* renderer);
		void UnRegisterRenderer(Renderer* renderer);
		void SortCamera();
		void SortRenderer();
		void SetCamSortDirty() { m_needCamDepthSort = true; }
		void SetRendererSortDirty() { m_needRenderOrderSort = true; }

		/// <summary>
		/// 렌더링을 시작합니다.
		/// 1. BeginDraw
		/// 2. Clear
		/// </summary>
		void StartRender();

		/// <summary>
		/// 렌더목록을 이용해여 렌더타겟에 그립니다.
		/// 1. Camera Sort
		/// 2. Renderer Sort
		/// 3. Make Camera View Matrix
		/// 4. Set Viewport
		/// 5. Camera -> Renderer loop
		/// 6. Reset Viewport
		/// </summary>
		void Render();

		/// <summary>
		/// 렌더링을 종료하고 렌더타겟을 화면에 송출합니다.
		/// 1. EndDraw
		/// 2. SwapBuffer
		/// </summary>
		void EndRender();

		IRenderBitmap* CreateRenderBitmap(std::wstring filePath);
		IRenderFont* CreateRenderFontFromFilePath(std::wstring filePath);
		IRenderFont* CreateRenderFontFromOS(std::wstring fontName);

		bool m_needCamDepthSort = false;
		bool m_needRenderOrderSort = false;
	};
}

#define RENDER_DRAW_RECT GOTOEngine::RenderManager::Get()->DrawRect