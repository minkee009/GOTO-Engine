#pragma once
#include "Color.h"
#include "Rect.h"
#include "Matrix3x3.h"

namespace GOTOEngine
{
	struct RenderAPIMemoryStatus
	{
		std::string vramUsage;
		std::string dramUsage;
		std::string pageFileUsage;
	};

	class IWindow;
	class IRenderFont;
	class IRenderBitmap;
	enum IRenderFontStyle;
	class IRenderAPI
	{
	public:
		virtual bool Initialize(IWindow* window) = 0;
		virtual void Release() = 0;
		virtual void Clear() = 0;

		virtual void DrawBitmap(const IRenderBitmap* bitmap, const Matrix3x3& mat, const Rect& sourceRect) = 0;
		virtual void DrawString(const wchar_t* string, const Rect& rect, const IRenderFont* font, size_t size, const IRenderFontStyle& fontStyle, Color color, const Matrix3x3& mat,  int hAlignment, int vAlignment, bool useScreenPos) = 0;
		virtual void DrawRect(const Rect& rect, bool fill, Color color) = 0;

		virtual IRenderBitmap* CreateRenderBitmap(std::wstring filePath) = 0;
		virtual IRenderFont* CreateRenderFontFromFilePath(std::wstring filePath) = 0;
		virtual IRenderFont* CreateRenderFontFromOS(std::wstring fontName) = 0;
		virtual RenderAPIMemoryStatus CollectMemoryUsage() = 0;

		virtual void SetViewport(Rect rect) = 0;
		virtual void ResetViewport() = 0;

		virtual void SwapBuffer() = 0;
		virtual void ChangeBufferSize(int newWidth, int newHeight) = 0;
		virtual IWindow& GetWindow() const = 0;
		virtual void SetVSyncInterval(int interval) = 0;
		virtual ~IRenderAPI() = default;
	};
}