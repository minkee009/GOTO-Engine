#pragma once
#include <functional>  

namespace GOTOEngine
{
	class IWindow {
	public:
		virtual void SetOnChangedWindowSize(std::function<void(int, int)> callback) = 0;
		virtual bool Create(int width, int height, const wchar_t* title) = 0;
		virtual void* GetNativeHandle() const = 0;
		virtual void PollEvents() = 0;
		virtual bool ShouldClose() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual bool IsFullScreen() const = 0;
		virtual void ToggleFullScreen() = 0;
		virtual ~IWindow() = default;
	};
}