#pragma once
#include "Color.h"

namespace GOTOEngine
{
	class IWindow;
	class IRenderImage;
	class IRenderFont;
	class IRenderAPI
	{
	public:
		virtual bool Initialize(IWindow* window) = 0;
		virtual void Release() = 0;
		virtual void Clear() = 0;

		virtual void DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image) = 0;
		virtual void DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color) = 0;
		virtual void DrawRect(int x, int y, int width, int height, bool fill, Color color) = 0;

		virtual void SwapBuffer() = 0;
		virtual void ChangeBufferSize(int newWidth, int newHeight) = 0;
		virtual IWindow& GetWindow() const = 0;
		virtual void SetVSyncInterval(int interval) = 0;
		virtual ~IRenderAPI() = default;
	};
}