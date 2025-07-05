#pragma once
#include <string>

namespace GOTOEngine
{
	struct Color;
	class IRenderImage
	{
	public:
		virtual ~IRenderImage() = default;
		virtual const std::wstring& GetFilePath() const = 0;
		virtual void* GetNativeHandle() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};
}