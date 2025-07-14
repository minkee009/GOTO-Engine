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
	class IRenderAPI
	{
	public:
		virtual bool Initialize(IWindow* window) = 0;
		virtual void Release() = 0;
		virtual void Clear() = 0;

		//virtual void DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image) = 0;
		virtual void DrawBitmap(const Matrix3x3& mat, IRenderBitmap* bitmap) = 0;
		virtual void DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color) = 0;
		virtual void DrawRect(int x, int y, int width, int height, bool fill, Color color) = 0;

		virtual IRenderBitmap* CreateBitmap(std::wstring filePath) = 0;
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