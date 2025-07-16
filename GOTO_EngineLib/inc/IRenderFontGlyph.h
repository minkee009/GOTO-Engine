#pragma once

namespace GOTOEngine
{
	enum IRenderFontStyle
	{
		Normal = 0,
		Bold = 1,
		Italic = 2,
		BoldItalic = 3
	};

	class IRenderFontGlyph
	{
	public:
		virtual ~IRenderFontGlyph() = default;
		virtual void* GetNativeHandle() const = 0;
		virtual IRenderFontStyle GetStyle() const = 0;
		virtual int GetSize() const = 0;
	};
}

