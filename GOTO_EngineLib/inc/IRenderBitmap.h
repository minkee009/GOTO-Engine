#pragma once
#include <string>

namespace GOTOEngine
{
	class IRenderBitmap
	{
	public:
		virtual ~IRenderBitmap() = default;
		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;
	};
}